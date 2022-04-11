#include "estimator.h"
#include "stateMachine.h"
#include "math.h"

#include "flags.h"
#include "Eigen/Eigen"
#include <string>

#include "sensors.h"
#include "Storage/systemstatus.h"
#include "Storage/logController.h"


Estimator::Estimator(SystemStatus& systemstatus,LogController& logcontroller):
_systemstatus(systemstatus),
_logcontroller(logcontroller),
update_frequency(5000),//200Hz update
_homeSet(false),
madgwick(0.5f,0.005f), // beta | gyroscope sample time step (s)
localizationkf(_logcontroller)
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

void Estimator::update(const SensorStructs::raw_measurements_t& raw_sensors){
   

   unsigned long dt = (unsigned long)(micros() - last_update); //explictly casting to prevent micros() overflow cuasing issues
   

   if (dt > update_frequency){

      last_update = micros(); // update last_update 
      float dt_seconds = float(dt)*0.000001F; //conversion to seconds

      if (0 && _systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_IMU)){

         if (_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_GPS) && _systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BARO)){
            //no data so we cant calculate any nav solution
            changeEstimatorState(ESTIMATOR_STATE::NOSOLUTION,"no data, cannot compute navigation solution");
            return;
         }

         if (_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_GPS)){
            //baro only update
            changeEstimatorState(ESTIMATOR_STATE::PARTIAL_BARO,"no gps and imu, raw baro navigation solution");
            return;
         }

         if (_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BARO)){
            //gps only update, no fusion
            state.position = localizationkf.GPStoNED(raw_sensors.gps.lat,
                                                     raw_sensors.gps.lng,
                                                     raw_sensors.gps.alt);
            state.velocity = Eigen::Vector3f{raw_sensors.gps.v_n/1000.0f,
                                             raw_sensors.gps.v_e/1000.0f,
                                             raw_sensors.gps.v_d/1000.0f};
            changeEstimatorState(ESTIMATOR_STATE::PARTIAL_GPS,"no baro and imu, raw gps navigation solution");
            return;
         }

         changeEstimatorState(ESTIMATOR_STATE::PARTIAL_GPS_BARO,"no imu, partial navigation solution");
         return;

      }

      // imu present, call estimator functions which depend on imu
      updateAngularRates(raw_sensors.accelgyro.gx,
                         raw_sensors.accelgyro.gy,
                         raw_sensors.accelgyro.gz);

      updateOrientation(raw_sensors.accelgyro.gx,
                        raw_sensors.accelgyro.gy,
                        raw_sensors.accelgyro.gz,
                        raw_sensors.accelgyro.ax,
                        raw_sensors.accelgyro.ay,
                        raw_sensors.accelgyro.az,
                        raw_sensors.mag.mx,
                        raw_sensors.mag.my,
                        raw_sensors.mag.mz,
                        dt_seconds);

      updateLinearAcceleration(raw_sensors.accelgyro.ax,
                               raw_sensors.accelgyro.ay,
                               raw_sensors.accelgyro.az);

      if (!_homeSet){
         changeEstimatorState(ESTIMATOR_STATE::NOHOME,"Home not set, Localization not avaliable");
         return;
      }

      if (0 && _systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_GPS) && _systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BARO)){
         //no data so only orientation avalibale
         changeEstimatorState(ESTIMATOR_STATE::PARTIAL_IMU,"no gps and baro, cannot compute positional navigation solution");
         return;
      }

      localizationkf.predict((state.acceleration * g),dt_seconds);
      

      if (0 && _systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_GPS)){
         //baro only update
         changeEstimatorState(ESTIMATOR_STATE::PARTIAL_BARO_IMU,"no gps, only baro kf update");
         state.position = localizationkf.getPosition();
         state.velocity = localizationkf.getVelocity();
         return;
      }else{
         // if there is no error in the gps check if gps data is updated and we have a valid fixat perform kf update
         if (raw_sensors.gps.updated && raw_sensors.gps.fix >= 3){
            localizationkf.gpsUpdate(raw_sensors.gps.lat,
                                     raw_sensors.gps.lng,
                                     raw_sensors.gps.alt,
                                     raw_sensors.gps.v_n,
                                     raw_sensors.gps.v_e,
                                     raw_sensors.gps.v_d);
         }
      }
      

      if (0 && _systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BARO)){
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

void Estimator::setHome(const SensorStructs::raw_measurements_t& raw_sensors){
   // record current gps coordinates as home
   state.gps_launch_lat = raw_sensors.gps.lat;
   state.gps_launch_long = raw_sensors.gps.lng;
   state.gps_launch_alt = raw_sensors.gps.alt;
   //update barometer reference altitude
   state.baro_ref_alt = raw_sensors.baro.alt;
   //log the new home position
   _logcontroller.log("Home Position Updated to Lat: " 
                           + std::to_string(state.gps_launch_lat)
                           + " Long: "
                           + std::to_string(state.gps_launch_long)
                           + " Alt: "
                           + std::to_string(state.gps_launch_alt)
                           + " Baro Ref Alt: "
                           + std::to_string(state.baro_ref_alt));
   //update reference coordinates in position estimation
   localizationkf.updateGPSReference(state.gps_launch_lat,state.gps_launch_long,state.gps_launch_alt);
   //update baro alt in position estimation
   localizationkf.setup(); // reinitialize the filter 
   _homeSet = true;
}

void Estimator::updateLinearAcceleration(const float& ax,const float& ay,const float& az){
   //LINEAR ACCELERATION CALCULATION//
   //add raw accelerations into matrix form -> acceleration values in g's
   Eigen::Vector3f raw_accel(ax,ay,az);

   //calculate linear acceleration in NED frame
   state.acceleration = (madgwick.getInverseRotationMatrix()*raw_accel) - gravity_vector;
};

void Estimator::updateAngularRates(const float& gx,const float& gy,const float& gz){
   //update angular rates
   state.angularRates = Eigen::Vector3f{gx,
                                        gy,
                                        gz};
};

void Estimator::updateOrientation(const float& gx,const float& gy,const float& gz,const float& ax,const float& ay,const float& az,const float& mx,const float& my,const float& mz,float dt){
   //calculate orientation solution
   madgwick.setDeltaT(dt); // update integration time
   madgwick.update(_flipConstant*gx,
                   _flipConstant*gy,
                   _flipConstant*gz,
                   _flipConstant*ax,
                   _flipConstant*ay,
                   _flipConstant*az,
                   _flipConstant*mx, 
                   _flipConstant*my,
                   _flipConstant*mz); 
   //update orientation
   state.orientation = madgwick.getOrientation();
   state.eulerAngles = madgwick.getEulerAngles();
};

void Estimator::changeEstimatorState(ESTIMATOR_STATE status,std::string logmessage){
   if (state.estimator_state != static_cast<uint8_t>(status)){ // check if we already have logged this
            state.estimator_state = static_cast<uint8_t>(status);
            if (status != ESTIMATOR_STATE::NOMINAL){
               _systemstatus.new_message(SYSTEM_FLAG::ERROR_ESTIMATOR,logmessage);
            }else if (_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_ESTIMATOR)){
               _systemstatus.delete_message(SYSTEM_FLAG::ERROR_ESTIMATOR,logmessage);
            }
   }
}

void Estimator::changeBeta(float beta){
   madgwick.setBeta(beta);
}

void Estimator::resetOrientation(){
   madgwick.reset();
}

void Estimator::setIgnitionTime(uint32_t time)
{
    state.ignitionTime = time;
}

void Estimator::setLaunchTime(uint32_t time)
{
   state.launchTime = time;
}

void Estimator::setApogeeTime(uint32_t time)
{
   state.apogeeTime = time;
}

const SensorStructs::state_t& Estimator::getData()
{
   //again as with sensors, this should be updated to return the data in a threadsafe manner
    return state;
}