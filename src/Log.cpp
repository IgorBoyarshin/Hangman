#include "Log.h"


Log Log::m_LogInstance{};


Log::Log() : Log(OutputLevel::None) {}


Log::Log(OutputLevel outputLevel) : 
    m_ClassName(""), m_FuncName(""), m_LogLevel(LogLevel::Info) {
        m_OutputLevel = outputLevel;
}


Log& Log::setClass(std::string className) {
    m_ClassName = className;

    return *this;
}


Log& Log::setFunc(std::string funcName) {
    m_FuncName = funcName;

    return *this;
}


Log& Log::setLevel(LogLevel level) {
    m_LogLevel = level;

    return *this;
}


Log& Log::info() {
    return m_LogInstance.setLevel(LogLevel::Info);
}


Log& Log::error() {
    return m_LogInstance.setLevel(LogLevel::Error);
}


std::ostream& operator<<(Log& log, const std::string& str) {
    std::cout << log << str;

    return std::cout;
}


std::ostream& operator<<(std::ostream& stream, const Log& log) {
    stream << "[LOG:" << log.m_LogLevel;
    if (!(log.m_ClassName == "")) stream << ":" << log.m_ClassName;
    if (!(log.m_FuncName == "")) stream << ":" << log.m_FuncName;
    stream << "] ";

    return stream;
}


std::ostream& operator<<(std::ostream& stream, const Log::LogLevel& level) {
    switch (level) {
        case Log::LogLevel::Error:
            stream << "ERROR";
            break;
        case Log::LogLevel::Info:
            stream << "INFO";
            break;
        default:
            stream << "<unknown>";
    }

    return stream;
}
