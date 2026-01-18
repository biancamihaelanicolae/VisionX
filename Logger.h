//
// Created by Bianca Mihaela on 18/01/2026.
//

#ifndef OOP_LOGGER_H
#define OOP_LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

class Logger {
private:
    std::ofstream fisier_log;
    static Logger* instance;
    Logger();
public:
    ~Logger();
    static Logger& getInstance();
    void log(const std::string& mesaj);

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};

#endif //OOP_LOGGER_H