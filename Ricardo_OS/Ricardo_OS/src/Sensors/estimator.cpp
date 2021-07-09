#include "estimator.h"
#include "stateMachine.h"
#include "math.h"
#include "Storage/utils.h"
#include "flags.h"


Estimator::Estimator(stateMachine* sm):
_sm(sm),
update_frequency(2000),//500Hz update
madgwick(0.9,0.002) // beta | gyroscope sample time step (s)
{};

void Estimator::setup(){
//update board orientation this is applied when converthing back to sensor frame where the orientaiton of sensor matters
//upside down should be retireved from config file
   if (upsideDown){
      flipConstant = 1;
   }else{
      flipConstant = -1;
   }
   
};

void Estimator::update(){
   

   unsigned long dt = (unsigned long)(micros() - last_update); //explictly casting to prevent micros() overflow cuasing issues
   

   if (dt > update_frequency){

      last_update = micros(); // update last_update 
      float dt_seconds = float(dt)*0.000001F; //conversion to seconds

      if (_sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_IMU)){

         if (_sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_GPS) && _sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BARO)){
            //no data so we cant calculate any nav solution
            changeEstimatorState(ESTIMATOR_STATE::NOSOLUTION,"no data, cannot compute navigation solution");
            return;
         }

         if (_sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_GPS)){
            //baro only update
            changeEstimatorState(ESTIMATOR_STATE::PARTIAL_BARO,"no gps and imu, raw baro navigation solution");
            return;
         }

         if (_sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BARO)){
            //gps only update
            changeEstimatorState(ESTIMATOR_STATE::PARTIAL_GPS,"no baro and imu, raw gps navigation solution");
            return;
         }

         changeEstimatorState(ESTIMATOR_STATE::PARTIAL_GPS_BARO,"no imu, partial navigation solution");
         return;

      }

      // imu present, call estimator functions which depend on imu
      updateAngularRates();
      updateOrientation(dt_seconds);
      updateLinearAcceleration();

      if (_sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_GPS) && _sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BARO)){
         //no data so only orientation avalibale
         changeEstimatorState(ESTIMATOR_STATE::PARTIAL_IMU,"no gps and baro, cannot compute positional navigation solution");
         return;
      }

      if (_sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_GPS)){
         //baro only update
         changeEstimatorState(ESTIMATOR_STATE::PARTIAL_BARO_IMU,"no gps, only baro kf update");
         return;
      }

      if (_sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BARO)){
         //gps only update
         changeEstimatorState(ESTIMATOR_STATE::PARTIAL_GPS_IMU,"no baro, only gps kf update");
         return;
      }

      changeEstimatorState(ESTIMATOR_STATE::NOMINAL,"all data avaliable, full update");

   }
    
};

void Estimator::setHome(){
   // record current gps coordinates as home
   state.gps_launch_lat = _sm->sensors.sensors_raw.gps_lat;
   state.gps_launch_long = _sm->sensors.sensors_raw.gps_long;
   state.gps_launch_alt = _sm->sensors.sensors_raw.gps_alt;
   //log the new home position
   _sm->logcontroller.log("Home Position Updated to Lat: " 
                           + utils::tostring(state.gps_launch_lat)
                           + " Long: "
                           + utils::tostring(state.gps_launch_long)
                           + " Alt: "
                           + utils::tostring(state.gps_launch_alt));
}

void Estimator::updateLinearAcceleration(){
   //LINEAR ACCELERATION CALCULATION//
   //add raw accelerations into matrix form -> acceleration values in g's
   Eigen::Vector3f raw_accel(_sm->sensors.sensors_raw.ax,_sm->sensors.sensors_raw.ay,_sm->sensors.sensors_raw.az);
   
   //calculate linear acceleration in NED frame
   state.acceleration = (madgwick.getInverseRotationMatrix()*raw_accel) - gravity_vector;
};

void Estimator::updateAngularRates(){
   //update angular rates
   state.angularRates = Eigen::Vector3f{_sm->sensors.sensors_raw.gx,
                                       _sm->sensors.sensors_raw.gy,
                                       _sm->sensors.sensors_raw.gz};
};

void Estimator::updateOrientation(float dt){
   //calculate orientation solution
   madgwick.setDeltaT(dt); // update integration time
   madgwick.update(_sm->sensors.sensors_raw.gx,
                  _sm->sensors.sensors_raw.gy,
                  _sm->sensors.sensors_raw.gz,
                  _sm->sensors.sensors_raw.ax,
                  _sm->sensors.sensors_raw.ay,
                  _sm->sensors.sensors_raw.az,
                  -(_sm->sensors.sensors_raw.mx), // lsm9ds1 magnetometer x axis is opposite to gyro and accel
                  _sm->sensors.sensors_raw.my,
                  _sm->sensors.sensors_raw.mz); 
   //update orientation
   state.orientation = madgwick.getOrientation();
   state.eulerAngles = madgwick.getEulerAngles();
};

void Estimator::changeEstimatorState(ESTIMATOR_STATE status,std::string logmessage){
   if (state.estimator_state != static_cast<uint8_t>(status)){ // check if we already have logged this
            state.estimator_state = static_cast<uint8_t>(status);
            if (status != ESTIMATOR_STATE::NOMINAL){
               _sm->systemstatus.new_message(SYSTEM_FLAG::ERROR_ESTIMATOR,logmessage);
            }else if (_sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_ESTIMATOR)){
               _sm->systemstatus.delete_message(SYSTEM_FLAG::ERROR_ESTIMATOR,logmessage);
            }
   }
}