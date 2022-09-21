#ifndef POLYMORPH_HPP
#define POLYMORPH_HPP

#include <string>
#include <iostream>
#include "ASpell.hpp"

using namespace std;

class Polymorph : public ASpell {

    public:
        Polymorph() : ASpell("Polymorph", "turned into a critter") {}
        ~Polymorph() {}
        virtual ASpell* clone() const {
            return new Polymorph();
        }
};

#endif