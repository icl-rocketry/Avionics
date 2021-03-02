EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 6
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
L power:GND #PWR066
U 1 1 5DC33EC0
P 10150 6800
F 0 "#PWR066" H 10150 6550 50  0001 C CNN
F 1 "GND" H 10155 6627 50  0000 C CNN
F 2 "" H 10150 6800 50  0001 C CNN
F 3 "" H 10150 6800 50  0001 C CNN
	1    10150 6800
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 5DE04AED
P 4750 2700
AR Path="/5DDE6CFA/5DE04AED" Ref="J?"  Part="1" 
AR Path="/5DDE1103/5DE04AED" Ref="J7"  Part="1" 
F 0 "J7" H 4668 2375 50  0000 C CNN
F 1 "Screw_Terminal_01x02" H 4668 2466 50  0000 C CNN
F 2 "iclr-hw:PHOENIX_1757242" H 4750 2700 50  0001 C CNN
F 3 "~" H 4750 2700 50  0001 C CNN
	1    4750 2700
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 5DE04AFB
P 8000 4000
AR Path="/5DDE6CFA/5DE04AFB" Ref="J?"  Part="1" 
AR Path="/5DDE1103/5DE04AFB" Ref="J8"  Part="1" 
F 0 "J8" V 7964 3812 50  0000 R CNN
F 1 "Screw_Terminal_01x02" V 7873 3812 50  0000 R CNN
F 2 "iclr-hw:PHOENIX_1757242" H 8000 4000 50  0001 C CNN
F 3 "~" H 8000 4000 50  0001 C CNN
	1    8000 4000
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5DE04B01
P 7350 4550
AR Path="/5DDE6CFA/5DE04B01" Ref="R?"  Part="1" 
AR Path="/5DDE1103/5DE04B01" Ref="R32"  Part="1" 
F 0 "R32" H 7420 4596 50  0000 L CNN
F 1 "1K" H 7420 4505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7280 4550 50  0001 C CNN
F 3 "~" H 7350 4550 50  0001 C CNN
	1    7350 4550
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D?
U 1 1 5DE04B07
P 7350 4250
AR Path="/5DDE6CFA/5DE04B07" Ref="D?"  Part="1" 
AR Path="/5DDE1103/5DE04B07" Ref="D14"  Part="1" 
F 0 "D14" V 7389 4133 50  0000 R CNN
F 1 "FIRING" V 7298 4133 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Castellated" H 7350 4250 50  0001 C CNN
F 3 "~" H 7350 4250 50  0001 C CNN
	1    7350 4250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7350 4100 7800 4100
$Comp
L Device:Q_NMOS_GSD Q?
U 1 1 5DE04B13
P 8000 5150
AR Path="/5DDE6CFA/5DE04B13" Ref="Q?"  Part="1" 
AR Path="/5DDE1103/5DE04B13" Ref="Q5"  Part="1" 
F 0 "Q5" H 8206 5196 50  0000 L CNN
F 1 "PMV20ENR" H 8206 5105 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 8200 5250 50  0001 C CNN
F 3 "~" H 8000 5150 50  0001 C CNN
	1    8000 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 5150 7600 5150
Wire Wire Line
	8100 5350 8100 5550
$Comp
L power:GND #PWR?
U 1 1 5DE04B1B
P 8100 5550
AR Path="/5DDE6CFA/5DE04B1B" Ref="#PWR?"  Part="1" 
AR Path="/5DDE1103/5DE04B1B" Ref="#PWR063"  Part="1" 
F 0 "#PWR063" H 8100 5300 50  0001 C CNN
F 1 "GND" H 8105 5377 50  0000 C CNN
F 2 "" H 8100 5550 50  0001 C CNN
F 3 "" H 8100 5550 50  0001 C CNN
	1    8100 5550
	1    0    0    -1  
$EndComp
Connection ~ 8100 4250
Wire Wire Line
	8100 4250 8100 4300
