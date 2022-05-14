EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 8150 7650 0    50   ~ 0
17.11.2021
Text Notes 8150 7650 0    50   ~ 0
17.11.2021
Text Notes 7350 7500 0    79   ~ 0
Key_Pad
NoConn ~ 2900 6900
Text Notes 7000 6650 0    50   ~ 0
Key pad controler for ESP32 Game console. ©Piotr Mijakowski
Text HLabel 1750 3750 0    50   BiDi ~ 0
SCL
Text HLabel 1750 3850 0    50   BiDi ~ 0
SDA
Text HLabel 1750 6300 0    50   BiDi ~ 0
SCL
Text HLabel 1750 6400 0    50   BiDi ~ 0
SDA
Wire Wire Line
	1600 1000 2400 1000
Wire Wire Line
	2400 1000 3250 1000
Connection ~ 2400 1000
Wire Wire Line
	3250 2300 2400 2300
Wire Wire Line
	2400 2300 1600 2300
Connection ~ 2400 2300
$Comp
L power:GND #PWR039
U 1 1 61C8CC6E
P 2400 2450
F 0 "#PWR039" H 2400 2200 50  0001 C CNN
F 1 "GND" H 2405 2277 50  0000 C CNN
F 2 "" H 2400 2450 50  0001 C CNN
F 3 "" H 2400 2450 50  0001 C CNN
	1    2400 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 2450 2400 2300
$Comp
L power:+3.3V #PWR038
U 1 1 61CDDC3D
P 2400 850
F 0 "#PWR038" H 2400 700 50  0001 C CNN
F 1 "+3.3V" H 2415 1023 50  0000 C CNN
F 2 "" H 2400 850 50  0001 C CNN
F 3 "" H 2400 850 50  0001 C CNN
	1    2400 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 850  2400 1000
Text Label 8200 1500 0    50   ~ 0
IC1_AD0
Text Label 9000 1500 0    50   ~ 0
IC1_AD1
Text Label 9850 1500 0    50   ~ 0
IC1_AD2
Text Label 1700 1500 0    50   ~ 0
IC0_AD0
Text Label 2500 1500 0    50   ~ 0
IC0_AD1
Text Label 3350 1500 0    50   ~ 0
IC0_AD2
Wire Wire Line
	3350 1500 3250 1500
Wire Wire Line
	2500 1500 2400 1500
Wire Wire Line
	1600 1500 1700 1500
Text Label 1800 7000 2    50   ~ 0
IC1_AD0
Text Label 1800 6900 2    50   ~ 0
IC1_AD1
Text Label 1800 6800 2    50   ~ 0
IC1_AD2
Wire Wire Line
	1800 7000 1900 7000
Wire Wire Line
	1900 6900 1800 6900
Wire Wire Line
	1800 6800 1900 6800
Text HLabel 1750 6600 0    50   Output ~ 0
~INT1
Wire Wire Line
	1750 6600 1900 6600
Wire Wire Line
	1750 6300 1900 6300
Wire Wire Line
	1900 6400 1750 6400
Text Label 1800 4450 2    50   ~ 0
IC0_AD0
Text Label 1800 4350 2    50   ~ 0
IC0_AD1
Text Label 1800 4250 2    50   ~ 0
IC0_AD2
Wire Wire Line
	1800 4450 1900 4450
Wire Wire Line
	1900 4350 1800 4350
Wire Wire Line
	1800 4250 1900 4250
Text HLabel 1750 4050 0    50   Output ~ 0
~INT0
Wire Wire Line
	1750 4050 1900 4050
$Comp
L power:+3.3V #PWR040
U 1 1 61E383DD
P 2400 3100
F 0 "#PWR040" H 2400 2950 50  0001 C CNN
F 1 "+3.3V" H 2415 3273 50  0000 C CNN
F 2 "" H 2400 3100 50  0001 C CNN
F 3 "" H 2400 3100 50  0001 C CNN
	1    2400 3100
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR042
U 1 1 61E471C3
P 2400 5700
F 0 "#PWR042" H 2400 5550 50  0001 C CNN
F 1 "+3.3V" H 2415 5873 50  0000 C CNN
F 2 "" H 2400 5700 50  0001 C CNN
F 3 "" H 2400 5700 50  0001 C CNN
	1    2400 5700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR041
U 1 1 61E9450B
P 2400 4750
F 0 "#PWR041" H 2400 4500 50  0001 C CNN
F 1 "GND" H 2405 4577 50  0000 C CNN
F 2 "" H 2400 4750 50  0001 C CNN
F 3 "" H 2400 4750 50  0001 C CNN
	1    2400 4750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR043
U 1 1 61ED057F
P 2400 7300
F 0 "#PWR043" H 2400 7050 50  0001 C CNN
F 1 "GND" H 2405 7127 50  0000 C CNN
F 2 "" H 2400 7300 50  0001 C CNN
F 3 "" H 2400 7300 50  0001 C CNN
	1    2400 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 4650 2400 4750
Wire Wire Line
	2400 7200 2400 7300
Wire Wire Line
	2400 5700 2400 5800
$Comp
L Device:C C19
U 1 1 6200764D
P 2150 5800
F 0 "C19" V 1900 5800 50  0000 C CNN
F 1 "100n" V 1989 5800 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 2188 5650 50  0001 C CNN
F 3 "~" H 2150 5800 50  0001 C CNN
	1    2150 5800
	0    1    1    0   
$EndComp
Wire Wire Line
	2300 5800 2400 5800
Connection ~ 2400 5800
$Comp
L power:GND #PWR037
U 1 1 6201022B
P 1650 5900
F 0 "#PWR037" H 1650 5650 50  0001 C CNN
F 1 "GND" H 1655 5727 50  0000 C CNN
F 2 "" H 1650 5900 50  0001 C CNN
F 3 "" H 1650 5900 50  0001 C CNN
	1    1650 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 5900 1650 5800
