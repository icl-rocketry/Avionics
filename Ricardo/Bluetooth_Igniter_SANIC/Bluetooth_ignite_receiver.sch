EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RF_Module:ESP32-WROOM-32 U1
U 1 1 6038790F
P 5800 3500
F 0 "U1" H 5800 5081 50  0000 C CNN
F 1 "ESP32-WROOM-32" H 5800 4990 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 5800 2000 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf" H 5500 3550 50  0001 C CNN
	1    5800 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 2300 4850 2300
Text Label 4850 2300 0    50   ~ 0
EN
Wire Wire Line
	6400 4600 6750 4600
Wire Wire Line
	6400 4500 6750 4500
Wire Wire Line
	6400 4400 6750 4400
Wire Wire Line
	6400 4300 6750 4300
$Sheet
S 9350 5350 1000 600 
U 603A281D
F0 "pyros" 50
F1 "pyros.sch" 50
$EndSheet
Text Label 6550 4300 0    50   ~ 0
Nuke1
Text Label 6550 4400 0    50   ~ 0
Nuke2
Text Label 6550 4500 0    50   ~ 0
Cont1
Text Label 6550 4600 0    50   ~ 0
Cont2
Text HLabel 9350 5450 2    50   Input ~ 0
Nuke1
Text HLabel 9350 5550 2    50   Input ~ 0
Nuke2
Text HLabel 9350 5650 2    50   Input ~ 0
Cont1
Text HLabel 9350 5750 2    50   Input ~ 0
Cont2
Wire Wire Line
	9350 5450 9050 5450
Wire Wire Line
	9350 5550 9050 5550
Wire Wire Line
	9350 5650 9050 5650
Wire Wire Line
	9350 5750 9050 5750
Text Label 9050 5450 0    50   ~ 0
Nuke1
Text Label 9050 5550 0    50   ~ 0
Nuke2
Text Label 9050 5650 0    50   ~ 0
Cont1
Text Label 9050 5750 0    50   ~ 0
Cont2
$Comp
L power:GND #PWR0101
U 1 1 603E7988
P 5800 4900
F 0 "#PWR0101" H 5800 4650 50  0001 C CNN
F 1 "GND" H 5805 4727 50  0000 C CNN
F 2 "" H 5800 4900 50  0001 C CNN
F 3 "" H 5800 4900 50  0001 C CNN
	1    5800 4900
	1    0    0    -1  
$EndComp
NoConn ~ 6400 2500
NoConn ~ 6400 2700
NoConn ~ 6400 2800
NoConn ~ 6400 2900
NoConn ~ 6400 3000
NoConn ~ 6400 3100
NoConn ~ 6400 3200
NoConn ~ 6400 3300
NoConn ~ 6400 3400
NoConn ~ 6400 3500
NoConn ~ 6400 3600
NoConn ~ 6400 3700
NoConn ~ 6400 3800
NoConn ~ 6400 3900
NoConn ~ 6400 4000
NoConn ~ 6400 4100
NoConn ~ 6400 4200
NoConn ~ 5200 3500
NoConn ~ 5200 3600
NoConn ~ 5200 3700
NoConn ~ 5200 3800
NoConn ~ 5200 3900
NoConn ~ 5200 4000
NoConn ~ 5200 2500
NoConn ~ 5200 2600
$Comp
L power:+3V3 #PWR0102
U 1 1 603E97E4
P 5300 1150
F 0 "#PWR0102" H 5300 1000 50  0001 C CNN
F 1 "+3V3" H 5315 1323 50  0000 C CNN
F 2 "" H 5300 1150 50  0001 C CNN
F 3 "" H 5300 1150 50  0001 C CNN
	1    5300 1150
	1    0    0    -1  
$EndComp
$Sheet
S 1800 1500 900  300 
U 603E92A7
F0 "power" 50
F1 "power.sch" 50
$EndSheet
$Comp
L Device:C_Small C?
U 1 1 604692B6
P 5300 1400
F 0 "C?" H 5392 1446 50  0000 L CNN
F 1 "100uF" H 5392 1355 50  0000 L CNN
F 2 "" H 5300 1400 50  0001 C CNN
F 3 "~" H 5300 1400 50  0001 C CNN
	1    5300 1400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60469B68
P 5600 1400
F 0 "C?" H 5692 1446 50  0000 L CNN
F 1 "1uF" H 5692 1355 50  0000 L CNN
F 2 "" H 5600 1400 50  0001 C CNN
F 3 "~" H 5600 1400 50  0001 C CNN
	1    5600 1400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60469F35