$Comp
L Isolator:LTV-817S U?
U 1 1 5DE04B23
P 8950 4800
AR Path="/5DDE6CFA/5DE04B23" Ref="U?"  Part="1" 
AR Path="/5DDE1103/5DE04B23" Ref="U10"  Part="1" 
F 0 "U10" H 8950 5125 50  0000 C CNN
F 1 "LTV-817S" H 8950 5034 50  0000 C CNN
F 2 "Package_DIP:SMDIP-4_W9.53mm" H 8950 4500 50  0001 C CNN
F 3 "http://www.us.liteon.com/downloads/LTV-817-827-847.PDF" H 8600 5100 50  0001 C CNN
	1    8950 4800
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9250 4250 9250 4700
Wire Wire Line
	8100 4250 9250 4250
$Comp
L Device:R R?
U 1 1 5DE04B2B
P 9250 5150
AR Path="/5DDE6CFA/5DE04B2B" Ref="R?"  Part="1" 
AR Path="/5DDE1103/5DE04B2B" Ref="R34"  Part="1" 
F 0 "R34" H 9320 5196 50  0000 L CNN
F 1 "500" H 9320 5105 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9180 5150 50  0001 C CNN
F 3 "~" H 9250 5150 50  0001 C CNN
	1    9250 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 5350 8950 5350
Wire Wire Line
	8950 5350 8950 5400
$Comp
L power:GND #PWR?
U 1 1 5DE04B33
P 8950 5400
AR Path="/5DDE6CFA/5DE04B33" Ref="#PWR?"  Part="1" 
AR Path="/5DDE1103/5DE04B33" Ref="#PWR065"  Part="1" 
F 0 "#PWR065" H 8950 5150 50  0001 C CNN
F 1 "GND" H 8955 5227 50  0000 C CNN
F 2 "" H 8950 5400 50  0001 C CNN
F 3 "" H 8950 5400 50  0001 C CNN
	1    8950 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5DE04B39
P 8650 5150
AR Path="/5DDE6CFA/5DE04B39" Ref="R?"  Part="1" 
AR Path="/5DDE1103/5DE04B39" Ref="R33"  Part="1" 
F 0 "R33" H 8720 5196 50  0000 L CNN
F 1 "500" H 8720 5105 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8580 5150 50  0001 C CNN
F 3 "~" H 8650 5150 50  0001 C CNN
	1    8650 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 5350 8950 5350
Connection ~ 8950 5350
Wire Wire Line
	8650 4900 8400 4900
Text Notes 4300 2600 0    50   ~ 0
External ematch supply
$Comp
L Device:D_Schottky D?
U 1 1 5DE04B52
P 8100 4450
AR Path="/5DAD2180/5DE04B52" Ref="D?"  Part="1" 
AR Path="/5DB4FA2A/5DE04B52" Ref="D?"  Part="1" 
AR Path="/5DDE6CFA/5DE04B52" Ref="D?"  Part="1" 
AR Path="/5DDE1103/5DE04B52" Ref="D15"  Part="1" 
F 0 "D15" V 8146 4371 50  0000 R CNN
F 1 "D_Schottky" V 8055 4371 50  0000 R CNN
F 2 "Diode_SMD:D_SOD-123F" H 8100 4450 50  0001 C CNN
F 3 "http://www.smc-diodes.com/propdf/DSS12U%20THRU%20DSS125U%20N1873%20REV.A.pdf" H 8100 4450 50  0001 C CNN
	1    8100 4450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8650 5300 8650 5350
Wire Wire Line
	9250 5300 9250 5350
Wire Wire Line
	9250 5000 9250 4900
Wire Wire Line
	8650 5000 8650 4900
Connection ~ 8650 4900
$Comp
L Device:R R?
U 1 1 5DE04B5E
P 9650 4750
AR Path="/5DDE6CFA/5DE04B5E" Ref="R?"  Part="1" 
AR Path="/5DDE1103/5DE04B5E" Ref="R35"  Part="1" 
F 0 "R35" H 9720 4796 50  0000 L CNN
F 1 "1K" H 9720 4705 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9580 4750 50  0001 C CNN
F 3 "~" H 9650 4750 50  0001 C CNN
	1    9650 4750
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D?
U 1 1 5DE04B64
P 9650 4450
AR Path="/5DDE6CFA/5DE04B64" Ref="D?"  Part="1" 
AR Path="/5DDE1103/5DE04B64" Ref="D16"  Part="1" 
F 0 "D16" V 9689 4333 50  0000 R CNN
F 1 "CONTINUITY" V 9598 4333 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Castellated" H 9650 4450 50  0001 C CNN
F 3 "~" H 9650 4450 50  0001 C CNN
	1    9650 4450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9250 4250 9650 4250