Wire Wire Line
	1650 5800 2000 5800
$Comp
L Device:C C18
U 1 1 6201965D
P 2150 3200
F 0 "C18" V 1900 3200 50  0000 C CNN
F 1 "100n" V 1989 3200 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 2188 3050 50  0001 C CNN
F 3 "~" H 2150 3200 50  0001 C CNN
	1    2150 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	1650 3300 1650 3200
Wire Wire Line
	1650 3200 2000 3200
Wire Wire Line
	2400 3100 2400 3200
Wire Wire Line
	2300 3200 2400 3200
Connection ~ 2400 3200
$Comp
L power:GND #PWR036
U 1 1 62056B8E
P 1650 3300
F 0 "#PWR036" H 1650 3050 50  0001 C CNN
F 1 "GND" H 1655 3127 50  0000 C CNN
F 2 "" H 1650 3300 50  0001 C CNN
F 3 "" H 1650 3300 50  0001 C CNN
	1    1650 3300
	1    0    0    -1  
$EndComp
Text Label 3000 3650 0    50   ~ 0
B_R
Text Label 3000 3750 0    50   ~ 0
B_A
Text Label 3000 3850 0    50   ~ 0
B_B
Text Label 3000 3950 0    50   ~ 0
B_X
Text Label 3000 4050 0    50   ~ 0
B_Y
Text Label 3000 4150 0    50   ~ 0
B_START
Text Label 3000 4250 0    50   ~ 0
B_SELECT
Text Label 3000 4350 0    50   ~ 0
B_MUTE
Text Label 3000 6200 0    50   ~ 0
B_L
Text Label 3000 6300 0    50   ~ 0
B_UP
Text Label 3000 6400 0    50   ~ 0
B_LEFT
Text Label 3000 6500 0    50   ~ 0
B_RIGHT
Text Label 3000 6600 0    50   ~ 0
B_DOWN
Text Label 3000 6700 0    50   ~ 0
B_VOL_UP
Text Label 3000 6800 0    50   ~ 0
B_VOL_DOWN
Wire Wire Line
	3000 6800 2900 6800
Wire Wire Line
	3000 6700 2900 6700
Wire Wire Line
	3000 6600 2900 6600
Wire Wire Line
	3000 6500 2900 6500
Wire Wire Line
	2900 6400 3000 6400
Wire Wire Line
	3000 6300 2900 6300
Wire Wire Line
	2900 6200 3000 6200
Wire Wire Line
	3000 4350 2900 4350
Wire Wire Line
	3000 4250 2900 4250
Wire Wire Line
	2900 4150 3000 4150
Wire Wire Line
	3000 4050 2900 4050
Wire Wire Line
	2900 3950 3000 3950
Wire Wire Line
	3000 3850 2900 3850
Wire Wire Line
	2900 3750 3000 3750
Wire Wire Line
	3000 3650 2900 3650
Wire Wire Line
	1750 3750 1900 3750
Wire Wire Line
	1900 3850 1750 3850
Wire Wire Line
	8250 3200 8250 3300
Wire Wire Line
	8250 4100 8250 4200
Wire Wire Line
	8250 5000 8250 5100
Wire Wire Line
	8250 5900 8250 6000
Wire Wire Line
	9600 2950 9600 3200
$Comp
L Switch:SW_Push SW?
U 1 1 6216B86D
P 9900 2950
AR Path="/61956A9D/6216B86D" Ref="SW?"  Part="1" 
AR Path="/6196605E/6216B86D" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/6216B86D" Ref="SW?"  Part="1" 
AR Path="/6216B86D" Ref="SW?"  Part="1" 
AR Path="/61E37777/6216B86D" Ref="SW19"  Part="1" 
F 0 "SW19" H 10050 3050 50  0000 C CNN
F 1 "SW_A_B" H 9900 3144 50  0000 C CNN
F 2 "SKPM:SKPMAPE010" H 9900 3150 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 9900 3150 50  0001 C CNN
	1    9900 2950
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 6216B873
P 9900 3200
AR Path="/61956A9D/6216B873" Ref="SW?"  Part="1" 
AR Path="/6196605E/6216B873" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/6216B873" Ref="SW?"  Part="1" 
AR Path="/6216B873" Ref="SW?"  Part="1" 
AR Path="/61E37777/6216B873" Ref="SW20"  Part="1" 
F 0 "SW20" H 10050 3400 50  0000 C CNN
F 1 "SW_A_A" H 9900 3100 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH-12mm" H 9900 3500 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 9900 3500 50  0001 C CNN
	1    9900 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	10100 2950 10200 2950
Wire Wire Line
	10100 3200 10200 3200
Wire Wire Line
	9700 2950 9600 2950
Wire Wire Line
	9600 3200 9700 3200
Wire Wire Line
	10200 2950 10200 3200
$Comp
L power:GND #PWR059
U 1 1 6216B880
P 10300 3300
F 0 "#PWR059" H 10300 3050 50  0001 C CNN
F 1 "GND" H 10305 3127 50  0000 C CNN
F 2 "" H 10300 3300 50  0001 C CNN
F 3 "" H 10300 3300 50  0001 C CNN
	1    10300 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 3200 10300 3200
Wire Wire Line
	10300 3200 10300 3300
Connection ~ 10200 3200
Wire Wire Line
	9600 2950 9500 2950
Connection ~ 9600 2950
Wire Wire Line
	9600 3850 9600 4100
