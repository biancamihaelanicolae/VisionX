//
// Created by Bianca Mihaela on 07/11/2025.
//

#ifndef OOP_UTILIZATOR_H
#define OOP_UTILIZATOR_H
#include <iostream>
#include <string>


class Utilizator {
private:
    std::string username;
    std::string tip; // normal/ student/ elev

public:
    Utilizator();

    Utilizator(const std::string& u, const std::string& t);

    [[nodiscard]]const std::string& getTip() const;

    [[nodiscard]]const std::string& getUsername() const;

    void citire_utilizator();
};


#endif //OOP_UTILIZATOR_H