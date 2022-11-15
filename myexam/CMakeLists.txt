cmake_minimum_required(VERSION 3.22)
project(myexam)

set(CMAKE_CXX_STANDARD 14)

include_directories(exam05/result/cpp_module_00)
include_directories(exam05/result/cpp_module_01)
include_directories(exam05/result/cpp_module_02)
include_directories(my05)

add_executable(myexam
        exam05/result/cpp_module_00/main.cpp
        exam05/result/cpp_module_00/Warlock.cpp
        exam05/result/cpp_module_00/Warlock.hpp
        exam05/result/cpp_module_01/ASpell.cpp
        exam05/result/cpp_module_01/ASpell.hpp
        exam05/result/cpp_module_01/ATarget.cpp
        exam05/result/cpp_module_01/ATarget.hpp
        exam05/result/cpp_module_01/Dummy.cpp
        exam05/result/cpp_module_01/Dummy.hpp
        exam05/result/cpp_module_01/Fwoosh.cpp
        exam05/result/cpp_module_01/Fwoosh.hpp
        exam05/result/cpp_module_01/main.cpp
        exam05/result/cpp_module_01/Warlock.cpp
        exam05/result/cpp_module_01/Warlock.hpp
        exam05/result/cpp_module_02/ASpell.cpp
        exam05/result/cpp_module_02/ASpell.hpp
        exam05/result/cpp_module_02/ATarget.cpp
        exam05/result/cpp_module_02/ATarget.hpp
        exam05/result/cpp_module_02/BrickWall.cpp
        exam05/result/cpp_module_02/BrickWall.hpp
        exam05/result/cpp_module_02/Dummy.cpp
        exam05/result/cpp_module_02/Dummy.hpp
        exam05/result/cpp_module_02/Fireball.cpp
        exam05/result/cpp_module_02/Fireball.hpp
        exam05/result/cpp_module_02/Fwoosh.cpp
        exam05/result/cpp_module_02/Fwoosh.hpp
        exam05/result/cpp_module_02/main.cpp
        exam05/result/cpp_module_02/Polymorph.cpp
        exam05/result/cpp_module_02/Polymorph.hpp
        exam05/result/cpp_module_02/SpellBook.cpp
        exam05/result/cpp_module_02/SpellBook.hpp
        exam05/result/cpp_module_02/TargetGenerator.cpp
        exam05/result/cpp_module_02/TargetGenerator.hpp
        exam05/result/cpp_module_02/Warlock.cpp
        exam05/result/cpp_module_02/Warlock.hpp
        my05/Warlock.cpp
        my05/Warlock.hpp)
