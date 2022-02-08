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

using namespace std;

#define DRONE_LIMIT 10000

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
    auto drone = make_shared<Drone>();
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