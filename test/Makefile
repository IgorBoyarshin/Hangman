GTEST_DIR = ../lib/googletest/googletest
GMOCK_DIR = ../lib/googletest/googlemock

CPPFLAGS += -isystem $(GTEST_DIR)/include -isystem $(GMOCK_DIR)/include
CXXFLAGS += -g -std=gnu++17 -Wall -Wextra -pthread

TEST_NAME = main_test
SOURCE_DIR = ../src
LIB_DIR = ../lib
TEST_DIR = .

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GMOCK_HEADERS = $(GMOCK_DIR)/include/gmock/*.h \
                $(GMOCK_DIR)/include/gmock/internal/*.h

all : $(TEST_NAME)
	@echo "Running all"

MockPlayer.o : $(SOURCE_DIR)/Communicator.h $(SOURCE_DIR)/NetworkManager.h $(SOURCE_DIR)/NetworkManager.cpp MockPlayer.cpp
	$(CXX) $(CXXFLAGS) -c MockPlayer.cpp

mockplayer : MockPlayer.o NetworkManager.o Server.o Client.o
	$(CXX) $(CXXFLAGS) $^ -o mockplayer


clean :
	rm -rf $(TEST_NAME) gtest.a gtest_main.a *.o gmock.a *.gch .*.gch


GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h
GMOCK_SRCS_ = $(GMOCK_DIR)/src/*.cc

gtest-all.o : $(GTEST_SRCS_) $(GTEST_HEADERS)
	@echo "Running oo 1"
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc

gmock-all.o : $(GMOCK_SRCS_) $(GMOCK_HEADERS) gtest-all.o
	@echo "Running oo 2"
	$(CXX) $(CPPFLAGS) -I$(GMOCK_DIR) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GMOCK_DIR)/src/gmock-all.cc

gtest.a : gtest-all.o
	@echo "Running aa 1"
	$(AR) $(ARFLAGS) $@ $^

gmock.a : gmock-all.o
	@echo "Running aa 2"
	$(AR) $(ARFLAGS) $@ $^


Game.o : $(SOURCE_DIR)/Game.cpp $(SOURCE_DIR)/Game.h $(SOURCE_DIR)/Display.cpp $(SOURCE_DIR)/Display.h $(SOURCE_DIR)/Keyboard.cpp $(SOURCE_DIR)/Keyboard.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SOURCE_DIR)/Game.cpp

Renderer.o : $(SOURCE_DIR)/Renderer.cpp $(SOURCE_DIR)/Renderer.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SOURCE_DIR)/Renderer.cpp

Keyboard.o : $(SOURCE_DIR)/Keyboard.cpp $(SOURCE_DIR)/Keyboard.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SOURCE_DIR)/Keyboard.cpp

Key.o : $(SOURCE_DIR)/Key.cpp $(SOURCE_DIR)/Key.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SOURCE_DIR)/Key.cpp

Server.o : $(SOURCE_DIR)/Server.cpp $(SOURCE_DIR)/Server.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SOURCE_DIR)/Server.cpp

Client.o : $(SOURCE_DIR)/Client.cpp $(SOURCE_DIR)/Client.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SOURCE_DIR)/Client.cpp

NetworkManager.o : $(SOURCE_DIR)/NetworkManager.cpp $(SOURCE_DIR)/NetworkManager.h $(SOURCE_DIR)/Client.h $(SOURCE_DIR)/Client.cpp $(SOURCE_DIR)/Server.h $(SOURCE_DIR)/Server.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SOURCE_DIR)/NetworkManager.cpp

utils.o : $(SOURCE_DIR)/utils.cpp $(SOURCE_DIR)/utils.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SOURCE_DIR)/utils.cpp

Display.o : $(SOURCE_DIR)/Display.cpp $(SOURCE_DIR)/Display.h $(SOURCE_DIR)/Keyboard.cpp $(SOURCE_DIR)/Keyboard.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SOURCE_DIR)/Display.cpp

$(TEST_NAME).o : $(SOURCE_DIR)/Drawer.h $(SOURCE_DIR)/Communicator.h $(TEST_DIR)/$(TEST_NAME).cpp $(SOURCE_DIR)/Game.cpp $(SOURCE_DIR)/Game.h $(SOURCE_DIR)/Display.cpp $(SOURCE_DIR)/Display.h $(SOURCE_DIR)/Key.cpp $(SOURCE_DIR)/Key.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) -I$(SOURCE_DIR) $(CXXFLAGS) -c $(TEST_DIR)/$(TEST_NAME).cpp

$(TEST_NAME) : Server.o Client.o Renderer.o Display.o Game.o Key.o Keyboard.o NetworkManager.o utils.o $(TEST_NAME).o gtest.a gmock.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -L. -lncurses
