.PHONY: clean dialog
dialog:
	g++ -g -std=c++2a dialog.cpp Constraint.cpp ConstraintGraph.cpp  Method.cpp Variable.cpp Deltablue.cpp -o dialog
	./dialog

clean:
	rm dialog