$Comp
L Switch:SW_Push SW?
U 1 1 6216FE81
P 9900 3850
AR Path="/61956A9D/6216FE81" Ref="SW?"  Part="1" 
AR Path="/6196605E/6216FE81" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/6216FE81" Ref="SW?"  Part="1" 
AR Path="/6216FE81" Ref="SW?"  Part="1" 
AR Path="/61E37777/6216FE81" Ref="SW21"  Part="1" 
F 0 "SW21" H 10050 3950 50  0000 C CNN
F 1 "SW_B_B" H 9900 4044 50  0000 C CNN
F 2 "SKPM:SKPMAPE010" H 9900 4050 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 9900 4050 50  0001 C CNN
	1    9900 3850
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 6216FE87
P 9900 4100
AR Path="/61956A9D/6216FE87" Ref="SW?"  Part="1" 
AR Path="/6196605E/6216FE87" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/6216FE87" Ref="SW?"  Part="1" 
AR Path="/6216FE87" Ref="SW?"  Part="1" 
AR Path="/61E37777/6216FE87" Ref="SW22"  Part="1" 
F 0 "SW22" H 10050 4300 50  0000 C CNN
F 1 "SW_B_A" H 9900 4000 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH-12mm" H 9900 4400 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 9900 4400 50  0001 C CNN
	1    9900 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	10100 3850 10200 3850
Wire Wire Line
	10100 4100 10200 4100
Wire Wire Line
	9700 3850 9600 3850
Wire Wire Line
	9600 4100 9700 4100
Wire Wire Line
	10200 3850 10200 4100
$Comp
L power:GND #PWR060
U 1 1 6216FE94
P 10300 4200
F 0 "#PWR060" H 10300 3950 50  0001 C CNN
F 1 "GND" H 10305 4027 50  0000 C CNN
F 2 "" H 10300 4200 50  0001 C CNN
F 3 "" H 10300 4200 50  0001 C CNN
	1    10300 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 4100 10300 4100
Wire Wire Line
	10300 4100 10300 4200
Connection ~ 10200 4100
Wire Wire Line
	9600 3850 9500 3850
Connection ~ 9600 3850
Wire Wire Line
	9600 4750 9600 5000
$Comp
L Switch:SW_Push SW?
U 1 1 62174A14
P 9900 4750
AR Path="/61956A9D/62174A14" Ref="SW?"  Part="1" 
AR Path="/6196605E/62174A14" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/62174A14" Ref="SW?"  Part="1" 
AR Path="/62174A14" Ref="SW?"  Part="1" 
AR Path="/61E37777/62174A14" Ref="SW23"  Part="1" 
F 0 "SW23" H 10050 4850 50  0000 C CNN
F 1 "SW_X_B" H 9900 4944 50  0000 C CNN
F 2 "SKPM:SKPMAPE010" H 9900 4950 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 9900 4950 50  0001 C CNN
	1    9900 4750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 62174A1A
P 9900 5000
AR Path="/61956A9D/62174A1A" Ref="SW?"  Part="1" 
AR Path="/6196605E/62174A1A" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/62174A1A" Ref="SW?"  Part="1" 
AR Path="/62174A1A" Ref="SW?"  Part="1" 
AR Path="/61E37777/62174A1A" Ref="SW24"  Part="1" 
F 0 "SW24" H 10050 5200 50  0000 C CNN
F 1 "SW_X_A" H 9900 4900 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH-12mm" H 9900 5300 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 9900 5300 50  0001 C CNN
	1    9900 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10100 4750 10200 4750
Wire Wire Line
	10100 5000 10200 5000
Wire Wire Line
	9700 4750 9600 4750
Wire Wire Line
	9600 5000 9700 5000
Wire Wire Line
	10200 4750 10200 5000
$Comp
L power:GND #PWR061
U 1 1 62174A27
P 10300 5100
F 0 "#PWR061" H 10300 4850 50  0001 C CNN
F 1 "GND" H 10305 4927 50  0000 C CNN
F 2 "" H 10300 5100 50  0001 C CNN
F 3 "" H 10300 5100 50  0001 C CNN
	1    10300 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 5000 10300 5000
Wire Wire Line
	10300 5000 10300 5100
Connection ~ 10200 5000
Wire Wire Line
	9600 4750 9500 4750
Connection ~ 9600 4750
Wire Wire Line
	9600 5650 9600 5900
$Comp
L Switch:SW_Push SW?
U 1 1 6217A6BD
P 9900 5650
AR Path="/61956A9D/6217A6BD" Ref="SW?"  Part="1" 
AR Path="/6196605E/6217A6BD" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/6217A6BD" Ref="SW?"  Part="1" 
AR Path="/6217A6BD" Ref="SW?"  Part="1" 
AR Path="/61E37777/6217A6BD" Ref="SW25"  Part="1" 
F 0 "SW25" H 10050 5750 50  0000 C CNN
F 1 "SW_Y_B" H 9900 5844 50  0000 C CNN
F 2 "SKPM:SKPMAPE010" H 9900 5850 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 9900 5850 50  0001 C CNN
	1    9900 5650
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 6217A6C3
P 9900 5900
AR Path="/61956A9D/6217A6C3" Ref="SW?"  Part="1" 
AR Path="/6196605E/6217A6C3" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/6217A6C3" Ref="SW?"  Part="1" 
AR Path="/6217A6C3" Ref="SW?"  Part="1" 
AR Path="/61E37777/6217A6C3" Ref="SW26"  Part="1" 
F 0 "SW26" H 10050 6100 50  0000 C CNN
F 1 "SW_Y_A" H 9900 5800 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH-12mm" H 9900 6200 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 9900 6200 50  0001 C CNN
	1    9900 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	10100 5650 10200 5650
Wire Wire Line
	10100 5900 10200 5900
Wire Wire Line
	9700 5650 9600 5650
