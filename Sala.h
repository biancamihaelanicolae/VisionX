//
// Created by Bianca Mihaela on 07/11/2025.
//

#ifndef OOP_SALA_H
#define OOP_SALA_H
#pragma once
#include <iosfwd>
#include <string>
#include <vector>

const std::string ANSI_RESET = "\033[0m";
const std::string ANSI_RED = "\033[31m";
const std::string ANSI_GREEN = "\033[32m";


class Sala {
    private:
    int numar;
    int capacitate;
    std::vector<bool> locuri_ocupate; //true = ocupat; false = liber;

    public:
    explicit Sala(int n = 0, int cap = 0);
    Sala(const Sala& s);
    Sala& operator=(const Sala& s);
    ~Sala();
    bool rezervare_multipla(const std::vector<int>& locuri);
    [[nodiscard]]bool sala_plina() const;
    [[nodiscard]]int locuri_libere() const;
    void vizualizare_locuri() const;
    [[nodiscard]]int getCapacitate() const;
    [[nodiscard]]int getNumar() const;
    friend std::ostream& operator<<(std::ostream& os, const Sala& s);
};


#endif //OOP_SALA_H