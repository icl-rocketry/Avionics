/*
**********************
* PINS               *
**********************
 */


#ifndef PINS_H
#define PINS_H

//#define JTAG


#define Buzzer 2
#define LoraReset 4
#define LoraCs 5


#ifdef JTAG
    #define ImuCs 34
    #define BaroCs 35
    #define MagCs 27
    #define FlashCs 26
#else
    #define ImuCs 12
    #define BaroCs 13
    #define MagCs 14
    #define FlashCs 15
#endif // JTAG

#define RxRadio 16
#define TxRadio 17
#define _SCLK 18
#define _MISO 19
#define _SDA 21
#define _SCL 22
#define _MOSI 23
#define SdCs 25
#define Nuke_1 26
#define Nuke_2 27
#define TxCan 32
#define RxCan 33
#define Cont_1 34
#define Cont_2 35
#define BattVolt 36
#define LoraInt 39

#endif