#include <iostream>
#include <errno.h>
#include <thread>
#include <vector>
#include <stdexcept>
#include <stdlib.h>
#include <sstream>

#include <loguru/loguru.hpp>
#include <cxxopts/cxxopts.hpp>

#include "drone/Data_Drone.h"
#include "threads/Abstract_ThreadClass.h"
#include "threads/DroneReceiver_ThreadClass.h"
#include "threads/Database_ThreadClass.h"

using namespace std;

#define DRONE_LIMIT 10000

void initDrone(shared_ptr<Drone> drone, string serialPath, int serialBaudrate)
{
    LOG_F(INFO, "Try connecting to drone on %s with baudrate %d", serialPath.data(), serialBaudrate);
    drone->open(serialPath.data(), serialBaudrate);
    if (drone->init_communication() != 0)
    {
        stringstream ss;
        ss << "Cannot init communication : " << strerror(errno);
        throw runtime_error(ss.str());
    }

    LOG_F(INFO, "Try init drone parameters");
    if (drone->init_parameters(DRONE_LIMIT) != 0)
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

    // Read command line arguments
    cxxopts::Options options("Test", "Test2");
    options.add_options()
        ("b,baudrate", "The baudrate to communicate with drone", cxxopts::value<int>()->default_value("57600"))
        ("s,serial", "The path on which communicate with drone", cxxopts::value<string>()->default_value("/dev/ttyUSB0"));
    auto optionsParsed = options.parse(argc, argv);

    int serialBaudrate = optionsParsed["baudrate"].as<int>();
    string serialPath = optionsParsed["serial"].as<string>();

    // In a shared pointer because the final app will share this instance
    // it will be easier to refactor code
    auto drone = make_shared<Drone>();

    try
    {
        initDrone(drone, serialPath, serialBaudrate);
    }
    catch (const std::exception &e)
    {
        LOG_F(ERROR, "Error init drone. %s. Exit the app", e.what());
        sleep(1);
        exit(EXIT_FAILURE);
    }

    vector<unique_ptr<Abstract_ThreadClass>> threads;
    // threads.push_back(make_unique<Database_ThreadClass>(1000, 200));
    threads.push_back(make_unique<DroneReceiver_ThreadClass>(1000, 200, drone));

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