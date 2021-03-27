EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 7
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
L pspice:INDUCTOR L2
U 1 1 5DBA17F7
P 5900 3200
F 0 "L2" H 5900 3415 50  0000 C CNN
F 1 "1u5" H 5900 3324 50  0000 C CNN
F 2 "Inductor_SMD:L_Taiyo-Yuden_NR-30xx" H 5900 3200 50  0001 C CNN
F 3 "~" H 5900 3200 50  0001 C CNN
	1    5900 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 3600 6400 3200
Wire Wire Line
	6400 3200 6150 3200
Wire Wire Line
	6400 4000 6400 3800
Connection ~ 6400 3800
$Comp
L power:+3V3 #PWR052
U 1 1 5DBA4350
P 7100 3800
F 0 "#PWR052" H 7100 3650 50  0001 C CNN
F 1 "+3V3" H 7115 3973 50  0000 C CNN
F 2 "" H 7100 3800 50  0001 C CNN
F 3 "" H 7100 3800 50  0001 C CNN
	1    7100 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 3800 6550 3800
$Comp
L Device:C C11
U 1 1 5DBA6514
P 6550 4000
F 0 "C11" H 6665 4046 50  0000 L CNN
F 1 "10uF" H 6665 3955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6588 3850 50  0001 C CNN
F 3 "~" H 6550 4000 50  0001 C CNN
	1    6550 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 3800 6550 3850
Connection ~ 6550 3800
Wire Wire Line
	6550 3800 6900 3800
Wire Wire Line
	6350 3600 6400 3600
Wire Wire Line
	6350 3800 6400 3800
Wire Wire Line
	6350 4000 6400 4000
Wire Wire Line
	5650 3200 5400 3200
Wire Wire Line
	5400 3200 5400 3600
Wire Wire Line
	5400 3600 5450 3600
Wire Wire Line
	5700 4600 5700 4700
Wire Wire Line
	5700 4700 5750 4700
Wire Wire Line
	5800 4700 5800 4600
Connection ~ 5800 4700
$Comp
L power:GND #PWR051
U 1 1 5DBB0345
P 5750 4700
F 0 "#PWR051" H 5750 4450 50  0001 C CNN
F 1 "GND" H 5755 4527 50  0000 C CNN
F 2 "" H 5750 4700 50  0001 C CNN
F 3 "" H 5750 4700 50  0001 C CNN
	1    5750 4700
	1    0    0    -1  
$EndComp
Connection ~ 5750 4700
Wire Wire Line
	5750 4700 5800 4700
Wire Wire Line
	5450 3900 5400 3900
Wire Wire Line
	5400 3900 5400 4000
Wire Wire Line
	5400 4000 5450 4000
Wire Wire Line
	5400 4000 5400 4200
Wire Wire Line
	5400 4200 5450 4200
Connection ~ 5400 4000
$Comp
L power:GND #PWR050
U 1 1 5DBBE1E1
P 4600 4300
F 0 "#PWR050" H 4600 4050 50  0001 C CNN
F 1 "GND" H 4605 4127 50  0000 C CNN
F 2 "" H 4600 4300 50  0001 C CNN
F 3 "" H 4600 4300 50  0001 C CNN
	1    4600 4300
	1    0    0    -1  
$EndComp
$Comp
L power:VDD #PWR049
U 1 1 5DBCE035
P 5150 3750
F 0 "#PWR049" H 5150 3600 50  0001 C CNN
F 1 "VDD" H 5167 3923 50  0000 C CNN
F 2 "" H 5150 3750 50  0001 C CNN
F 3 "" H 5150 3750 50  0001 C CNN
	1    5150 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:C C10
U 1 1 5DBCE6FA
P 4600 4050
F 0 "C10" H 4715 4096 50  0000 L CNN
F 1 "10uF" H 4715 4005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4638 3900 50  0001 C CNN
F 3 "~" H 4600 4050 50  0001 C CNN
	1    4600 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 4200 4600 4300
Wire Wire Line
	4600 3900 4600 3800
Connection ~ 4600 3800
Wire Wire Line
	4600 3800 5150 3800
Wire Wire Line
	6550 4150 6550 4700
