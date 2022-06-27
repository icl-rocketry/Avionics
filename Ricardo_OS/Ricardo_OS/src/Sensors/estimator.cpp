#include "estimator.h"
#include "stateMachine.h"
#include "math.h"

#include "flags.h"
#include "Eigen/Eigen"
#include <string>

#include "sensors.h"
#include "Storage/systemstatus.h"
#include "Storage/logController.h"

#include <ArduinoJson.h>

Estimator::Estimator(SystemStatus &systemstatus, LogController &logcontroller) : _systemstatus(systemstatus),
                                                                                 _logcontroller(logcontroller),
                                                                                 update_frequency(5000), // 200Hz update
                                                                                 _homeSet(false),
                                                                                 madgwick(0.5f, 0.005f) // beta | gyroscope sample time step (s)
                                                                                 {};

void Estimator::setup()
{
   // update board orientation this is applied when converthing back to sensor frame where the orientaiton of sensor matters
   // upside down should be retireved from config file

   localizationkf.reset();
};

void Estimator::update(const SensorStructs::raw_measurements_t &raw_sensors)
{

   unsigned long dt = (unsigned long)(micros() - last_update); // explictly casting to prevent micros() overflow cuasing issues

   if (dt > update_frequency)
   {

      last_update = micros();                   // update last_update
      float dt_seconds = float(dt) * 0.000001F; // conversion to seconds

      if (_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_IMU))
      {

         if (_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_GPS) && _systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BARO))
         {
            // no data so we cant calculate any nav solution
            changeEstimatorState(ESTIMATOR_STATE::NOSOLUTION, "no data, cannot compute navigation solution");
            return;
         }

         if (_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_GPS))
         {
            // baro only update
            // TODO - add a z velocity estimate using first order filter and disrete derivative

            if (_homeSet) // if no home, this falls thru to the no home
            {
               localizationkf.baroUpdate(raw_sensors.baro.alt - state.baro_ref_alt);
               changeEstimatorState(ESTIMATOR_STATE::PARTIAL_NO_IMU_NO_GPS, "no IMU and GPS");
               predictLocalizationKF(dt_seconds);
               return;
            }
         }

         if (_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BARO))
         {
            // gps only update, no fusion as filtering this data will only result in a worse solution
            if (_homeSet) // if false, this falls thru to the more important error which is no home set
            {
               state.position = localizationkf.GPStoNED(raw_sensors.gps.lat,
                                                        raw_sensors.gps.lng,
                                                        raw_sensors.gps.alt);
               state.velocity = Eigen::Vector3f{raw_sensors.gps.v_n / 1000.0f,
                                                raw_sensors.gps.v_e / 1000.0f,
                                                raw_sensors.gps.v_d / 1000.0f};
               changeEstimatorState(ESTIMATOR_STATE::PARTIAL_NO_IMU_NO_BARO, "no IMU and BARO, raw gps navigation solution");
               return;
            }
         }
      }
      else
      { // we have imu so calculate orientation and update localizationkf
         if (_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_MAG))
         {
            updateOrientation(raw_sensors.accelgyro.gx, raw_sensors.accelgyro.gy, raw_sensors.accelgyro.gz,
                              raw_sensors.accelgyro.ax, raw_sensors.accelgyro.ay, raw_sensors.accelgyro.az,
                              dt_seconds);
            // check that there isnt a bigger error
            if (_homeSet)
            {
               changeEstimatorState(ESTIMATOR_STATE::PARTIAL_NO_MAG, "no mag, heading unreliable");
            }
         }
         else
         {
            updateOrientation(raw_sensors.accelgyro.gx, raw_sensors.accelgyro.gy, raw_sensors.accelgyro.gz,
                              raw_sensors.accelgyro.ax, raw_sensors.accelgyro.ay, raw_sensors.accelgyro.az,
                              raw_sensors.mag.mx, raw_sensors.mag.my, raw_sensors.mag.mz,
                              dt_seconds);
         }
         // transform angular rates from body frame to earth frame
         updateAngularRates(raw_sensors.accelgyro.gx, raw_sensors.accelgyro.gy, raw_sensors.accelgyro.gz);
         localizationkf.accelUpdate(getLinearAcceleration(raw_sensors.accelgyro.ax, raw_sensors.accelgyro.ay, raw_sensors.accelgyro.az) * g);
         // only update with high-g accelerometer if the low g acceleromter is working
         if (!_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_HACCEL))
         {
            localizationkf.HaccelUpdate(getLinearAcceleration(raw_sensors.accel.ax, raw_sensors.accel.ay, raw_sensors.accel.az) * g);
         }
      }

      if (!_homeSet)
      {
         changeEstimatorState(ESTIMATOR_STATE::NO_HOME, "Home not set, Localization not avaliable");
         return;
      }

      if (_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_GPS))
      {
         if (_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BARO))
         {
            // no data so only orientation avalibale
            changeEstimatorState(ESTIMATOR_STATE::PARTIAL_NO_GPS_NO_BARO, "no gps and baro, position and velocity estimates unreliable!");
            predictLocalizationKF(dt_seconds);
            return;
         }
         // baro only update
         changeEstimatorState(ESTIMATOR_STATE::PARTIAL_NO_GPS, "no gps, N and E positon and velocity estimates unreliable!");
      }
      else
      {
         // if there is no error in the gps check if gps data is updated
         if (raw_sensors.gps.updated)
         {
            localizationkf.gpsUpdate(raw_sensors.gps.lat,
                                     raw_sensors.gps.lng,
                                     raw_sensors.gps.alt,
                                     raw_sensors.gps.v_n,
                                     raw_sensors.gps.v_e,
                                     raw_sensors.gps.v_d);
         }
      }

      if (_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BARO))
      {
         changeEstimatorState(ESTIMATOR_STATE::PARTIAL_NO_BARO, "no baro");
      }
      else
      {
         localizationkf.baroUpdate(raw_sensors.baro.alt - state.baro_ref_alt);
      }

      predictLocalizationKF(dt_seconds);

      if (!_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_IMU, SYSTEM_FLAG::ERROR_GPS, SYSTEM_FLAG::ERROR_BARO, SYSTEM_FLAG::ERROR_MAG, SYSTEM_FLAG::ERROR_HACCEL))
      {
         // if there are no errors with sensors then the estimator state must be nominal
         changeEstimatorState(ESTIMATOR_STATE::NOMINAL, "all data avaliable, full update");
      }
   }
};

