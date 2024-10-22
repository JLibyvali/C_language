CC = gcc
CFLAGS += -g -ggdb3 -O0 -Wall

BUILD_DIR = build

all : regex

$(BUILD_DIR) : 
	@if [ -d $@ ]; then	\
		rm -r $@; \
	fi;		\
	mkdir -p $@;	

regex : regular_ex.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

.PHONY : all clean

clean :
	-rm -f $(BUILD_DIR)/*