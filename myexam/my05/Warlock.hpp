//
// Created by jaewkim on 2022/11/15.
//

#ifndef MYEXAM_WARLOCK_H
#define MYEXAM_WARLOCK_H
#include <iostream>
#include <string>


class Warlock {
private :
	std::string name;
	std::string title;
	Warlock();
	//오퍼레이터=
	//복사생성자
	Warlock(Warlock const &other);
	Warlock &operator=(Warlock const &other);
public:
	//퍼블릭
};


#endif //MYEXAM_WARLOCK_H