Wire Wire Line
	9650 4250 9650 4300
Connection ~ 9250 4250
$Comp
L power:GND #PWR?
U 1 1 5DE04B6D
P 9650 4950
AR Path="/5DDE6CFA/5DE04B6D" Ref="#PWR?"  Part="1" 
AR Path="/5DDE1103/5DE04B6D" Ref="#PWR067"  Part="1" 
F 0 "#PWR067" H 9650 4700 50  0001 C CNN
F 1 "GND" H 9655 4777 50  0000 C CNN
F 2 "" H 9650 4950 50  0001 C CNN
F 3 "" H 9650 4950 50  0001 C CNN
	1    9650 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 4950 9650 4900
$Comp
L power:+3V3 #PWR?
U 1 1 5DE04B7A
P 8650 4700
AR Path="/5DDE6CFA/5DE04B7A" Ref="#PWR?"  Part="1" 
AR Path="/5DDE1103/5DE04B7A" Ref="#PWR064"  Part="1" 
F 0 "#PWR064" H 8650 4550 50  0001 C CNN
F 1 "+3V3" H 8665 4873 50  0000 C CNN
F 2 "" H 8650 4700 50  0001 C CNN
F 3 "" H 8650 4700 50  0001 C CNN
	1    8650 4700
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 5DE32338
P 3450 4050
AR Path="/5DDE6CFA/5DE32338" Ref="J?"  Part="1" 
AR Path="/5DDE1103/5DE32338" Ref="J6"  Part="1" 
F 0 "J6" V 3414 3862 50  0000 R CNN
F 1 "Screw_Terminal_01x02" V 3323 3862 50  0000 R CNN
F 2 "iclr-hw:PHOENIX_1757242" H 3450 4050 50  0001 C CNN
F 3 "~" H 3450 4050 50  0001 C CNN
	1    3450 4050
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5DE3233E
P 2800 4600
AR Path="/5DDE6CFA/5DE3233E" Ref="R?"  Part="1" 
AR Path="/5DDE1103/5DE3233E" Ref="R27"  Part="1" 
F 0 "R27" H 2870 4646 50  0000 L CNN
F 1 "1K" H 2870 4555 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 2730 4600 50  0001 C CNN
F 3 "~" H 2800 4600 50  0001 C CNN
	1    2800 4600
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D?
U 1 1 5DE32344
P 2800 4300
AR Path="/5DDE6CFA/5DE32344" Ref="D?"  Part="1" 
AR Path="/5DDE1103/5DE32344" Ref="D10"  Part="1" 
F 0 "D10" V 2839 4183 50  0000 R CNN
F 1 "FIRING" V 2748 4183 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Castellated" H 2800 4300 50  0001 C CNN
F 3 "~" H 2800 4300 50  0001 C CNN
	1    2800 4300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2800 4150 3250 4150
$Comp
L Device:Q_NMOS_GSD Q?
U 1 1 5DE32350
P 3500 5200
AR Path="/5DDE6CFA/5DE32350" Ref="Q?"  Part="1" 
AR Path="/5DDE1103/5DE32350" Ref="Q4"  Part="1" 
F 0 "Q4" H 3706 5246 50  0000 L CNN
F 1 "PMV20ENR" H 3706 5155 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3700 5300 50  0001 C CNN
F 3 "~" H 3500 5200 50  0001 C CNN
	1    3500 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 5200 3100 5200
Wire Wire Line
	3600 5400 3600 5600
$Comp
L power:GND #PWR?
U 1 1 5DE32358
P 3600 5600
AR Path="/5DDE6CFA/5DE32358" Ref="#PWR?"  Part="1" 
AR Path="/5DDE1103/5DE32358" Ref="#PWR056"  Part="1" 
F 0 "#PWR056" H 3600 5350 50  0001 C CNN
F 1 "GND" H 3605 5427 50  0000 C CNN
F 2 "" H 3600 5600 50  0001 C CNN
F 3 "" H 3600 5600 50  0001 C CNN
	1    3600 5600
	1    0    0    -1  
