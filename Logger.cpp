//
// Created by Bianca Mihaela on 18/01/2026.
//
#include "Logger.h"

Logger* Logger::instance = nullptr;

Logger::Logger() {
    fisier_log.open("activitate.log", std::ios::app);
}

Logger::~Logger() {
    if (fisier_log.is_open()) fisier_log.close();
}

Logger& Logger::getInstance() {
    if (instance == nullptr) {
        instance = new Logger();
    }
    return *instance;
}

void Logger::log(const std::string& mesaj) {
    time_t acum = time(0);
    char* dt = ctime(&acum);
    std::string timestamp(dt);
    if (!timestamp.empty()) timestamp.pop_back();

    if (fisier_log.is_open()) {
        fisier_log << "[" << timestamp << "] " << mesaj << std::endl;
    }
}