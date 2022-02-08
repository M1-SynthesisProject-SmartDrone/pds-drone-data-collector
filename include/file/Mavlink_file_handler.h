/**
 * file_handle for read a file and write on it 
 * @author Enzo Kalinowski
 */
#include "../../lib/mavlink/common/mavlink.h"
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <fstream> 
#include <filesystem>


class MavlinkFileHandler
{    

private:
    std::fstream mavlink_altitude_fileStream ;
    std::fstream mavlink_highres_imu_fileStream ;
    std::fstream mavlink_battery_status_fileStream ;

    std::string folderPath;

    char separator;

public:

    MavlinkFileHandler(std::string folderPath);
    void write_mavlink_altitude_file(mavlink_altitude_t altitude);
    void write_mavlink_highres_imu_file(mavlink_highres_imu_t highres_imu);
    void write_mavlink_battery_status_file(mavlink_battery_status_t battery_status);
    
};