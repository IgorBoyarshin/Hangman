#include "Log.h"


Log Log::m_LogInstance{};


Log::Log() : Log(OutputLevel::None) {}


Log::Log(OutputLevel outputLevel)
    : m_OutputLevel(outputLevel),
      m_ClassName(""), m_FuncName(""),
      m_LogLevel(LogLevel::Info),
      m_Destination(Destination::Console),
      m_Start(std::chrono::system_clock::now()) {
      }


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
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    auto min = std::chrono::duration_cast<std::chrono::minutes>(now - log.m_Start).count();
    auto sec = std::chrono::duration_cast<std::chrono::seconds>(now - log.m_Start).count();
    auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(now - log.m_Start).count() - (1000 * sec);
    /* std::time_t currentTime = std::chrono::system_clock::to_time_t(now); */
    /* std::tm tm = *std::localtime(&currentTime); */
    switch (log.m_Destination) {
        case Log::Destination::Console:
            std::cout << "(" << std::setw(2) << min << "m ";
            std::cout << std::setw(2) << sec << "s ";
            std::cout << std::setw(3) << millisec << "ms) ";
            /* std::cout << "[" << std::put_time(&tm, "%T") << "] "; */
            std::cout << log << str;
            return std::cout;
        case Log::Destination::File:
            log.m_File << "(" << std::setw(2) << min << "m ";
            log.m_File << std::setw(2) << sec << "s ";
            log.m_File << std::setw(3) << millisec << "ms) ";
            /* log.m_File << "[" << std::put_time(&tm, "%T") << "] "; */
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
