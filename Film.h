//
// Created by Bianca Mihaela on 07/11/2025.
//

#ifndef OOP_FILM_H
#define OOP_FILM_H
#pragma once
#include <string>


class Film {
private:
    std::string titlu;
    std::string gen;
    int durata; // in minute

public:
    explicit Film(const std::string& t="", const std::string& g="", int d=0);
    Film(const Film& f);
    Film& operator=(const Film& f);
    ~Film();
    [[nodiscard]]const std::string& getTitlu() const;
    [[nodiscard]]const std::string& getGen() const;
    friend std::ostream& operator<<(std::ostream& os, const Film& f);
};


#endif //OOP_FILM_H