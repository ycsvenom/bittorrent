#include <iomanip>
#include <iostream>

#define RESET std::setw(0) << std::dec
#define HEX std::setw(2) << std::setfill('0') << std::hex

#define AS_HEX(x) HEX << static_cast<int>(x) << RESET
#define DEBUG_VAR(x) std::cerr << #x << ": " << (x) << std::endl
#define DEBUG_PRINT(x) std::cerr << "DEBUGGER: " << (x) << std::endl
#define DEBUG_CONTAINER_TO_N(x, size)     \
	std::cerr << #x << ": ";              \
	for (size_t i = 0; i < size; i++) {   \
		std::cerr << AS_HEX(x[i]) << " "; \
	}                                     \
	std::cerr << std::endl
#define DEBUG_CONTAINER(x) DEBUG_CONTAINER_TO_N(x, x.size())
