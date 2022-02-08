#ifndef __DATABASE_THREADCLASS_H__
#define __DATABASE_THREADCLASS_H__

/**
 * The thread that will wait for messages from the drone and store them in database.
 * 
 * @author Aldric Vitali Silvestre
 */

#include "./Abstract_ThreadClass.h"
#include "../drone/Data_Drone.h"

class Database_ThreadClass : public Abstract_ThreadClass
{
private:
    // TODO Add connectivity to postgresql (maybe with a wrapper)
public:
    Database_ThreadClass(int task_period, int task_deadline);
    ~Database_ThreadClass();

    void run();
};
#endif // __DATABASE_THREADCLASS_H__