$Comp
L Device:C_Small C?
U 1 1 5E076608
P 4300 5400
AR Path="/5DE3360C/5E076608" Ref="C?"  Part="1" 
AR Path="/5DB9F80E/5E076608" Ref="C13"  Part="1" 
F 0 "C13" H 4392 5446 50  0000 L CNN
F 1 "10uF" H 4392 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4300 5400 50  0001 C CNN
F 3 "~" H 4300 5400 50  0001 C CNN
	1    4300 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5E07660E
P 4000 5400
AR Path="/5DE3360C/5E07660E" Ref="C?"  Part="1" 
AR Path="/5DB9F80E/5E07660E" Ref="C12"  Part="1" 
F 0 "C12" H 4092 5446 50  0000 L CNN
F 1 "100nF" H 4092 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4000 5400 50  0001 C CNN
F 3 "~" H 4000 5400 50  0001 C CNN
	1    4000 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 5300 4000 5200
Wire Wire Line
	4300 5200 4300 5300
Wire Wire Line
	4000 5500 4000 5600
Wire Wire Line
	4300 5600 4300 5500
$Comp
L Device:C_Small C?
U 1 1 5E078029
P 4900 5400
AR Path="/5DE3360C/5E078029" Ref="C?"  Part="1" 
AR Path="/5DB9F80E/5E078029" Ref="C15"  Part="1" 
F 0 "C15" H 4992 5446 50  0000 L CNN
F 1 "10uF" H 4992 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4900 5400 50  0001 C CNN
F 3 "~" H 4900 5400 50  0001 C CNN
	1    4900 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5E07802F
P 4600 5400
AR Path="/5DE3360C/5E07802F" Ref="C?"  Part="1" 
AR Path="/5DB9F80E/5E07802F" Ref="C14"  Part="1" 
F 0 "C14" H 4692 5446 50  0000 L CNN
F 1 "100nF" H 4692 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4600 5400 50  0001 C CNN
F 3 "~" H 4600 5400 50  0001 C CNN
	1    4600 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 5300 4600 5200
Wire Wire Line
	4900 5200 4900 5300
Wire Wire Line
	4600 5500 4600 5600
Wire Wire Line
	4900 5600 4900 5500
$Comp
L Device:C_Small C?
U 1 1 5E07D75B
P 5500 5400
AR Path="/5DE3360C/5E07D75B" Ref="C?"  Part="1" 
AR Path="/5DB9F80E/5E07D75B" Ref="C17"  Part="1" 
F 0 "C17" H 5592 5446 50  0000 L CNN
F 1 "10uF" H 5592 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5500 5400 50  0001 C CNN
F 3 "~" H 5500 5400 50  0001 C CNN
	1    5500 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5E07D761
P 5200 5400
AR Path="/5DE3360C/5E07D761" Ref="C?"  Part="1" 
AR Path="/5DB9F80E/5E07D761" Ref="C16"  Part="1" 
F 0 "C16" H 5292 5446 50  0000 L CNN
F 1 "100nF" H 5292 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5200 5400 50  0001 C CNN
F 3 "~" H 5200 5400 50  0001 C CNN
	1    5200 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 5300 5200 5200
Wire Wire Line
	5500 5200 5500 5300
Wire Wire Line
	5200 5500 5200 5600
Wire Wire Line
	5500 5600 5500 5500
$Comp
L Device:C_Small C?
U 1 1 5E07D77F
P 6100 5400
AR Path="/5DE3360C/5E07D77F" Ref="C?"  Part="1" 
AR Path="/5DB9F80E/5E07D77F" Ref="C19"  Part="1" 
F 0 "C19" H 6192 5446 50  0000 L CNN
F 1 "10uF" H 6192 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6100 5400 50  0001 C CNN
F 3 "~" H 6100 5400 50  0001 C CNN
	1    6100 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5E07D785
P 5800 5400
AR Path="/5DE3360C/5E07D785" Ref="C?"  Part="1" 
AR Path="/5DB9F80E/5E07D785" Ref="C18"  Part="1" 
F 0 "C18" H 5892 5446 50  0000 L CNN
F 1 "100nF" H 5892 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5800 5400 50  0001 C CNN
F 3 "~" H 5800 5400 50  0001 C CNN
	1    5800 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 5300 5800 5200
