EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 7
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
L Device:R R42
U 1 1 6043A0B1
P 7950 3200
F 0 "R42" V 7743 3200 50  0000 C CNN
F 1 "5.1k" V 7834 3200 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7880 3200 50  0001 C CNN
F 3 "~" H 7950 3200 50  0001 C CNN
	1    7950 3200
	-1   0    0    1   
$EndComp
Wire Wire Line
	7950 2950 7950 3050
$Comp
L Device:R R41
U 1 1 6043AE1B
P 7450 3200
F 0 "R41" V 7657 3200 50  0000 C CNN
F 1 "5.1k" V 7566 3200 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7380 3200 50  0001 C CNN
F 3 "~" H 7450 3200 50  0001 C CNN
	1    7450 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 2950 7950 2950
Wire Wire Line
	5550 3050 7450 3050
$Comp
L power:GND #PWR01
U 1 1 60488D63
P 7450 3750
F 0 "#PWR01" H 7450 3500 50  0001 C CNN
F 1 "GND" H 7455 3577 50  0000 C CNN
F 2 "" H 7450 3750 50  0001 C CNN
F 3 "" H 7450 3750 50  0001 C CNN
	1    7450 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR055
U 1 1 60489008
P 7950 3750
F 0 "#PWR055" H 7950 3500 50  0001 C CNN
F 1 "GND" H 7955 3577 50  0000 C CNN
F 2 "" H 7950 3750 50  0001 C CNN
F 3 "" H 7950 3750 50  0001 C CNN
	1    7950 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 3350 7450 3450
Wire Wire Line
	7450 3650 7450 3750
Wire Wire Line
	7950 3750 7950 3650
$Comp
L Device:LED_Small D?
U 1 1 604A9E91
P 7950 3550
AR Path="/604A9E91" Ref="D?"  Part="1" 
AR Path="/602F075A/604A9E91" Ref="D17"  Part="1" 
F 0 "D17" H 7950 3345 50  0000 C CNN
F 1 "up_led" H 7950 3436 50  0000 C CNN
F 2 "LED_SMD:LED_0603_1608Metric_Castellated" V 7950 3550 50  0001 C CNN
F 3 "~" V 7950 3550 50  0001 C CNN
	1    7950 3550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7950 3450 7950 3350
$Comp
L Connector:USB_C_Receptacle J1
U 1 1 604B4652
P 4950 3750
F 0 "J1" H 5057 5017 50  0000 C CNN
F 1 "USB_C_Receptacle" H 5057 4926 50  0000 C CNN
F 2 "Connector_USB:USB_C_Receptacle_Amphenol_12401548E4-2A" H 5100 3750 50  0001 C CNN
F 3 "https://www.usb.org/sites/default/files/documents/usb_type-c.zip" H 5100 3750 50  0001 C CNN
	1    4950 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 5350 4950 5350
Wire Wire Line
	4950 5500 4950 5350
$Comp
L power:GND #PWR0110
U 1 1 60735E2B
P 4950 5500
F 0 "#PWR0110" H 4950 5250 50  0001 C CNN
F 1 "GND" H 4955 5327 50  0000 C CNN
F 2 "" H 4950 5500 50  0001 C CNN
F 3 "" H 4950 5500 50  0001 C CNN
	1    4950 5500
	1    0    0    -1  
$EndComp
Connection ~ 4950 5350
Wire Wire Line
	5550 2750 6250 2750
Text HLabel 6250 2750 2    50   Input ~ 0
usb_VBUS
Wire Wire Line
	6300 3550 5550 3550
Wire Wire Line
	5550 3250 6300 3250
Text HLabel 6300 3250 2    50   Input ~ 0
usb_D-
Text HLabel 6300 3550 2    50   Input ~ 0
usb_D+
Wire Wire Line
	5550 3350 5550 3250
Connection ~ 5550 3250
Wire Wire Line
	5550 3450 5550 3550
Connection ~ 5550 3550
NoConn ~ 5550 3750
NoConn ~ 5550 3850
NoConn ~ 5550 4050
NoConn ~ 5550 4150
NoConn ~ 5550 4350
NoConn ~ 5550 4450
NoConn ~ 5550 4650
NoConn ~ 5550 4750
NoConn ~ 5550 4950
NoConn ~ 5550 5050
NoConn ~ 300  2700
$Comp
L Device:LED_Small D?
U 1 1 6053D295
P 7450 3550
AR Path="/6053D295" Ref="D?"  Part="1" 
AR Path="/602F075A/6053D295" Ref="D13"  Part="1" 
F 0 "D13" H 7450 3345 50  0000 C CNN
F 1 "down_led" H 7450 3436 50  0000 C CNN
F 2 "LED_SMD:LED_0603_1608Metric_Castellated" V 7450 3550 50  0001 C CNN
F 3 "~" V 7450 3550 50  0001 C CNN
	1    7450 3550
	0    -1   -1   0   
$EndComp
$EndSCHEMATC
