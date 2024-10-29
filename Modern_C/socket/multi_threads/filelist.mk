SRC_FILES += chat_server.c chat_client.c  pthreads_test.c cond_var_test.c

CHAT_SERVER := $(BUILD)/$(MULTI_THREADS)/chat_server.o
CHAT_CLIENT := $(BUILD)/$(MULTI_THREADS)/chat_client.o

PTHREADS_TEST := $(BUILD)/$(MULTI_THREADS)/pthreads_test.o
COND_VAR_TEST := $(BUILD)/$(MULTI_THREADS)/cond_var_test.o