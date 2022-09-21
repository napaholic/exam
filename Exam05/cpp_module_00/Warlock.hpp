#ifndef Warlock_hpp
#define Warlock_hpp

#include <string>
#include <iostream>

using namespace std;

class Warlock {
    private:
        string name;
        string title;

        Warlock();
        Warlock& operator=(const Warlock&);
        Warlock(const Warlock&);

    public:
        Warlock(const string& name, const string& title): name(name), title(title) {
            cout << getName() << ": This looks like another boring day.\n";
        }
        ~Warlock() {
            cout << getName() << ": My job here is done!\n";
        }
        const string& getName() const {
            return this->name;
        }
        const string& getTitle() const {
            return this->title;
        }
        void setTitle(const string& title) {
            this->title = title;
        }
        void introduce() const {
            cout << getName() << ": I am " << getName() << ", " << getTitle() << "!\n";
        }
};

#endif