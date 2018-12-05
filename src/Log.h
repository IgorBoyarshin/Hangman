#ifndef LOG_H
#define LOG_H


#include <iostream>
#include <fstream>


class Log {
    public:
        enum class LogLevel : unsigned char { Error, Info };
        enum class OutputLevel : unsigned char { Verbose, Basic, None };
        enum class Destination : unsigned char { Console, File };

    private:
        OutputLevel m_OutputLevel;
        std::string m_ClassName;
        std::string m_FuncName;
        LogLevel m_LogLevel;
        Destination m_Destination;
        std::ofstream m_File;

        Log();
        Log(OutputLevel outputLevel);
        ~Log();

        static Log m_LogInstance;

    public:
        Log& setClass(std::string className);
        Log& setFunc(std::string funcName);
        Log& setLevel(LogLevel level);
        static void setDestination(Destination destination);
        static Log& info();
        static Log& error();
        static Log& inst();

        friend std::ostream& operator<<(Log& log, const std::string& str);
        friend std::ostream& operator<<(std::ostream& stream, const Log& log);
        friend std::ostream& operator<<(std::ostream& stream, const LogLevel& level);
};


#endif