Wire Wire Line
	9600 5900 9700 5900
Wire Wire Line
	10200 5650 10200 5900
$Comp
L power:GND #PWR062
U 1 1 6217A6D0
P 10300 6000
F 0 "#PWR062" H 10300 5750 50  0001 C CNN
F 1 "GND" H 10305 5827 50  0000 C CNN
F 2 "" H 10300 6000 50  0001 C CNN
F 3 "" H 10300 6000 50  0001 C CNN
	1    10300 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 5900 10300 5900
Wire Wire Line
	10300 5900 10300 6000
Connection ~ 10200 5900
Wire Wire Line
	9600 5650 9500 5650
Connection ~ 9600 5650
$Comp
L power:GND #PWR048
U 1 1 621BBCD0
P 5950 3700
F 0 "#PWR048" H 5950 3450 50  0001 C CNN
F 1 "GND" H 5955 3527 50  0000 C CNN
F 2 "" H 5950 3700 50  0001 C CNN
F 3 "" H 5950 3700 50  0001 C CNN
	1    5950 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 3600 5950 3700
Wire Wire Line
	5750 3600 5950 3600
Wire Wire Line
	5150 3600 5350 3600
$Comp
L Switch:SW_Push SW?
U 1 1 621DA142
P 5550 4300
AR Path="/61956A9D/621DA142" Ref="SW?"  Part="1" 
AR Path="/6196605E/621DA142" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/621DA142" Ref="SW?"  Part="1" 
AR Path="/621DA142" Ref="SW?"  Part="1" 
AR Path="/61E37777/621DA142" Ref="SW7"  Part="1" 
F 0 "SW7" H 5700 4500 50  0000 C CNN
F 1 "SW_SELECT" H 5550 4200 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_Push_1P1T_NO_6x6mm_H9.5mm" H 5550 4600 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 5550 4600 50  0001 C CNN
	1    5550 4300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR049
U 1 1 621DA14A
P 5950 4400
F 0 "#PWR049" H 5950 4150 50  0001 C CNN
F 1 "GND" H 5955 4227 50  0000 C CNN
F 2 "" H 5950 4400 50  0001 C CNN
F 3 "" H 5950 4400 50  0001 C CNN
	1    5950 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 4300 5950 4400
Wire Wire Line
	5750 4300 5950 4300
Wire Wire Line
	5150 4300 5350 4300
$Comp
L Switch:SW_Push SW?
U 1 1 621E00F1
P 5550 5100
AR Path="/61956A9D/621E00F1" Ref="SW?"  Part="1" 
AR Path="/6196605E/621E00F1" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/621E00F1" Ref="SW?"  Part="1" 
AR Path="/621E00F1" Ref="SW?"  Part="1" 
AR Path="/61E37777/621E00F1" Ref="SW8"  Part="1" 
F 0 "SW8" H 5700 5300 50  0000 C CNN
F 1 "SW_VOL_UP" H 5550 5000 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_Push_1P1T_NO_6x6mm_H9.5mm" H 5550 5400 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 5550 5400 50  0001 C CNN
	1    5550 5100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR050
U 1 1 621E00F9
P 5950 5200
F 0 "#PWR050" H 5950 4950 50  0001 C CNN
F 1 "GND" H 5955 5027 50  0000 C CNN
F 2 "" H 5950 5200 50  0001 C CNN
F 3 "" H 5950 5200 50  0001 C CNN
	1    5950 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 5100 5950 5200
Wire Wire Line
	5750 5100 5950 5100
Wire Wire Line
	5150 5100 5350 5100
$Comp
L Switch:SW_Push SW?
U 1 1 621E6DF5
P 5550 5850
AR Path="/61956A9D/621E6DF5" Ref="SW?"  Part="1" 
AR Path="/6196605E/621E6DF5" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/621E6DF5" Ref="SW?"  Part="1" 
AR Path="/621E6DF5" Ref="SW?"  Part="1" 
AR Path="/61E37777/621E6DF5" Ref="SW9"  Part="1" 
F 0 "SW9" H 5700 6050 50  0000 C CNN
F 1 "SW_VOL_DOWN" H 5550 5750 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_Push_1P1T_NO_6x6mm_H9.5mm" H 5550 6150 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 5550 6150 50  0001 C CNN
	1    5550 5850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR051
U 1 1 621E6DFD
P 5950 5950
F 0 "#PWR051" H 5950 5700 50  0001 C CNN
F 1 "GND" H 5955 5777 50  0000 C CNN
F 2 "" H 5950 5950 50  0001 C CNN
F 3 "" H 5950 5950 50  0001 C CNN
	1    5950 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 5850 5950 5950
Wire Wire Line
	5750 5850 5950 5850
Wire Wire Line
	5150 5850 5350 5850
$Comp
L power:GND #PWR046
U 1 1 621EDC85
P 5950 2150
F 0 "#PWR046" H 5950 1900 50  0001 C CNN
F 1 "GND" H 5955 1977 50  0000 C CNN
F 2 "" H 5950 2150 50  0001 C CNN
F 3 "" H 5950 2150 50  0001 C CNN
	1    5950 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 2050 5950 2150
Wire Wire Line
	5750 2050 5950 2050
Wire Wire Line
	5150 2050 5350 2050
$Comp
L Switch:SW_SPST SW4
U 1 1 621FC60D
P 5550 2050
F 0 "SW4" H 5550 2200 50  0000 C CNN
F 1 "SW_L" H 5550 1900 50  0000 C CNN
F 2 "Button_Switch_THT:SW_Tactile_SPST_Angled_PTS645Vx39-2LFS" H 5550 2050 50  0001 C CNN
F 3 "~" H 5550 2050 50  0001 C CNN
	1    5550 2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR047
