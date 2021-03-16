EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 4
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
L RF_Module:ESP32-WROOM-32 U2
U 1 1 6038790F
P 5800 3500
F 0 "U2" H 5800 5081 50  0000 C CNN
F 1 "ESP32-WROOM-32" H 5800 4990 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 5800 2000 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf" H 5500 3550 50  0001 C CNN
	1    5800 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 2300 4850 2300
Text Label 4850 2300 0    50   ~ 0
EN
Wire Wire Line
	6400 4600 6750 4600
Wire Wire Line
	6400 4500 6750 4500
Wire Wire Line
	6400 4400 6750 4400
Text Label 6750 4400 0    50   ~ 0
Nuke2
Text Label 6750 4500 0    50   ~ 0
Cont1
Text Label 6750 4600 0    50   ~ 0
Cont2
Wire Wire Line
	9350 5450 9050 5450
Wire Wire Line
	9350 5550 9050 5550
Wire Wire Line
	9350 5650 9050 5650
Wire Wire Line
	9350 5750 9050 5750
Text Label 9050 5450 0    50   ~ 0
Nuke1
Text Label 9050 5550 0    50   ~ 0
Nuke2
Text Label 9050 5650 0    50   ~ 0
Cont1
Text Label 9050 5750 0    50   ~ 0
Cont2
$Comp
L power:GND #PWR08
U 1 1 603E7988
P 5800 4900
F 0 "#PWR08" H 5800 4650 50  0001 C CNN
F 1 "GND" H 5805 4727 50  0000 C CNN
F 2 "" H 5800 4900 50  0001 C CNN
F 3 "" H 5800 4900 50  0001 C CNN
	1    5800 4900
	1    0    0    -1  
$EndComp
NoConn ~ 6400 2500
NoConn ~ 6400 2700
NoConn ~ 6400 2800
NoConn ~ 6400 2900
NoConn ~ 6400 3000
NoConn ~ 6400 3100
NoConn ~ 6400 3200
NoConn ~ 6400 3300
NoConn ~ 6400 3400
NoConn ~ 6400 3500
NoConn ~ 6400 3600
NoConn ~ 6400 3700
NoConn ~ 6400 3800
NoConn ~ 6400 3900
NoConn ~ 6400 4000
NoConn ~ 6400 4100
NoConn ~ 6400 4200
NoConn ~ 5200 3500
NoConn ~ 5200 3600
NoConn ~ 5200 3700
NoConn ~ 5200 3800
NoConn ~ 5200 3900
NoConn ~ 5200 4000
NoConn ~ 5200 2500
NoConn ~ 5200 2600
$Comp
L power:+3V3 #PWR06
U 1 1 603E97E4
P 5300 1150
F 0 "#PWR06" H 5300 1000 50  0001 C CNN
F 1 "+3V3" H 5315 1323 50  0000 C CNN
F 2 "" H 5300 1150 50  0001 C CNN
F 3 "" H 5300 1150 50  0001 C CNN
	1    5300 1150
	1    0    0    -1  
$EndComp
$Sheet
S 600  600  900  300 
U 603E92A7
F0 "power" 50
F1 "power.sch" 50
$EndSheet
$Comp
L Device:C_Small C1
U 1 1 604692B6
P 5300 1400
F 0 "C1" H 5392 1446 50  0000 L CNN
F 1 "100uF" H 5392 1355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5300 1400 50  0001 C CNN
F 3 "~" H 5300 1400 50  0001 C CNN
	1    5300 1400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C2
U 1 1 60469B68
P 5600 1400
F 0 "C2" H 5692 1446 50  0000 L CNN
F 1 "1uF" H 5692 1355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5600 1400 50  0001 C CNN
F 3 "~" H 5600 1400 50  0001 C CNN
	1    5600 1400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 60469F35
P 5300 1700
F 0 "#PWR07" H 5300 1450 50  0001 C CNN
F 1 "GND" H 5305 1527 50  0000 C CNN
F 2 "" H 5300 1700 50  0001 C CNN
F 3 "" H 5300 1700 50  0001 C CNN
	1    5300 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R6
U 1 1 6046A825
P 6500 1450
F 0 "R6" H 6559 1496 50  0000 L CNN
F 1 "470" H 6559 1405 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 6500 1450 50  0001 C CNN
F 3 "~" H 6500 1450 50  0001 C CNN
	1    6500 1450
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D3
U 1 1 6046B11D
P 6500 1700
F 0 "D3" V 6539 1582 50  0000 R CNN
F 1 "Power_LED" V 6448 1582 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Castellated" H 6500 1700 50  0001 C CNN
F 3 "~" H 6500 1700 50  0001 C CNN
	1    6500 1700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5300 1150 5300 1300
