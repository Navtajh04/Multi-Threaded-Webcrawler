TARGET_EXEC := paster

CC = gcc
CFLAGS = -Wall -g
LD = gcc
LDFLAGS = -g -L"C:/Users/navta/Downloads/zlib-1.2.13.tar/zlib-1.2.13" -lz

SRC_DIRS = ./src

BUILD_DIR := ./build

SRCS := $(shell find $(SRC_DIRS) -name '*.c' -or -name '*.s')

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(LD) $(OBJS) -o $@ -lz -lcurl -pthread -g

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

-include $(DEPS)