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

// Standard LiPo Cell application 
const int MAX1720X_ADDR = 0x36;
const int MAX1720X_VCELL_ADDR = 0x09;       // Lowest cell voltage of a pack, or the cell voltage for a single cell
const int MAX1720X_TEMP_ADDR = 0x08;        // Temperature
const int MAX1720X_VBAT_ADDR = 0xDA;        // Battery pack voltage
const int MAX1720X_AVCELL_ADDR = 0x17;      // Battery cycles
const int MAX1720X_COMMAND_ADDR = 0x60;     // Command register
const int MAX1720X_CONFIG2_ADDR = 0xbb;     // Command register
const int MAX1720X_REPSOC_ADDR = 0x06;      // Reported state of charge
const int MAX1720X_CURENT_ADDR = 0x0A;      // Battery current

//Primary Cell application ONLY
const int MAX1720X_RAW_COULOMB_QH_ADDR = 0x4D; // Coulomb Register QH added - Friedl
const int MAX1720X_RAW_COULOMB_QL_ADDR = 0x4E; // Coulomb Register QL added - Friedl

// Class for interfacing the MAX1720X Li+ fuel gauges
class max1720x
{
public:
	max1720x();
    double getVoltage();
    double getTemperature();
    uint8_t reset();
    double getCoulombsQH();
    double getCoulombsQL();
    double getSOC();
    double getCurrent();


private:
    uint16_t getStatus();
};

#endif  // max1720x