Wire Wire Line
	6100 5200 6100 5300
Wire Wire Line
	5800 5500 5800 5600
Wire Wire Line
	6100 5600 6100 5500
$Comp
L Device:C_Small C?
U 1 1 5E08499D
P 6650 5400
AR Path="/5DE3360C/5E08499D" Ref="C?"  Part="1" 
AR Path="/5DB9F80E/5E08499D" Ref="C21"  Part="1" 
F 0 "C21" H 6742 5446 50  0000 L CNN
F 1 "10uF" H 6742 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6650 5400 50  0001 C CNN
F 3 "~" H 6650 5400 50  0001 C CNN
	1    6650 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5E0849A3
P 6350 5400
AR Path="/5DE3360C/5E0849A3" Ref="C?"  Part="1" 
AR Path="/5DB9F80E/5E0849A3" Ref="C20"  Part="1" 
F 0 "C20" H 6442 5446 50  0000 L CNN
F 1 "100nF" H 6442 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6350 5400 50  0001 C CNN
F 3 "~" H 6350 5400 50  0001 C CNN
	1    6350 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 5300 6350 5200
Wire Wire Line
	6650 5200 6650 5300
Wire Wire Line
	6350 5500 6350 5600
Wire Wire Line
	6650 5600 6650 5500
$Comp
L Device:C_Small C?
U 1 1 5E0849C1
P 7250 5400
AR Path="/5DE3360C/5E0849C1" Ref="C?"  Part="1" 
AR Path="/5DB9F80E/5E0849C1" Ref="C23"  Part="1" 
F 0 "C23" H 7342 5446 50  0000 L CNN
F 1 "10uF" H 7342 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7250 5400 50  0001 C CNN
F 3 "~" H 7250 5400 50  0001 C CNN
	1    7250 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5E0849C7
P 6950 5400
AR Path="/5DE3360C/5E0849C7" Ref="C?"  Part="1" 
AR Path="/5DB9F80E/5E0849C7" Ref="C22"  Part="1" 
F 0 "C22" H 7042 5446 50  0000 L CNN
F 1 "100nF" H 7042 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6950 5400 50  0001 C CNN
F 3 "~" H 6950 5400 50  0001 C CNN
	1    6950 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 5300 6950 5200
Wire Wire Line
	7250 5200 7250 5300
Wire Wire Line
	6950 5500 6950 5600
Wire Wire Line
	7250 5600 7250 5500
$Comp
L Device:C_Small C?
U 1 1 5E0849E5
P 7850 5400
AR Path="/5DE3360C/5E0849E5" Ref="C?"  Part="1" 
AR Path="/5DB9F80E/5E0849E5" Ref="C25"  Part="1" 
F 0 "C25" H 7942 5446 50  0000 L CNN
F 1 "10uF" H 7942 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7850 5400 50  0001 C CNN
F 3 "~" H 7850 5400 50  0001 C CNN
	1    7850 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5E0849EB
P 7550 5400
AR Path="/5DE3360C/5E0849EB" Ref="C?"  Part="1" 
AR Path="/5DB9F80E/5E0849EB" Ref="C24"  Part="1" 
F 0 "C24" H 7642 5446 50  0000 L CNN
F 1 "100nF" H 7642 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7550 5400 50  0001 C CNN
F 3 "~" H 7550 5400 50  0001 C CNN
	1    7550 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 5300 7550 5200
Wire Wire Line
	7850 5200 7850 5300
Wire Wire Line
	7550 5500 7550 5600
Wire Wire Line
	7850 5600 7850 5500
$Comp
L Device:C_Small C?
U 1 1 5E084A09
P 8450 5400
AR Path="/5DE3360C/5E084A09" Ref="C?"  Part="1" 
AR Path="/5DB9F80E/5E084A09" Ref="C27"  Part="1" 
F 0 "C27" H 8542 5446 50  0000 L CNN
F 1 "10uF" H 8542 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8450 5400 50  0001 C CNN
F 3 "~" H 8450 5400 50  0001 C CNN
	1    8450 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5E084A0F
