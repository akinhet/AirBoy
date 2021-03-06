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
L Device:C C5
U 1 1 61A669EC
P 4750 3100
F 0 "C5" H 4550 3150 50  0000 L CNN
F 1 "100n" H 4450 3050 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 4788 2950 50  0001 C CNN
F 3 "~" H 4750 3100 50  0001 C CNN
	1    4750 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C6
U 1 1 61A66F37
P 5050 3100
F 0 "C6" H 5165 3146 50  0000 L CNN
F 1 "10u" H 5165 3055 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3528-21_Kemet-B_Pad1.50x2.35mm_HandSolder" H 5088 2950 50  0001 C CNN
F 3 "~" H 5050 3100 50  0001 C CNN
	1    5050 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 2850 5050 2850
Wire Wire Line
	4750 2850 4750 2950
Wire Wire Line
	5050 2950 5050 2850
Connection ~ 5050 2850
Wire Wire Line
	5400 3350 5400 3200
Wire Wire Line
	5400 3200 5500 3200
Connection ~ 5500 3200
Wire Wire Line
	5500 3200 5500 2850
Wire Wire Line
	5500 3200 5500 3350
$Comp
L power:GND #PWR017
U 1 1 61A69849
P 4900 3450
F 0 "#PWR017" H 4900 3200 50  0001 C CNN
F 1 "GND" H 4905 3277 50  0000 C CNN
F 2 "" H 4900 3450 50  0001 C CNN
F 3 "" H 4900 3450 50  0001 C CNN
	1    4900 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 2850 5050 2850
Wire Wire Line
	4750 3250 4750 3350
Wire Wire Line
	4750 3350 4900 3350
Wire Wire Line
	5050 3350 5050 3250
Wire Wire Line
	4900 3450 4900 3350
Connection ~ 4900 3350
Wire Wire Line
	4900 3350 5050 3350
$Comp
L power:+5V #PWR019
U 1 1 61A6A393
P 5500 2700
F 0 "#PWR019" H 5500 2550 50  0001 C CNN
F 1 "+5V" H 5515 2873 50  0000 C CNN
F 2 "" H 5500 2700 50  0001 C CNN
F 3 "" H 5500 2700 50  0001 C CNN
	1    5500 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 2700 5500 2850
Connection ~ 5500 2850
Wire Wire Line
	5600 5350 5600 5500
Wire Wire Line
	5600 5500 5750 5500
Wire Wire Line
	6050 5500 6050 5350
Wire Wire Line
	5900 5350 5900 5500
Connection ~ 5900 5500
Wire Wire Line
	5900 5500 6050 5500
Wire Wire Line
	5750 5350 5750 5500
Connection ~ 5750 5500
Wire Wire Line
	5750 5500 5900 5500
$Comp
L power:GND #PWR020
U 1 1 61A6B22E
P 6050 5650
F 0 "#PWR020" H 6050 5400 50  0001 C CNN
F 1 "GND" H 6055 5477 50  0000 C CNN
F 2 "" H 6050 5650 50  0001 C CNN
F 3 "" H 6050 5650 50  0001 C CNN
	1    6050 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 5500 6050 5650
Connection ~ 6050 5500
Wire Wire Line
	6050 3350 6050 3200
$Comp
L Device:R R9
U 1 1 61A6BF95
P 6450 2900
F 0 "R9" V 6550 2850 50  0000 L CNN
F 1 "100k" V 6350 2800 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 6380 2900 50  0001 C CNN
F 3 "~" H 6450 2900 50  0001 C CNN
	1    6450 2900
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR021
U 1 1 61A707B4
P 6200 2800
F 0 "#PWR021" H 6200 2650 50  0001 C CNN
F 1 "+5V" H 6215 2973 50  0000 C CNN
F 2 "" H 6200 2800 50  0001 C CNN
F 3 "" H 6200 2800 50  0001 C CNN
	1    6200 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 2800 6200 2900
$Comp
L Device:R R8
U 1 1 61A71EB5
P 6150 2150
F 0 "R8" V 6250 2100 50  0000 L CNN
F 1 "100k" V 6050 2050 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 6080 2150 50  0001 C CNN
F 3 "~" H 6150 2150 50  0001 C CNN
	1    6150 2150
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R7
U 1 1 61A723DB
P 5650 2150
F 0 "R7" V 5750 2100 50  0000 L CNN
F 1 "100k" V 5550 2050 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 5580 2150 50  0001 C CNN
F 3 "~" H 5650 2150 50  0001 C CNN
	1    5650 2150
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR022
U 1 1 61A730B5
P 6450 2250
F 0 "#PWR022" H 6450 2000 50  0001 C CNN
F 1 "GND" H 6455 2077 50  0000 C CNN
F 2 "" H 6450 2250 50  0001 C CNN
F 3 "" H 6450 2250 50  0001 C CNN
	1    6450 2250
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR018
U 1 1 61A75E08
P 5400 2000
F 0 "#PWR018" H 5400 1850 50  0001 C CNN
F 1 "+5V" H 5415 2173 50  0000 C CNN
F 2 "" H 5400 2000 50  0001 C CNN
F 3 "" H 5400 2000 50  0001 C CNN
	1    5400 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 2150 5500 2150