void Estimator::setHome(const SensorStructs::raw_measurements_t &raw_sensors)
{
   // record current gps coordinates as home
   state.gps_launch_lat = raw_sensors.gps.lat;
   state.gps_launch_long = raw_sensors.gps.lng;
   state.gps_launch_alt = raw_sensors.gps.alt;
   // update barometer reference altitude
   state.baro_ref_alt = raw_sensors.baro.alt;
   // log the new home position
   _logcontroller.log("Home Position Updated to Lat: " + std::to_string(state.gps_launch_lat) + " Long: " + std::to_string(state.gps_launch_long) + " Alt: " + std::to_string(state.gps_launch_alt) + " Baro Ref Alt: " + std::to_string(state.baro_ref_alt));
   // update reference coordinates in position estimation
   localizationkf.updateGPSReference(state.gps_launch_lat, state.gps_launch_long, state.gps_launch_alt);
   localizationkf.reset(); // reinitialize the filter
   _homeSet = true;
}

void Estimator::updateOrientation(const float &gx, const float &gy, const float &gz,
                                  const float &ax, const float &ay, const float &az,
                                  const float &mx, const float &my, const float &mz, float dt)
{

   // calculate orientation solution
   madgwick.setDeltaT(dt); // update integration time
   madgwick.update(gx, gy, gz, ax, ay, az, mx, my, mz);
   // update orientation
   state.orientation = madgwick.getOrientation();
   state.eulerAngles = madgwick.getEulerAngles();
}

void Estimator::updateOrientation(const float &gx, const float &gy, const float &gz,
                                  const float &ax, const float &ay, const float &az, float dt)
{

   // calculate orientation solution
   madgwick.setDeltaT(dt); // update integration time
   madgwick.updateIMU(gx, gy, gz, ax, ay, az);
   // update orientation
   state.orientation = madgwick.getOrientation();
   state.eulerAngles = madgwick.getEulerAngles();
}

void Estimator::updateAngularRates(const float &gx, const float &gy, const float &gz)
{
   madgwick.getInverseRotationMatrix() * Eigen::Vector3f{gx, gy, gz};
}

Eigen::Vector3f Estimator::getLinearAcceleration(const float &ax, const float &ay, const float &az)
{
   return (madgwick.getInverseRotationMatrix() * Eigen::Vector3f{ax, ay, az}) - Eigen::Vector3f{0, 0, 1};
};

void Estimator::changeEstimatorState(ESTIMATOR_STATE status, std::string logmessage)
{
   if (state.estimator_state != static_cast<uint8_t>(status))
   { // check if we already have logged this
      state.estimator_state = static_cast<uint8_t>(status);
      if (status != ESTIMATOR_STATE::NOMINAL)
      {
         _systemstatus.new_message(SYSTEM_FLAG::ERROR_ESTIMATOR, logmessage);
      }
      else if (_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_ESTIMATOR))
      {
         _systemstatus.delete_message(SYSTEM_FLAG::ERROR_ESTIMATOR, logmessage);
      }
   }
}

void Estimator::changeBeta(float beta)
{
   madgwick.setBeta(beta);
}

void Estimator::resetOrientation()
{
   madgwick.reset();
}

void Estimator::resetLocalization()
{
   localizationkf.reset();
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

const SensorStructs::state_t &Estimator::getData()
{
   // again as with sensors, this should be updated to return the data in a threadsafe manner
   return state;
}

void Estimator::predictLocalizationKF(const float &dt)
{
   localizationkf.predict(dt);
   state.acceleration = localizationkf.getAcceleration();
   state.velocity = localizationkf.getVelocity();
   state.position = localizationkf.getPosition();
}