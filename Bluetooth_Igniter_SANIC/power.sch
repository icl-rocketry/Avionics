EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 4
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
L Regulator_Switching:TPS63060 U5
U 1 1 603EAA6B
P 5200 3250
F 0 "U5" H 5200 3917 50  0000 C CNN
F 1 "TPS63060" H 5200 3826 50  0000 C CNN
F 2 "Package_SON:Texas_S-PWSON-N10_ThermalVias" H 5200 2600 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tps63060.pdf" H 5200 3250 50  0001 C CNN
	1    5200 3250
	1    0    0    -1  
$EndComp
$Comp
L pspice:INDUCTOR L1
U 1 1 603EB843
P 5200 2400
F 0 "L1" H 5200 2615 50  0000 C CNN
F 1 "1uH" H 5200 2524 50  0000 C CNN
F 2 "Inductor_SMD:L_Taiyo-Yuden_NR-30xx" H 5200 2400 50  0001 C CNN
F 3 "~" H 5200 2400 50  0001 C CNN
	1    5200 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 2850 5900 2850
Wire Wire Line
	5900 2850 5900 2400
Wire Wire Line
	5900 2400 5450 2400
Wire Wire Line
	4800 2850 4500 2850
Wire Wire Line
	4500 2850 4500 2400
Wire Wire Line
	4500 2400 4950 2400
Wire Wire Line
	4800 3050 4700 3050
Wire Wire Line
	5600 3050 5750 3050
Wire Wire Line
	4800 3150 4700 3150
Wire Wire Line
	4700 3150 4700 3050
Connection ~ 4700 3050
Wire Wire Line
	4700 3050 4200 3050
Wire Wire Line
	5600 3250 5750 3250
Wire Wire Line
	4800 3250 4500 3250
$Comp
L Device:R_Small R19
U 1 1 60424E9E
P 5750 3150
F 0 "R19" H 5809 3196 50  0000 L CNN
F 1 "620k" H 5809 3105 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5750 3150 50  0001 C CNN
F 3 "~" H 5750 3150 50  0001 C CNN
	1    5750 3150
	1    0    0    -1  
$EndComp
Connection ~ 5750 3050
Connection ~ 5750 3250
Wire Wire Line
	5750 3250 5850 3250
$Comp
L Device:R_Small R21
U 1 1 6042564C
P 6600 3300
F 0 "R21" H 6659 3346 50  0000 L CNN
F 1 "R_Small" H 6659 3255 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 6600 3300 50  0001 C CNN
F 3 "~" H 6600 3300 50  0001 C CNN
	1    6600 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 3200 6600 3050
$Comp
L Device:R_Small R20
U 1 1 604268C6
P 5850 3350
F 0 "R20" H 5909 3396 50  0000 L CNN
F 1 "111k" H 5909 3305 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5850 3350 50  0001 C CNN
F 3 "~" H 5850 3350 50  0001 C CNN
	1    5850 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 3550 5600 3850
Connection ~ 5850 3250
Wire Wire Line
	5850 3250 6100 3250
$Comp
L power:GND #PWR032
U 1 1 6042871B
P 5850 3450
F 0 "#PWR032" H 5850 3200 50  0001 C CNN
F 1 "GND" H 5855 3277 50  0000 C CNN
F 2 "" H 5850 3450 50  0001 C CNN
F 3 "" H 5850 3450 50  0001 C CNN
	1    5850 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C9
U 1 1 6042A643
P 6100 3550
F 0 "C9" H 6192 3596 50  0000 L CNN
F 1 "10pF" H 6192 3505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6100 3550 50  0001 C CNN
F 3 "~" H 6100 3550 50  0001 C CNN
	1    6100 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 3250 6100 3450
Connection ~ 6600 3050
Wire Wire Line
	5750 3050 6600 3050
$Comp
L power:GND #PWR033
U 1 1 6042C935
P 6100 3650
F 0 "#PWR033" H 6100 3400 50  0001 C CNN
F 1 "GND" H 6105 3477 50  0000 C CNN
F 2 "" H 6100 3650 50  0001 C CNN
F 3 "" H 6100 3650 50  0001 C CNN
	1    6100 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 3050 7150 3050
Wire Wire Line
	7150 3050 7150 3250
Connection ~ 7150 3050
$Comp
L Device:C_Small C10
U 1 1 6042D8E9
P 7150 3350
F 0 "C10" H 7242 3396 50  0000 L CNN
F 1 "22uF" H 7242 3305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7150 3350 50  0001 C CNN
F 3 "~" H 7150 3350 50  0001 C CNN
	1    7150 3350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR034
