#include "Except.h"
#include <cstdio>

namespace Except {
void react() {
	try {
		throw;
	} catch (std::exception& e) {
		std::cout << "Property model has throwed an exception: \n";
		std::cout << e.what() << std::endl;
	} catch (...) {
		std::cout << "Unknown exception." << std::endl;
	}
}
} // namespace Except
