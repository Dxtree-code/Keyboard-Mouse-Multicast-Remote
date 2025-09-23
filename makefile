define rwildcard
  $(wildcard $1$2) \
  $(foreach d,$(wildcard $1*/),$(call rwildcard,$d,$2))
endef

CC = g++
BUILD_FOLDER = ./build
INCLUDES = -I ./vendor/asio/asio/include
CFLAGS = -Wall -g -std=c++17 -m64

# Detect platform
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    LDFLAGS =
endif

ifeq ($(UNAME_S),Darwin)  # macOS
    LDFLAGS = -framework CoreFoundation -framework CoreGraphics -framework ApplicationServices
endif

ifeq ($(OS),Windows_NT)   # Windows (MinGW or similar)
    CFLAGS += -D_WIN32_WINNT=0x0A00
    LDFLAGS = -static -lws2_32
    EXE = .exe
else
    EXE =
endif

SRCS  = main_server.cpp main_client.cpp test.cpp $(call rwildcard,internal_lib/,*.cpp)
OBJS = $(patsubst %.cpp,$(BUILD_FOLDER)/%.o,$(notdir $(SRCS))) 

SRCSS  = main_server.cpp $(call rwildcard,internal_lib/,*.cpp)
OBJSS = $(patsubst %.cpp,$(BUILD_FOLDER)/%.o,$(notdir $(SRCSS))) 

SRCSC = main_client.cpp $(call rwildcard,internal_lib/,*.cpp)
OBJSC = $(patsubst %.cpp,$(BUILD_FOLDER)/%.o,$(notdir $(SRCSC)))

SRCST = test.cpp $(call rwildcard,internal_lib/,*.cpp)
OBJST = $(patsubst %.cpp,$(BUILD_FOLDER)/%.o,$(notdir $(SRCST)))

vpath %.cpp $(sort $(dir $(SRCS))) # mapping

all: $(BUILD_FOLDER)/main_server$(EXE) $(BUILD_FOLDER)/main_client$(EXE)

test: $(BUILD_FOLDER)/test$(EXE)

$(BUILD_FOLDER)/test$(EXE): $(OBJST) | $(BUILD_FOLDER)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)

$(BUILD_FOLDER)/main_server$(EXE): $(OBJSS) | $(BUILD_FOLDER)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)

$(BUILD_FOLDER)/main_client$(EXE): $(OBJSC) | $(BUILD_FOLDER)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)

$(BUILD_FOLDER)/%.o: %.cpp | $(BUILD_FOLDER)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_FOLDER):
	mkdir -p $(BUILD_FOLDER)
