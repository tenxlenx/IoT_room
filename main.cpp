// C++ includes
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include "monitor_station.h"
#include "json.hpp"
#include <mosquitto.h>
#include <string>
#include "mqtt_publisher.h"
// for convenience
using json = nlohmann::json;


float convertToPercentage(uint8_t num) {

	if (num == 0) {
		return 0;
	}
	float max = 5;
	float min = 28;
	float diff = min-max;

	float tmp = num-max;
	float numReturn = 100-(tmp/diff*100);
	if (numReturn > 100 || numReturn < 0) {
	    numReturn = 0;
	}


	return numReturn;
}

int main() {

	
	
    Monitor_station monitor;
  
    char *host = "demo.thingsboard.io";
    char *access_token = "rhfLJQ3py7F64YYz4WQH";
    char *access_token_tanks = "wkuNzThe4xNZg1RPCIu4";

    Mqtt_publisher mqtt(access_token,"v1/devices/me/telemetry", host, 1883);
    Mqtt_publisher mqtt_tank(access_token_tanks,"v1/devices/me/telemetry", host, 1883);

    std::cout << " start monitoring " << std::endl;

    int dtnum = 0;
    while (1) {
    	std::vector<uint8_t> read_array = monitor.read_data_i2c();

    	json dht22, tanks;
    	dht22["temperature"] = read_array[4];
    	dht22["humidity"] = read_array[5];

    	tanks["tank0"] = convertToPercentage(read_array[0]);
    	tanks["tank1"] = convertToPercentage(read_array[1]);
    	tanks["tank2"] = convertToPercentage(read_array[2]);
    	tanks["tank3"] = convertToPercentage(read_array[3]);

		mqtt.send_message(dht22.dump().c_str());
		mqtt_tank.send_message(tanks.dump().c_str());
		
    
    	printf("data read %d->\n tank0:%u\n tank1:%u\n tank2:%u\n tank3:%u\n temperature:%u\n humidity:%u\n<---", 
            dtnum, read_array[0], read_array[1], read_array[2], read_array[3], read_array[4], read_array[5]);
    	
    	std::this_thread::sleep_for(std::chrono::seconds(6));
    	dtnum++;
    }
    
    return 0;
}
