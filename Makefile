CC = g++
CFLAGS = -g -Wall -fPIC -pthread
#CFLAGS = -O2 -Wall -fPIC -pthread
#CFLAGS = -O3 -Wall -fPIC -pthread
LDFLAGS = -fPIC

RM = rm -f
CP = cp -f
MV = mv -f
AR = ar
RANLIB = ranlib
ECHO = echo

DEFINES = -D_REENTRANT


BIN_DIR = .
LIB_DIR = .
OBJ_DIR = .
DATA_DIR = test_data

INC_PATH = .

GTEST_LIB = -lgtest
LDLIBS    = -lpthread $(GTEST_LIB)

vpath %.o $(OBJ_DIR)
vpath %.a $(LIB_DIR)
vpath %.so $(JAVA_LIB_PATH)

TARGET_APP_NAME =
TARGET_LIB_NAME = Btools
TEST_APP_NAME   = Bfile_test Bstring_test Bcounter_example

TARGET_APP = $(TARGET_APP_NAME:%=$(BIN_DIR)/%)
TARGET_LIB = $(TARGET_LIB_NAME:%=$(LIB_DIR)/lib%.a)
TEST_APP   = $(TEST_APP_NAME:%=$(BIN_DIR)/%)


%.o: %.c
	$(CC) -c $(CFLAGS) $(DEFINES) $(INC_PATH:%=-I%) $< -o $(OBJ_DIR)/$@
%.o: %.cc
	$(CC) -c $(CFLAGS) $(DEFINES) $(INC_PATH:%=-I%) $< -o $(OBJ_DIR)/$@
%.o: %.cpp
	$(CC) -c $(CFLAGS) $(DEFINES) $(INC_PATH:%=-I%) $< -o $(OBJ_DIR)/$@


all:	$(TARGET_LIB) $(TARGET_APP) $(TEST_APP)

$(LIB_DIR)/libBtools.a: Bfile.o Bstring.o Btime.o Bcounter.o
	$(AR) rcv $@ $(addprefix $(OBJ_DIR)/,$(notdir $(filter %.o,$^))) $(filter-out %.o,$^)
	$(RANLIB) $@


Bfile_test: Bfile_test.o libBtools.a
	$(CC) -o $@ $(addprefix $(OBJ_DIR)/,$(notdir $(filter %.o,$^))) $(filter-out %.o,$^) $(LDFLAGS) $(LDLIBS)

Bstring_test: Bstring_test.o libBtools.a
	$(CC) -o $@ $(addprefix $(OBJ_DIR)/,$(notdir $(filter %.o,$^))) $(filter-out %.o,$^) $(LDFLAGS) $(LDLIBS)

Bcounter_example: Bcounter_example.o libBtools.a
	$(CC) -o $@ $(addprefix $(OBJ_DIR)/,$(notdir $(filter %.o,$^))) $(filter-out %.o,$^) $(LDFLAGS) $(LDLIBS)


clean:
	$(RM) $(TARGET_APP) $(TARGET_LIB) $(TEST_APP) $(OBJ_DIR)/*.o $(DATA_DIR)/*_output*

tag:
	ctags -B --C++-kinds=+l -R .

dep:
	$(CC) -MM $(CFLAGS) $(DEFINES) *.c *.cc *.cpp > .depend

-include .depend
