//
// Created by Bianca Mihaela on 12/01/2026.
//

#ifndef OOP_GESTIUNERESURSE_H
#define OOP_GESTIUNERESURSE_H

#include <vector>
#include <iostream>

template <typename T>
class GestiuneResurse {
private:

    std::vector<T> elemente;

public:

    void adauga(const T& element) {
        elemente.push_back(element);
    }

    const std::vector<T>& getToate() const {
        return elemente;
    }

    const std::vector<T>& getReferinta() const{
        return elemente;
    }

    std::vector<T>& getReferinta(){
        return elemente;
    }

    size_t nrElemente() const {
        return elemente.size();
    }

    void afiseazaToate() const {
        for (const auto& e : elemente) {
            std::cout << e << "\n";
        }
    }
};
#endif //OOP_GESTIUNERESURSE_H