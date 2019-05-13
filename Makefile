
SOURCES_DIR := src
SOURCES := $(wildcard $(SOURCES_DIR)/*.cpp)

all:
	@clear
	@g++ -g -std=c++11 $(SOURCES) -o ./wwo
	@./wwo
