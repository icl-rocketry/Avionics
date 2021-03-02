EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
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
L RF_Module:ESP32-WROOM-32 U?
U 1 1 602C1F0F
P 5450 3500
F 0 "U?" H 4900 4950 50  0000 C CNN
F 1 "ESP32-WROOM-32" H 5000 4900 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 5450 2000 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf" H 5150 3550 50  0001 C CNN
	1    5450 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 2300 6200 2300
Wire Wire Line
	5450 2100 5450 1950
Wire Wire Line
	6050 2400 6200 2400
Wire Wire Line
	6050 2600 6200 2600
Wire Wire Line
	4850 3500 4700 3500
Wire Wire Line
	4850 3600 4700 3600
Wire Wire Line
	4850 2300 4700 2300
Wire Wire Line
	5450 4900 5450 5000
$Comp
L power:GND #PWR?
U 1 1 602C4EAF
P 5450 5000
F 0 "#PWR?" H 5450 4750 50  0001 C CNN
F 1 "GND" H 5455 4827 50  0000 C CNN
F 2 "" H 5450 5000 50  0001 C CNN
F 3 "" H 5450 5000 50  0001 C CNN
	1    5450 5000
	1    0    0    -1  
$EndComp
Text Label 6200 2400 0    50   ~ 0
TX
Text Label 6200 2600 0    50   ~ 0
RX
Text Label 4700 2300 0    50   ~ 0
EN
$Comp
L power:+3V3 #PWR?
U 1 1 602C5CFD
P 5450 1950
F 0 "#PWR?" H 5450 1800 50  0001 C CNN
F 1 "+3V3" H 5465 2123 50  0000 C CNN
F 2 "" H 5450 1950 50  0001 C CNN
F 3 "" H 5450 1950 50  0001 C CNN
	1    5450 1950
	1    0    0    -1  
$EndComp
Text Label 4700 3500 0    50   ~ 0
SDA
Text Label 4700 3600 0    50   ~ 0
SCL
$Sheet
S 9450 5500 1450 750 
U 602C7242
F0 "power" 50
F1 "power.sch" 50
$EndSheet
$EndSCHEMATC