P 5300 1700
F 0 "#PWR?" H 5300 1450 50  0001 C CNN
F 1 "GND" H 5305 1527 50  0000 C CNN
F 2 "" H 5300 1700 50  0001 C CNN
F 3 "" H 5300 1700 50  0001 C CNN
	1    5300 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 6046A825
P 6500 1450
F 0 "R?" H 6559 1496 50  0000 L CNN
F 1 "470" H 6559 1405 50  0000 L CNN
F 2 "" H 6500 1450 50  0001 C CNN
F 3 "~" H 6500 1450 50  0001 C CNN
	1    6500 1450
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D?
U 1 1 6046B11D
P 6500 1700
F 0 "D?" V 6539 1582 50  0000 R CNN
F 1 "Power_LED" V 6448 1582 50  0000 R CNN
F 2 "" H 6500 1700 50  0001 C CNN
F 3 "~" H 6500 1700 50  0001 C CNN
	1    6500 1700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5300 1150 5300 1300
Wire Wire Line
	5600 1500 5600 1550
Wire Wire Line
	5300 1500 5300 1550
Wire Wire Line
	5300 1300 5600 1300
Connection ~ 5300 1300
Wire Wire Line
	5600 1550 5300 1550
Connection ~ 5300 1550
Wire Wire Line
	5300 1550 5300 1700
Wire Wire Line
	5600 1300 5800 1300
Wire Wire Line
	6500 1300 6500 1350
Connection ~ 5600 1300
Wire Wire Line
	5800 1300 5800 2100
Connection ~ 5800 1300
Wire Wire Line
	5800 1300 6500 1300
$Comp
L power:GND #PWR?
U 1 1 6047111A
P 6500 1850
F 0 "#PWR?" H 6500 1600 50  0001 C CNN
F 1 "GND" H 6505 1677 50  0000 C CNN
F 2 "" H 6500 1850 50  0001 C CNN
F 3 "" H 6500 1850 50  0001 C CNN
	1    6500 1850
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 60472246
P 9800 850
F 0 "SW?" H 9800 1135 50  0000 C CNN
F 1 "SW_Push" H 9800 1044 50  0000 C CNN
F 2 "" H 9800 1050 50  0001 C CNN
F 3 "~" H 9800 1050 50  0001 C CNN
	1    9800 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 850  9150 850 
Wire Wire Line
	9150 850  9150 1200
Wire Wire Line
	10000 850  10150 850 
Text Label 10850 850  0    50   ~ 0
EN
Wire Wire Line
	9150 1200 9550 1200
Wire Wire Line
	10150 1200 10150 850 
Connection ~ 9150 1200
Wire Wire Line
	9150 1200 9150 1300
Connection ~ 10150 850 
Wire Wire Line
	10150 850  10350 850 
$Comp
L Device:R_Small R?
U 1 1 6047462D
P 10450 850
F 0 "R?" V 10254 850 50  0000 C CNN
F 1 "470" V 10345 850 50  0000 C CNN
F 2 "" H 10450 850 50  0001 C CNN
F 3 "~" H 10450 850 50  0001 C CNN
	1    10450 850 
	0    1    1    0   
$EndComp
Wire Wire Line
	10550 850  10950 850 
$Comp
L Device:C_Small C?
U 1 1 60475423
P 9650 1200
F 0 "C?" V 9421 1200 50  0000 C CNN
F 1 "1nF" V 9512 1200 50  0000 C CNN
F 2 "" H 9650 1200 50  0001 C CNN
F 3 "~" H 9650 1200 50  0001 C CNN
	1    9650 1200
	0    1    1    0   
$EndComp
Wire Wire Line
	9750 1200 10150 1200
$Comp
L power:GND #PWR?
U 1 1 60475F37
P 9150 1300
F 0 "#PWR?" H 9150 1050 50  0001 C CNN
F 1 "GND" H 9155 1127 50  0000 C CNN
F 2 "" H 9150 1300 50  0001 C CNN
F 3 "" H 9150 1300 50  0001 C CNN
	1    9150 1300
	1    0    0    -1  
$EndComp
$Comp
L Interface_USB:CP2102N-A01-GQFN24 U?
U 1 1 6047F012
P 2150 3800
F 0 "U?" H 2150 4881 50  0000 C CNN
F 1 "CP2102N-A01-GQFN24" H 2150 4790 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-24-1EP_4x4mm_P0.5mm_EP2.6x2.6mm" H 2600 3000 50  0001 L CNN
F 3 "https://www.silabs.com/documents/public/data-sheets/cp2102n-datasheet.pdf" H 2200 2750 50  0001 C CNN
	1    2150 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 2300 6900 2300
Wire Wire Line
	6400 2400 6900 2400
Wire Wire Line
	6400 2600 6900 2600
