cmake_minimum_required(VERSION 3.26)
project(pm)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(what dialog.cpp
        Constraint.cpp
        ConstraintGraph.cpp
        Deltablue.cpp
        dialog.cpp
        Method.cpp
        Variable.cpp
    )
