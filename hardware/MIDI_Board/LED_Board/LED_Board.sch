EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 3
Title "Board_TOP"
Date "2021-04-25"
Rev "0"
Comp "Cadsims UK Ltd"
Comment1 "Drawn by: Aravind"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	4700 2650 5650 2650
Text Label 5100 2650 0    50   ~ 0
+3.3V
Wire Wire Line
	4700 2550 5650 2550
Wire Wire Line
	4700 3700 5650 3700
Wire Wire Line
	4700 3600 5650 3600
Text Label 5100 2550 0    50   ~ 0
+5V
Wire Wire Line
	4700 3000 5650 3000
Wire Wire Line
	4700 3300 5650 3300
Wire Wire Line
	4700 3900 5650 3900
Text Label 4950 3000 0    50   ~ 0
ADC0_RDY
Text Label 4950 3300 0    50   ~ 0
ADC1_READY
Text Label 4950 3600 0    50   ~ 0
I2C_SCL
Text Label 4950 3700 0    50   ~ 0
I2C_SDA
Text Label 4950 3900 0    50   ~ 0
IO_EXP_INT_N
Text Notes 4000 1700 0    236  ~ 47
MIDI Board
Text Notes 2850 4550 0    197  ~ 39
MCU\nUSB\nPOWER\nDISPLAY\nENCODER
Text Notes 6550 3800 0    197  ~ 39
ADC\nLED
$Sheet
S 5650 2200 1900 2900
U 60A110B3
F0 "ADC LED" 50
F1 "ADC.sch" 50
F2 "+3.3V" I L 5650 2650 50 
F3 "I2C_SCL" I L 5650 3600 50 
F4 "I2C_SDA" I L 5650 3700 50 
F5 "+5V" I L 5650 2550 50 
F6 "ADC0_RDY" O L 5650 3000 50 
F7 "ADC1_RDY" O L 5650 3300 50 
F8 "IO_EXP_INT_N" O L 5650 3900 50 
$EndSheet
$Sheet
S 2700 2200 2000 2900
U 60A00A51
F0 "MCU USB POWER IO" 50
F1 "MCU_PWR_IO.sch" 50
F2 "+3.3V" O R 4700 2650 50 
F3 "+5V" O R 4700 2550 50 
F4 "ADC0_RDY" I R 4700 3000 50 
F5 "ADC1_RDY" I R 4700 3300 50 
F6 "I2C_SCL" O R 4700 3600 50 
F7 "I2C_SDA" O R 4700 3700 50 
F8 "IO_EXP_INT_N" I R 4700 3900 50 
$EndSheet
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 60A20306
P 8800 3100
F 0 "H1" V 8754 3250 50  0000 L CNN
F 1 "MountingHole_Pad" V 8845 3250 50  0000 L CNN
F 2 "MountingHole:MountingHole_3mm_Pad_Via" H 8800 3100 50  0001 C CNN
F 3 "~" H 8800 3100 50  0001 C CNN
	1    8800 3100
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 60A21578
P 8800 3350
F 0 "H2" V 8754 3500 50  0000 L CNN
F 1 "MountingHole_Pad" V 8845 3500 50  0000 L CNN
F 2 "MountingHole:MountingHole_3mm_Pad_Via" H 8800 3350 50  0001 C CNN
F 3 "~" H 8800 3350 50  0001 C CNN
	1    8800 3350
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H3
U 1 1 60A217E9
P 8800 3600
F 0 "H3" V 8754 3750 50  0000 L CNN
F 1 "MountingHole_Pad" V 8845 3750 50  0000 L CNN
F 2 "MountingHole:MountingHole_3mm_Pad_Via" H 8800 3600 50  0001 C CNN
F 3 "~" H 8800 3600 50  0001 C CNN
	1    8800 3600
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H4
U 1 1 60A21B53
P 8800 3900
F 0 "H4" V 8754 4050 50  0000 L CNN
F 1 "MountingHole_Pad" V 8845 4050 50  0000 L CNN
F 2 "MountingHole:MountingHole_3mm_Pad_Via" H 8800 3900 50  0001 C CNN
F 3 "~" H 8800 3900 50  0001 C CNN
	1    8800 3900
	0    1    1    0   
$EndComp
Wire Wire Line
	8700 3100 8500 3100
Wire Wire Line
	8500 3900 8700 3900
Wire Wire Line
	8700 3600 8500 3600
Connection ~ 8500 3600
Wire Wire Line
	8500 3600 8500 3900
Wire Wire Line
	8700 3350 8500 3350
Wire Wire Line
	8500 3100 8500 3350
Connection ~ 8500 3350
Wire Wire Line
	8500 3350 8500 3600
$Comp
L power:GND #PWR0105
U 1 1 60A222A3
P 8500 4700
F 0 "#PWR0105" H 8500 4450 50  0001 C CNN
F 1 "GND" H 8505 4527 50  0000 C CNN
F 2 "" H 8500 4700 50  0001 C CNN
F 3 "" H 8500 4700 50  0001 C CNN
	1    8500 4700
	1    0    0    -1  
$EndComp
Wire Notes Line width 39
	9650 2900 9650 4650
Wire Notes Line width 39
	9650 4650 8300 4650
Wire Notes Line width 39
	8300 4650 8300 2900
Wire Notes Line width 39
	8300 2900 9650 2900
Text Notes 8300 2800 0    118  ~ 24
Mounting Holes
$Comp
L Mechanical:MountingHole_Pad H5
U 1 1 6117E0CE
P 8800 4200
F 0 "H5" V 8754 4350 50  0000 L CNN
F 1 "MountingHole_Pad" V 8845 4350 50  0000 L CNN
F 2 "MountingHole:MountingHole_3mm_Pad_Via" H 8800 4200 50  0001 C CNN
F 3 "~" H 8800 4200 50  0001 C CNN
	1    8800 4200
	0    1    1    0   
$EndComp
Wire Wire Line
	8500 4200 8700 4200
$Comp
L Mechanical:MountingHole_Pad H6
U 1 1 6117E746
P 8800 4500
F 0 "H6" V 8754 4650 50  0000 L CNN
F 1 "MountingHole_Pad" V 8845 4650 50  0000 L CNN
F 2 "MountingHole:MountingHole_3mm_Pad_Via" H 8800 4500 50  0001 C CNN
F 3 "~" H 8800 4500 50  0001 C CNN
	1    8800 4500
	0    1    1    0   
$EndComp
Wire Wire Line
	8500 4500 8700 4500
Wire Wire Line
	8500 3900 8500 4200
Connection ~ 8500 3900
Connection ~ 8500 4200
Wire Wire Line
	8500 4200 8500 4500
Connection ~ 8500 4500
Wire Wire Line
	8500 4500 8500 4700
$EndSCHEMATC
