CC = clang
CFLAGS = -g -ggdb -O0 -Wall -std=c11 -Iinclude
CPPFLAGS = $(CFLAGS)
LDFLAGS = -Wl,-Map=$(BUILD)/Symbol.map

BUILD = BUILD
SevSRC  = server.c
CliSRC  =  client.c


Sevobj = $(patsubst %.c,$(BUILD)/%.o,$(SevSRC))
Cliobj = $(patsubst %.c,$(BUILD)/%.o,$(CliSRC))


all : target


target : $(BUILD) $(BUILD)/server  $(BUILD)/client

$(BUILD):
	@if [ -d $@ ]; then     \
		rm -r $@;   \
	fi;             \
	mkdir -p $@;


$(BUILD)/%.o : %.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/server : $(Sevobj)
	$(CC) $(CFLAGS) $(Sevobj) $(LDFLAGS) -o $@

$(BUILD)/client : $(Cliobj)
	$(CC) $(CFLAGS) $(Cliobj) $(LDFLAGS) -o $@


.PHONY : all clean
clean:
	-rm -rf $(BUILD) .cache
	