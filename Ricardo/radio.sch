EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 6
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
L RF_Module:RFM95W-868S2 U11
U 1 1 5DE3478D
P 5650 3750
F 0 "U11" H 6050 4350 50  0000 C CNN
F 1 "RFM95W-868S2" H 6050 4250 50  0000 C CNN
F 2 "RF_Module:HOPERF_RFM9XW_SMD" H 2350 5400 50  0001 C CNN
F 3 "https://www.hoperf.com/data/upload/portal/20181127/5bfcbea20e9ef.pdf" H 2350 5400 50  0001 C CNN
	1    5650 3750
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_Coaxial J5
U 1 1 5DE351BF
P 6750 3450
F 0 "J5" H 6850 3425 50  0000 L CNN
F 1 "Conn_Coaxial" H 6850 3334 50  0000 L CNN
F 2 "Connector_Coaxial:SMA_Molex_73251-1153_EdgeMount_Horizontal" H 6750 3450 50  0001 C CNN
F 3 " ~" H 6750 3450 50  0001 C CNN
	1    6750 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 3450 6550 3450
$Comp
L power:GND #PWR0102
U 1 1 5DE36243
P 6750 3700
F 0 "#PWR0102" H 6750 3450 50  0001 C CNN
F 1 "GND" H 6755 3527 50  0000 C CNN
F 2 "" H 6750 3700 50  0001 C CNN
F 3 "" H 6750 3700 50  0001 C CNN
	1    6750 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 3700 6750 3650
$Comp
L power:GND #PWR0103
U 1 1 5DE37EF9
P 5750 4450
F 0 "#PWR0103" H 5750 4200 50  0001 C CNN
F 1 "GND" H 5755 4277 50  0000 C CNN
F 2 "" H 5750 4450 50  0001 C CNN
F 3 "" H 5750 4450 50  0001 C CNN
	1    5750 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 4450 5750 4400
Wire Wire Line
	5550 4350 5550 4400
Wire Wire Line
	5550 4400 5650 4400
Connection ~ 5750 4400
Wire Wire Line
	5750 4400 5750 4350
Wire Wire Line
	5650 4350 5650 4400
Connection ~ 5650 4400
Wire Wire Line
	5650 4400 5750 4400
$Comp
L power:+3V3 #PWR0104
U 1 1 5DE3966F
P 5650 3200
F 0 "#PWR0104" H 5650 3050 50  0001 C CNN
F 1 "+3V3" H 5665 3373 50  0000 C CNN
F 2 "" H 5650 3200 50  0001 C CNN
F 3 "" H 5650 3200 50  0001 C CNN
	1    5650 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 3200 5650 3250
NoConn ~ 6150 3650
NoConn ~ 6150 3750
NoConn ~ 6150 3850
NoConn ~ 6150 3950
NoConn ~ 6150 4050
Text HLabel 5150 3450 0    50   Input ~ 0
SCLK
Text HLabel 5150 3550 0    50   Input ~ 0
MOSI
Text HLabel 5150 3650 0    50   Output ~ 0
MISO
Text HLabel 5150 3750 0    50   Input ~ 0
~CS
Text HLabel 5150 3950 0    50   Input ~ 0
RESET
Text HLabel 6150 4150 2    50   Output ~ 0
~INT
$EndSCHEMATC
