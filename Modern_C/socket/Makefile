# project structure
BUILD = BUILD
BASIC_TCP = basic_tcp
BASIC_UDP = basic_udp
HALF_TCP = half_close

# Inlcude all filelist.mk
FILELIST := $(shell find . -name "filelist.mk" )
$(foreach filelist, $(FILELIST), $(eval include $(filelist)))

# Compiler settings
CC = clang
CFLAGS = -g -ggdb -O0 -Wall -std=c11 -Iinclude
CPPFLAGS = $(CFLAGS)
LDFLAGS = -Wl,-Map=$(BUILD)/Symbol.map

OBJ_FILES := $(patsubst %.c,$(BUILD)/%.o,$(SRC_FILES))

all : basic_tcp basic_udp half_tcp

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


half_tcp : $(BUILD)/$(HALF_TCP)/server $(BUILD)/$(HALF_TCP)/client

$(BUILD)/$(HALF_TCP)/server : $(HALF_SERVER)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

$(BUILD)/$(HALF_TCP)/client : $(HALF_CLIENT)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@



.PHONY : all clean
clean:
	-rm -rf $(BUILD) .cache
	