Wire Wire Line
	5600 1500 5600 1550
Wire Wire Line
	5300 1500 5300 1550
Wire Wire Line
	5300 1300 5600 1300
Wire Wire Line
	5600 1550 5300 1550
Connection ~ 5300 1550
Wire Wire Line
	5300 1550 5300 1700
Wire Wire Line
	5600 1300 5800 1300
Wire Wire Line
	6500 1300 6500 1350
Connection ~ 5600 1300
Wire Wire Line
	5800 1300 5800 2100
Connection ~ 5800 1300
Wire Wire Line
	5800 1300 6500 1300
$Comp
L power:GND #PWR09
U 1 1 6047111A
P 6500 1850
F 0 "#PWR09" H 6500 1600 50  0001 C CNN
F 1 "GND" H 6505 1677 50  0000 C CNN
F 2 "" H 6500 1850 50  0001 C CNN
F 3 "" H 6500 1850 50  0001 C CNN
	1    6500 1850
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW1
U 1 1 60472246
P 9800 850
F 0 "SW1" H 9800 1135 50  0000 C CNN
F 1 "SW_Push" H 9800 1044 50  0000 C CNN
F 2 "" H 9800 1050 50  0001 C CNN
F 3 "~" H 9800 1050 50  0001 C CNN
	1    9800 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 850  9150 850 
Wire Wire Line
	9150 850  9150 1200
Wire Wire Line
	10000 850  10150 850 
Text Label 10850 850  0    50   ~ 0
EN
Wire Wire Line
	9150 1200 9550 1200
Wire Wire Line
	10150 1200 10150 850 
Connection ~ 9150 1200
Wire Wire Line
	9150 1200 9150 1300
Connection ~ 10150 850 
Wire Wire Line
	10150 850  10350 850 
$Comp
L Device:R_Small R9
U 1 1 6047462D
P 10450 850
F 0 "R9" V 10254 850 50  0000 C CNN
F 1 "470" V 10345 850 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 10450 850 50  0001 C CNN
F 3 "~" H 10450 850 50  0001 C CNN
	1    10450 850 
	0    1    1    0   
$EndComp
Wire Wire Line
	10550 850  10950 850 
$Comp
L Device:C_Small C4
U 1 1 60475423
P 9650 1200
F 0 "C4" V 9421 1200 50  0000 C CNN
F 1 "1nF" V 9512 1200 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9650 1200 50  0001 C CNN
F 3 "~" H 9650 1200 50  0001 C CNN
	1    9650 1200
	0    1    1    0   
$EndComp
Wire Wire Line
	9750 1200 10150 1200
$Comp
L power:GND #PWR013
U 1 1 60475F37
P 9150 1300
F 0 "#PWR013" H 9150 1050 50  0001 C CNN
F 1 "GND" H 9155 1127 50  0000 C CNN
F 2 "" H 9150 1300 50  0001 C CNN
F 3 "" H 9150 1300 50  0001 C CNN
	1    9150 1300
	1    0    0    -1  
$EndComp
$Comp
L Interface_USB:CP2102N-A01-GQFN24 U1
U 1 1 6047F012
P 2700 4650
F 0 "U1" H 2700 5731 50  0000 C CNN
F 1 "CP2102N-A01-GQFN24" H 2700 5640 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-24-1EP_4x4mm_P0.5mm_EP2.6x2.6mm" H 3150 3850 50  0001 L CNN
F 3 "https://www.silabs.com/documents/public/data-sheets/cp2102n-datasheet.pdf" H 2750 3600 50  0001 C CNN
	1    2700 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 2300 6900 2300
Wire Wire Line
	6400 2400 6900 2400
Wire Wire Line
	6400 2600 6900 2600
Text Label 6900 2300 0    50   ~ 0
BOOT
Text Label 6900 2400 0    50   ~ 0
RXD
Text Label 6900 2600 0    50   ~ 0
TXD
$Comp
L Switch:SW_Push SW2
U 1 1 604A2321
P 9800 1850
F 0 "SW2" H 9800 2135 50  0000 C CNN
F 1 "SW_Push" H 9800 2044 50  0000 C CNN
F 2 "" H 9800 2050 50  0001 C CNN
F 3 "~" H 9800 2050 50  0001 C CNN
	1    9800 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 1850 9150 1850
Wire Wire Line
	9150 1850 9150 2200
