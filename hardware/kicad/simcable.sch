EESchema Schematic File Version 2  date Di 21 Jun 2011 14:36:44 CEST
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:smartcard
LIBS:SIMtrace-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 1
Title "SIM card for FFC"
Date "21 jun 2011"
Rev "v1.0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ISO7816_CARD SIM1
U 1 1 4E0085A5
P 5900 4000
F 0 "SIM1" H 5900 4250 60  0000 C CNN
F 1 "ISO7816_CARD" H 5900 3750 60  0000 C CNN
	1    5900 4000
	1    0    0    -1  
$EndComp
$Comp
L FFC P3
U 1 1 4E0084E2
P 5900 5250
F 0 "P3" H 5950 5600 60  0000 C CNN
F 1 "FFC" V 6100 5250 60  0000 C CNN
	1    5900 5250
	0    1    1    0   
$EndComp
$Comp
L FFC P2
U 1 1 4E0084DD
P 7550 4000
F 0 "P2" H 7600 4350 60  0000 C CNN
F 1 "FFC" V 7750 4000 60  0000 C CNN
	1    7550 4000
	1    0    0    -1  
$EndComp
$Comp
L FFC P1
U 1 1 4E0084D9
P 5900 2750
F 0 "P1" H 5950 3100 60  0000 C CNN
F 1 "FFC" V 6100 2750 60  0000 C CNN
	1    5900 2750
	0    -1   -1   0   
$EndComp
$Comp
L FFC P4
U 1 1 4E0084D4
P 4200 4000
F 0 "P4" H 4250 4350 60  0000 C CNN
F 1 "FFC" V 4400 4000 60  0000 C CNN
	1    4200 4000
	-1   0    0    1   
$EndComp
NoConn ~ 5350 4150
NoConn ~ 6450 4150
Text Label 5650 3150 3    60   ~ 0
VCC
Text Label 5750 3150 3    60   ~ 0
RST
Text Label 5850 3150 3    60   ~ 0
CLK
Text Label 5950 3150 3    60   ~ 0
I/O
Text Label 6050 3150 3    60   ~ 0
VPP
Text Label 6150 3150 3    60   ~ 0
GND
Text Label 5650 4800 1    60   ~ 0
GND
Text Label 5750 4800 1    60   ~ 0
VPP
Text Label 5850 4800 1    60   ~ 0
I/O
Text Label 5950 4800 1    60   ~ 0
CLK
Text Label 6050 4800 1    60   ~ 0
RST
Text Label 6150 4800 1    60   ~ 0
VCC
Text Label 7000 4250 0    60   ~ 0
GND
Text Label 7000 4150 0    60   ~ 0
VPP
Text Label 7000 4050 0    60   ~ 0
I/O
Text Label 7000 3950 0    60   ~ 0
CLK
Text Label 7000 3850 0    60   ~ 0
RST
Text Label 7000 3750 0    60   ~ 0
VCC
Text Label 6500 4050 0    60   ~ 0
I/O
Text Label 6500 3950 0    60   ~ 0
VPP
Text Label 6500 3850 0    60   ~ 0
GND
Text Label 5150 4050 0    60   ~ 0
CLK
Text Label 5150 3950 0    60   ~ 0
RST
Text Label 5150 3850 0    60   ~ 0
VCC
Wire Wire Line
	7200 4250 6950 4250
Wire Wire Line
	7200 4050 6950 4050
Wire Wire Line
	7200 3850 6950 3850
Wire Wire Line
	6050 3100 6050 3350
Wire Wire Line
	5850 3100 5850 3350
Wire Wire Line
	5650 3100 5650 3350
Wire Wire Line
	6700 4050 6450 4050
Wire Wire Line
	6450 3950 6700 3950
Wire Wire Line
	5100 3950 5350 3950
Wire Wire Line
	5650 4900 5650 4600
Wire Wire Line
	5850 4900 5850 4600
Wire Wire Line
	6050 4900 6050 4600
Wire Wire Line
	4550 4250 4800 4250
Wire Wire Line
	4550 4050 4800 4050
Wire Wire Line
	4550 3850 4800 3850
Wire Wire Line
	4550 3750 4800 3750
Wire Wire Line
	4550 3950 4800 3950
Wire Wire Line
	4550 4150 4800 4150
Wire Wire Line
	6150 4900 6150 4600
Wire Wire Line
	5950 4900 5950 4600
Wire Wire Line
	5750 4900 5750 4600
Wire Wire Line
	5100 3850 5350 3850
Wire Wire Line
	5100 4050 5350 4050
Wire Wire Line
	6450 3850 6700 3850
Wire Wire Line
	5750 3100 5750 3350
Wire Wire Line
	5950 3100 5950 3350
Wire Wire Line
	6150 3100 6150 3350
Wire Wire Line
	7200 3950 6950 3950
Wire Wire Line
	7200 3750 6950 3750
Wire Wire Line
	7200 4150 6950 4150
Text Label 4600 4250 0    60   ~ 0
VCC
Text Label 4600 4150 0    60   ~ 0
RST
Text Label 4600 4050 0    60   ~ 0
CLK
Text Label 4600 3950 0    60   ~ 0
I/O
Text Label 4600 3850 0    60   ~ 0
VPP
Text Label 4600 3750 0    60   ~ 0
GND
$EndSCHEMATC
