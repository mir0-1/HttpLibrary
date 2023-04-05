#pragma once
#include <stdlib.h>
#include <iostream>

template <class Value>
void assertEquals(Value expected, Value actual, bool stopTestingOnFail, const std::string& extraInfo, std::ostream* debugOutput)
{
	if (debugOutput != nullptr)
		(*debugOutput) << "[ASSERTEQUALS] {" << extraInfo << "} Expected: " << expected << "; Actual: " << actual << std::endl;

	if (expected != actual)
	{
		if (stopTestingOnFail)
		{
			if (debugOutput != nullptr)
				(*debugOutput) << "[ASSERTEQUALS] {" << extraInfo << "} Halting the program..." << std::endl;
			exit(-1);
		}
	}
}