Text HLabel 4750 4600 0    50   Input ~ 0
LRCLK
Text HLabel 4750 4750 0    50   Input ~ 0
BCLK
Text HLabel 4750 4900 0    50   Input ~ 0
DIN
Wire Wire Line
	4750 4600 5000 4600
Wire Wire Line
	5000 4750 4750 4750
Wire Wire Line
	4750 4900 5000 4900
$Comp
L Connector_Generic:Conn_01x02 J5
U 1 1 61A7CEA5
P 7750 4650
F 0 "J5" H 7830 4642 50  0000 L CNN
F 1 "speaker" H 7830 4551 50  0000 L CNN
F 2 "Connector_JST:JST_EH_B2B-EH-A_1x02_P2.50mm_Vertical" H 7750 4650 50  0001 C CNN
F 3 "~" H 7750 4650 50  0001 C CNN
	1    7750 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 2150 5900 2150
Connection ~ 5900 2150
Wire Wire Line
	5900 2150 6000 2150
Wire Wire Line
	5900 2150 5900 3350
Wire Wire Line
	7200 2900 7200 3000
$Comp
L power:GND #PWR023
U 1 1 61A703E5
P 7200 3000
F 0 "#PWR023" H 7200 2750 50  0001 C CNN
F 1 "GND" H 7205 2827 50  0000 C CNN
F 2 "" H 7200 3000 50  0001 C CNN
F 3 "" H 7200 3000 50  0001 C CNN
	1    7200 3000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R10
U 1 1 61A6F6CF
P 6950 2900
F 0 "R10" V 6850 2850 50  0000 L CNN
F 1 "100k" V 7050 2800 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 6880 2900 50  0001 C CNN
F 3 "~" H 6950 2900 50  0001 C CNN
	1    6950 2900
	0    1    1    0   
$EndComp
$Comp
L MAX98357AETE+T:MAX98357AETE+T IC1
U 1 1 61A5F4F7
P 5000 4350
F 0 "IC1" H 6150 3300 50  0000 L CNN
F 1 "MAX98357AETE+T" H 6150 3400 50  0000 L CNN
F 2 "MAX98357AETE+T:QFN50P300X300X80-17N" H 6450 5150 50  0001 L CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/MAX98357A-MAX98357B.pdf" H 6450 5050 50  0001 L CNN
F 4 "Audio Amplifiers Digital Input Class D Amplifier" H 6450 4950 50  0001 L CNN "Description"
F 5 "0.8" H 6450 4850 50  0001 L CNN "Height"
F 6 "Maxim Integrated" H 6450 4750 50  0001 L CNN "Manufacturer_Name"
F 7 "MAX98357AETE+T" H 6450 4650 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "700-MAX98357AETE+T" H 6450 4550 50  0001 L CNN "Mouser Part Number"
F 9 "https://www.mouser.co.uk/ProductDetail/Maxim-Integrated/MAX98357AETE%2bT?qs=AAveGqk956HhNpoJjF5x2g%3D%3D" H 6450 4450 50  0001 L CNN "Mouser Price/Stock"
F 10 "" H 6450 4350 50  0001 L CNN "Arrow Part Number"
F 11 "" H 6450 4250 50  0001 L CNN "Arrow Price/Stock"
	1    5000 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 2900 6700 2900
Wire Wire Line
	7200 2900 7100 2900
Wire Wire Line
	6300 2900 6200 2900
Wire Wire Line
	6700 3200 6700 2900
Wire Wire Line
	6050 3200 6700 3200
Connection ~ 6700 2900
Wire Wire Line
	6700 2900 6800 2900
Wire Wire Line
	6450 2150 6450 2250
Wire Wire Line
	6300 2150 6450 2150
Wire Wire Line
	5400 2000 5400 2150
$Comp
L Device:Ferrite_Bead FB1
U 1 1 61C0C8CF
P 6950 4450
F 0 "FB1" V 7100 4450 50  0000 C CNN
F 1 "Ferrite_Bead" V 6767 4450 50  0000 C CNN
F 2 "Inductor_SMD:L_0805_2012Metric" V 6880 4450 50  0001 C CNN
F 3 "~" H 6950 4450 50  0001 C CNN
	1    6950 4450
	0    1    1    0   
$EndComp
$Comp
L Device:Ferrite_Bead FB2
U 1 1 61C0D7B7
P 6950 4950
F 0 "FB2" V 6800 4950 50  0000 C CNN
F 1 "Ferrite_Bead" V 7100 4950 50  0000 C CNN
F 2 "Inductor_SMD:L_0805_2012Metric" V 6880 4950 50  0001 C CNN
F 3 "~" H 6950 4950 50  0001 C CNN
	1    6950 4950
	0    1    1    0   
$EndComp
Wire Wire Line
	6600 4650 6700 4650
Wire Wire Line
	6700 4650 6700 4450
Wire Wire Line
	6700 4450 6800 4450
Wire Wire Line
	7100 4450 7200 4450
Wire Wire Line
	7200 4450 7200 4650
Wire Wire Line
	7200 4650 7550 4650
Wire Wire Line
	7550 4750 7200 4750
Wire Wire Line
	7200 4750 7200 4950
Wire Wire Line
	7200 4950 7100 4950
Wire Wire Line
	6800 4950 6700 4950
Wire Wire Line
	6700 4950 6700 4750
Wire Wire Line
	6700 4750 6600 4750
$EndSCHEMATC
