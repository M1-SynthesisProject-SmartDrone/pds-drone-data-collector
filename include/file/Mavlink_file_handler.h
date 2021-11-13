/**
 * file_handle for read a file and write on it 
 */
#include "../../lib/mavlink/common/mavlink.h"
#include <iostream>
#include <string>
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <fstream> 
#include <filesystem>


class MavlinkFileHandler
{    

private:
    std::ofstream mavlink_altitude_file ;
    std::ofstream mavlink_highres_imu_file ;
    std::ofstream mavlink_battery_status_file ;
    char* folderPath;



public:

    MavlinkFileHandler(char* folderPath);
    void altitudeFilePathWrite(mavlink_altitude_t altitude);
    void highresImuFilePathWrite(mavlink_highres_imu_t highres_imu);
    void batteryStatusFilepathWrite(mavlink_battery_status_t battery_status);


};