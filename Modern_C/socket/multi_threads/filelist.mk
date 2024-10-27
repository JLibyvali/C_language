SRC_FILES += server.c client.c  pthreads_test.c

MULTI_THREADS_SERVER := $(BUILD)/$(MULTI_THREADS)/server.o
MULTI_THREADS_CLIENT := $(BUILD)/$(MULTI_THREADS)/client.o

PTHREADS_TEST := $(BUILD)/$(MULTI_THREADS)/pthreads_test.o