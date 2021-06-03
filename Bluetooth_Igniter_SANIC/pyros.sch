EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 4
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
L Connector:Screw_Terminal_01x02 J1
U 1 1 603A42ED
P 2900 3550
F 0 "J1" V 2772 3630 50  0000 L CNN
F 1 "Screw_Terminal_01x02" V 2863 3630 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_PT-1,5-2-5.0-H_1x02_P5.00mm_Horizontal" H 2900 3550 50  0001 C CNN
F 3 "~" H 2900 3550 50  0001 C CNN
	1    2900 3550
	0    1    1    0   
$EndComp
Wire Wire Line
	2800 3350 2800 3150
Wire Wire Line
	2800 3150 2500 3150
Wire Wire Line
	2500 3150 2500 3550
Wire Wire Line
	2500 3550 2200 3550
Wire Wire Line
	2200 3550 2200 3650
$Comp
L Device:LED D4
U 1 1 603A73D6
P 2200 3800
F 0 "D4" V 2239 3682 50  0000 R CNN
F 1 "LED" V 2148 3682 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Castellated" H 2200 3800 50  0001 C CNN
F 3 "~" H 2200 3800 50  0001 C CNN
	1    2200 3800
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R11
U 1 1 603A9084
P 2200 4100
F 0 "R11" H 2270 4146 50  0000 L CNN
F 1 "1k" H 2270 4055 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 2130 4100 50  0001 C CNN
F 3 "~" H 2200 4100 50  0001 C CNN
	1    2200 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 3350 2900 3150
Wire Wire Line
	2900 3150 3350 3150
$Comp
L Device:D_Schottky D5
U 1 1 603AA2C9
P 3350 3950
F 0 "D5" V 3396 3870 50  0000 R CNN
F 1 "D_Schottky" V 3305 3870 50  0000 R CNN
F 2 "Diode_SMD:D_SOD-123F" H 3350 3950 50  0001 C CNN
F 3 "~" H 3350 3950 50  0001 C CNN
	1    3350 3950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2200 4250 3350 4250
Wire Wire Line
	3350 4100 3350 4250
$Comp
L Device:Q_NMOS_GSD Q3
U 1 1 603AC2C0
P 3250 4550
F 0 "Q3" H 3454 4596 50  0000 L CNN
F 1 "Q_NMOS_GSD" H 3454 4505 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3450 4650 50  0001 C CNN
F 3 "~" H 3250 4550 50  0001 C CNN
	1    3250 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 4750 3350 5000
$Comp
L power:GND #PWR016
U 1 1 603AD57D
P 3350 5000
F 0 "#PWR016" H 3350 4750 50  0001 C CNN
F 1 "GND" H 3355 4827 50  0000 C CNN
F 2 "" H 3350 5000 50  0001 C CNN
F 3 "" H 3350 5000 50  0001 C CNN
	1    3350 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 4550 3050 4550
Wire Wire Line
	4750 3600 5250 3600
Wire Wire Line
	4750 3600 4750 4000
Wire Wire Line
	4700 4200 4700 4400
$Comp
L Device:R R13
U 1 1 603BD235
P 4700 4550
F 0 "R13" H 4770 4596 50  0000 L CNN
F 1 "500" H 4770 4505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4630 4550 50  0001 C CNN
F 3 "~" H 4700 4550 50  0001 C CNN
	1    4700 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 4700 4700 4850
Wire Wire Line
	4600 4200 4700 4200
Wire Wire Line
	4750 4000 4600 4000
Wire Wire Line
	4000 4850 4000 4700
Text HLabel 3750 4200 0    50   Input ~ 0
Cont1
Wire Wire Line
	4000 4200 3750 4200
$Comp
L Device:R R12
U 1 1 603C08B5
P 4000 4550
F 0 "R12" H 4070 4596 50  0000 L CNN
F 1 "500" H 4070 4505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3930 4550 50  0001 C CNN
F 3 "~" H 4000 4550 50  0001 C CNN
	1    4000 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 4400 4000 4200
Wire Wire Line
	4700 4850 4350 4850
Connection ~ 4350 4850
Wire Wire Line
	4350 4850 4000 4850
Wire Wire Line
	4350 4850 4350 5000
$Comp
L power:GND #PWR018
U 1 1 603C2975
P 4350 5000
F 0 "#PWR018" H 4350 4750 50  0001 C CNN
F 1 "GND" H 4355 4827 50  0000 C CNN
F 2 "" H 4350 5000 50  0001 C CNN
F 3 "" H 4350 5000 50  0001 C CNN
	1    4350 5000
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D6
U 1 1 603C55BC
P 5250 3750
F 0 "D6" V 5289 3632 50  0000 R CNN
F 1 "LED" V 5198 3632 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Castellated" H 5250 3750 50  0001 C CNN
F 3 "~" H 5250 3750 50  0001 C CNN
	1    5250 3750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5250 3900 5250 4000
$Comp
L Device:R R14
U 1 1 603C7858
P 5250 4150
F 0 "R14" H 5320 4196 50  0000 L CNN
F 1 "1k" H 5320 4105 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5180 4150 50  0001 C CNN
F 3 "~" H 5250 4150 50  0001 C CNN
	1    5250 4150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR019
