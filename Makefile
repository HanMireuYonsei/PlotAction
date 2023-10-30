CC = g++

TARGET = PlotAction

CFLAGS = -Wall `root-config --cflags` -I./include
LDFLAGS = `root-config --glibs`
DEBUG = _DEBUG

SRC_DIR = ./src
OBJ_DIR = ./obj

SRCS = $(notdir $(wildcard $(SRC_DIR)/*.cc))
OBJS = $(patsubst %.o,$(OBJ_DIR)/%.o,$(SRCS:.cc=.o))
DEPS = $(OBJS:.o=.d)

all : $(TARGET)

debug : CFLAGS += -D$(DEBUG)
debug : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@ -MD $(LDFLAGS)

.PHONY : clean all
clean :
	rm -f $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)