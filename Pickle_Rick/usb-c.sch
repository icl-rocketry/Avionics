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
$Comp
L Connector:USB_B_Micro J?
U 1 1 6049A0C6
P 1550 3050
AR Path="/6049A0C6" Ref="J?"  Part="1" 
AR Path="/602F075A/6049A0C6" Ref="J?"  Part="1" 
F 0 "J?" H 1607 3517 50  0000 C CNN
F 1 "USB_B_Micro" H 1607 3426 50  0000 C CNN
F 2 "Connector_USB:USB_Micro-B_Amphenol_10103594-0001LF_Horizontal" H 1700 3000 50  0001 C CNN
F 3 "~" H 1700 3000 50  0001 C CNN
	1    1550 3050
	1    0    0    -1  
$EndComp
Connection ~ 1550 3450
$Comp
L power:GND #PWR?
U 1 1 6049A0CD
P 1550 3550
AR Path="/6049A0CD" Ref="#PWR?"  Part="1" 
AR Path="/602F075A/6049A0CD" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1550 3300 50  0001 C CNN
F 1 "GND" H 1555 3377 50  0000 C CNN
F 2 "" H 1550 3550 50  0001 C CNN
F 3 "" H 1550 3550 50  0001 C CNN
	1    1550 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 3550 1550 3450
Wire Wire Line
	1550 3450 1450 3450
Text HLabel 5400 2700 2    50   Input ~ 0
usb_VBUS
Wire Wire Line
	1850 3050 2350 3050
Wire Wire Line
	1850 3150 2350 3150
Wire Wire Line
	2350 3250 1850 3250
Wire Wire Line
	1850 2850 2350 2850
Wire Wire Line
	4700 2700 5400 2700
Wire Wire Line
	4700 3200 4700 3300
Wire Wire Line
	4700 3200 5450 3200
Wire Wire Line
	4700 3400 4700 3500
Wire Wire Line
	5450 3500 4700 3500
$Comp
L power:GND #PWR?
U 1 1 60735E2B
P 4100 4350
F 0 "#PWR?" H 4100 4100 50  0001 C CNN
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
L Connector:USB_C_Receptacle_USB2.0 J?
U 1 1 607282EC
P 4100 3300
F 0 "J?" H 4207 4167 50  0000 C CNN
F 1 "USB_C_Receptacle_USB2.0" H 4207 4076 50  0000 C CNN
F 2 "" H 4250 3300 50  0001 C CNN
F 3 "https://www.usb.org/sites/default/files/documents/usb_type-c.zip" H 4250 3300 50  0001 C CNN
	1    4100 3300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6043327D
P 6400 2850
F 0 "#PWR?" H 6400 2600 50  0001 C CNN
F 1 "GND" V 6405 2722 50  0000 R CNN
F 2 "" H 6400 2850 50  0001 C CNN
F 3 "" H 6400 2850 50  0001 C CNN
	1    6400 2850
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 604354EC
P 6400 3050
F 0 "#PWR?" H 6400 2800 50  0001 C CNN
F 1 "GND" V 6405 2922 50  0000 R CNN
F 2 "" H 6400 3050 50  0001 C CNN
F 3 "" H 6400 3050 50  0001 C CNN
	1    6400 3050
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 6043A0B1
P 6050 2850
F 0 "R?" V 5843 2850 50  0000 C CNN
F 1 "5.1k" V 5934 2850 50  0000 C CNN
F 2 "" V 5980 2850 50  0001 C CNN
F 3 "~" H 6050 2850 50  0001 C CNN
	1    6050 2850
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 6043AE1B
P 6050 3050
F 0 "R?" V 6257 3050 50  0000 C CNN
F 1 "5.1k" V 6166 3050 50  0000 C CNN
F 2 "" V 5980 3050 50  0001 C CNN
F 3 "~" H 6050 3050 50  0001 C CNN
	1    6050 3050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4700 2900 5800 2900
Wire Wire Line
	5800 2900 5800 2850
Wire Wire Line
	5800 2850 5900 2850
Wire Wire Line
	6200 2850 6400 2850
Wire Wire Line
	6400 3050 6200 3050
Wire Wire Line
	5900 3050 5800 3050
Wire Wire Line
	5800 3050 5800 3000
Wire Wire Line
	5800 3000 4700 3000
Text Notes 1300 2500 0    50   ~ 0
How it was before
Text Notes 3700 2300 0    50   ~ 0
Might do a thing...
Text Notes 2450 2850 0    50   ~ 0
vbus
Text Notes 2450 3050 0    50   ~ 0
d+
Text Notes 2450 3150 0    50   ~ 0
d-
Text Notes 2450 3300 0    50   ~ 0
detonator\n
Text HLabel 5500 3700 2    50   Input ~ 0
usb_DETONATOR
Text Notes 1600 3500 0    50   ~ 0
tought id was for "device identification"?\n
$EndSCHEMATC
