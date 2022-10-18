MAKE_DIR = $(shell pwd)

SRC_DIR       := $(MAKE_DIR)/src
INCLUDE_DIR   := $(MAKE_DIR)/include
OBJ_DIR       := $(MAKE_DIR)/build/obj
LIB_DIR       := $(MAKE_DIR)/lib
BUILD_DIR	  := $(MAKE_DIR)/build
BIN_DIR       := $(MAKE_DIR)/build/bin
EXTERNAL_DIR  := $(MAKE_DIR)/external

# Project DIR
EWS_DIR		  := $(MAKE_DIR)/src/ews
SERVER_DIR	  := $(MAKE_DIR)/src/server
UTIL_DIR	  := $(MAKE_DIR)/src/util
RENDER_DIR	  := $(MAKE_DIR)/src/render

# External Libraries DIR
LIBAL_DIR     := $(EXTERNAL_DIR)/libal
LIBCLOG_DIR   := $(EXTERNAL_DIR)/libclog
LIBINIH_DIR	  := $(EXTERNAL_DIR)/inih

INC_SRCH_PATH := 
INC_SRCH_PATH += -I$(SRC_DIR)
INC_SRCH_PATH += -I$(INCLUDE_DIR)
INC_SRCH_PATH += -I$(LIBAL_DIR)/include
INC_SRCH_PATH += -I$(LIBCLOG_DIR)
INC_SRCH_PATH += -I$(LIBINIH_DIR)

LIB_SRCH_PATH :=
LIB_SRCH_PATH += -L$(MAKE_DIR)/lib

CC = gcc
LD = ld

BIN = $(MAKE_DIR)/build/bin/ews

# Add in Reverse Order
LIBS := -lserver -lrender -lutil -linih -lal -lpthread

CFLAGS := -std=c99 -pedantic
CFLAGS += $(INC_SRCH_PATH) $(LIB_SRCH_PATH)

ifeq ($(BUILD),dev)
# "Dev" build - no optimization, and debugging symbols
CFLAGS += -g -ggdb3 -Og -DDEBUG -finstrument-functions -fdump-rtl-expand -fsanitize=address,undefined -fno-omit-frame-pointer -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
LIBS   += -lclog-dev
else
# "Release" build - optimization, and no debug symbols
CFLAGS += -O2 -s -DNDEBUG -Wall -Wextra
LIBS   += -lclog
endif

LDFLAGS :=

export MAKE_DIR CC LD CFLAGS LDFLAGS LIBS INC_SRCH_PATH OBJ_DIR LIB_DIR BIN_DIR BIN LIBAL_DIR LIBCLOG_DIR LIBINIH_DIR

all:
	@$(MAKE) -C $(EXTERNAL_DIR)
	@$(MAKE) -C $(SERVER_DIR)
	@$(MAKE) -C $(UTIL_DIR)
	@$(MAKE) -C $(RENDER_DIR)
	@$(MAKE) -C $(EWS_DIR)

dev:
	make "BUILD=dev"

run:
	$(BIN) $(ARGS)

valgrind:
	@mkdir -p $(BUILD_DIR)
	valgrind --leak-check=full \
        --show-leak-kinds=all \
        --track-origins=yes \
        --verbose \
        --log-file=./build/valgrind-out.txt \
        $(BIN) $(ARGS)

clangformat:
	find . -iname *.h -o -iname *.c | xargs clang-format -i

.PHONY: clean cleanall cleanexternal
clean:
	@$(MAKE) -C $(SERVER_DIR)     clean
	@$(MAKE) -C $(UTIL_DIR)       clean
	@$(MAKE) -C $(RENDER_DIR)     clean
	@$(MAKE) -C $(EWS_DIR)        clean

cleanall: clean
	@$(MAKE) -C $(EXTERNAL_DIR)   clean
	@$(RM) -rf  $(BUILD_DIR)
	@$(RM) -rf  $(LIB_DIR)

cleanexternal:
	@$(MAKE) -C $(EXTERNAL_DIR)   clean
	@$(RM) -rf  $(LIB_DIR)
