CC = g++
BUILD_FOLDER = ./build
INCLUDES = -I./vendor/asio/asio/include
CFLAGS = -Wall -g -std=c++17 
LDFLAGS = -static -lws2_32

CFLAGS += -D_WIN32_WINNT=0x0A00
CFLAGS += -m64

SRCS  = main_server.cpp main_client.cpp $(wildcard internal_lib/*.cpp)
OBJS = $(patsubst %.cpp,$(BUILD_FOLDER)/%.o,$(SRCS)) 

SRCSS  = main_server.cpp  $(wildcard internal_lib/*.cpp)
OBJSS = $(patsubst %.cpp,$(BUILD_FOLDER)/%.o,$(SRCSS)) 

SRCSC = main_client.cpp $(wildcard internal_lib/*.cpp)
OBJSC = $(patsubst %.cpp,$(BUILD_FOLDER)/%.o,$(SRCSC)) 


SRCST = test.cpp $(wildcard internal_lib/*.cpp)
OBJST = $(patsubst %.cpp,$(BUILD_FOLDER)/%.o,$(SRCST)) 

all: $(BUILD_FOLDER)/main_server.exe $(BUILD_FOLDER)/main_client.exe

test : $(BUILD_FOLDER)/test.exe

$(BUILD_FOLDER)/test.exe: $(OBJST) | $(BUILD_FOLDER)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@ ${LDFLAGS}

$(BUILD_FOLDER)/main_server.exe: $(OBJSS) | $(BUILD_FOLDER)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@ ${LDFLAGS}

$(BUILD_FOLDER)/main_client.exe: $(OBJSC) | $(BUILD_FOLDER)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@ ${LDFLAGS}


$(BUILD_FOLDER)/%.o: %.cpp | $(BUILD_FOLDER)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_FOLDER):
	mkdir "$(BUILD_FOLDER)/internal_lib"

clean:
	rm -rf $(BUILD_FOLDER)
