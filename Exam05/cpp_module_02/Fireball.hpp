#ifndef Fireball_HPP
#define Fireball_HPP

#include <string>
#include <iostream>
#include "ASpell.hpp"

using namespace std;

class Fireball : public ASpell {

    public:
        Fireball() : ASpell("Fireball", "burnt to a crisp") {}
        ~Fireball() {}
        virtual ASpell* clone() const {
            return new Fireball();
        }
};

#endif