Wire Wire Line
	10000 1850 10150 1850
Text Label 10850 1850 0    50   ~ 0
BOOT
Wire Wire Line
	9150 2200 9550 2200
Wire Wire Line
	10150 2200 10150 1850
Connection ~ 9150 2200
Wire Wire Line
	9150 2200 9150 2300
Connection ~ 10150 1850
Wire Wire Line
	10150 1850 10350 1850
$Comp
L Device:R_Small R10
U 1 1 604A2331
P 10450 1850
F 0 "R10" V 10254 1850 50  0000 C CNN
F 1 "470" V 10345 1850 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 10450 1850 50  0001 C CNN
F 3 "~" H 10450 1850 50  0001 C CNN
	1    10450 1850
	0    1    1    0   
$EndComp
Wire Wire Line
	10550 1850 10950 1850
$Comp
L Device:C_Small C5
U 1 1 604A2338
P 9650 2200
F 0 "C5" V 9421 2200 50  0000 C CNN
F 1 "1nF" V 9512 2200 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9650 2200 50  0001 C CNN
F 3 "~" H 9650 2200 50  0001 C CNN
	1    9650 2200
	0    1    1    0   
$EndComp
Wire Wire Line
	9750 2200 10150 2200
$Comp
L power:GND #PWR014
U 1 1 604A233F
P 9150 2300
F 0 "#PWR014" H 9150 2050 50  0001 C CNN
F 1 "GND" H 9155 2127 50  0000 C CNN
F 2 "" H 9150 2300 50  0001 C CNN
F 3 "" H 9150 2300 50  0001 C CNN
	1    9150 2300
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R7
U 1 1 604A7BC1
P 8250 900
F 0 "R7" H 8309 946 50  0000 L CNN
F 1 "12K" H 8309 855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 8250 900 50  0001 C CNN
F 3 "~" H 8250 900 50  0001 C CNN
	1    8250 900 
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C3
U 1 1 604A880C
P 8250 1200
F 0 "C3" H 8342 1246 50  0000 L CNN
F 1 "1nF" H 8342 1155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8250 1200 50  0001 C CNN
F 3 "~" H 8250 1200 50  0001 C CNN
	1    8250 1200
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR010
U 1 1 604A8FF0
P 8250 800
F 0 "#PWR010" H 8250 650 50  0001 C CNN
F 1 "+3V3" H 8265 973 50  0000 C CNN
F 2 "" H 8250 800 50  0001 C CNN
F 3 "" H 8250 800 50  0001 C CNN
	1    8250 800 
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 1000 8250 1100
Wire Wire Line
	8250 1100 8950 1100
Connection ~ 8250 1100
Text Label 8850 1100 0    50   ~ 0
EN
$Comp
L power:GND #PWR011
U 1 1 604B0B36
P 8250 1300
F 0 "#PWR011" H 8250 1050 50  0001 C CNN
F 1 "GND" H 8255 1127 50  0000 C CNN
F 2 "" H 8250 1300 50  0001 C CNN
F 3 "" H 8250 1300 50  0001 C CNN
	1    8250 1300
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR012
U 1 1 604B12D0
P 8250 1900
F 0 "#PWR012" H 8250 1750 50  0001 C CNN
F 1 "+3V3" H 8265 2073 50  0000 C CNN
F 2 "" H 8250 1900 50  0001 C CNN
F 3 "" H 8250 1900 50  0001 C CNN
	1    8250 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R8
U 1 1 604B1A2B
P 8250 2000
F 0 "R8" H 8309 2046 50  0000 L CNN
F 1 "5K" H 8309 1955 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 8250 2000 50  0001 C CNN
F 3 "~" H 8250 2000 50  0001 C CNN
	1    8250 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 2100 8250 2200
Wire Wire Line
	8250 2200 8950 2200
Text Label 8750 2200 0    50   ~ 0
BOOT
Wire Wire Line
	3300 4350 3600 4350
Wire Wire Line
	3300 4450 3600 4450
Text Label 3600 4350 0    50   ~ 0
RXD
Text Label 3600 4450 0    50   ~ 0
TXD
Wire Wire Line
	3300 4250 3600 4250
Wire Wire Line
	3300 4650 3600 4650
Wire Wire Line
	2700 5550 2750 5550
$Comp
L power:GND #PWR05
U 1 1 60485A85
P 2750 5550
F 0 "#PWR05" H 2750 5300 50  0001 C CNN
F 1 "GND" H 2755 5377 50  0000 C CNN
F 2 "" H 2750 5550 50  0001 C CNN
F 3 "" H 2750 5550 50  0001 C CNN
	1    2750 5550
	1    0    0    -1  
