EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
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
L Regulator_Switching:TPS63060 U?
U 1 1 603EAA6B
P 5200 3250
F 0 "U?" H 5200 3917 50  0000 C CNN
F 1 "TPS63060" H 5200 3826 50  0000 C CNN
F 2 "Package_SON:Texas_S-PWSON-N10_ThermalVias" H 5200 2600 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tps63060.pdf" H 5200 3250 50  0001 C CNN
	1    5200 3250
	1    0    0    -1  
$EndComp
$Comp
L pspice:INDUCTOR L?
U 1 1 603EB843
P 5200 2400
F 0 "L?" H 5200 2615 50  0000 C CNN
F 1 "INDUCTOR" H 5200 2524 50  0000 C CNN
F 2 "" H 5200 2400 50  0001 C CNN
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
$Comp
L Connector:TestPoint TP?
U 1 1 603EDF05
P 3750 3050
F 0 "TP?" V 3945 3122 50  0000 C CNN
F 1 "TestPoint" V 3854 3122 50  0000 C CNN
F 2 "" H 3950 3050 50  0001 C CNN
F 3 "~" H 3950 3050 50  0001 C CNN
	1    3750 3050
	0    -1   -1   0   
$EndComp
$EndSCHEMATC
