#include "../../include/file/Mavlink_file_handler.h"

namespace fs = std::filesystem;

MavlinkFileHandler::MavlinkFileHandler(String folderPath)

{
    folderPath=folderPath;
   
    if(!fs.exists(folderPath))
    {
        fs.create_directory(folderPath);
    }
    
    mavlink_altitude_file = new std::ofstream(folderPath+=("mavlink_altitude_t"));
    mavlink_highres_imu_file = new std::ofstream(folderPath+=("mavlink_highres_imu_t"));
    mavlink_battery_status_file = new std::ofstream(folderPath+=("mavlink_battery_status_t"));

}