$EndComp
Connection ~ 2750 5550
Wire Wire Line
	2750 5550 2800 5550
Wire Wire Line
	2700 3750 2700 3500
Wire Wire Line
	2700 3500 2600 3500
Wire Wire Line
	2600 3500 2600 3750
Wire Wire Line
	2600 3500 2600 3450
Connection ~ 2600 3500
$Comp
L power:+3V3 #PWR04
U 1 1 6048C69F
P 2600 3450
F 0 "#PWR04" H 2600 3300 50  0001 C CNN
F 1 "+3V3" H 2615 3623 50  0000 C CNN
F 2 "" H 2600 3450 50  0001 C CNN
F 3 "" H 2600 3450 50  0001 C CNN
	1    2600 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R5
U 1 1 604946B0
P 2100 3700
F 0 "R5" H 2159 3746 50  0000 L CNN
F 1 "1K" H 2159 3655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 2100 3700 50  0001 C CNN
F 3 "~" H 2100 3700 50  0001 C CNN
	1    2100 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 3500 2100 3500
Wire Wire Line
	2100 3500 2100 3600
Wire Wire Line
	2100 3800 2100 4050
NoConn ~ 3300 4050
NoConn ~ 3300 4150
NoConn ~ 3300 4550
NoConn ~ 3300 4750
NoConn ~ 3300 4950
NoConn ~ 3300 5050
NoConn ~ 3300 5150
NoConn ~ 3300 5250
NoConn ~ 2100 4350
NoConn ~ 2100 4450
Wire Wire Line
	1300 4650 1700 4650
Wire Wire Line
	1300 3550 1300 4650
$Comp
L Device:D_Schottky D1
U 1 1 604A7675
P 1300 3400
F 0 "D1" V 1254 3480 50  0000 L CNN
F 1 "D_Schottky" V 1345 3480 50  0000 L CNN
F 2 "Diode_SMD:D_SOD-123F" H 1300 3400 50  0001 C CNN
F 3 "~" H 1300 3400 50  0001 C CNN
	1    1300 3400
	0    1    1    0   
$EndComp
$Comp
L power:VDD #PWR01
U 1 1 604B5A84
P 1300 3250
F 0 "#PWR01" H 1300 3100 50  0001 C CNN
F 1 "VDD" H 1315 3423 50  0000 C CNN
F 2 "" H 1300 3250 50  0001 C CNN
F 3 "" H 1300 3250 50  0001 C CNN
	1    1300 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 3550 1600 3550
Wire Wire Line
	1600 3550 1600 3600
Connection ~ 1300 3550
$Comp
L Device:R_Small R3
U 1 1 604B79F6
P 1600 3700
F 0 "R3" H 1659 3746 50  0000 L CNN
F 1 "22K" H 1659 3655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 1600 3700 50  0001 C CNN
F 3 "~" H 1600 3700 50  0001 C CNN
	1    1600 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 3800 1600 3850
$Comp
L Device:R_Small R4
U 1 1 604B99D6
P 1600 4000
F 0 "R4" H 1659 4046 50  0000 L CNN
F 1 "43K" H 1659 3955 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 1600 4000 50  0001 C CNN
F 3 "~" H 1600 4000 50  0001 C CNN
	1    1600 4000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 604BA43F
P 1600 4100
F 0 "#PWR02" H 1600 3850 50  0001 C CNN
F 1 "GND" H 1605 3927 50  0000 C CNN
F 2 "" H 1600 4100 50  0001 C CNN
F 3 "" H 1600 4100 50  0001 C CNN
	1    1600 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 4950 1900 4950
Wire Wire Line
	1900 4950 1900 3850
Wire Wire Line
	1900 3850 1600 3850
Connection ~ 1600 3850
Wire Wire Line
	1600 3850 1600 3900
$Comp
L Power_Protection:SP0503BAHT D2
U 1 1 604BFF73
P 1800 5450
F 0 "D2" H 2005 5496 50  0000 L CNN
F 1 "SP0503BAHT" H 2005 5405 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-143" H 2025 5400 50  0001 L CNN
F 3 "http://www.littelfuse.com/~/media/files/littelfuse/technical%20resources/documents/data%20sheets/sp05xxba.pdf" H 1925 5575 50  0001 C CNN
	1    1800 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 5150 1800 5150
Wire Wire Line
	2100 5050 1900 5050
Wire Wire Line
	1800 5150 1800 5250