P 8150 5400
AR Path="/5DE3360C/5E084A0F" Ref="C?"  Part="1" 
AR Path="/5DB9F80E/5E084A0F" Ref="C26"  Part="1" 
F 0 "C26" H 8242 5446 50  0000 L CNN
F 1 "100nF" H 8242 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8150 5400 50  0001 C CNN
F 3 "~" H 8150 5400 50  0001 C CNN
	1    8150 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 5300 8150 5200
Wire Wire Line
	8150 5500 8150 5600
Connection ~ 4300 5200
Wire Wire Line
	4300 5200 4600 5200
Connection ~ 4600 5200
Connection ~ 4900 5200
Wire Wire Line
	4900 5200 5200 5200
Connection ~ 5200 5200
Connection ~ 5500 5200
Wire Wire Line
	5500 5200 5800 5200
Connection ~ 5800 5200
Connection ~ 6100 5200
Wire Wire Line
	6100 5200 6350 5200
Connection ~ 6350 5200
Connection ~ 6650 5200
Wire Wire Line
	6650 5200 6950 5200
Connection ~ 6950 5200
Connection ~ 7250 5200
Wire Wire Line
	7250 5200 7550 5200
Connection ~ 7550 5200
Connection ~ 7850 5200
Wire Wire Line
	7850 5200 8150 5200
Connection ~ 8150 5200
Connection ~ 4300 5600
Wire Wire Line
	4300 5600 4600 5600
Connection ~ 4600 5600
Connection ~ 4900 5600
Wire Wire Line
	4900 5600 5200 5600
Connection ~ 5200 5600
Connection ~ 5500 5600
Wire Wire Line
	5500 5600 5800 5600
Connection ~ 5800 5600
Connection ~ 6100 5600
Wire Wire Line
	6100 5600 6350 5600
Connection ~ 6350 5600
Connection ~ 6650 5600
Wire Wire Line
	6650 5600 6950 5600
Connection ~ 6950 5600
Connection ~ 7250 5600
Wire Wire Line
	7250 5600 7550 5600
Connection ~ 7550 5600
Connection ~ 7850 5600
Wire Wire Line
	7850 5600 8150 5600
Connection ~ 8150 5600
Wire Wire Line
	4600 5200 4900 5200
Wire Wire Line
	5200 5200 5500 5200
Wire Wire Line
	5800 5200 6100 5200
Wire Wire Line
	6350 5200 6650 5200
Wire Wire Line
	6950 5200 7250 5200
Wire Wire Line
	7550 5200 7850 5200
Wire Wire Line
	4600 5600 4900 5600
Wire Wire Line
	5200 5600 5500 5600
Wire Wire Line
	5800 5600 6100 5600
Wire Wire Line
	6350 5600 6650 5600
Wire Wire Line
	6950 5600 7250 5600
Wire Wire Line
	7550 5600 7850 5600
Wire Wire Line
	8150 5200 8450 5200
$Comp
L Device:C_Small C?
U 1 1 5E153428
P 9050 5400
AR Path="/5DE3360C/5E153428" Ref="C?"  Part="1" 
AR Path="/5DB9F80E/5E153428" Ref="C29"  Part="1" 
F 0 "C29" H 9142 5446 50  0000 L CNN
F 1 "10uF" H 9142 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9050 5400 50  0001 C CNN
F 3 "~" H 9050 5400 50  0001 C CNN
	1    9050 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5E15342E
P 8750 5400
AR Path="/5DE3360C/5E15342E" Ref="C?"  Part="1" 
AR Path="/5DB9F80E/5E15342E" Ref="C28"  Part="1" 
F 0 "C28" H 8842 5446 50  0000 L CNN
F 1 "100nF" H 8842 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8750 5400 50  0001 C CNN
F 3 "~" H 8750 5400 50  0001 C CNN
	1    8750 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8750 5300 8750 5200
Wire Wire Line
	9050 5200 9050 5300
Wire Wire Line
	8750 5500 8750 5600
Wire Wire Line
	9050 5600 9050 5500
