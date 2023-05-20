#pragma once
#include <stdlib.h>
#include <iostream>

void assertTrue(bool condition, bool exitOnFail, const std::string& msg, std::ostream* debugOutput)
{
	if (debugOutput)
		(*debugOutput) << "[ASSERT] " << msg << ": ";

	if (!condition)
	{
		if (debugOutput)
			(*debugOutput) << "Failure" << std::endl;

		if (exitOnFail)
			exit(-1);

		return;
	}

	if (debugOutput)
		(*debugOutput) << "Success" << std::endl;
}