Text Label 6900 2300 0    50   ~ 0
BOOT
Text Label 6900 2400 0    50   ~ 0
TX
Text Label 6900 2600 0    50   ~ 0
RX
$Comp
L Switch:SW_Push SW?
U 1 1 604A2321
P 9800 1850
F 0 "SW?" H 9800 2135 50  0000 C CNN
F 1 "SW_Push" H 9800 2044 50  0000 C CNN
F 2 "" H 9800 2050 50  0001 C CNN
F 3 "~" H 9800 2050 50  0001 C CNN
	1    9800 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 1850 9150 1850
Wire Wire Line
	9150 1850 9150 2200
Wire Wire Line
	10000 1850 10150 1850
Text Label 10850 1850 0    50   ~ 0
EN
Wire Wire Line
	9150 2200 9550 2200
Wire Wire Line
	10150 2200 10150 1850
Connection ~ 9150 2200
Wire Wire Line
	9150 2200 9150 2300
Connection ~ 10150 1850
Wire Wire Line
	10150 1850 10350 1850
$Comp
L Device:R_Small R?
U 1 1 604A2331
P 10450 1850
F 0 "R?" V 10254 1850 50  0000 C CNN
F 1 "470" V 10345 1850 50  0000 C CNN
F 2 "" H 10450 1850 50  0001 C CNN
F 3 "~" H 10450 1850 50  0001 C CNN
	1    10450 1850
	0    1    1    0   
$EndComp
Wire Wire Line
	10550 1850 10950 1850
$Comp
L Device:C_Small C?
U 1 1 604A2338
P 9650 2200
F 0 "C?" V 9421 2200 50  0000 C CNN
F 1 "1nF" V 9512 2200 50  0000 C CNN
F 2 "" H 9650 2200 50  0001 C CNN
F 3 "~" H 9650 2200 50  0001 C CNN
	1    9650 2200
	0    1    1    0   
$EndComp
Wire Wire Line
	9750 2200 10150 2200
$Comp
L power:GND #PWR?
U 1 1 604A233F
P 9150 2300
F 0 "#PWR?" H 9150 2050 50  0001 C CNN
F 1 "GND" H 9155 2127 50  0000 C CNN
F 2 "" H 9150 2300 50  0001 C CNN
F 3 "" H 9150 2300 50  0001 C CNN
	1    9150 2300
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 604A7BC1
P 8250 900
F 0 "R?" H 8309 946 50  0000 L CNN
F 1 "12K" H 8309 855 50  0000 L CNN
F 2 "" H 8250 900 50  0001 C CNN
F 3 "~" H 8250 900 50  0001 C CNN
	1    8250 900 
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 604A880C
P 8250 1200
F 0 "C?" H 8342 1246 50  0000 L CNN
F 1 "1nF" H 8342 1155 50  0000 L CNN
F 2 "" H 8250 1200 50  0001 C CNN
F 3 "~" H 8250 1200 50  0001 C CNN
	1    8250 1200
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 604A8FF0
P 8250 800
F 0 "#PWR?" H 8250 650 50  0001 C CNN
F 1 "+3V3" H 8265 973 50  0000 C CNN
F 2 "" H 8250 800 50  0001 C CNN
F 3 "" H 8250 800 50  0001 C CNN
	1    8250 800 
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 1000 8250 1100
Wire Wire Line
	8250 1100 8950 1100
Connection ~ 8250 1100
Text Label 8850 1100 0    50   ~ 0
EN
$Comp
L power:GND #PWR?
U 1 1 604B0B36
P 8250 1300
F 0 "#PWR?" H 8250 1050 50  0001 C CNN
F 1 "GND" H 8255 1127 50  0000 C CNN
F 2 "" H 8250 1300 50  0001 C CNN
F 3 "" H 8250 1300 50  0001 C CNN
	1    8250 1300
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 604B12D0
P 8250 1900
F 0 "#PWR?" H 8250 1750 50  0001 C CNN
F 1 "+3V3" H 8265 2073 50  0000 C CNN
F 2 "" H 8250 1900 50  0001 C CNN
F 3 "" H 8250 1900 50  0001 C CNN
	1    8250 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 604B1A2B
P 8250 2000
F 0 "R?" H 8309 2046 50  0000 L CNN
F 1 "5K" H 8309 1955 50  0000 L CNN
F 2 "" H 8250 2000 50  0001 C CNN
F 3 "~" H 8250 2000 50  0001 C CNN
	1    8250 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 2100 8250 2200
Wire Wire Line
	8250 2200 8950 2200
Text Label 8750 2200 0    50   ~ 0
BOOT
$EndSCHEMATC
