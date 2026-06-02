CXX ?= c++
AR ?= ar
CMAKE ?= cmake
CXXFLAGS ?= -std=c++23 -Wall -Wextra -Wpedantic -Iinclude

BUILD_DIR := build
LIB := $(BUILD_DIR)/libomc_osc.a
SRCS := $(sort $(wildcard src/*.cpp))
OBJS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
LIBLO_SRC_DIR := ../liblo/cmake
LIBLO_BUILD_DIR := $(BUILD_DIR)/liblo
LIBLO_LIB := $(LIBLO_BUILD_DIR)/liblo.a
LIBLO_CXXFLAGS := -I../liblo -I$(LIBLO_BUILD_DIR) -Wno-variadic-macros
LIBLO_LDFLAGS := $(LIBLO_LIB) -lm

.PHONY: all clean examples

all: examples

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIBLO_LIB): | $(BUILD_DIR)
	$(CMAKE) -S $(LIBLO_SRC_DIR) -B $(LIBLO_BUILD_DIR) \
		-DWITH_STATIC=ON \
		-DWITH_TOOLS=OFF \
		-DWITH_TESTS=OFF \
		-DWITH_EXAMPLES=OFF \
		-DWITH_CPP_TESTS=OFF \
		-DTHREADING=OFF
	$(CMAKE) --build $(LIBLO_BUILD_DIR) --target liblo_static

$(BUILD_DIR)/%.o: src/%.cpp include/omc-osc/omc_osc.hpp $(LIBLO_LIB) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LIBLO_CXXFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	$(AR) rcs $@ $(OBJS)

examples: $(LIB)
	$(CXX) $(CXXFLAGS) $(LIBLO_CXXFLAGS) -Iexamples examples/oms_server.cpp examples/example_complex_message.cpp $(LIB) $(LIBLO_LDFLAGS) -o $(BUILD_DIR)/oms_server
	$(CXX) $(CXXFLAGS) $(LIBLO_CXXFLAGS) -Iexamples examples/omc_client.cpp examples/example_complex_message.cpp $(LIB) $(LIBLO_LDFLAGS) -o $(BUILD_DIR)/omc_client

clean:
	rm -rf $(BUILD_DIR)
