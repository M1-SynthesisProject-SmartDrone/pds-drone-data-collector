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

class MavlinkFileHandler
{
private:

    std::ofstream mavlink_altitude_file ;
    std::ofstream mavlink_highres_imu_file ;
    std::ofstream mavlink_battery_status_file ;




public:

    MavlinkFileHandler(char* altitude_filepath, char* highres_imu_filepath, char* battery_status_filepath);


};