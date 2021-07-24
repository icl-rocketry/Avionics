#include "estimator.h"
#include "stateMachine.h"
#include "math.h"
#include "Storage/utils.h"
#include "flags.h"
#include "Eigen/Eigen"
#include <string>


Estimator::Estimator(stateMachine* sm):
_sm(sm),
update_frequency(5000),//200Hz update
_homeSet(false),
madgwick(0.5f,0.005f), // beta | gyroscope sample time step (s)
localizationkf(_sm->logcontroller)
{};

void Estimator::setup(){
//update board orientation this is applied when converthing back to sensor frame where the orientaiton of sensor matters
//upside down should be retireved from config file
   if (_upsideDown){
      _flipConstant = -1;
   }else{
      _flipConstant = 1;
   }
   localizationkf.setup();
   
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
            //gps only update, no fusion
            state.position = localizationkf.GPStoNED(_sm->sensors.sensors_raw.gps_lat,
                                                     _sm->sensors.sensors_raw.gps_long,
                                                     _sm->sensors.sensors_raw.gps_alt);
            state.velocity = Eigen::Vector3f{_sm->sensors.sensors_raw.gps_v_n/1000.0f,
                                             _sm->sensors.sensors_raw.gps_v_e/1000.0f,
                                             _sm->sensors.sensors_raw.gps_v_d/1000.0f};
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

      if (!_homeSet){
         changeEstimatorState(ESTIMATOR_STATE::NOHOME,"Home not set, Localization not avaliable");
         return;
      }

      if (_sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_GPS) && _sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BARO)){
         //no data so only orientation avalibale
         changeEstimatorState(ESTIMATOR_STATE::PARTIAL_IMU,"no gps and baro, cannot compute positional navigation solution");
         return;
      }

      localizationkf.predict((state.acceleration * g),dt_seconds);
      

      if (_sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_GPS)){
         //baro only update
         changeEstimatorState(ESTIMATOR_STATE::PARTIAL_BARO_IMU,"no gps, only baro kf update");
         state.position = localizationkf.getPosition();
         state.velocity = localizationkf.getVelocity();
         return;
      }else{
         // if there is no error in the gps check if gps data is updated and we have a valid fixat perform kf update
         if (_sm->sensors.sensors_raw.gps_updated && _sm->sensors.sensors_raw.gps_fix >= 3){
            localizationkf.gpsUpdate(_sm->sensors.sensors_raw.gps_lat,
                                     _sm->sensors.sensors_raw.gps_long,
                                     _sm->sensors.sensors_raw.gps_alt,
                                     _sm->sensors.sensors_raw.gps_v_n,
                                     _sm->sensors.sensors_raw.gps_v_e,
                                     _sm->sensors.sensors_raw.gps_v_d);
         }
      }
      

      if (_sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BARO)){
         //gps only update
         changeEstimatorState(ESTIMATOR_STATE::PARTIAL_GPS_IMU,"no baro, only gps kf update");
         state.position = localizationkf.getPosition();
         state.velocity = localizationkf.getVelocity();
         return;
      }else{
         //do baro update on kf -> also check there is a new reading to update with
         localizationkf.baroUpdate();
      }

      
      changeEstimatorState(ESTIMATOR_STATE::NOMINAL,"all data avaliable, full update");
      state.position = localizationkf.getPosition();
      state.velocity = localizationkf.getVelocity();
   }
    
};

void Estimator::setHome(){
   // record current gps coordinates as home
   state.gps_launch_lat = _sm->sensors.sensors_raw.gps_lat;
   state.gps_launch_long = _sm->sensors.sensors_raw.gps_long;
   state.gps_launch_alt = _sm->sensors.sensors_raw.gps_alt;
   //update barometer reference altitude
   state.baro_ref_alt = _sm->sensors.sensors_raw.baro_alt;
   //log the new home position
   _sm->logcontroller.log("Home Position Updated to Lat: " 
                           + std::to_string(state.gps_launch_lat)
                           + " Long: "
                           + std::to_string(state.gps_launch_long)
                           + " Alt: "
                           + std::to_string(state.gps_launch_alt)
                           + " Baro Ref Alt: ",
                           + std::to_string(state.baro_ref_alt));
   //update reference coordinates in position estimation
   localizationkf.updateGPSReference(state.gps_launch_lat,state.gps_launch_long,state.gps_launch_alt);
   //update baro alt in position estimation
   localizationkf.setup(); // reinitialize the filter 
   _homeSet = true;
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
   madgwick.update(_flipConstant*_sm->sensors.sensors_raw.gx,
                   _flipConstant*_sm->sensors.sensors_raw.gy,
                   _flipConstant*_sm->sensors.sensors_raw.gz,
                   _flipConstant*_sm->sensors.sensors_raw.ax,
                   _flipConstant*_sm->sensors.sensors_raw.ay,
                   _flipConstant*_sm->sensors.sensors_raw.az,
                   _flipConstant*_sm->sensors.sensors_raw.mx, 
                   _flipConstant*_sm->sensors.sensors_raw.my,
                   _flipConstant*_sm->sensors.sensors_raw.mz); 
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

void Estimator::changeBeta(float beta){
   madgwick.setBeta(beta);
}