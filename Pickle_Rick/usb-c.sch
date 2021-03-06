EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 7
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 5450 3500 2    50   Input ~ 0
usb_D+
Text HLabel 5450 3200 2    50   Input ~ 0
usb_D-
Text HLabel 5400 2700 2    50   Input ~ 0
usb_VBUS
Wire Wire Line
	4700 3200 4700 3300
Wire Wire Line
	4700 3200 5450 3200
Wire Wire Line
	4700 3400 4700 3500
Wire Wire Line
	5450 3500 4700 3500
$Comp
L power:GND #PWR0110
U 1 1 60735E2B
P 4100 4350
F 0 "#PWR0110" H 4100 4100 50  0001 C CNN
F 1 "GND" H 4105 4177 50  0000 C CNN
F 2 "" H 4100 4350 50  0001 C CNN
F 3 "" H 4100 4350 50  0001 C CNN
	1    4100 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 4350 4100 4200
NoConn ~ 4700 3800
NoConn ~ 4700 3900
Connection ~ 4100 4200
Connection ~ 4700 3200
Connection ~ 4700 3500
Wire Wire Line
	3800 4200 4100 4200
$Comp
L Connector:USB_C_Receptacle_USB2.0 J1
U 1 1 607282EC
P 4100 3300
F 0 "J1" H 4207 4167 50  0000 C CNN
F 1 "USB_C_Receptacle_USB2.0" H 4207 4076 50  0000 C CNN
F 2 "" H 4250 3300 50  0001 C CNN
F 3 "https://www.usb.org/sites/default/files/documents/usb_type-c.zip" H 4250 3300 50  0001 C CNN
	1    4100 3300
	1    0    0    -1  
$EndComp
$Comp
L Device:R R42
U 1 1 6043A0B1
P 7100 3150
F 0 "R42" V 6893 3150 50  0000 C CNN
F 1 "5.1k" V 6984 3150 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7030 3150 50  0001 C CNN
F 3 "~" H 7100 3150 50  0001 C CNN
	1    7100 3150
	-1   0    0    1   
$EndComp
Wire Wire Line
	7100 2900 7100 3000
Wire Wire Line
	4700 2700 5400 2700
$Comp
L Device:R R41
U 1 1 6043AE1B
P 6600 3150
F 0 "R41" V 6807 3150 50  0000 C CNN
F 1 "5.1k" V 6716 3150 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6530 3150 50  0001 C CNN
F 3 "~" H 6600 3150 50  0001 C CNN
	1    6600 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 2900 7100 2900
Wire Wire Line
	4700 3000 6600 3000
$Comp
L power:GND #PWR?
U 1 1 60488D63
P 6600 3700
F 0 "#PWR?" H 6600 3450 50  0001 C CNN
F 1 "GND" H 6605 3527 50  0000 C CNN
F 2 "" H 6600 3700 50  0001 C CNN
F 3 "" H 6600 3700 50  0001 C CNN
	1    6600 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60489008
P 7100 3700
F 0 "#PWR?" H 7100 3450 50  0001 C CNN
F 1 "GND" H 7105 3527 50  0000 C CNN
F 2 "" H 7100 3700 50  0001 C CNN
F 3 "" H 7100 3700 50  0001 C CNN
	1    7100 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Small D?
U 1 1 6053D295
P 6600 3500
AR Path="/6053D295" Ref="D?"  Part="1" 
AR Path="/602F075A/6053D295" Ref="D?"  Part="1" 
F 0 "D?" H 6600 3295 50  0000 C CNN
F 1 "USB_LED" H 6600 3386 50  0000 C CNN
F 2 "LED_SMD:LED_0603_1608Metric_Castellated" V 6600 3500 50  0001 C CNN
F 3 "~" V 6600 3500 50  0001 C CNN
	1    6600 3500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6600 3300 6600 3400
Wire Wire Line
	6600 3600 6600 3700
Wire Wire Line
	7100 3700 7100 3600
$Comp
L Device:LED_Small D?
U 1 1 604A9E91
P 7100 3500
AR Path="/604A9E91" Ref="D?"  Part="1" 
AR Path="/602F075A/604A9E91" Ref="D?"  Part="1" 
F 0 "D?" H 7100 3295 50  0000 C CNN
F 1 "USB_LED" H 7100 3386 50  0000 C CNN
F 2 "LED_SMD:LED_0603_1608Metric_Castellated" V 7100 3500 50  0001 C CNN
F 3 "~" V 7100 3500 50  0001 C CNN
	1    7100 3500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7100 3400 7100 3300
Text Notes 7450 4050 2    50   ~ 0
Upwards LED
Text Notes 6150 4050 0    50   ~ 0
Downwards LED
$EndSCHEMATC
