#pragma once
#include "HttpMapBuilder.h"
#include "assert.h"

void test_HttpMapBuilder_addSimpleString()
{
	HttpMapBuilder mapBuilder;

	mapBuilder.setValue("example", HttpValue("value"));

	const HttpMap& map = mapBuilder.getContainer();

	assertTrue(map.hasKey("example"), true, "Testing if HttpMap has string key (string case)", &std::cout);
	assertTrue(map.getValue("example").getAsString() == "value", true, "Testing if HttpMap string key has expected value", &std::cout);
}

void test_HttpMapBuilder_addSimpleInteger()
{
	HttpMapBuilder mapBuilder;

	mapBuilder.setValue("integer", HttpValue("53"));

	const HttpMap& map = mapBuilder.getContainer();

	assertTrue(map.hasKey("integer"), true, "Testing if HttpMap has key (int case)", &std::cout);
	assertTrue(map.getValue("integer").getAsInt() == 53, true, "Testing if HttpMap int key has expected value", &std::cout);
}

void test_HttpMapBuilder_addSimpleBool()
{
	HttpMapBuilder mapBuilder;

	mapBuilder.setValue("myBool", HttpValue("1"));

	const HttpMap& map = mapBuilder.getContainer();

	assertTrue(map.hasKey("myBool"), true, "Testing if HttpMap has key (bool case)", &std::cout);
	assertTrue(map.getValue("myBool").getAsBool() == true, true, "Testing if HttpMap bool key has expected value", &std::cout);
}

void test_HttpMapBuilder_checkNonExistentIsNull()
{
	HttpMapBuilder mapBuilder;

	const HttpMap& map = mapBuilder.getContainer();

	assertTrue(!map.hasKey("example"), true, "Testing if HttpMap has key (null case)", &std::cout);
	assertTrue(map.getValue("example").isNull(), true, "Testing if HttpMap non-existent key is null", &std::cout);
}

void test_HttpMapBuilder_override()
{
	HttpMapBuilder mapBuilder;

	mapBuilder.setValue("myBool", HttpValue("1"));
	mapBuilder.setValue("myBool", HttpValue("0"));

	const HttpMap& map = mapBuilder.getContainer();

	assertTrue(map.hasKey("myBool"), true, "Testing if HttpMap has key (bool override case)", &std::cout);
	assertTrue(map.getValue("myBool").getAsBool() == false, true, "Testing if HttpMap bool key has expected value", &std::cout);
}

void test_HttpMapBuilder_lockAfterGetContainer()
{
	HttpMapBuilder mapBuilder;

	mapBuilder.setValue("example1", HttpValue("1"));
	mapBuilder.setValue("example2", HttpValue("anything"));

	const HttpMap& map = mapBuilder.getContainer();

	mapBuilder.setValue("example3", HttpValue("something"));

	assertTrue(map.hasKey("example1"), true, "Testing if HttpMap has key example1 (lock case)", &std::cout);
	assertTrue(map.getValue("example1").getAsBool() == true, true, "Testing if HttpMap bool example1 key has expected value (lock case)", &std::cout);
	assertTrue(map.hasKey("example2"), true, "Testing if HttpMap has key example2 (lock case)", &std::cout);
	assertTrue(map.getValue("example2").getAsString() == "anything", true, "Testing if HttpMap example2 key has expected value (lock case)", &std::cout);
	assertTrue(!map.hasKey("example3"), true, "Testing if HttpMap example3 key is absent (lock case)", &std::cout);
	assertTrue(map.getValue("example3").isNull(), true, "Testing if example3 key is null (lock case)", &std::cout);
}