Wire Wire Line
	8450 5200 8750 5200
Connection ~ 8750 5200
Connection ~ 8750 5600
Wire Wire Line
	8750 5200 9050 5200
Wire Wire Line
	8750 5600 9050 5600
$Comp
L Connector:TestPoint TP?
U 1 1 5E403DEA
P 4600 3800
AR Path="/5E403DEA" Ref="TP?"  Part="1" 
AR Path="/5DB9F80E/5E403DEA" Ref="TP16"  Part="1" 
F 0 "TP16" V 4554 3988 50  0000 L CNN
F 1 "TestPoint" V 4645 3988 50  0000 L CNN
F 2 "TestPoint:TestPoint_THTPad_1.0x1.0mm_Drill0.5mm" H 4800 3800 50  0001 C CNN
F 3 "~" H 4800 3800 50  0001 C CNN
	1    4600 3800
	0    -1   -1   0   
$EndComp
$Comp
L Connector:TestPoint TP?
U 1 1 5E4076DE
P 7100 3800
AR Path="/5E4076DE" Ref="TP?"  Part="1" 
AR Path="/5DB9F80E/5E4076DE" Ref="TP17"  Part="1" 
F 0 "TP17" V 7054 3988 50  0000 L CNN
F 1 "TestPoint" V 7145 3988 50  0000 L CNN
F 2 "TestPoint:TestPoint_THTPad_1.0x1.0mm_Drill0.5mm" H 7300 3800 50  0001 C CNN
F 3 "~" H 7300 3800 50  0001 C CNN
	1    7100 3800
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J11
U 1 1 5DE7E179
P 2400 3400
F 0 "J11" V 2462 3444 50  0000 L CNN
F 1 "Conn_01x02_Male" V 2553 3444 50  0000 L CNN
F 2 "Connector_JST:JST_PH_S2B-PH-K_1x02_P2.00mm_Horizontal" H 2400 3400 50  0001 C CNN
F 3 "~" H 2400 3400 50  0001 C CNN
	1    2400 3400
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR059
U 1 1 5DE8310C
P 2300 4250
F 0 "#PWR059" H 2300 4000 50  0001 C CNN
F 1 "GND" H 2305 4077 50  0000 C CNN
F 2 "" H 2300 4250 50  0001 C CNN
F 3 "" H 2300 4250 50  0001 C CNN
	1    2300 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 5300 8450 5200
Wire Wire Line
	8150 5600 8450 5600
Connection ~ 8450 5200
Wire Wire Line
	8450 5500 8450 5600
Connection ~ 8450 5600
Wire Wire Line
	8450 5600 8750 5600
Wire Wire Line
	4000 5600 4300 5600
Wire Wire Line
	4000 5200 4300 5200
$Comp
L Mainboard-rescue:+3.3V-Mainboard-cache #PWR0106
U 1 1 5E4CE71E
P 4000 5200
AR Path="/5E4CE71E" Ref="#PWR0106"  Part="1" 
AR Path="/5DB9F80E/5E4CE71E" Ref="#PWR0106"  Part="1" 
F 0 "#PWR0106" H 4000 5050 50  0001 C CNN
F 1 "+3.3V" H 4015 5373 50  0000 C CNN
F 2 "" H 4000 5200 50  0001 C CNN
F 3 "" H 4000 5200 50  0001 C CNN
	1    4000 5200
	1    0    0    -1  
$EndComp
Connection ~ 4000 5200
$Comp
L power:GND #PWR0107
U 1 1 5E4CEA96
P 9050 5600
F 0 "#PWR0107" H 9050 5350 50  0001 C CNN
F 1 "GND" H 9055 5427 50  0000 C CNN
F 2 "" H 9050 5600 50  0001 C CNN
F 3 "" H 9050 5600 50  0001 C CNN
	1    9050 5600
	1    0    0    -1  
$EndComp
Connection ~ 9050 5600
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 5E1B8AC2
P 8050 3950
F 0 "H1" H 8150 3999 50  0000 L CNN
F 1 "MountingHole_Pad" H 8150 3908 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_Pad" H 8050 3950 50  0001 C CNN
F 3 "~" H 8050 3950 50  0001 C CNN
	1    8050 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 4050 7900 4050
