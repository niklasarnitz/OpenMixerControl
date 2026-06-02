#
# Makefile
#

COPTS           ?= -mcpu=arm926ej-s -Os -fno-caller-saves -pipe -funit-at-a-time -msoft-float -fno-plt -fno-unwind-tables -fno-asynchronous-unwind-tables
CC              = arm-linux-gnueabi-gcc
CXX             = arm-linux-gnueabi-g++
AR              = arm-linux-gnueabi-ar
LD              = arm-linux-gnueabi-ld

LIB_DIR			= ../lib
LVGL_DIR        ?= ../lib/lv_port_linux
LVGL_PATH       = demosAreDisabledByThisHack
GLAZE_PATH		= ../lib/glaze/include
ARTNET_PATH		= ../lib/libartnet/artnet

WARNINGS        := -Wall -Wshadow -Wundef -Wextra -Wno-unused-function -Wno-error=strict-prototypes -Wpointer-arith \
                   -fno-strict-aliasing -Wno-error=cpp -Wuninitialized -Wmaybe-uninitialized -Wno-unused-parameter -Wno-missing-field-initializers -Wtype-limits \
                   -Wsizeof-pointer-memaccess -Wno-format-nonliteral -Wno-cast-qual -Wunreachable-code -Wno-switch-default -Wreturn-type -Wmultichar -Wformat-security \
                   -Wno-ignored-qualifiers -Wno-error=pedantic -Wno-sign-compare -Wno-error=missing-prototypes -Wdouble-promotion -Wclobbered -Wdeprecated -Wempty-body \
                   -Wshift-negative-value -Wstack-usage=2048 -Wno-unused-value

DEPFLAGS        = -MMD -MP

# normal build
CFLAGS          ?= -std=c11 -flto=auto $(COPTS) -g0 -D_GNU_SOURCE -I$(LIB_DIR)/ -I$(LVGL_DIR)/ -I$(GLAZE_PATH)/ -I$(ARTNET_PATH)/ $(WARNINGS) $(DEPFLAGS)
CXXFLAGS        ?= -std=c++23 -flto=auto $(COPTS) -g0 -D_GNU_SOURCE -I$(LIB_DIR)/ -I$(LVGL_DIR)/ -I$(GLAZE_PATH)/ -I$(ARTNET_PATH)/ $(WARNINGS) $(DEPFLAGS)

# debug build
#CFLAGS          ?= -std=c11 -g -I$(LVGL_DIR)/ -I$(GLAZE_PATH)/ $(WARNINGS) $(DEPFLAGS)
#CXXFLAGS         ?= -std=c++23 -g -I$(LVGL_DIR)/ -I$(GLAZE_PATH)/ $(WARNINGS) $(DEPFLAGS)

LDFLAGS         ?= -flto=auto -lm -lrt -lpthread -lartnet

BIN             = omc
BUILD_DIR       = ../build
BUILD_OBJ_DIR   = $(BUILD_DIR)/obj
BUILD_BIN_DIR   = $(BUILD_DIR)

prefix          ?= /usr
bindir          ?= $(prefix)/bin

# Collect source files recursively
CSRCS           := $(shell find -type f -name '*.c' -print) \
			 	   $(shell find ../lib \
						-path '../lib/lv_port_linux/example' -prune -o \
                        -path '../lib/lv_port_linux/lvgl/demos' -prune -o \
                        -path '../lib/lv_port_linux/lvgl/examples' -prune -o \
                        -path '../lib/lv_port_linux/lvgl/tests' -prune -o \
						-path '../lib/glaze' -prune -o \
					  -type f -name '*.c' -not -path '../lib/lv_port_linux/src/main.c' -print)
CXXSRCS         := $(shell find -type f -name '*.cpp' -print) \
				   $(shell find ../lib \
						-path '../lib/lv_port_linux/example' -prune -o \
                        -path '../lib/lv_port_linux/lvgl/demos' -prune -o \
                        -path '../lib/lv_port_linux/lvgl/examples' -prune -o \
                        -path '../lib/lv_port_linux/lvgl/tests' -prune -o \
						-path '../lib/glaze' -prune -o \
					  -type f -name '*.cpp' -not -path '../lib/lv_port_linux/src/main.c' -print)

all: copy default

# Include LVGL sources
include $(LVGL_DIR)/lvgl/lvgl.mk

OBJEXT          ?= .o

COBJS           = $(CSRCS:.c=$(OBJEXT))
CXXOBJS         = $(CXXSRCS:.cpp=$(OBJEXT))
AOBJS           = $(ASRCS:.S=$(OBJEXT))

SRCS            = $(ASRCS) $(CSRCS) $(CXXSRCS)
OBJS            = $(AOBJS) $(COBJS) $(CXXOBJS)
TARGET          = $(addprefix $(BUILD_OBJ_DIR)/, $(patsubst ./%, %, $(OBJS)))
DEPS            = $(TARGET:.o=.d)

$(BUILD_OBJ_DIR)/%.o: %.c lv_conf.h
	@mkdir -p $(dir $@)
	@$(CC)  $(CFLAGS) -c $< -o $@
	@echo "CC  $<"

$(BUILD_OBJ_DIR)/%.o: %.cpp lv_conf.h
	@mkdir -p $(dir $@)
	@$(CXX)  $(CXXFLAGS) -c $< -o $@
	@echo "CXX $<"

$(BUILD_OBJ_DIR)/%.o: %.S lv_conf.h
	@mkdir -p $(dir $@)
	@$(CC)  $(CFLAGS) -c $< -o $@
	@echo "AS  $<"

copy:
	cp ../files/Makefile.lvgl $(LVGL_DIR)/Makefile

	@if ! diff -q ../files/lv_conf.h $(LVGL_DIR)/lv_conf.h >/dev/null 2>&1; then \
		cp ../files/lv_conf.h $(LVGL_DIR)/lv_conf.h; \
		echo "Update $(LVGL_DIR)/lv_conf.h (changed content)"; \
	else \
		echo "lv_conf.h is up to date."; \
	fi

	@if ! diff -q ../files/lv_conf.h lv_conf.h >/dev/null 2>&1; then \
		cp ../files/lv_conf.h lv_conf.h; \
		echo "Update $(LVGL_DIR)/lv_conf.h (changed content)"; \
	else \
		echo "lv_conf.h is up to date."; \
	fi

	@if ! diff -q ../files/libartnet_network.c ../lib/libartnet/artnet/network.c >/dev/null 2>&1; then \
		cp ../files/libartnet_network.c ../lib/libartnet/artnet/network.c; \
		echo "Update ../lib/libartnet/artnet/network.c (changed content)"; \
	else \
		echo "../lib/libartnet/artnet/network.c is up to date."; \
	fi

default: $(TARGET)
	@mkdir -p $(dir $(BUILD_BIN_DIR)/)
	$(CXX) -o $(BUILD_BIN_DIR)/$(BIN) $(TARGET) $(LDFLAGS)

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)
