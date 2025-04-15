#include "TestDialog.h"
#include "Except.h"

int main() {
	try {
		Tests::Dialog::run_dialog_test();
	} catch (...) {
		Except::react();
	}
	return 0;
}
