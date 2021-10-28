/**
 * 
 * @author Aldric Vitali Silvestre
 */

#include "../../lib/loguru/loguru.hpp"

#include "../../include/threads/Database_ThreadClass.h"

Database_ThreadClass::Database_ThreadClass(int task_period, int task_deadline)
    : Abstract_ThreadClass(task_period, task_deadline)
{
}

Database_ThreadClass::~Database_ThreadClass()
{
}

void Database_ThreadClass::run()
{
    loguru::set_thread_name("database");
    LOG_F(INFO, "Start Database thread class");
}