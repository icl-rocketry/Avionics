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
P 2100 3650
AR Path="/6049A0C6" Ref="J?"  Part="1" 
AR Path="/602F075A/6049A0C6" Ref="J?"  Part="1" 
F 0 "J?" H 2157 4117 50  0000 C CNN
F 1 "USB_B_Micro" H 2157 4026 50  0000 C CNN
F 2 "Connector_USB:USB_Micro-B_Amphenol_10103594-0001LF_Horizontal" H 2250 3600 50  0001 C CNN
F 3 "~" H 2250 3600 50  0001 C CNN
	1    2100 3650
	1    0    0    -1  
$EndComp
Connection ~ 2100 4050
$Comp
L power:GND #PWR?
U 1 1 6049A0CD
P 2100 4150
AR Path="/6049A0CD" Ref="#PWR?"  Part="1" 
AR Path="/602F075A/6049A0CD" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2100 3900 50  0001 C CNN
F 1 "GND" H 2105 3977 50  0000 C CNN
F 2 "" H 2100 4150 50  0001 C CNN
F 3 "" H 2100 4150 50  0001 C CNN
	1    2100 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 4150 2100 4050
Wire Wire Line
	2100 4050 2000 4050
Text HLabel 5400 2700 2    50   Input ~ 0
usb_VBUS
Wire Wire Line
	2400 3650 2900 3650
Wire Wire Line
	2400 3750 2900 3750
Wire Wire Line
	2900 3850 2400 3850
Wire Wire Line
	2400 3450 2900 3450
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
	3800 4200 4100 4200
Wire Wire Line
	4100 4350 4100 4200
Connection ~ 4700 3500
Connection ~ 4700 3200
Connection ~ 4100 4200
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
Text HLabel 5450 2900 2    50   Input ~ 0
usb_ID
NoConn ~ 4700 3800
NoConn ~ 4700 3900
Wire Wire Line
	4700 3000 4700 2900
Wire Wire Line
	5450 2900 4700 2900
Connection ~ 4700 2900
Text HLabel 8400 3500 2    50   Input ~ 0
usb_D+
Text HLabel 8400 3200 2    50   Input ~ 0
usb_D-
Text HLabel 8350 2700 2    50   Input ~ 0
usb_VBUS
Wire Wire Line
	7650 2700 8350 2700
Wire Wire Line
	7650 3200 7650 3300
Wire Wire Line
	7650 3200 8400 3200
Wire Wire Line
	7650 3400 7650 3500
Wire Wire Line
	8400 3500 7650 3500
$Comp
L power:GND #PWR?
U 1 1 60743CCA
P 7050 4350
F 0 "#PWR?" H 7050 4100 50  0001 C CNN
F 1 "GND" H 7055 4177 50  0000 C CNN
F 2 "" H 7050 4350 50  0001 C CNN
F 3 "" H 7050 4350 50  0001 C CNN
	1    7050 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 4200 7050 4200
Wire Wire Line
	7050 4350 7050 4200
Text HLabel 8600 3900 2    50   Input ~ 0
usb_ID
Connection ~ 7650 3500
Connection ~ 7650 3200
Connection ~ 7050 4200
$Comp
L Connector:USB_C_Receptacle_USB2.0 J?
U 1 1 60743CD5
P 7050 3300
F 0 "J?" H 7157 4167 50  0000 C CNN
F 1 "USB_C_Receptacle_USB2.0" H 7157 4076 50  0000 C CNN
F 2 "" H 7200 3300 50  0001 C CNN
F 3 "https://www.usb.org/sites/default/files/documents/usb_type-c.zip" H 7200 3300 50  0001 C CNN
	1    7050 3300
	1    0    0    -1  
$EndComp
NoConn ~ 7650 2900
NoConn ~ 7650 3000
Wire Wire Line
	7650 3800 7650 3900
Wire Wire Line
	7650 3900 8600 3900
Connection ~ 7650 3900
Text Notes 4900 2300 0    89   ~ 0
One of these is right for sure...
$EndSCHEMATC