U 1 1 621FE330
P 5950 2850
F 0 "#PWR047" H 5950 2600 50  0001 C CNN
F 1 "GND" H 5955 2677 50  0000 C CNN
F 2 "" H 5950 2850 50  0001 C CNN
F 3 "" H 5950 2850 50  0001 C CNN
	1    5950 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 2750 5950 2850
Wire Wire Line
	5750 2750 5950 2750
Wire Wire Line
	5150 2750 5350 2750
$Comp
L Switch:SW_SPST SW5
U 1 1 621FE340
P 5550 2750
F 0 "SW5" H 5550 2900 50  0000 C CNN
F 1 "SW_R" H 5550 2600 50  0000 C CNN
F 2 "Button_Switch_THT:SW_Tactile_SPST_Angled_PTS645Vx39-2LFS" H 5550 2750 50  0001 C CNN
F 3 "~" H 5550 2750 50  0001 C CNN
	1    5550 2750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 621BBCC3
P 5550 3600
AR Path="/61956A9D/621BBCC3" Ref="SW?"  Part="1" 
AR Path="/6196605E/621BBCC3" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/621BBCC3" Ref="SW?"  Part="1" 
AR Path="/621BBCC3" Ref="SW?"  Part="1" 
AR Path="/61E37777/621BBCC3" Ref="SW6"  Part="1" 
F 0 "SW6" H 5700 3800 50  0000 C CNN
F 1 "SW_START" H 5550 3500 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_Push_1P1T_NO_6x6mm_H9.5mm" H 5550 3900 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 5550 3900 50  0001 C CNN
	1    5550 3600
	1    0    0    -1  
$EndComp
Text Label 5150 2050 2    50   ~ 0
B_L
Text Label 5150 2750 2    50   ~ 0
B_R
$Comp
L Switch:SW_Push SW?
U 1 1 61C3DC2D
P 5550 6600
AR Path="/61956A9D/61C3DC2D" Ref="SW?"  Part="1" 
AR Path="/6196605E/61C3DC2D" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/61C3DC2D" Ref="SW?"  Part="1" 
AR Path="/61C3DC2D" Ref="SW?"  Part="1" 
AR Path="/61E37777/61C3DC2D" Ref="SW10"  Part="1" 
F 0 "SW10" H 5700 6800 50  0000 C CNN
F 1 "SW_MUTE" H 5550 6500 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_Push_1P1T_NO_6x6mm_H9.5mm" H 5550 6900 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 5550 6900 50  0001 C CNN
	1    5550 6600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR052
U 1 1 61C3DC35
P 5950 6700
F 0 "#PWR052" H 5950 6450 50  0001 C CNN
F 1 "GND" H 5955 6527 50  0000 C CNN
F 2 "" H 5950 6700 50  0001 C CNN
F 3 "" H 5950 6700 50  0001 C CNN
	1    5950 6700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 6600 5950 6700
Wire Wire Line
	5750 6600 5950 6600
Wire Wire Line
	5150 6600 5350 6600
Text Label 5150 3600 2    50   ~ 0
B_START
Text Label 5150 4300 2    50   ~ 0
B_SELECT
Text Label 5150 5100 2    50   ~ 0
B_VOL_UP
Text Label 5150 5850 2    50   ~ 0
B_VOL_DOWN
Text Label 5150 6600 2    50   ~ 0
B_MUTE
Text Label 9500 2950 2    50   ~ 0
B_A
Text Label 9500 3850 2    50   ~ 0
B_B
Text Label 9500 4750 2    50   ~ 0
B_X
Text Label 9500 5650 2    50   ~ 0
B_Y
$Comp
L Device:R R22
U 1 1 61CC2D2A
P 1600 1250
F 0 "R22" H 1670 1296 50  0000 L CNN
F 1 "10k" H 1670 1205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1530 1250 50  0001 C CNN
F 3 "~" H 1600 1250 50  0001 C CNN
	1    1600 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper JP1
U 1 1 61CC8845
P 1600 1900
F 0 "JP1" V 1646 1812 50  0000 R CNN
F 1 "Jumper" V 1555 1812 50  0000 R CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 1600 1900 50  0001 C CNN
F 3 "~" H 1600 1900 50  0001 C CNN
	1    1600 1900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1600 2300 1600 2200
Wire Wire Line
	1600 1600 1600 1500
Wire Wire Line
	1600 1100 1600 1000
Connection ~ 1600 1500
Wire Wire Line
	1600 1500 1600 1400
$Comp
L Device:R R23
U 1 1 61D2FDEC
P 2400 1250
F 0 "R23" H 2470 1296 50  0000 L CNN
F 1 "10k" H 2470 1205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 2330 1250 50  0001 C CNN
F 3 "~" H 2400 1250 50  0001 C CNN
	1    2400 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper JP2
U 1 1 61D2FDF2
P 2400 1900
F 0 "JP2" V 2446 1812 50  0000 R CNN
F 1 "Jumper" V 2355 1812 50  0000 R CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 2400 1900 50  0001 C CNN
F 3 "~" H 2400 1900 50  0001 C CNN
	1    2400 1900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2400 2300 2400 2200
$Comp
L Device:R R24
U 1 1 61D37909
P 3250 1250
F 0 "R24" H 3320 1296 50  0000 L CNN
F 1 "10k" H 3320 1205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 3180 1250 50  0001 C CNN
F 3 "~" H 3250 1250 50  0001 C CNN
	1    3250 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper JP3
U 1 1 61D3790F
P 3250 1900
F 0 "JP3" V 3296 1812 50  0000 R CNN
F 1 "Jumper" V 3205 1812 50  0000 R CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 3250 1900 50  0001 C CNN
F 3 "~" H 3250 1900 50  0001 C CNN
	1    3250 1900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3250 2300 3250 2200
