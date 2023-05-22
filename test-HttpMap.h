#pragma once
#include "HttpMap.h"
#include "assert.h"

void test_HttpMap_insertKeyValuePairs()
{
	HttpMap map;

	map.setValue("test", HttpValue("1234"));
	map.setValue("alpha", HttpValue("more"));
	map.setValue("rabbit", HttpValue("1"));

	assertTrue(map.hasKey("test"), true, "Testing if test is present in map", &std::cout);
	assertTrue(map.hasKey("alpha"), true, "Testing if alpha is present in map", &std::cout);
	assertTrue(map.hasKey("rabbit"), true, "Testing if rabbit is present in map", &std::cout);

	assertTrue(map.getValue("test").getAsInt() == 1234, true, "Testing test value correctness", &std::cout);
	assertTrue(map.getValue("alpha").getAsString() == "more", true, "Testing alpha value correctness", &std::cout);
	assertTrue(map.getValue("rabbit").getAsBool() == true, true, "Testing rabbit value correctness", &std::cout);
}

void test_HttpMap_iterateKeyValuePairs()
{
	HttpMap map;

	const char* expectedKeysForIteration[] = {"test", "alpha", "rabbit"};
	const char* expectedValuesForIteration[] = {"1234", "more", "1"};
	int index;

	map.setValue("test", HttpValue("1234"));
	map.setValue("alpha", HttpValue("more"));
	map.setValue("rabbit", HttpValue("1"));

	const int expectedIterationCount = 3;
	int actualIterationCount = 0;
	std::cout << "Starting iteration now..." << std::endl;
	map.resetIterator();
	while (!map.isIteratorAtEnd())
	{
		std::cout << "At iteration " << actualIterationCount << std::endl;
		std::pair<std::string, HttpValue> pair = map.getNextPairFromIterator();

		for(index = 0; pair.first != expectedKeysForIteration[index] && index < 3; index++);

		assertTrue(pair.first == expectedKeysForIteration[index], true, "Testing iteration key", &std::cout);
		assertTrue(pair.second.getAsString() == expectedValuesForIteration[index], true, "Testing iteration value", &std::cout);
		actualIterationCount++;

		if (map.isIteratorReset())
			break;

	}

	assertTrue(actualIterationCount == expectedIterationCount, true, "Testing overall iteration count", &std::cout);
}

void test_HttpMap_iterateEmpty()
{
	HttpMap map;

	const int expectedIterationCount = 0;
	int actualIterationCount = 0;
	map.resetIterator();
	while (!map.isIteratorAtEnd())
	{
		actualIterationCount++;

		std::pair<std::string, HttpValue> pair = map.getNextPairFromIterator();

		if (map.isIteratorReset())
			break;
	}

	assertTrue(expectedIterationCount == actualIterationCount, true, "Testing overall iteration count (should not iterate)", &std::cout);
}
