 /**
 * @file Mavlink_file_handler.cpp
 * @author Enzo Kalinowski
 * 
 */
#include "../../include/file/Mavlink_file_handler.h"

 namespace fs= std::filesystem;


MavlinkFileHandler::MavlinkFileHandler(std::string folderPath)
{
    folderPath=folderPath;
    separator= ';';
    if(!fs::exists(folderPath))
    {
        if(!fs::create_directory(folderPath))
        {
            exit(1);
        }
    }
    mavlink_altitude_fileStream = std::fstream(folderPath+"/mavlink_altitude_t_.csv", std::ios_base::out );
    mavlink_highres_imu_fileStream = std::fstream(folderPath+"/mavlink_highres_imu_t.csv", std::ios_base::out );
    mavlink_battery_status_fileStream = std::fstream(folderPath+"/mavlink_battery_status_t.csv", std::ios_base::out );
   
}


void MavlinkFileHandler::write_mavlink_altitude_file(mavlink_altitude_t altitude)
{
    std::string filePath=folderPath+"/mavlink_altitude_t_.csv";

    if(mavlink_altitude_fileStream.tellg()<=0)
    {
        mavlink_altitude_fileStream.setf(std::ios_base::fixed);
        mavlink_altitude_fileStream << "time_usec"<<separator<< "altitude_monotonic" <<separator<< "altitude_amsl" <<separator<< "altitude_local" <<separator<< "altitude_relative" <<separator<< "altitude_terrain" <<separator<< "bottom_clearance" << std::endl ;
    }
        
    mavlink_altitude_fileStream << altitude.time_usec <<separator<< altitude.altitude_monotonic <<separator<< altitude.altitude_amsl <<separator<< altitude.altitude_local <<separator<< altitude.altitude_relative <<separator<< altitude.altitude_terrain <<separator<< altitude.bottom_clearance << std::endl; 
}

void MavlinkFileHandler::write_mavlink_highres_imu_file(mavlink_highres_imu_t highres_imu)
{
    std::string filePath=folderPath+"/mavlink_highres_imu_t.csv";
    std::string buffer;
    if(mavlink_highres_imu_fileStream.tellg()<=0)
    {
        mavlink_highres_imu_fileStream.setf(std::ios_base::fixed);
        //buffer="time_usec" +separator+ "xacc" +separator+ "yacc" +separator+ "zacc" +separator+ "xgyro" +separator+ "ygyro" +separator+ "zgyro" +separator+ "xmag" +separator+ "ymag" +separator+ "zmag" +separator+ "abs_pressure" +separator+ "diff_pressure" +separator+ "pressure_alt" +separator+ "temperature" +separator+ "field_updated";
        mavlink_highres_imu_fileStream << "time_usec" <<separator<< "xacc" <<separator<< "yacc" <<separator<< "zacc" <<separator<< "xgyro" <<separator<< "ygyro" <<separator<< "zgyro" <<separator<< "xmag" <<separator<< "ymag" <<separator<< "zmag" <<separator<< "abs_pressure" <<separator<< "diff_pressure" <<separator<< "pressure_alt" <<separator<< "temperature" <<separator<< "field_updated" <<  std::endl ;
    }
       // buffer=highres_imu.time_usec +separator+ highres_imu.xacc +separator+ highres_imu.yacc +separator+ highres_imu.zacc +separator+ highres_imu.xgyro +separator+ highres_imu.ygyro +separator+ highres_imu.zgyro +separator+ highres_imu.xmag +separator+ highres_imu.ymag +separator+ highres_imu.zmag +separator+ highres_imu.abs_pressure +separator+ highres_imu.diff_pressure +separator+ highres_imu.pressure_alt +separator+ highres_imu.temperature +separator+ highres_imu.fields_updated ;
        mavlink_highres_imu_fileStream << highres_imu.time_usec <<separator<< highres_imu.xacc <<separator<< highres_imu.yacc <<separator<< highres_imu.zacc <<separator<< highres_imu.xgyro <<separator<< highres_imu.ygyro <<separator<< highres_imu.zgyro <<separator<< highres_imu.xmag <<separator<< highres_imu.ymag <<separator<< highres_imu.zmag <<separator<< highres_imu.abs_pressure <<separator<< highres_imu.diff_pressure <<separator<< highres_imu.pressure_alt <<separator<< highres_imu.temperature <<separator<< highres_imu.fields_updated <<  std::endl ;
}

void MavlinkFileHandler::write_mavlink_battery_status_file(mavlink_battery_status_t battery_status)
{
    std::string filePath=folderPath+"/mavlink_battery_status_t.csv";

    if(mavlink_battery_status_fileStream.tellg()<=0)
    {
        mavlink_battery_status_fileStream << "current_consumed" <<separator<< "energy_consumed" <<separator<< "temperature" <<separator<< "voltages" <<separator<< "current_battery" <<separator<< "id" <<separator<< "battery_function" <<separator<< "type" <<separator<< "battery_remaining" <<separator<< "time_remaining" <<separator<< "charge_state"  <<  std::endl ;
    }
        
        mavlink_battery_status_fileStream << battery_status.current_consumed <<separator<< battery_status.energy_consumed <<separator<< battery_status.temperature <<separator<< battery_status.voltages <<separator<< battery_status.current_battery <<separator<< battery_status.id <<separator<< battery_status.battery_function <<separator<< battery_status.type <<separator<< battery_status.battery_remaining <<separator<< battery_status.time_remaining <<separator<< battery_status.charge_state <<  std::endl ;
}
