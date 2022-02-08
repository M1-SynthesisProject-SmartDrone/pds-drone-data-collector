/**
 * This test will add a row in the testdb database (owned by tester)
 * and display all rows that are in table test.
 * 
 * This is a simple test to unsure that libpqxx is working correctly.
 * 
 * @author Aldric Vitali Silvestre
 */

#include "../lib/loguru/loguru.hpp"

#include <string>
#include <pqxx/pqxx>

#include <iostream>
#include <ctime>
#include <unistd.h>

// * Constants to connect to database
#define DB_USER "tester"
#define DB_DATABASE_NAME "testdb"
#define DB_PASSWORD "testmdp"
#define DB_HOST "127.0.0.1"
#define DB_PORT "5432"

std::string gen_random(const int len);
void insertRandomRow(pqxx::connection &connection);
void displayAllRows(pqxx::connection &connection);
void displayAllRowsStream(pqxx::connection &connection);

// int main(int argc, char *argv[])
// {
//     loguru::init(argc, argv);
//     LOG_F(INFO, "Start TestLibpqxx app");

//     std::stringstream connectionStringStream;
//     connectionStringStream << "user=" << DB_USER << " password=" << DB_PASSWORD << " dbname=" << DB_DATABASE_NAME << " hostaddr=" << DB_HOST << " port=" << DB_PORT;

//     LOG_F(INFO, "Connect with user \"%s\" in database \"%s\"", DB_USER, DB_DATABASE_NAME);

//     try
//     {
//         // Create the connection
//         pqxx::connection connection(connectionStringStream.str());
//         if (connection.is_open())
//         {
//             LOG_F(INFO, "Connected to database");
//         }
//         else
//         {
//             LOG_F(ERROR, "Cannot open database");
//             return EXIT_FAILURE;
//         }

//         insertRandomRow(connection);
//         displayAllRows(connection);
//     }
//     catch (std::exception &exception)
//     {
//         LOG_F(ERROR, "Error with database : %s", exception.what());
//         return EXIT_FAILURE;
//     }

//     return EXIT_SUCCESS;
// }

void insertRandomRow(pqxx::connection &connection)
{
    // Prepare the transaction statement
    // NOTE : all prepared statements have to be done when creating connection, in order to avoid doing it again and again
    connection.prepare("insert_test", "INSERT INTO test(name, num_tel) VALUES($1, $2);");

    // Start a transaction
    std::string name = gen_random(10);
    std::string num = gen_random(10);
    pqxx::work transaction(connection);
    LOG_F(INFO, "Insert new name : %s with num", name.c_str());
    transaction.exec_prepared("insert_test", name, "0102030405");
    transaction.commit();
}

void displayAllRows(pqxx::connection &connection)
{
    pqxx::work transaction(connection);
    pqxx::result result(transaction.exec("SELECT * FROM test;"));
    LOG_F(INFO, "Display all rows");
    // This can be better to use stream_to, in order to don't have all in memory
    for (auto row : result)
    {
        std::cout << "Name : " << row["name"] << " Num : " << row["num_tel"] << std::endl;
    }
    transaction.commit();
}

void displayAllRowsStream(pqxx::connection &connection)
{
    // DONT WORK !! (not really important for now)
    // pqxx::work transaction(connection);
    // auto stream = pqxx::stream_from(transaction, "SELECT * FROM test;");
    // // must have the form of the wanted row result
    // std::tuple<int, std::string, std::string> row;
    // while(stream >> row)
    // {
    //     std::cout << "Name : " << std::get<1>(row) << " Num : " << std::get<2>(row) << std::endl;
    // }
    // transaction.commit();
}

// https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
std::string gen_random(const int len)
{
    std::string tmp_s;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    srand((unsigned)time(NULL) * getpid());

    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];

    return tmp_s;
}