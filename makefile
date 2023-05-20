all: remote

OUTPUT_NAME=httplib.out
COMPILER=g++ -g
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
