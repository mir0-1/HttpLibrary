#include "HttpRequest.h"
#include "ImmutableMapBuilder.h"

int main()
{
	char test[] = "PUT path/to/resource?well=shit HTTP/1.1";
	HttpRequest httpRequest(test);

	ImmutableMapBuilder<int, int> builder;

	builder.add(0, 1);
	builder.add(1, 11);
	builder.add(2, 111);

	ImmutableMap<int, int>& map = builder.build();

	std::cout << map << std::endl;
}