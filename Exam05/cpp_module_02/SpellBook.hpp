#ifndef SPELLBOOK_HPP
#define SPELLBOOK_HPP

#include <string>
#include <iostream>
#include <map>
#include "ASpell.hpp"

using namespace std;

class SpellBook {
        SpellBook& operator=(const SpellBook&);
        SpellBook(const SpellBook&) {}

        map<string, ASpell*> book;

    public:
        SpellBook() {}
        ~SpellBook() {}
        void learnSpell(ASpell* spell) {
            book.insert(make_pair(spell->getName(), spell->clone()));
        }
        void forgetSpell(string const & name) {
            if (book.find(name) == book.end())
                return ;
            book.erase(name);
        }
        ASpell* createSpell(string const & name) {
            if (book.find(name) == book.end())
                return NULL;
            return book.at(name);
        }
};

#endif