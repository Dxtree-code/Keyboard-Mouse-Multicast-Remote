CC = g++
BUILD_FOLDER = ./build
INCLUDES = -I./vendor/asio/asio/include
CFLAGS = -Wall -g -std=c++17 
LDFLAGS = -lws2_32

CFLAGS += -D_WIN32_WINNT=0x0601

SRCS = main_server.cpp $(wildcard internal_lib/*.cpp)
OBJS = $(patsubst %.cpp,$(BUILD_FOLDER)/%.o,$(SRCS)) 

all: $(BUILD_FOLDER)/main_server.exe 

$(BUILD_FOLDER)/main_server.exe: $(OBJS) | $(BUILD_FOLDER)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@ ${LDFLAGS}

$(BUILD_FOLDER)/%.o: %.cpp | $(BUILD_FOLDER)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_FOLDER):
	mkdir "$(BUILD_FOLDER)/internal_lib"

clean:
	rm -rf $(BUILD_FOLDER)
