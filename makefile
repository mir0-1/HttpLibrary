all: remote

OUTPUT_NAME=httplib
COMPILER=g++
CONFIG=
BUILD=$(COMPILER) $^ $(CONFIG) -o $(OUTPUT_NAME)

local: *.cpp
		make clean
		$(BUILD)

remote: *.cpp
		make clean
		git pull
		$(BUILD)

clean:
	rm -f $(OUTPUT_NAME)
