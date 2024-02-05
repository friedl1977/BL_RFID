/**
 * Name: max1720x
 * Author: Luka Mustafa - Institute IRNAS Race { info@irnas.eu }
 * Version: 1.0
 * Description: A library for interfacing the MAXIM MAX17201/MAX17205
 * 				Li+ fuel gauges.
 * Source: https://github.com/pAIgn10/LiFuelGauge
 * License: Copyright (c) 2017 Nick Lamprianidis 
 *          This library is licensed under the GPL license
 *          http://www.opensource.org/licenses/mit-license.php
 * Inspiration: The library is inspired by: https://github.com/pAIgn10/LiFuelGauge
 * Filename: max1720x.h
 * File description: Definitions and methods for the max1720x library
 */

#ifndef max1720x
#define max1720x_h

//#include <Arduino.h>
#include <Wire.h>

const int MAX1720X_ADDR = 0x36;
const int MAX1720X_STATUS_ADDR = 0x00;      // Contains alert status and chip status
const int MAX1720X_VCELL_ADDR = 0x09;       // Lowest cell voltage of a pack, or the cell voltage for a single cell
const int MAX1720X_REPSOC_ADDR = 0x06;      // Reported state of charge
const int MAX1720X_REPCAP_ADDR = 0x05;      // Reported remaining capacity
const int MAX1720X_TEMP_ADDR = 0x08;        // Temperature
const int MAX1720X_CURENT_ADDR = 0x0A;      // Battery current
const int MAX1720X_TTE_ADDR = 0x11;         // Time to empty
const int MAX1720X_TTF_ADDR = 0x20;         // Time to full
const int MAX1720X_CAPACITY_ADDR = 0x10;    // Full capacity estimation
const int MAX1720X_VBAT_ADDR = 0xDA;        // Battery pack voltage
const int MAX1720X_AVCELL_ADDR = 0x17;      // Battery cycles
const int MAX1720X_COMMAND_ADDR = 0x60;     // Command register
const int MAX1720X_CONFIG2_ADDR = 0xbb;     // Command register
//const int MAX1720X_CAPACITY_ADDR = 0x10; // Full capacity estimation

//Primary Cell application ONLY
const int MAX1720X_RAW_COULOMB_QH_ADDR = 0x4D; // Coulomb Register QH added - Friedl
const int MAX1720X_RAW_COULOMB_QL_ADDR = 0x4E; // Coulomb Register QL added - Friedl

typedef struct Max1720xConfiguration {
	/// Capacity in mA. The conversion will be done in the configure method
	uint16_t capacity = 0x0000;

	/// Alternate Initial Value: 0xA561 (VE=3.3V, VR=3.88V)
	/// The nVempty register sets thresholds related to empty
	/// detection during operation. Figure 23 shows the register
	/// format.
	/// VE: Empty Voltage. Sets the voltage level for detecting empty.
	/// A 20mV resolution gives a 0 to 5.11V range. This value is writ-
	/// ten to 3.3V after reset if nonvolatile backup is disabled.
	/// VR: Recovery Voltage. Sets the voltage level for clearing
	/// empty detection. Once the cell voltage rises above this
	/// point, empty voltage detection is reenabled. A 40mV reso-
	/// lution gives a 0 to 5.08V range. This value is written to
	/// 3.88V after reset if nonvolatile backup is disabled.
	//uint16_t vEmpty = 0x0000; // 0x19E
	//uint16_t learnCfg = 0x2602; // 0x19F

	//uint16_t qRTable00 = 0x3C00; // 0x1A0
	//uint16_t qRTable10 = 0x1B80; // 0x1A1
	//uint16_t qRTable20 = 0x0B04; // 0x1A2
	//uint16_t qRTable30 = 0x0885; // 0x1A3

	/// Typical current seen in by the application a the point
	/// where the cell reaches empty. Factory default 0x0000
	//uint16_t iAvgEmpty = 0x0000; // 0x1A8

	/// The nConvgCfg register configures operation of the
	/// converge to empty feature. The recommended value for
	/// nConvgCfg is 0x2241
	//uint16_t convgCfg = 0x2241; // 0x1B7

	// uint16_t nVCfg0 = 0x0100; // 0x1B8
	// uint16_t nVCfg1 = 0x0006; // 0x1B9
	// uint16_t nVCfg2 = 0xFF0A; // 0x1BA
} Max1720xConfiguration;

// Class for interfacing the MAX1720X Li+ fuel gauges
class max1720x
{
public:
	max1720x();
    double getVoltage();
    double getSOC();
    double getTemperature();
    double getCurrent();
    double getCapacity();
    double getTTE();//
    double getTTF();
    double getCoulombsQL();
    double getCoulombsQH();
    //double setFULLCAP();
    uint8_t reset();
    
private:
    uint16_t getStatus();
};

#endif  // max1720x
