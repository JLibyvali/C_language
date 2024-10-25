SRC_FILES += server.c client.c io_server.c io_client.c fork_test.c

MULTIPROCESS_SERVER := $(BUILD)/$(MULTI_PROCESS)/server.o
MULTIPROCESS_CLIENT := $(BUILD)/$(MULTI_PROCESS)/client.o

MULTIPROCESS_IODIVID_SERVER := $(BUILD)/$(MULTI_PROCESS)/io_server.o
MULTIPROCESS_IODIVID_CLIENT := $(BUILD)/$(MULTI_PROCESS)/io_client.o

FORK_TEST := $(BUILD)/$(MULTI_PROCESS)/fork_test.o