Wire Wire Line
	2400 1400 2400 1500
Wire Wire Line
	3250 1400 3250 1500
Connection ~ 3250 1500
Wire Wire Line
	3250 1500 3250 1600
Connection ~ 2400 1500
Wire Wire Line
	2400 1500 2400 1600
Wire Wire Line
	3250 1100 3250 1000
Wire Wire Line
	2400 1100 2400 1000
Wire Wire Line
	8100 1000 8900 1000
Wire Wire Line
	8900 1000 9750 1000
Connection ~ 8900 1000
Wire Wire Line
	9750 2300 8900 2300
Wire Wire Line
	8900 2300 8100 2300
Connection ~ 8900 2300
$Comp
L power:GND #PWR058
U 1 1 61D78CE8
P 8900 2450
F 0 "#PWR058" H 8900 2200 50  0001 C CNN
F 1 "GND" H 8905 2277 50  0000 C CNN
F 2 "" H 8900 2450 50  0001 C CNN
F 3 "" H 8900 2450 50  0001 C CNN
	1    8900 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	8900 2450 8900 2300
$Comp
L power:+3.3V #PWR057
U 1 1 61D78CEF
P 8900 850
F 0 "#PWR057" H 8900 700 50  0001 C CNN
F 1 "+3.3V" H 8915 1023 50  0000 C CNN
F 2 "" H 8900 850 50  0001 C CNN
F 3 "" H 8900 850 50  0001 C CNN
	1    8900 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	8900 850  8900 1000
Wire Wire Line
	9850 1500 9750 1500
Wire Wire Line
	9000 1500 8900 1500
Wire Wire Line
	8100 1500 8200 1500
$Comp
L Device:R R25
U 1 1 61D78CFC
P 8100 1250
F 0 "R25" H 8170 1296 50  0000 L CNN
F 1 "10k" H 8170 1205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 8030 1250 50  0001 C CNN
F 3 "~" H 8100 1250 50  0001 C CNN
	1    8100 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper JP4
U 1 1 61D78D02
P 8100 1900
F 0 "JP4" V 8146 1812 50  0000 R CNN
F 1 "Jumper" V 8055 1812 50  0000 R CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 8100 1900 50  0001 C CNN
F 3 "~" H 8100 1900 50  0001 C CNN
	1    8100 1900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8100 2300 8100 2200
Wire Wire Line
	8100 1600 8100 1500
Wire Wire Line
	8100 1100 8100 1000
Connection ~ 8100 1500
Wire Wire Line
	8100 1500 8100 1400
$Comp
L Device:R R26
U 1 1 61D78D0D
P 8900 1250
F 0 "R26" H 8970 1296 50  0000 L CNN
F 1 "10k" H 8970 1205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 8830 1250 50  0001 C CNN
F 3 "~" H 8900 1250 50  0001 C CNN
	1    8900 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper JP5
U 1 1 61D78D13
P 8900 1900
F 0 "JP5" V 8946 1812 50  0000 R CNN
F 1 "Jumper" V 8855 1812 50  0000 R CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 8900 1900 50  0001 C CNN
F 3 "~" H 8900 1900 50  0001 C CNN
	1    8900 1900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8900 2300 8900 2200
$Comp
L Device:R R27
U 1 1 61D78D1A
P 9750 1250
F 0 "R27" H 9820 1296 50  0000 L CNN
F 1 "10k" H 9820 1205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 9680 1250 50  0001 C CNN
F 3 "~" H 9750 1250 50  0001 C CNN
	1    9750 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper JP6
U 1 1 61D78D20
P 9750 1900
F 0 "JP6" V 9796 1812 50  0000 R CNN
F 1 "Jumper" V 9705 1812 50  0000 R CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 9750 1900 50  0001 C CNN
F 3 "~" H 9750 1900 50  0001 C CNN
	1    9750 1900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9750 2300 9750 2200
Wire Wire Line
	8900 1400 8900 1500
Wire Wire Line
	9750 1400 9750 1500
Connection ~ 9750 1500
Wire Wire Line
	9750 1500 9750 1600
Connection ~ 8900 1500
Wire Wire Line
	8900 1500 8900 1600
Wire Wire Line
	9750 1100 9750 1000
Wire Wire Line
	8900 1100 8900 1000
Text Label 7450 5650 2    50   ~ 0
B_RIGHT
Text Label 7450 4750 2    50   ~ 0
B_LEFT
Text Label 7450 3850 2    50   ~ 0
B_DOWN
Text Label 7450 2950 2    50   ~ 0
B_UP
Connection ~ 7550 5650
Wire Wire Line
	7550 5650 7450 5650
Connection ~ 8150 5900
Wire Wire Line
	8150 5900 8250 5900
$Comp
L power:GND #PWR056
U 1 1 6216718E
P 8250 6000
F 0 "#PWR056" H 8250 5750 50  0001 C CNN
F 1 "GND" H 8255 5827 50  0000 C CNN
F 2 "" H 8250 6000 50  0001 C CNN
F 3 "" H 8250 6000 50  0001 C CNN
	1    8250 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 5650 8150 5900
Wire Wire Line
	7550 5900 7650 5900
Wire Wire Line
	7650 5650 7550 5650
Wire Wire Line
	8050 5900 8150 5900
Wire Wire Line
	8050 5650 8150 5650
