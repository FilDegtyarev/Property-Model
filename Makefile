build:
    g++ -std=c++20 test.cpp Constraint.cpp ConstraintGraph.cpp  Method.cpp Variable.cpp -o test

run:
	./test