U 1 1 603C83CD
P 5250 4400
F 0 "#PWR019" H 5250 4150 50  0001 C CNN
F 1 "GND" H 5255 4227 50  0000 C CNN
F 2 "" H 5250 4400 50  0001 C CNN
F 3 "" H 5250 4400 50  0001 C CNN
	1    5250 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 4300 5250 4400
$Comp
L power:+3V3 #PWR017
U 1 1 603C95D6
P 3950 3950
F 0 "#PWR017" H 3950 3800 50  0001 C CNN
F 1 "+3V3" H 3965 4123 50  0000 C CNN
F 2 "" H 3950 3950 50  0001 C CNN
F 3 "" H 3950 3950 50  0001 C CNN
	1    3950 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 4000 3950 4000
Wire Wire Line
	3950 4000 3950 3950
Connection ~ 3350 4250
Wire Wire Line
	3350 4250 3350 4350
Wire Wire Line
	3350 3150 3350 3600
Connection ~ 4750 3600
Wire Wire Line
	4750 3600 3350 3600
Connection ~ 3350 3600
Wire Wire Line
	3350 3600 3350 3800
$Comp
L Connector:Screw_Terminal_01x02 J3
U 1 1 603DB3E4
P 7550 3600
F 0 "J3" V 7422 3680 50  0000 L CNN
F 1 "Screw_Terminal_01x02" V 7513 3680 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_PT-1,5-2-5.0-H_1x02_P5.00mm_Horizontal" H 7550 3600 50  0001 C CNN
F 3 "~" H 7550 3600 50  0001 C CNN
	1    7550 3600
	0    1    1    0   
$EndComp
Wire Wire Line
	7450 3400 7450 3200
Wire Wire Line
	7450 3200 7150 3200
Wire Wire Line
	7150 3200 7150 3600
Wire Wire Line
	7150 3600 6850 3600
Wire Wire Line
	6850 3600 6850 3700
$Comp
L Device:LED D7
U 1 1 603DB539
P 6850 3850
F 0 "D7" V 6889 3732 50  0000 R CNN
F 1 "LED" V 6798 3732 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Castellated" H 6850 3850 50  0001 C CNN
F 3 "~" H 6850 3850 50  0001 C CNN
	1    6850 3850
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R15
U 1 1 603DB543
P 6850 4150
F 0 "R15" H 6920 4196 50  0000 L CNN
F 1 "1k" H 6920 4105 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6780 4150 50  0001 C CNN
F 3 "~" H 6850 4150 50  0001 C CNN
	1    6850 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 3400 7550 3200
Wire Wire Line
	7550 3200 8000 3200
$Comp
L Device:D_Schottky D8
U 1 1 603DB54F
P 8000 4000
F 0 "D8" V 8046 3920 50  0000 R CNN
F 1 "D_Schottky" V 7955 3920 50  0000 R CNN
F 2 "Diode_SMD:D_SOD-123F" H 8000 4000 50  0001 C CNN
F 3 "~" H 8000 4000 50  0001 C CNN
	1    8000 4000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6850 4300 8000 4300
Wire Wire Line
	8000 4150 8000 4300
$Comp
L Device:Q_NMOS_GSD Q4
U 1 1 603DB55B
P 7900 4600
F 0 "Q4" H 8104 4646 50  0000 L CNN
F 1 "Q_NMOS_GSD" H 8104 4555 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 8100 4700 50  0001 C CNN
F 3 "~" H 7900 4600 50  0001 C CNN
	1    7900 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 4800 8000 5050
$Comp
L power:GND #PWR020
U 1 1 603DB566
P 8000 5050
F 0 "#PWR020" H 8000 4800 50  0001 C CNN
F 1 "GND" H 8005 4877 50  0000 C CNN
F 2 "" H 8000 5050 50  0001 C CNN
F 3 "" H 8000 5050 50  0001 C CNN
	1    8000 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 4600 7700 4600
Text HLabel 7300 4600 0    50   Input ~ 0
Nuke2
Wire Wire Line
	9400 3650 9900 3650
Wire Wire Line
	9400 3650 9400 4050
Wire Wire Line
	9350 4250 9350 4450
$Comp
L Device:R R17
U 1 1 603DB575
P 9350 4600
F 0 "R17" H 9420 4646 50  0000 L CNN
F 1 "500" H 9420 4555 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9280 4600 50  0001 C CNN
F 3 "~" H 9350 4600 50  0001 C CNN
	1    9350 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 4750 9350 4900
Wire Wire Line
	9250 4250 9350 4250
Wire Wire Line
	9400 4050 9250 4050
Wire Wire Line
	8650 4900 8650 4750
Text HLabel 8400 4250 0    50   Input ~ 0
Cont2
Wire Wire Line
	8650 4250 8400 4250
$Comp
L Device:R R16
U 1 1 603DB590
P 8650 4600
F 0 "R16" H 8720 4646 50  0000 L CNN
F 1 "500" H 8720 4555 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8580 4600 50  0001 C CNN
F 3 "~" H 8650 4600 50  0001 C CNN
	1    8650 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 4450 8650 4250