U 1 1 6042E1A4
P 7150 3450
F 0 "#PWR034" H 7150 3200 50  0001 C CNN
F 1 "GND" H 7155 3277 50  0000 C CNN
F 2 "" H 7150 3450 50  0001 C CNN
F 3 "" H 7150 3450 50  0001 C CNN
	1    7150 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 3850 6600 3850
Wire Wire Line
	6600 3400 6600 3850
Connection ~ 6600 3850
Wire Wire Line
	6600 3850 7050 3850
Wire Wire Line
	4800 3450 4800 3600
$Comp
L power:GND #PWR030
U 1 1 604347BE
P 4800 3600
F 0 "#PWR030" H 4800 3350 50  0001 C CNN
F 1 "GND" H 4805 3427 50  0000 C CNN
F 2 "" H 4800 3600 50  0001 C CNN
F 3 "" H 4800 3600 50  0001 C CNN
	1    4800 3600
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C8
U 1 1 60435134
P 4500 3350
F 0 "C8" H 4592 3396 50  0000 L CNN
F 1 "0.1uF" H 4592 3305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4500 3350 50  0001 C CNN
F 3 "~" H 4500 3350 50  0001 C CNN
	1    4500 3350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR029
U 1 1 60435A81
P 4500 3450
F 0 "#PWR029" H 4500 3200 50  0001 C CNN
F 1 "GND" H 4505 3277 50  0000 C CNN
F 2 "" H 4500 3450 50  0001 C CNN
F 3 "" H 4500 3450 50  0001 C CNN
	1    4500 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 3050 4200 3300
$Comp
L Device:C_Small C7
U 1 1 60436971
P 4200 3400
F 0 "C7" H 4292 3446 50  0000 L CNN
F 1 "10uF" H 4292 3355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4200 3400 50  0001 C CNN
F 3 "~" H 4200 3400 50  0001 C CNN
	1    4200 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 3500 4200 3650
$Comp
L power:GND #PWR027
U 1 1 604370BE
P 4200 3650
F 0 "#PWR027" H 4200 3400 50  0001 C CNN
F 1 "GND" H 4205 3477 50  0000 C CNN
F 2 "" H 4200 3650 50  0001 C CNN
F 3 "" H 4200 3650 50  0001 C CNN
	1    4200 3650
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR037
U 1 1 60439764
P 8200 3050
F 0 "#PWR037" H 8200 2900 50  0001 C CNN
F 1 "+3V3" H 8215 3223 50  0000 C CNN
F 2 "" H 8200 3050 50  0001 C CNN
F 3 "" H 8200 3050 50  0001 C CNN
	1    8200 3050
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J4
U 1 1 60437B13
P 2650 1850
F 0 "J4" H 2568 1525 50  0000 C CNN
F 1 "Screw_Terminal_01x02" H 2568 1616 50  0000 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_PT-1,5-2-5.0-H_1x02_P5.00mm_Horizontal" H 2650 1850 50  0001 C CNN
F 3 "~" H 2650 1850 50  0001 C CNN
	1    2650 1850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2850 1850 3600 1850
Wire Wire Line
	3600 1850 3600 1450
$Comp
L power:GND #PWR024
U 1 1 6043B88B
P 3600 1450
F 0 "#PWR024" H 3600 1200 50  0001 C CNN
F 1 "GND" H 3605 1277 50  0000 C CNN
F 2 "" H 3600 1450 50  0001 C CNN
F 3 "" H 3600 1450 50  0001 C CNN
	1    3600 1450
	-1   0    0    1   
$EndComp
Wire Wire Line
	2850 1950 4250 1950
Wire Wire Line
	5100 3850 5100 3900
Wire Wire Line
	5100 3900 5150 3900
Wire Wire Line
	5200 3900 5200 3850
$Comp
L power:GND #PWR031
U 1 1 60452734
P 5150 3900
F 0 "#PWR031" H 5150 3650 50  0001 C CNN
F 1 "GND" H 5155 3727 50  0000 C CNN
F 2 "" H 5150 3900 50  0001 C CNN
F 3 "" H 5150 3900 50  0001 C CNN
	1    5150 3900
	1    0    0    -1  
$EndComp
Connection ~ 5150 3900
Wire Wire Line
	5150 3900 5200 3900
Wire Wire Line
	7150 3050 7500 3050
$Comp
L Device:C_Small C11
U 1 1 60497866
P 7500 3350
F 0 "C11" H 7592 3396 50  0000 L CNN
F 1 "22uF" H 7592 3305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7500 3350 50  0001 C CNN
F 3 "~" H 7500 3350 50  0001 C CNN
	1    7500 3350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR035
U 1 1 6049786C
P 7500 3450
F 0 "#PWR035" H 7500 3200 50  0001 C CNN
F 1 "GND" H 7505 3277 50  0000 C CNN
F 2 "" H 7500 3450 50  0001 C CNN
F 3 "" H 7500 3450 50  0001 C CNN
	1    7500 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C12