Connection ~ 1800 5150
Wire Wire Line
	1900 5050 1900 5250
Connection ~ 1900 5050
Wire Wire Line
	1700 4650 1700 5250
Connection ~ 1700 4650
Wire Wire Line
	1700 4650 2100 4650
$Comp
L power:GND #PWR03
U 1 1 604CC07D
P 1800 5650
F 0 "#PWR03" H 1800 5400 50  0001 C CNN
F 1 "GND" H 1805 5477 50  0000 C CNN
F 2 "" H 1800 5650 50  0001 C CNN
F 3 "" H 1800 5650 50  0001 C CNN
	1    1800 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 4650 1300 4950
Wire Wire Line
	1300 4950 1100 4950
Connection ~ 1300 4650
Wire Wire Line
	1100 5050 1900 5050
Wire Wire Line
	1100 5150 1800 5150
Text Label 3600 4250 0    50   ~ 0
RTS
Text Label 3600 4650 0    50   ~ 0
DTR
$Comp
L Transistor_BJT:MMDT2222A Q1
U 1 1 604E26C4
P 1650 6550
F 0 "Q1" H 1840 6596 50  0000 L CNN
F 1 "MMDT2222A" H 1840 6505 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-363_SC-70-6" H 1850 6650 50  0001 C CNN
F 3 "http://www.diodes.com/_files/datasheets/ds30125.pdf" H 1650 6550 50  0001 C CNN
	1    1650 6550
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:MMDT2222A Q2
U 1 1 604E3CBA
P 1650 7400
F 0 "Q2" H 1840 7354 50  0000 L CNN
F 1 "MMDT2222A" H 1840 7445 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-363_SC-70-6" H 1850 7500 50  0001 C CNN
F 3 "http://www.diodes.com/_files/datasheets/ds30125.pdf" H 1650 7400 50  0001 C CNN
	1    1650 7400
	1    0    0    1   
$EndComp
Wire Wire Line
	1750 7150 900  7150
Wire Wire Line
	1450 6550 1250 6550
Wire Wire Line
	900  6550 750  6550
Connection ~ 900  6550
Wire Wire Line
	1750 6750 950  6750
Wire Wire Line
	950  6750 950  7400
Wire Wire Line
	950  7400 750  7400
Wire Wire Line
	1450 7400 1250 7400
Connection ~ 950  7400
Wire Wire Line
	1750 7600 1750 7700
Wire Wire Line
	1750 7700 2150 7700
Wire Wire Line
	1750 6350 1750 6250
Wire Wire Line
	1750 6250 2150 6250
Text Label 2150 6250 0    50   ~ 0
EN
Text Label 2150 7700 0    50   ~ 0
BOOT
$Comp
L Device:R_Small R1
U 1 1 604FBE3D
P 1150 6550
F 0 "R1" V 954 6550 50  0000 C CNN
F 1 "10K" V 1045 6550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 1150 6550 50  0001 C CNN
F 3 "~" H 1150 6550 50  0001 C CNN
	1    1150 6550
	0    1    1    0   
$EndComp
Wire Wire Line
	1050 6550 900  6550
$Comp
L Device:R_Small R2
U 1 1 604FC7BC
P 1150 7400
F 0 "R2" V 1346 7400 50  0000 C CNN
F 1 "10K" V 1255 7400 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 1150 7400 50  0001 C CNN
F 3 "~" H 1150 7400 50  0001 C CNN
	1    1150 7400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1050 7400 950  7400
Wire Wire Line
	900  7150 900  6550
Wire Wire Line
	1750 7200 1750 7150
Text Label 750  6550 2    50   ~ 0
DTR
Text Label 750  7400 2    50   ~ 0
RTS
$Sheet
S 600  4900 500  350 
U 604CC72F
F0 "usb" 50
F1 "usb.sch" 50
F2 "VBUS" I R 1100 4950 50 
F3 "D-" I R 1100 5150 50 
F4 "D+" I R 1100 5050 50 
$EndSheet
Wire Wire Line
	6400 4300 6750 4300
Text Label 6750 4300 0    50   ~ 0
Nuke1
$Sheet
S 9350 5350 1000 600 
U 603A281D
F0 "pyros" 50
F1 "pyros.sch" 50
F2 "Cont1" I L 9350 5650 50 
F3 "Nuke2" I L 9350 5550 50 
F4 "Cont2" I L 9350 5750 50 
F5 "Nuke1" I L 9350 5450 50 
$EndSheet
Connection ~ 5300 1300
$EndSCHEMATC