$EndComp
Connection ~ 3600 4300
Wire Wire Line
	3600 4300 3600 4350
$Comp
L Isolator:LTV-817S U?
U 1 1 5DE32360
P 4450 4850
AR Path="/5DDE6CFA/5DE32360" Ref="U?"  Part="1" 
AR Path="/5DDE1103/5DE32360" Ref="U9"  Part="1" 
F 0 "U9" H 4450 5175 50  0000 C CNN
F 1 "LTV-817S" H 4450 5084 50  0000 C CNN
F 2 "Package_DIP:SMDIP-4_W9.53mm" H 4450 4550 50  0001 C CNN
F 3 "http://www.us.liteon.com/downloads/LTV-817-827-847.PDF" H 4100 5150 50  0001 C CNN
	1    4450 4850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4750 4300 4750 4750
Wire Wire Line
	3600 4300 4750 4300
$Comp
L Device:R R?
U 1 1 5DE32368
P 4750 5200
AR Path="/5DDE6CFA/5DE32368" Ref="R?"  Part="1" 
AR Path="/5DDE1103/5DE32368" Ref="R29"  Part="1" 
F 0 "R29" H 4820 5246 50  0000 L CNN
F 1 "500" H 4820 5155 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4680 5200 50  0001 C CNN
F 3 "~" H 4750 5200 50  0001 C CNN
	1    4750 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 5400 4450 5400
Wire Wire Line
	4450 5400 4450 5450
$Comp
L power:GND #PWR?
U 1 1 5DE32370
P 4450 5450
AR Path="/5DDE6CFA/5DE32370" Ref="#PWR?"  Part="1" 
AR Path="/5DDE1103/5DE32370" Ref="#PWR058"  Part="1" 
F 0 "#PWR058" H 4450 5200 50  0001 C CNN
F 1 "GND" H 4455 5277 50  0000 C CNN
F 2 "" H 4450 5450 50  0001 C CNN
F 3 "" H 4450 5450 50  0001 C CNN
	1    4450 5450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5DE32376
P 4150 5200
AR Path="/5DDE6CFA/5DE32376" Ref="R?"  Part="1" 
AR Path="/5DDE1103/5DE32376" Ref="R28"  Part="1" 
F 0 "R28" H 4220 5246 50  0000 L CNN
F 1 "500" H 4220 5155 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4080 5200 50  0001 C CNN
F 3 "~" H 4150 5200 50  0001 C CNN
	1    4150 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 5400 4450 5400
Connection ~ 4450 5400
Wire Wire Line
	4150 4950 3900 4950
$Comp
L Device:D_Schottky D?
U 1 1 5DE3238F
P 3600 4500
AR Path="/5DAD2180/5DE3238F" Ref="D?"  Part="1" 
AR Path="/5DB4FA2A/5DE3238F" Ref="D?"  Part="1" 
AR Path="/5DDE6CFA/5DE3238F" Ref="D?"  Part="1" 
AR Path="/5DDE1103/5DE3238F" Ref="D11"  Part="1" 
F 0 "D11" V 3646 4421 50  0000 R CNN
F 1 "D_Schottky" V 3555 4421 50  0000 R CNN
F 2 "Diode_SMD:D_SOD-123F" H 3600 4500 50  0001 C CNN
F 3 "http://www.smc-diodes.com/propdf/DSS12U%20THRU%20DSS125U%20N1873%20REV.A.pdf" H 3600 4500 50  0001 C CNN
	1    3600 4500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4150 5350 4150 5400
Wire Wire Line
	4750 5350 4750 5400
Wire Wire Line
	4750 5050 4750 4950
Wire Wire Line
	4150 5050 4150 4950
