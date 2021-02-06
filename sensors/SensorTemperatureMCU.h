/*
* The MySensors Arduino library handles the wireless radio link and protocol
* between your home built sensors/actuators and HA controller of choice.
* The sensors forms a self healing radio network with optional repeaters. Each
* repeater and gateway builds a routing tables in EEPROM which keeps track of the
* network topology allowing messages to be routed to nodes.
*
* Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
* Copyright (C) 2013-2017 Sensnology AB
* Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
*
* Documentation: http://www.mysensors.org
* Support Forum: http://forum.mysensors.org
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* version 2 as published by the Free Software Foundation.
*/
#ifndef SensorTemperatureMCU_h
#define SensorTemperatureMCU_h

/*
SensorTemperatureMCU: read internal MCU temperature sensor 
*/

class SensorTemperatureMCU: public Sensor {
public:
	SensorTemperatureMCU(uint8_t child_id = 0): Sensor(-1) {
		_name = "TEMPERATURE_MCU";
		children.allocateBlocks(1);
		new Child(this, FLOAT, nodeManager.getAvailableChildId(child_id), S_TEMP, V_TEMP, _name);
	};


	// define what to do during setup
	void onSetup() {
	};

	// define what to do during setup
	void onLoop(Child* child) {
		float temperature = 0;
		
#ifdef CHIP_NRF5		
		//copy-paste from MySensors\hal\architecture\NRF5\MyHwNRF5.cpp
		//hwCPUTemperature return INT, instead of float with 0.25 precision
		nrf_temp_init();

		for (byte i = 0; i < 10; i++) {

			NRF_TEMP->TASKS_START = 1; /** Start the temperature measurement. */

			/* Busy wait while temperature measurement is not finished, you can skip waiting if you enable interrupt for DATARDY event and read the result in the interrupt. */
			/*lint -e{845} // A zero has been given as right argument to operator '|'" */
			while (NRF_TEMP->EVENTS_DATARDY == 0) {
				// Do nothing.
			}

			NRF_TEMP->EVENTS_DATARDY = 0;

			/**@note Workaround for PAN_028 rev2.0A anomaly 29 - TEMP: Stop task clears the TEMP register. */
			temperature += nrf_temp_read();

			/**@note Workaround for PAN_028 rev2.0A anomaly 30 - TEMP: Temp module analog front end does not power down when DATARDY event occurs. */
			NRF_TEMP->TASKS_STOP = 1; /** Stop the temperature measurement. */

			delay(10);
		}

		//10 values for average
		//Temperature in degC (0.25deg steps)
		temperature = temperature / 40;

#else
		temperature = hwCPUTemperature();
#endif		
		if ( temperature != -127 )
			child->setValue(temperature);
	};

};
#endif
