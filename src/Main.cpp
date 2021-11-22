#include <iostream>
#include <errno.h>
#include <thread>
#include <vector>
#include <stdexcept>
#include <stdlib.h>

#include "../lib/loguru/loguru.hpp"
#include <pqxx/pqxx>

#include "../include/drone/Data_Drone.h"
#include "../include/threads/Abstract_ThreadClass.h"
#include "../include/threads/DroneReceiver_ThreadClass.h"
#include "../include/threads/Database_ThreadClass.h"

#include "blc_channel.h"
#include "blc_program.h"

using namespace std;

#define DRONE_LIMIT 10000

// Define all blc channels
blc_channel blc_control_arm("/pixhawk.control.arm", BLC_CHANNEL_READ, 'IN16', 'NDEF', 1, 1); // 1=on, 0=off;
blc_channel blc_control_remote_vector("/pixhawk.control.remoteVectors", BLC_CHANNEL_WRITE, 'FL32', 'NDEF', 1, 4);
    //Channels use to control manually the drone  
blc_channel blc_control_motors("/pixhawk.control.motors", BLC_CHANNEL_READ, 'FL32', 'NDEF', 1, 6); //x, y, z, r

    //Channels use to command the drone
blc_channel blc_control_commands("/pixhawk.control.commands", BLC_CHANNEL_READ, 'FL32', 'NDEF', 1, 7); // 1=on, 0=off
blc_channel blc_highres_imu("/pixhawk.sensors.imu", BLC_CHANNEL_WRITE, 'FL32', 'NDEF', 1, 9);
	//Channel use to know altitude of drone
blc_channel blc_attitude("/pixhawk.sensors.attitude", BLC_CHANNEL_WRITE, 'FL32', 'NDEF', 1, 7);
    //Channel use to know GPS values of drone
blc_channel blc_local_position_ned("/pixhawk.sensors.local_position_ned", BLC_CHANNEL_WRITE, 'FL32', 'NDEF', 1, 6);
blc_channel blc_global_position("/pixhawk.sensors.global_position", BLC_CHANNEL_WRITE, 'FL32', 'NDEF', 1, 3);

    //Different blc channel use to publish data
blc_channel blc_heartbeat; //Hearbeat message  PG: PB ????? pas d'init  pour l'instant ????
blc_channel blc_test_detection("/pixhawk.test.detection", BLC_CHANNEL_READ, 'IN16', 'NDEF', 1, 7);

void initDrone(shared_ptr<Drone> drone, char *serialPath, int serialBaudrate)
{
    LOG_F(INFO, "Try connecting to drone on %s with baudrate %d", serialPath, serialBaudrate);
    drone.get()->open(serialPath, serialBaudrate);
    if (drone.get()->init_communication() != 0)
    {
        stringstream ss;
        ss << "Cannot init communication : " << strerror(errno);
        throw runtime_error(ss.str());
    }

    LOG_F(INFO, "Try init drone parameters");
    if (drone.get()->init_parameters(DRONE_LIMIT) != 0)
    {
        stringstream ss;
        ss << "Cannot init parameters : " << strerror(errno);
        throw runtime_error(ss.str());
    }
}

int main(int argc, char *argv[])
{
    loguru::init(argc, argv);
    LOG_F(INFO, "Start the 'DroneCollector' application");

    // In a shared pointer because the final app will share this instance
    // it will be easier to refactor code
    shared_ptr<Drone> drone(new Drone());
    char *serialPath = "/dev/ttyACM0";
    int serialBaudrate = 57600;

    try
    {
        initDrone(drone, serialPath, serialBaudrate);
    }
    catch (const std::exception &e)
    {
        delete drone.get();
        LOG_F(ERROR, "Error init drone. %s. Exit the app", e.what());
        sleep(1);
        exit(EXIT_FAILURE);
    }

    vector<Abstract_ThreadClass *> threads;
    threads.push_back(new Database_ThreadClass(1000, 200));
    threads.push_back(new DroneReceiver_ThreadClass(1000, 200, drone));

    // start all threads
    LOG_F(INFO, "Stored %ld threads", threads.size());
    for (auto &thread : threads)
    {
        thread->start();
    }

    for (auto &thread : threads)
    {
        thread->join();
    }

    return EXIT_SUCCESS;
}