Connection ~ 4150 4950
$Comp
L Device:R R?
U 1 1 5DE3239B
P 5150 4800
AR Path="/5DDE6CFA/5DE3239B" Ref="R?"  Part="1" 
AR Path="/5DDE1103/5DE3239B" Ref="R30"  Part="1" 
F 0 "R30" H 5220 4846 50  0000 L CNN
F 1 "1K" H 5220 4755 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5080 4800 50  0001 C CNN
F 3 "~" H 5150 4800 50  0001 C CNN
	1    5150 4800
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D?
U 1 1 5DE323A1
P 5150 4500
AR Path="/5DDE6CFA/5DE323A1" Ref="D?"  Part="1" 
AR Path="/5DDE1103/5DE323A1" Ref="D12"  Part="1" 
F 0 "D12" V 5189 4383 50  0000 R CNN
F 1 "CONTINUITY" V 5098 4383 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Castellated" H 5150 4500 50  0001 C CNN
F 3 "~" H 5150 4500 50  0001 C CNN
	1    5150 4500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4750 4300 5150 4300
Wire Wire Line
	5150 4300 5150 4350
Connection ~ 4750 4300
$Comp
L power:GND #PWR?
U 1 1 5DE323AA
P 5150 5000
AR Path="/5DDE6CFA/5DE323AA" Ref="#PWR?"  Part="1" 
AR Path="/5DDE1103/5DE323AA" Ref="#PWR060"  Part="1" 
F 0 "#PWR060" H 5150 4750 50  0001 C CNN
F 1 "GND" H 5155 4827 50  0000 C CNN
F 2 "" H 5150 5000 50  0001 C CNN
F 3 "" H 5150 5000 50  0001 C CNN
	1    5150 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 5000 5150 4950
$Comp
L power:+3V3 #PWR?
U 1 1 5DE323B7
P 4150 4750
AR Path="/5DDE6CFA/5DE323B7" Ref="#PWR?"  Part="1" 
AR Path="/5DDE1103/5DE323B7" Ref="#PWR057"  Part="1" 
F 0 "#PWR057" H 4150 4600 50  0001 C CNN
F 1 "+3V3" H 4165 4923 50  0000 C CNN
F 2 "" H 4150 4750 50  0001 C CNN
F 3 "" H 4150 4750 50  0001 C CNN
	1    4150 4750
	1    0    0    -1  
$EndComp
Text HLabel 3100 5200 0    50   Input ~ 0
Nuke1
Text HLabel 3900 4950 0    50   Input ~ 0
Cont1
Text HLabel 7600 5150 0    50   Input ~ 0
Nuke2
Text HLabel 8400 4900 0    50   Input ~ 0
Cont2
Wire Wire Line
	2800 4150 2250 4150
Connection ~ 2800 4150
Wire Wire Line
	7350 4100 6850 4100
Connection ~ 7350 4100
Wire Wire Line
	5850 2700 5850 2950
$Comp
L power:GND #PWR0101
U 1 1 5DEC1515
P 5850 2950
F 0 "#PWR0101" H 5850 2700 50  0001 C CNN
F 1 "GND" H 5855 2777 50  0000 C CNN
F 2 "" H 5850 2950 50  0001 C CNN
F 3 "" H 5850 2950 50  0001 C CNN
	1    5850 2950
	1    0    0    -1  
$EndComp
Text Label 5600 2800 2    50   ~ 0
External_Supply
Wire Wire Line
	4950 2800 5600 2800
Text Label 2250 4150 0    50   ~ 0
External_Supply
Text Label 6850 4100 0    50   ~ 0
External_Supply
Wire Wire Line
	8100 4600 8100 4700
Wire Wire Line
	7350 4700 8100 4700
Connection ~ 8100 4700
Wire Wire Line
	8100 4700 8100 4950
Wire Wire Line
	7800 4100 7800 3800
Wire Wire Line
	7800 3800 7900 3800
Wire Wire Line
	8100 3800 8000 3800
Wire Wire Line
	8100 3800 8100 4250
Wire Wire Line
	3600 4650 3600 4750
Wire Wire Line
	2800 4750 3600 4750
Connection ~ 3600 4750
Wire Wire Line
	3600 4750 3600 5000
Wire Wire Line
	3600 3850 3450 3850
Wire Wire Line
	3600 3850 3600 4300
Wire Wire Line
	3350 3850 3250 3850
Wire Wire Line
	3250 3850 3250 4150
Wire Wire Line
	4950 2700 5850 2700
$EndSCHEMATC
