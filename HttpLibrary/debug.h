#pragma once
#include <iostream>

#ifdef DEBUG_LOGS
#define DEBUG_OUT(description, val) std::cout << description << " " << val << std::endl
#else
#define DEBUG_OUT(description, val)
#endif