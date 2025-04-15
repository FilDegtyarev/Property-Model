.PHONY: clean dialog
.PHONY: clean mixer
dialog:
	g++ -g -std=c++2a TestDialog.cpp Except.cpp Constraint.cpp ConstraintGraph.cpp  Method.cpp Variable.cpp Deltablue.cpp PrettyPrint.cpp -o dialog
	./dialog

mixer:
	g++ -g -std=c++2a TestSoundMixer.cpp Except.cpp Constraint.cpp ConstraintGraph.cpp  Method.cpp Variable.cpp Deltablue.cpp PrettyPrint.cpp -o mixer
	./mixer

clean:
	rm dialog
	rm mixer