U 1 1 60498C8D
P 7850 3350
F 0 "C12" H 7942 3396 50  0000 L CNN
F 1 "22uF" H 7942 3305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7850 3350 50  0001 C CNN
F 3 "~" H 7850 3350 50  0001 C CNN
	1    7850 3350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR036
U 1 1 60498C93
P 7850 3450
F 0 "#PWR036" H 7850 3200 50  0001 C CNN
F 1 "GND" H 7855 3277 50  0000 C CNN
F 2 "" H 7850 3450 50  0001 C CNN
F 3 "" H 7850 3450 50  0001 C CNN
	1    7850 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 3250 7500 3050
Connection ~ 7500 3050
Wire Wire Line
	7500 3050 7850 3050
Wire Wire Line
	7850 3250 7850 3050
Connection ~ 7850 3050
Wire Wire Line
	7850 3050 8200 3050
Wire Wire Line
	3850 3050 3600 3050
Wire Wire Line
	3850 3050 3850 3300
$Comp
L Device:C_Small C6
U 1 1 6049C9AB
P 3850 3400
F 0 "C6" H 3942 3446 50  0000 L CNN
F 1 "10uF" H 3942 3355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3850 3400 50  0001 C CNN
F 3 "~" H 3850 3400 50  0001 C CNN
	1    3850 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 3500 3850 3650
$Comp
L power:GND #PWR026
U 1 1 6049C9B2
P 3850 3650
F 0 "#PWR026" H 3850 3400 50  0001 C CNN
F 1 "GND" H 3855 3477 50  0000 C CNN
F 2 "" H 3850 3650 50  0001 C CNN
F 3 "" H 3850 3650 50  0001 C CNN
	1    3850 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 3050 3850 3050
Connection ~ 4200 3050
Connection ~ 3850 3050
$Comp
L power:VDD #PWR028
U 1 1 604AC6CE
P 4250 1950
F 0 "#PWR028" H 4250 1800 50  0001 C CNN
F 1 "VDD" H 4265 2123 50  0000 C CNN
F 2 "" H 4250 1950 50  0001 C CNN
F 3 "" H 4250 1950 50  0001 C CNN
	1    4250 1950
	-1   0    0    1   
$EndComp
$Comp
L power:VDD #PWR025
U 1 1 604AF69E
P 3600 3050
F 0 "#PWR025" H 3600 2900 50  0001 C CNN
F 1 "VDD" H 3615 3223 50  0000 C CNN
F 2 "" H 3600 3050 50  0001 C CNN
F 3 "" H 3600 3050 50  0001 C CNN
	1    3600 3050
	1    0    0    -1  
$EndComp
Text HLabel 7050 3850 2    50   Input ~ 0
PG
$Comp
L power:VDD #PWR0102
U 1 1 605331BA
P 7000 1100
F 0 "#PWR0102" H 7000 950 50  0001 C CNN
F 1 "VDD" H 7015 1273 50  0000 C CNN
F 2 "" H 7000 1100 50  0001 C CNN
F 3 "" H 7000 1100 50  0001 C CNN
	1    7000 1100
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 60534408
P 7000 1100
F 0 "#FLG0101" H 7000 1175 50  0001 C CNN
F 1 "PWR_FLAG" H 7000 1273 50  0000 C CNN
F 2 "" H 7000 1100 50  0001 C CNN
F 3 "~" H 7000 1100 50  0001 C CNN
	1    7000 1100
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 60538466
P 8050 1100
F 0 "#PWR0103" H 8050 850 50  0001 C CNN
F 1 "GND" H 8055 927 50  0000 C CNN
F 2 "" H 8050 1100 50  0001 C CNN
F 3 "" H 8050 1100 50  0001 C CNN
	1    8050 1100
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 60537967
P 8050 1100
F 0 "#FLG0103" H 8050 1175 50  0001 C CNN
F 1 "PWR_FLAG" H 8050 1273 50  0000 C CNN
F 2 "" H 8050 1100 50  0001 C CNN
F 3 "~" H 8050 1100 50  0001 C CNN
	1    8050 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 3050 3600 4400
Wire Wire Line
	3600 4400 5850 4400
Connection ~ 3600 3050
$Comp
L Connector:Conn_01x02_Male J10
U 1 1 60E29B82
P 5850 4600
F 0 "J10" V 6004 4412 50  0000 R CNN
F 1 "Conn_01x02_Male" V 5913 4412 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 5850 4600 50  0001 C CNN
F 3 "~" H 5850 4600 50  0001 C CNN
	1    5850 4600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8200 3050 8200 4400
Wire Wire Line
	8200 4400 5950 4400
Connection ~ 8200 3050
$EndSCHEMATC