Wire Wire Line
	7900 4050 7900 4150
Connection ~ 8050 4050
$Comp
L power:GND #PWR027
U 1 1 5E1C87F6
P 7900 4150
F 0 "#PWR027" H 7900 3900 50  0001 C CNN
F 1 "GND" H 7905 3977 50  0000 C CNN
F 2 "" H 7900 4150 50  0001 C CNN
F 3 "" H 7900 4150 50  0001 C CNN
	1    7900 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 3750 5150 3800
Connection ~ 5150 3800
Wire Wire Line
	5150 3800 5450 3800
Connection ~ 7100 3800
$Comp
L Device:C C30
U 1 1 5E2A869F
P 6900 4000
F 0 "C30" H 7015 4046 50  0000 L CNN
F 1 "10uF" H 7015 3955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6938 3850 50  0001 C CNN
F 3 "~" H 6900 4000 50  0001 C CNN
	1    6900 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 3850 6900 3800
Connection ~ 6900 3800
Wire Wire Line
	6900 3800 7100 3800
Wire Wire Line
	6550 4700 6900 4700
Wire Wire Line
	6900 4700 6900 4150
Connection ~ 6550 4700
$Comp
L Device:C C31
U 1 1 5E2B517B
P 5200 4100
F 0 "C31" H 5315 4146 50  0000 L CNN
F 1 "100nF" H 5315 4055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5238 3950 50  0001 C CNN
F 3 "~" H 5200 4100 50  0001 C CNN
	1    5200 4100
	-1   0    0    1   
$EndComp
Wire Wire Line
	5200 3950 5200 3900
Wire Wire Line
	5200 3900 5400 3900
Connection ~ 5400 3900
$Comp
L power:GND #PWR0109
U 1 1 5E2C5248
P 5200 4300
F 0 "#PWR0109" H 5200 4050 50  0001 C CNN
F 1 "GND" H 5205 4127 50  0000 C CNN
F 2 "" H 5200 4300 50  0001 C CNN
F 3 "" H 5200 4300 50  0001 C CNN
	1    5200 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 4300 5200 4250
$Comp
L Analog_Switch:MAX40200AUK U12
U 1 1 5E63069C
P 2800 3950
F 0 "U12" H 2800 4317 50  0000 C CNN
F 1 "MAX40200AUK" H 2800 4226 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 2800 4450 50  0001 C CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/MAX40200.pdf" H 2800 4450 50  0001 C CNN
	1    2800 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 3600 2400 3850
Wire Wire Line
	2400 3850 2400 4050
Connection ~ 2400 3850
Wire Wire Line
	2300 4250 2800 4250
Wire Wire Line
	2300 3600 2300 4250
Connection ~ 2300 4250
$Comp
L power:VDD #PWR054
U 1 1 5E65F78E
P 3300 3850
F 0 "#PWR054" H 3300 3700 50  0001 C CNN
F 1 "VDD" H 3317 4023 50  0000 C CNN
F 2 "" H 3300 3850 50  0001 C CNN
F 3 "" H 3300 3850 50  0001 C CNN
	1    3300 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 3850 3200 3850
$Comp
L Mainboard-rescue:TPS63031-iclr U8
U 1 1 5DE04FA4
P 5900 4000
F 0 "U8" H 5733 4665 50  0000 C CNN
F 1 "TPS63031" H 5733 4574 50  0000 C CNN
F 2 "iclr:VSON-10-1EP_2.5x2.5mm_Pitch0.5mm_ThermalPad" H 5900 4600 50  0001 C CNN
F 3 "" H 5900 4600 50  0001 C CNN
	1    5900 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 4700 6550 4700
Wire Wire Line
	8050 4050 8300 4050
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 5E1B83DE
P 8300 3950
F 0 "H2" H 8400 3999 50  0000 L CNN
F 1 "MountingHole_Pad" H 8400 3908 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_Pad" H 8300 3950 50  0001 C CNN
F 3 "~" H 8300 3950 50  0001 C CNN
	1    8300 3950
	1    0    0    -1  
$EndComp
$EndSCHEMATC
