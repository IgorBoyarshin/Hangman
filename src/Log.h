#ifndef LOG_H
#define LOG_H


#include <iostream>


class Log {
    public:
        enum class LogLevel : unsigned char { Error, Info };
        enum class OutputLevel : unsigned char { Verbose, Basic, None };

    private:
        OutputLevel m_OutputLevel;
        std::string m_ClassName;
        std::string m_FuncName;
        LogLevel m_LogLevel;

    public:
        Log();
        Log(OutputLevel outputLevel);

    // private:
        Log& setClass(std::string className);
        Log& setFunc(std::string funcName);
        Log& setLevel(LogLevel level);


    friend std::ostream& operator<<(std::ostream& stream, const Log& log);
    friend std::ostream& operator<<(std::ostream& stream, const LogLevel& level);
};


#endif
