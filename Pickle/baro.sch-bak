EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 5
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
L Sensor_Pressure:MS5607-02BA U?
U 1 1 61F648E4
P 5800 3750
F 0 "U?" H 6130 3796 50  0000 L CNN
F 1 "MS5607-02BA" H 6130 3705 50  0000 L CNN
F 2 "Package_LGA:LGA-8_3x5mm_P1.25mm" H 5800 3750 50  0001 C CNN
F 3 "https://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Data+Sheet%7FMS5607-02BA03%7FB2%7Fpdf%7FEnglish%7FENG_DS_MS5607-02BA03_B2.pdf%7FCAT-BLPS0035" H 5800 3750 50  0001 C CNN
	1    5800 3750
	1    0    0    -1  
$EndComp
Text HLabel 5400 3650 0    50   Input ~ 0
MISO
Text HLabel 5400 3750 0    50   Input ~ 0
MOSI
Text HLabel 5400 3850 0    50   Input ~ 0
SCLK
Text HLabel 5400 3950 0    50   Input ~ 0
BARO_CS
Wire Wire Line
	5400 3550 4900 3550
Wire Wire Line
	4900 3550 4900 4150
Wire Wire Line
	4900 4150 5800 4150
Wire Wire Line
	4900 4150 4700 4150
Wire Wire Line
	4700 4150 4700 3450
Connection ~ 4900 4150
Wire Wire Line
	4700 4150 4700 4300
Connection ~ 4700 4150
$Comp
L power:GND #PWR?
U 1 1 61F65393
P 4700 4300
F 0 "#PWR?" H 4700 4050 50  0001 C CNN
F 1 "GND" H 4705 4127 50  0000 C CNN
F 2 "" H 4700 4300 50  0001 C CNN
F 3 "" H 4700 4300 50  0001 C CNN
	1    4700 4300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 61F655D0
P 4700 3300
F 0 "C?" H 4815 3346 50  0000 L CNN
F 1 "100nF" H 4815 3255 50  0000 L CNN
F 2 "" H 4738 3150 50  0001 C CNN
F 3 "~" H 4700 3300 50  0001 C CNN
	1    4700 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 3150 4700 3000
Wire Wire Line
	4700 3000 5800 3000
Wire Wire Line
	5800 3000 5800 3350
Connection ~ 4700 3000
Wire Wire Line
	4700 3000 4700 2900
$Comp
L power:+3V3 #PWR?
U 1 1 61F65AE2
P 4700 2900
F 0 "#PWR?" H 4700 2750 50  0001 C CNN
F 1 "+3V3" H 4715 3073 50  0000 C CNN
F 2 "" H 4700 2900 50  0001 C CNN
F 3 "" H 4700 2900 50  0001 C CNN
	1    4700 2900
	1    0    0    -1  
$EndComp
Text Notes 4800 4250 0    50   ~ 0
Pulling PS low selects SPI
$EndSCHEMATC
