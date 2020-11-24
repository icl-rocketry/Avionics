//command id and definition. find description on excel doc

#ifndef COMMANDS_H
#define COMMANDS_H

enum class COMMANDS {
    Launch = 1, 
    Reset = 2,
    Abort = 3,
    Zero_Sensors = 4,
    Start_Logging = 5,
    Stop_Logging = 6,
    Play_Song = 7,
    Telemetry = 8,
    Clear_Flash = 10,
    Clear_SD = 11,
    Print_Flash_filesystem = 12,
    Print_Sd_filesystem = 13,
    Raw_Sensors = 20,
    Detailed_All_Sensors = 50,
    Callibrate_Accel = 60,
    Callibrate_Mag= 61,
    Callibrate_Gyro = 62,
    Callibrate_Baro = 63,
    Enter_USBMode = 100,
    Enter_Groundstation = 101,
    Enter_Countdown = 102,
    Enter_Flight = 103,
    Enter_Recovery = 104,
    Exit_USBMode = 105,
    Exit_to_USBMode = 106,
    Set_Throttle  = 180,
    Fire_pyro = 200,
};



#endif