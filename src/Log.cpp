#include "Log.h"


Log Log::m_LogInstance{};


Log::Log() : Log(OutputLevel::None) {}


Log::Log(OutputLevel outputLevel)
    : m_OutputLevel(outputLevel),
      m_ClassName(""), m_FuncName(""),
      m_LogLevel(LogLevel::Info),
      m_Destination(Destination::Console) {}


Log::~Log() {
    if (m_Destination == Destination::File) {
        m_File.close();
    }
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


void Log::setDestination(Destination destination, const std::string& fileName/* = ""*/) {
    m_LogInstance.m_Destination = destination;
    if (destination == Destination::File) {
        if (m_LogInstance.m_File.is_open()) m_LogInstance.m_File.close(); // done with previous file (if any)
        m_LogInstance.m_File.open(fileName, std::ios::trunc);
    }
}


Log& Log::info() {
    return m_LogInstance.setLevel(LogLevel::Info);
}


Log& Log::error() {
    return m_LogInstance.setLevel(LogLevel::Error);
}


Log& Log::inst() {
    return m_LogInstance;
}

Log& Log::resetContext() {
    m_LogInstance.m_ClassName = "";
    m_LogInstance.m_FuncName = "";
    return m_LogInstance;
}


std::ostream& operator<<(Log& log, const std::string& str) {
    switch (log.m_Destination) {
        case Log::Destination::Console:
            std::cout << log << str;
            return std::cout;
        case Log::Destination::File:
            log.m_File << log << str;
            return log.m_File;
        default:
            // TODO: error
            exit(-1);
            break;
    }
}


std::ostream& operator<<(std::ostream& stream, const Log& log) {
    stream << "[LOG:" << log.m_LogLevel;
    if (!(log.m_ClassName == "")) stream << ":" << log.m_ClassName;
    if (!(log.m_FuncName == "")) stream << "::" << log.m_FuncName << "()";
    stream << "] ";

    // Reset for future logging
    log.resetContext();

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
