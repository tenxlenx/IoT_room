
	
#ifndef ROBOT_CONTROLLER_H
#define MONITOR_STATION_H
 
#include "i2c_transfer.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
 
class Monitor_station
{
    public:
        Monitor_station() {}
         
        //----READ SENSORS--------------------
        std::vector<uint8_t> read_data_i2c() {
            return i2c_transfer.read_data();
        }
         
        
        //-------------------------------------
 
    private:
        I2C_transfer i2c_transfer;
};
 
#endif MONITOR_STATION_H
