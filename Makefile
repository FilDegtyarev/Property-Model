build:
    g++ -std=c++20 test.cpp Constraint.cpp ConstraintGraph.cpp  Method.cpp Variable.cpp -o test

dialog:
	g++ -std=c++20 dialog.cpp Constraint.cpp ConstraintGraph.cpp  Method.cpp Variable.cpp Deltablue.cpp -o dialog
