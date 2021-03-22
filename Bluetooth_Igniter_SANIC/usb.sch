EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
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
L Connector:USB_C_Receptacle_USB2.0 J5
U 1 1 604D8C4F
P 5100 3400
F 0 "J5" H 5207 4267 50  0000 C CNN
F 1 "USB_C_Receptacle_USB2.0" H 5207 4176 50  0000 C CNN
F 2 "Connector_USB:USB_C_Receptacle_XKB_U262-16XN-4BVC11" H 5250 3400 50  0001 C CNN
F 3 "https://www.usb.org/sites/default/files/documents/usb_type-c.zip" H 5250 3400 50  0001 C CNN
	1    5100 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 2800 6450 2800
Text HLabel 6450 2800 2    50   Input ~ 0
VBUS
NoConn ~ 5700 3900
NoConn ~ 5700 4000
Wire Wire Line
	5700 3000 7950 3000
Wire Wire Line
	5700 3100 7500 3100
Wire Wire Line
	7500 3100 7500 3150
Wire Wire Line
	7950 3000 7950 3150
$Comp
L power:GND #PWR039
U 1 1 604DAD27
P 7500 3700
F 0 "#PWR039" H 7500 3450 50  0001 C CNN
F 1 "GND" H 7505 3527 50  0000 C CNN
F 2 "" H 7500 3700 50  0001 C CNN
F 3 "" H 7500 3700 50  0001 C CNN
	1    7500 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR040
U 1 1 604DB3F7
P 7950 3700
F 0 "#PWR040" H 7950 3450 50  0001 C CNN
F 1 "GND" H 7955 3527 50  0000 C CNN
F 2 "" H 7950 3700 50  0001 C CNN
F 3 "" H 7950 3700 50  0001 C CNN
	1    7950 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R22
U 1 1 604DBB85
P 7500 3250
F 0 "R22" H 7559 3296 50  0000 L CNN
F 1 "5.1K" H 7559 3205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 7500 3250 50  0001 C CNN
F 3 "~" H 7500 3250 50  0001 C CNN
	1    7500 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 3350 7500 3400
$Comp
L Device:R_Small R23
U 1 1 604DC456
P 7950 3250
F 0 "R23" H 8009 3296 50  0000 L CNN
F 1 "5.1K" H 8009 3205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 7950 3250 50  0001 C CNN
F 3 "~" H 7950 3250 50  0001 C CNN
	1    7950 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 3350 7950 3400
$Comp
L Device:LED D10
U 1 1 604DCB0E
P 7500 3550
F 0 "D10" V 7539 3432 50  0000 R CNN
F 1 "LED" V 7448 3432 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Castellated" H 7500 3550 50  0001 C CNN
F 3 "~" H 7500 3550 50  0001 C CNN
	1    7500 3550
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D11
U 1 1 604DD87E
P 7950 3550
F 0 "D11" V 7989 3432 50  0000 R CNN
F 1 "LED" V 7898 3432 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Castellated" H 7950 3550 50  0001 C CNN
F 3 "~" H 7950 3550 50  0001 C CNN
	1    7950 3550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5700 3300 5900 3300
Text HLabel 6700 3300 2    50   Input ~ 0
D-
Wire Wire Line
	5700 3400 5900 3400
Wire Wire Line
	5900 3400 5900 3300
Connection ~ 5900 3300
Wire Wire Line
	5900 3300 6700 3300
Wire Wire Line
	5700 3500 5900 3500
Wire Wire Line
	5700 3600 5900 3600
Wire Wire Line
	5900 3600 5900 3500
Connection ~ 5900 3500
Wire Wire Line
	5900 3500 6700 3500
Text HLabel 6700 3500 2    50   Input ~ 0
D+
Wire Wire Line
	4800 4300 4800 4350
Wire Wire Line
	4800 4350 4950 4350
Wire Wire Line
	5100 4350 5100 4300
Wire Wire Line
	4950 4350 4950 4400
Connection ~ 4950 4350
Wire Wire Line
	4950 4350 5100 4350
$Comp
L power:GND #PWR038
U 1 1 604DF64D
P 4950 4400
F 0 "#PWR038" H 4950 4150 50  0001 C CNN
F 1 "GND" H 4955 4227 50  0000 C CNN
F 2 "" H 4950 4400 50  0001 C CNN
F 3 "" H 4950 4400 50  0001 C CNN
	1    4950 4400
	1    0    0    -1  
$EndComp
$EndSCHEMATC