$Comp
L Switch:SW_Push SW?
U 1 1 62167181
P 7850 5900
AR Path="/61956A9D/62167181" Ref="SW?"  Part="1" 
AR Path="/6196605E/62167181" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/62167181" Ref="SW?"  Part="1" 
AR Path="/62167181" Ref="SW?"  Part="1" 
AR Path="/61E37777/62167181" Ref="SW18"  Part="1" 
F 0 "SW18" H 8000 6100 50  0000 C CNN
F 1 "SW_RIGHT_A" H 7850 5800 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH-12mm" H 7850 6200 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 7850 6200 50  0001 C CNN
	1    7850 5900
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 6216717B
P 7850 5650
AR Path="/61956A9D/6216717B" Ref="SW?"  Part="1" 
AR Path="/6196605E/6216717B" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/6216717B" Ref="SW?"  Part="1" 
AR Path="/6216717B" Ref="SW?"  Part="1" 
AR Path="/61E37777/6216717B" Ref="SW17"  Part="1" 
F 0 "SW17" H 8000 5750 50  0000 C CNN
F 1 "SW_RIGHT_B" H 7850 5844 50  0000 C CNN
F 2 "SKPM:SKPMAPE010" H 7850 5850 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 7850 5850 50  0001 C CNN
	1    7850 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 5650 7550 5900
Connection ~ 7550 4750
Wire Wire Line
	7550 4750 7450 4750
Connection ~ 8150 5000
Wire Wire Line
	8150 5000 8250 5000
$Comp
L power:GND #PWR055
U 1 1 62163AF5
P 8250 5100
F 0 "#PWR055" H 8250 4850 50  0001 C CNN
F 1 "GND" H 8255 4927 50  0000 C CNN
F 2 "" H 8250 5100 50  0001 C CNN
F 3 "" H 8250 5100 50  0001 C CNN
	1    8250 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 4750 8150 5000
Wire Wire Line
	7550 5000 7650 5000
Wire Wire Line
	7650 4750 7550 4750
Wire Wire Line
	8050 5000 8150 5000
Wire Wire Line
	8050 4750 8150 4750
$Comp
L Switch:SW_Push SW?
U 1 1 62163AE8
P 7850 5000
AR Path="/61956A9D/62163AE8" Ref="SW?"  Part="1" 
AR Path="/6196605E/62163AE8" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/62163AE8" Ref="SW?"  Part="1" 
AR Path="/62163AE8" Ref="SW?"  Part="1" 
AR Path="/61E37777/62163AE8" Ref="SW16"  Part="1" 
F 0 "SW16" H 8000 5200 50  0000 C CNN
F 1 "SW_LEFT_A" H 7850 4900 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH-12mm" H 7850 5300 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 7850 5300 50  0001 C CNN
	1    7850 5000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 62163AE2
P 7850 4750
AR Path="/61956A9D/62163AE2" Ref="SW?"  Part="1" 
AR Path="/6196605E/62163AE2" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/62163AE2" Ref="SW?"  Part="1" 
AR Path="/62163AE2" Ref="SW?"  Part="1" 
AR Path="/61E37777/62163AE2" Ref="SW15"  Part="1" 
F 0 "SW15" H 8000 4850 50  0000 C CNN
F 1 "SW_LEFT_B" H 7850 4944 50  0000 C CNN
F 2 "SKPM:SKPMAPE010" H 7850 4950 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 7850 4950 50  0001 C CNN
	1    7850 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 4750 7550 5000
Connection ~ 7550 3850
Wire Wire Line
	7550 3850 7450 3850
Connection ~ 8150 4100
Wire Wire Line
	8150 4100 8250 4100
$Comp
L power:GND #PWR054
U 1 1 62160E58
P 8250 4200
F 0 "#PWR054" H 8250 3950 50  0001 C CNN
F 1 "GND" H 8255 4027 50  0000 C CNN
F 2 "" H 8250 4200 50  0001 C CNN
F 3 "" H 8250 4200 50  0001 C CNN
	1    8250 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 3850 8150 4100
Wire Wire Line
	7550 4100 7650 4100
Wire Wire Line
	7650 3850 7550 3850
Wire Wire Line
	8050 4100 8150 4100
Wire Wire Line
	8050 3850 8150 3850
$Comp
L Switch:SW_Push SW?
U 1 1 62160E4B
P 7850 4100
AR Path="/61956A9D/62160E4B" Ref="SW?"  Part="1" 
AR Path="/6196605E/62160E4B" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/62160E4B" Ref="SW?"  Part="1" 
AR Path="/62160E4B" Ref="SW?"  Part="1" 
AR Path="/61E37777/62160E4B" Ref="SW14"  Part="1" 
F 0 "SW14" H 8000 4300 50  0000 C CNN
F 1 "SW_DOWN_A" H 7850 4000 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH-12mm" H 7850 4400 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 7850 4400 50  0001 C CNN
	1    7850 4100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 62160E45
P 7850 3850
AR Path="/61956A9D/62160E45" Ref="SW?"  Part="1" 
AR Path="/6196605E/62160E45" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/62160E45" Ref="SW?"  Part="1" 
AR Path="/62160E45" Ref="SW?"  Part="1" 
AR Path="/61E37777/62160E45" Ref="SW13"  Part="1" 
F 0 "SW13" H 8000 3950 50  0000 C CNN
F 1 "SW_DOWN_B" H 7850 4044 50  0000 C CNN
F 2 "SKPM:SKPMAPE010" H 7850 4050 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 7850 4050 50  0001 C CNN
	1    7850 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 3850 7550 4100
Connection ~ 7550 2950
Wire Wire Line
	7550 2950 7450 2950
Connection ~ 8150 3200
Wire Wire Line
	8150 3200 8250 3200
$Comp
L power:GND #PWR053
U 1 1 62159D4B
P 8250 3300
F 0 "#PWR053" H 8250 3050 50  0001 C CNN
F 1 "GND" H 8255 3127 50  0000 C CNN
F 2 "" H 8250 3300 50  0001 C CNN
F 3 "" H 8250 3300 50  0001 C CNN
	1    8250 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 2950 8150 3200
