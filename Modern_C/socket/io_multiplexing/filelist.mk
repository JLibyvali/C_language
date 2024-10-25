SRC_FILES += server.c client.c pipe_test.c select_test.c

IO_MULTIPLEXING_SERVER := $(BUILD)/$(IO_MULTIPLEXING)/server.o
IO_MULTIPLEXING_CLIENT := $(BUILD)/$(IO_MULTIPLEXING)/client.o

PIPE_TEST := $(BUILD)/$(IO_MULTIPLEXING)/pipe_test.o
SELECT_TEST := $(BUILD)/$(IO_MULTIPLEXING)/select_test.o