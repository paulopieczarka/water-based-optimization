
SOURCES_DIR := src
SOURCES := $(wildcard $(SOURCES_DIR)/*.cpp)

all:
	@clear
	@g++ -g $(SOURCES) -o ./wwo
	@./wwo