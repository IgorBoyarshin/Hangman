#include <iostream>

#include "Client.h"
#include "Server.h"
#include "Log.h"


int main() {
    std::cout << "---------- Start ----------" << std::endl;

    Log::error() << "df" << std::endl;

    // Log log;
    // log.setLevel(Log::LogLevel::Info) << std::endl;


    std::cout << "----------  End  ----------" << std::endl;
    return 0;
}