Wire Wire Line
	9350 4900 9000 4900
Connection ~ 9000 4900
Wire Wire Line
	9000 4900 8650 4900
Wire Wire Line
	9000 4900 9000 5050
$Comp
L power:GND #PWR022
U 1 1 603DB59F
P 9000 5050
F 0 "#PWR022" H 9000 4800 50  0001 C CNN
F 1 "GND" H 9005 4877 50  0000 C CNN
F 2 "" H 9000 5050 50  0001 C CNN
F 3 "" H 9000 5050 50  0001 C CNN
	1    9000 5050
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D9
U 1 1 603DB5A9
P 9900 3800
F 0 "D9" V 9939 3682 50  0000 R CNN
F 1 "LED" V 9848 3682 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Castellated" H 9900 3800 50  0001 C CNN
F 3 "~" H 9900 3800 50  0001 C CNN
	1    9900 3800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9900 3950 9900 4050
$Comp
L Device:R R18
U 1 1 603DB5B4
P 9900 4200
F 0 "R18" H 9970 4246 50  0000 L CNN
F 1 "1k" H 9970 4155 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9830 4200 50  0001 C CNN
F 3 "~" H 9900 4200 50  0001 C CNN
	1    9900 4200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR023
U 1 1 603DB5BE
P 9900 4450
F 0 "#PWR023" H 9900 4200 50  0001 C CNN
F 1 "GND" H 9905 4277 50  0000 C CNN
F 2 "" H 9900 4450 50  0001 C CNN
F 3 "" H 9900 4450 50  0001 C CNN
	1    9900 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 4350 9900 4450
$Comp
L power:+3V3 #PWR021
U 1 1 603DB5C9
P 8600 4000
F 0 "#PWR021" H 8600 3850 50  0001 C CNN
F 1 "+3V3" H 8615 4173 50  0000 C CNN
F 2 "" H 8600 4000 50  0001 C CNN
F 3 "" H 8600 4000 50  0001 C CNN
	1    8600 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 4050 8600 4050
Wire Wire Line
	8600 4050 8600 4000
Connection ~ 8000 4300
Wire Wire Line
	8000 4300 8000 4400
Wire Wire Line
	8000 3200 8000 3650
Connection ~ 9400 3650
Wire Wire Line
	9400 3650 8000 3650
Connection ~ 8000 3650
Wire Wire Line
	8000 3650 8000 3850
Wire Wire Line
	2200 3550 1500 3550
Connection ~ 2200 3550
Wire Wire Line
	6850 3600 6300 3600
Connection ~ 6850 3600
$Comp
L Connector:Screw_Terminal_01x02 J2
U 1 1 6047AC74
P 3400 1700
F 0 "J2" H 3480 1692 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 3480 1601 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_PT-1,5-2-5.0-H_1x02_P5.00mm_Horizontal" H 3400 1700 50  0001 C CNN
F 3 "~" H 3400 1700 50  0001 C CNN
	1    3400 1700
	1    0    0    -1  
$EndComp
Text Label 2900 1800 2    50   ~ 0
Ext_supply
Text Label 1500 3550 2    50   ~ 0
Ext_supply
Text Label 6300 3600 2    50   ~ 0
Ext_supply
Text HLabel 2650 4550 0    50   Input ~ 0
Nuke1
Wire Wire Line
	3200 1800 2900 1800
Wire Wire Line
	2250 1700 2250 1800
Wire Wire Line
	2250 1700 3200 1700
$Comp
L power:GND #PWR0101
U 1 1 60A35AE3
P 2250 1800
F 0 "#PWR0101" H 2250 1550 50  0001 C CNN
F 1 "GND" H 2255 1627 50  0000 C CNN
F 2 "" H 2250 1800 50  0001 C CNN
F 3 "" H 2250 1800 50  0001 C CNN
	1    2250 1800
	1    0    0    -1  
$EndComp
$Comp
L Isolator:LTV-827S U3
U 1 1 60B943F7
P 4300 4100
F 0 "U3" H 4300 4425 50  0000 C CNN
F 1 "LTV-827S" H 4300 4334 50  0000 C CNN
F 2 "Package_DIP:SMDIP-8_W9.53mm" H 4300 3800 50  0001 C CNN
F 3 "http://www.us.liteon.com/downloads/LTV-817-827-847.PDF" H 3500 4650 50  0001 C CNN
	1    4300 4100
	1    0    0    -1  
$EndComp
$Comp
L Isolator:LTV-827S U3
U 2 1 60B94E21
P 8950 4150
F 0 "U3" H 8950 4475 50  0000 C CNN
F 1 "LTV-827S" H 8950 4384 50  0000 C CNN
F 2 "Package_DIP:SMDIP-8_W9.53mm" H 8950 3850 50  0001 C CNN
F 3 "http://www.us.liteon.com/downloads/LTV-817-827-847.PDF" H 8150 4700 50  0001 C CNN
	2    8950 4150
	1    0    0    -1  
$EndComp
Connection ~ 4000 4200
Connection ~ 8650 4250
$EndSCHEMATC
