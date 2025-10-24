define rwildcard
  $(wildcard $1$2) \
  $(foreach d,$(wildcard $1*/),$(call rwildcard,$d,$2))
endef

CC = g++
BUILD_FOLDER = ./build
INCLUDES = -I ./vendor/asio/asio/include -I ./include
CFLAGS = -Wall -g -std=c++17 
LDFLAGS = -static -lws2_32

CFLAGS += -D_WIN32_WINNT=0x0A00
CFLAGS += -m64

SRCS  = main_server.cpp main_client.cpp test.cpp $(call rwildcard,src/,*.cpp)
OBJS = $(patsubst %.cpp,$(BUILD_FOLDER)/%.o,$(notdir $(SRCS))) 

SRCSS  = main_server.cpp  $(call rwildcard,src/,*.cpp)
OBJSS =  $(patsubst %.cpp,$(BUILD_FOLDER)/%.o,$(notdir $(SRCSS))) 

SRCSC = main_client.cpp $(call rwildcard,src/,*.cpp)
OBJSC =  $(patsubst %.cpp,$(BUILD_FOLDER)/%.o,$(notdir $(SRCSC)))

SRCST = test.cpp $(call rwildcard,src/,*.cpp)
OBJST =  $(patsubst %.cpp,$(BUILD_FOLDER)/%.o,$(notdir $(SRCST)))

vpath %.cpp $(sort $(dir $(SRCS))) #penting buat mapping

SUBDIRS := $(wildcard src/*/)

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
	mkdir "$(BUILD_FOLDER)/"

echo:
	@echo "${SRCSS}"
clean:
	rm -rf $(BUILD_FOLDER)
