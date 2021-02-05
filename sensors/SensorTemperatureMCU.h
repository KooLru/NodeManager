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
		float temperature = hwCPUTemperature();
		if ( temperature != -127 )
			child->setValue(temperature);
	};

};
#endif