Wire Wire Line
	7550 3200 7650 3200
Wire Wire Line
	7650 2950 7550 2950
Wire Wire Line
	8050 3200 8150 3200
Wire Wire Line
	8050 2950 8150 2950
$Comp
L Switch:SW_Push SW?
U 1 1 6213C4E4
P 7850 3200
AR Path="/61956A9D/6213C4E4" Ref="SW?"  Part="1" 
AR Path="/6196605E/6213C4E4" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/6213C4E4" Ref="SW?"  Part="1" 
AR Path="/6213C4E4" Ref="SW?"  Part="1" 
AR Path="/61E37777/6213C4E4" Ref="SW12"  Part="1" 
F 0 "SW12" H 8000 3400 50  0000 C CNN
F 1 "SW_UP_A" H 7850 3100 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH-12mm" H 7850 3500 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 7850 3500 50  0001 C CNN
	1    7850 3200
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 6213C4DE
P 7850 2950
AR Path="/61956A9D/6213C4DE" Ref="SW?"  Part="1" 
AR Path="/6196605E/6213C4DE" Ref="SW?"  Part="1" 
AR Path="/61A2DE08/6213C4DE" Ref="SW?"  Part="1" 
AR Path="/6213C4DE" Ref="SW?"  Part="1" 
AR Path="/61E37777/6213C4DE" Ref="SW11"  Part="1" 
F 0 "SW11" H 8000 3050 50  0000 C CNN
F 1 "SW_UP_B" H 7850 3144 50  0000 C CNN
F 2 "SKPM:SKPMAPE010" H 7850 3150 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 7850 3150 50  0001 C CNN
	1    7850 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 2950 7550 3200
$Comp
L power:GND #PWR045
U 1 1 61B8917B
P 3150 5900
F 0 "#PWR045" H 3150 5650 50  0001 C CNN
F 1 "GND" H 3155 5727 50  0000 C CNN
F 2 "" H 3150 5900 50  0001 C CNN
F 3 "" H 3150 5900 50  0001 C CNN
	1    3150 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C21
U 1 1 61B910F3
P 2700 5800
F 0 "C21" V 2950 5750 50  0000 L CNN
F 1 "10u" V 2850 5800 50  0000 C CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3528-21_Kemet-B_Pad1.50x2.35mm_HandSolder" H 2738 5650 50  0001 C CNN
F 3 "~" H 2700 5800 50  0001 C CNN
	1    2700 5800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2400 5800 2550 5800
Wire Wire Line
	2850 5800 3150 5800
Wire Wire Line
	3150 5800 3150 5900
$Comp
L Device:CP C20
U 1 1 61BEFD2E
P 2700 3200
F 0 "C20" V 2950 3150 50  0000 L CNN
F 1 "10u" V 2850 3200 50  0000 C CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3528-21_Kemet-B_Pad1.50x2.35mm_HandSolder" H 2738 3050 50  0001 C CNN
F 3 "~" H 2700 3200 50  0001 C CNN
	1    2700 3200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2400 3200 2550 3200
Wire Wire Line
	2850 3200 3150 3200
Wire Wire Line
	3150 3200 3150 3300
$Comp
L power:GND #PWR044
U 1 1 61BF893D
P 3150 3300
F 0 "#PWR044" H 3150 3050 50  0001 C CNN
F 1 "GND" H 3155 3127 50  0000 C CNN
F 2 "" H 3150 3300 50  0001 C CNN
F 3 "" H 3150 3300 50  0001 C CNN
	1    3150 3300
	1    0    0    -1  
$EndComp
$Comp
L Interface_Expansion:MCP23008-xSO U5
U 1 1 61BB036D
P 2400 4050
F 0 "U5" H 2800 3450 50  0000 C CNN
F 1 "MCP23008-xSO" H 1850 3450 50  0000 C CNN
F 2 "Package_SO:SOIC-18W_7.5x11.6mm_P1.27mm" H 2400 3000 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/MCP23008-MCP23S08-Data-Sheet-20001919F.pdf" H 3700 2850 50  0001 C CNN
	1    2400 4050
	1    0    0    -1  
$EndComp
$Comp
L Interface_Expansion:MCP23008-xSO U6
U 1 1 61BB1726
P 2400 6600
F 0 "U6" H 2000 6000 50  0000 C CNN
F 1 "MCP23008-xSO" H 2750 6000 50  0000 C CNN
F 2 "Package_SO:SOIC-18W_7.5x11.6mm_P1.27mm" H 2400 5550 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/MCP23008-MCP23S08-Data-Sheet-20001919F.pdf" H 3700 5400 50  0001 C CNN
	1    2400 6600
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR034
U 1 1 61C139EE
P 1300 3450
F 0 "#PWR034" H 1300 3300 50  0001 C CNN
F 1 "+3.3V" H 1315 3623 50  0000 C CNN
F 2 "" H 1300 3450 50  0001 C CNN
F 3 "" H 1300 3450 50  0001 C CNN
	1    1300 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 3450 1300 3650
Wire Wire Line
	1300 3650 1900 3650
$Comp
L power:+3.3V #PWR035
U 1 1 61C6DD9B
P 1300 6000
F 0 "#PWR035" H 1300 5850 50  0001 C CNN
F 1 "+3.3V" H 1315 6173 50  0000 C CNN
F 2 "" H 1300 6000 50  0001 C CNN
F 3 "" H 1300 6000 50  0001 C CNN
	1    1300 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 6000 1300 6200
Wire Wire Line
	1300 6200 1900 6200
Wire Wire Line
	2400 3200 2400 3450
Wire Wire Line
	2400 5800 2400 6000
$EndSCHEMATC
