SRC_FILES += server.c client.c io_server.c io_client.c

MULTI_SERVER := $(BUILD)/$(MULTI_PROCESS)/server.o
MULTI_CLIENT := $(BUILD)/$(MULTI_PROCESS)/client.o

IO_MULTI_SERVER := $(BUILD)/$(MULTI_PROCESS)/io_server.o
IO_MULTI_CLIENT := $(BUILD)/$(MULTI_PROCESS)/io_client.o