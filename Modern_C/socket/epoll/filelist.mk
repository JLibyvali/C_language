SRC_FILES += level_server.c level_client.c edge_server.c edge_client.c

EPOLL_LEVEL_SERVER := $(BUILD)/$(EPOLL)/level_server.o
EPOLL_LEVEL_CLIENT := $(BUILD)/$(EPOLL)/level_client.o

EPOLL_EDGE_SERVER := $(BUILD)/$(EPOLL)/edge_server.o
EPOLL_EDGE_CLIENT := $(BUILD)/$(EPOLL)/edge_client.o