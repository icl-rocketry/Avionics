EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 3
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
L RF_Module:ESP32-WROOM-32 U?
U 1 1 6038790F
P 5800 3500
F 0 "U?" H 5800 5081 50  0000 C CNN
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
Wire Wire Line
	6400 4300 6750 4300
$Sheet
S 9200 4950 1000 600 
U 603A281D
F0 "pyros" 50
F1 "pyros.sch" 50
$EndSheet
Text Label 6550 4300 0    50   ~ 0
Nuke1
Text Label 6550 4400 0    50   ~ 0
Nuke2
Text Label 6550 4500 0    50   ~ 0
Cont1
Text Label 6550 4600 0    50   ~ 0
Cont2
Text HLabel 9200 5050 2    50   Input ~ 0
Nuke1
Text HLabel 9200 5150 2    50   Input ~ 0
Nuke2
Text HLabel 9200 5250 2    50   Input ~ 0
Cont1
Text HLabel 9200 5350 2    50   Input ~ 0
Cont2
Wire Wire Line
	9200 5050 8900 5050
Wire Wire Line
	9200 5150 8900 5150
Wire Wire Line
	9200 5250 8900 5250
Wire Wire Line
	9200 5350 8900 5350
Text Label 8900 5050 0    50   ~ 0
Nuke1
Text Label 8900 5150 0    50   ~ 0
Nuke2
Text Label 8900 5250 0    50   ~ 0
Cont1
Text Label 8900 5350 0    50   ~ 0
Cont2
$Comp
L power:GND #PWR?
U 1 1 603E7988
P 5800 4900
F 0 "#PWR?" H 5800 4650 50  0001 C CNN
F 1 "GND" H 5805 4727 50  0000 C CNN
F 2 "" H 5800 4900 50  0001 C CNN
F 3 "" H 5800 4900 50  0001 C CNN
	1    5800 4900
	1    0    0    -1  
$EndComp
NoConn ~ 6400 2300
NoConn ~ 6400 2400
NoConn ~ 6400 2500
NoConn ~ 6400 2600
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
L power:+3V3 #PWR?
U 1 1 603E97E4
P 5800 1600
F 0 "#PWR?" H 5800 1450 50  0001 C CNN
F 1 "+3V3" H 5815 1773 50  0000 C CNN
F 2 "" H 5800 1600 50  0001 C CNN
F 3 "" H 5800 1600 50  0001 C CNN
	1    5800 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 2100 5800 1600
$Sheet
S 1450 1200 900  300 
U 603E92A7
F0 "power" 50
F1 "power.sch" 50
$EndSheet
$EndSCHEMATC
