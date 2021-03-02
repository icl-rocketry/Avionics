EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 6
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
L RF_GPS:MAX-M8Q U6
U 1 1 5DAF40DC
P 6000 4050
F 0 "U6" H 6400 4700 50  0000 C CNN
F 1 "MAX-M8Q" H 6250 3400 50  0000 C CNN
F 2 "RF_GPS:ublox_MAX" H 6400 3400 50  0001 C CNN
F 3 "https://www.u-blox.com/sites/default/files/MAX-M8-FW3_DataSheet_%28UBX-15031506%29.pdf" H 6000 4050 50  0001 C CNN
	1    6000 4050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR039
U 1 1 5DAF558D
P 6000 4950
F 0 "#PWR039" H 6000 4700 50  0001 C CNN
F 1 "GND" H 6005 4777 50  0000 C CNN
F 2 "" H 6000 4950 50  0001 C CNN
F 3 "" H 6000 4950 50  0001 C CNN
	1    6000 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 4750 6000 4800
Text HLabel 5100 3950 0    50   Input ~ 0
SDA
Text HLabel 5100 4050 0    50   Input ~ 0
SCL
$Comp
L power:+3V3 #PWR038
U 1 1 5DAF8CAF
P 5800 3100
F 0 "#PWR038" H 5800 2950 50  0001 C CNN
F 1 "+3V3" H 5815 3273 50  0000 C CNN
F 2 "" H 5800 3100 50  0001 C CNN
F 3 "" H 5800 3100 50  0001 C CNN
	1    5800 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 3350 5800 3250
Wire Wire Line
	5800 3250 5900 3250
Wire Wire Line
	5900 3250 5900 3350
Connection ~ 5800 3250
Wire Wire Line
	5800 3250 5800 3100
Wire Wire Line
	5900 3250 6000 3250
Wire Wire Line
	6000 3250 6000 3350
Connection ~ 5900 3250
NoConn ~ 5400 4350
NoConn ~ 6600 3850
Wire Wire Line
	5100 4050 5400 4050
Wire Wire Line
	5100 3950 5400 3950
$Comp
L Device:R R25
U 1 1 5DB38D7E
P 6350 3250
F 0 "R25" V 6143 3250 50  0000 C CNN
F 1 "10" V 6234 3250 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6280 3250 50  0001 C CNN
F 3 "~" H 6350 3250 50  0001 C CNN
	1    6350 3250
	0    1    1    0   
$EndComp
Wire Wire Line
	6200 3250 6200 3350
$Comp
L Device:C C4
U 1 1 5DB393F9
P 6850 3400
F 0 "C4" H 6965 3446 50  0000 L CNN
F 1 "10nF" H 6965 3355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6888 3250 50  0001 C CNN
F 3 "~" H 6850 3400 50  0001 C CNN
	1    6850 3400
	1    0    0    -1  
$EndComp
$Comp
L pspice:INDUCTOR L1
U 1 1 5DB3AAFD
P 6650 2900
F 0 "L1" V 6696 2856 50  0000 R CNN
F 1 "27nH" V 6605 2856 50  0000 R CNN
F 2 "Inductor_SMD:L_0603_1608Metric" H 6650 2900 50  0001 C CNN
F 3 "~" H 6650 2900 50  0001 C CNN
	1    6650 2900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6500 3250 6650 3250
$Comp
L power:GND #PWR040
U 1 1 5DB39CF5
P 6850 3650
F 0 "#PWR040" H 6850 3400 50  0001 C CNN
F 1 "GND" H 6855 3477 50  0000 C CNN
F 2 "" H 6850 3650 50  0001 C CNN
F 3 "" H 6850 3650 50  0001 C CNN
	1    6850 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 3150 6650 3250
Connection ~ 6650 3250
Wire Wire Line
	6650 3250 6850 3250
Wire Wire Line
	6850 3550 6850 3650
Wire Wire Line
	6600 4050 7250 4050
$Comp
L power:+3V3 #PWR037
U 1 1 5DB3F51D
P 4700 4450
F 0 "#PWR037" H 4700 4300 50  0001 C CNN
F 1 "+3V3" H 4715 4623 50  0000 C CNN
F 2 "" H 4700 4450 50  0001 C CNN
F 3 "" H 4700 4450 50  0001 C CNN
	1    4700 4450
	1    0    0    -1  
$EndComp
NoConn ~ 6600 4350
NoConn ~ 5400 4250
Wire Wire Line
	6650 2650 6650 2600
Wire Wire Line
	6650 2600 7250 2600
Wire Wire Line
	7250 2600 7250 4050
Connection ~ 6650 2600
Wire Wire Line
	6650 2600 6650 2500
$Comp
L Connector:Conn_Coaxial J4
U 1 1 5DB44C2E
P 6650 2300
F 0 "J4" V 6887 2229 50  0000 C CNN
F 1 "Conn_Coaxial" V 6796 2229 50  0000 C CNN
F 2 "Connector_Coaxial:U.FL_Hirose_U.FL-R-SMT-1_Vertical" H 6650 2300 50  0001 C CNN
F 3 " ~" H 6650 2300 50  0001 C CNN
	1    6650 2300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6850 2300 7400 2300
Wire Wire Line
	7400 2300 7400 4800
Wire Wire Line
	7400 4800 6000 4800
Connection ~ 6000 4800
Wire Wire Line
	6000 4800 6000 4950
Text HLabel 5100 3650 0    50   Input ~ 0
RXD
Text HLabel 5100 3750 0    50   Input ~ 0
TXD
Wire Wire Line
	5400 3750 5100 3750
Wire Wire Line
	5100 3650 5400 3650
$Comp
L Device:R R24
U 1 1 5DB48655
P 5000 4450
F 0 "R24" V 5207 4450 50  0000 C CNN
F 1 "10K" V 5116 4450 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4930 4450 50  0001 C CNN
F 3 "~" H 5000 4450 50  0001 C CNN
	1    5000 4450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4850 4450 4700 4450
Wire Wire Line
	5150 4450 5400 4450
$EndSCHEMATC
