//command id and definition. find description on excel doc

#ifndef COMMANDS_H
#define COMMANDS_H

enum class COMMANDS {
    Abort = 1,
    Callibrate_Accel = 2 ,
    Callibrate_Baro = 3,
    Callibrate_Gyro = 4,
    Callibrate_Mag = 5,
    Clear_Flash = 6,
    Clear_SD ,
    Detailed_Accel,
    Detailed_All,
    Detailed_Baro,
    Detailed_Battery,
    Detailed_GPS,
    Detailed_Gyro,
    Detailed_Mag,
    Detailed_Throttle,
    Enter_Countdown,
    Enter_Flight,
    Enter_Groundstation,
    Enter_Recovery,
    Enter_USBMode,
    Estimator_Output,
    Fire_pyro_1,
    Fire_pyro_2,
    Launch,
    Play_Song,
    Print_Flash,
    Print_Sd,
    Raw_Output,
    Reset,
    Set_Throttle,
    Start_Logging,
    Stop_Logging,
    Zero_Sensors,
}



#endif