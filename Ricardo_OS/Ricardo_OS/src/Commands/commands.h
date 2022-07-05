//command id and definition. find description on excel doc

#pragma once

enum class COMMANDS:uint8_t {
    Nocommand = 0,
    Launch = 1, 
    Reset = 2,
    Abort = 3,
    Set_Home = 4,
    Start_Logging = 5,
    Stop_Logging = 6,
    Telemetry = 8,
    Clear_Flash = 10,
    Clear_SD = 11,
    Print_Flash_filesystem = 12,
    Print_Sd_filesystem = 13,
    Play_Song = 14,
    Skip_Song = 15,
    Clear_Song_Queue = 16,
    Reset_Orientation = 50,
    Reset_Localization = 51,
    Set_Beta = 52,
    Calibrate_AccelGyro_Bias = 60,
    Calibrate_Mag_Full = 61,
    Calibrate_Baro = 62,
    Enter_Debug = 100,
    Enter_Preflight = 101,
    // Enter_Groundstation = 102,
    Enter_Countdown = 103,
    Enter_Flight = 104,
    Enter_Recovery = 105,
    Exit_Debug = 106,
    Exit_to_Debug = 107,
    Engine_Info = 180,
    Set_Throttle  = 181,
    Pyro_info = 200,
    Fire_pyro = 201,
    Free_Ram = 250
};






