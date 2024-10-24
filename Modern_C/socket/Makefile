# project structure
BUILD = BUILD
BASIC_TCP = basic_tcp
BASIC_UDP = basic_udp
HALF_TCP = half_close
ADV_TCP = advanced_socket
MULTI_PROCESS =  multi_process

# Inlcude all filelist.mk
FILELIST := $(shell find . -name "filelist.mk" )
$(foreach filelist, $(FILELIST), $(eval include $(filelist)))

# Compiler settings
CC = clang
CFLAGS = -g -ggdb -O0 -Wall -std=c99 -Iinclude
CPPFLAGS = $(CFLAGS)
LDFLAGS = -Wl,-Map=$(BUILD)/Symbol.map

OBJ_FILES := $(patsubst %.c,$(BUILD)/%.o,$(SRC_FILES))

all : basic_tcp basic_udp half_tcp adv_socket multi_process io_multi_process

# Create the contents
$(BUILD)/%.o : %.c 
	@mkdir -p $(dir $@);	\
	$(CC) $(CFLAGS) -c $< -o $@

# basic_tcp
basic_tcp : $(BUILD)/$(BASIC_TCP)/server  $(BUILD)/$(BASIC_TCP)/client

$(BUILD)/$(BASIC_TCP)/server : $(BASIC_TCP_SERVER)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

$(BUILD)/$(BASIC_TCP)/client : $(BASIC_TCP_CLIENT)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@


# basic_udp
basic_udp : $(BUILD)/$(BASIC_UDP)/server $(BUILD)/$(BASIC_UDP)/client

$(BUILD)/$(BASIC_UDP)/server : $(BASIC_UDP_SERVER)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

$(BUILD)/$(BASIC_UDP)/client : $(BASIC_UDP_CLIENT)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@


# half closed tcp socket
half_tcp : $(BUILD)/$(HALF_TCP)/server $(BUILD)/$(HALF_TCP)/client

$(BUILD)/$(HALF_TCP)/server : $(HALF_SERVER)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

$(BUILD)/$(HALF_TCP)/client : $(HALF_CLIENT)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@


# advanced tcp socket coding
adv_socket : $(BUILD)/$(ADV_TCP)/adv_socket

$(BUILD)/$(ADV_TCP)/adv_socket : $(ADV_SOCKET)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@ 


# Multi process socket  programming
multi_process : $(BUILD)/$(MULTI_PROCESS)/server $(BUILD)/$(MULTI_PROCESS)/client

$(BUILD)/$(MULTI_PROCESS)/server : $(MULTI_SERVER)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

$(BUILD)/$(MULTI_PROCESS)/client : $(MULTI_CLIENT)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@


# IO divied multi process
io_multi_process : $(BUILD)/$(MULTI_PROCESS)/io_server  $(BUILD)/$(MULTI_PROCESS)/io_client

$(BUILD)/$(MULTI_PROCESS)/io_server : $(IO_MULTI_SERVER)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

$(BUILD)/$(MULTI_PROCESS)/io_client : $(IO_MULTI_CLIENT)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@




.PHONY : all clean
clean:
	-rm -rf $(BUILD) .cache
	