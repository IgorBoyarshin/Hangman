# Paths
GTEST_DIR = ../lib/googletest/googletest
GMOCK_DIR = ../lib/googletest/googlemock

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GMOCK_HEADERS = $(GMOCK_DIR)/include/gmock/*.h \
                $(GMOCK_DIR)/include/gmock/internal/*.h

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h
GMOCK_SRCS_ = $(GMOCK_DIR)/src/*.cc

TEST_NAME = main_test
TEST_NAME_DOCKER = main_test_docker
SOURCE_DIR = ../src
LIB_DIR = ../lib
TEST_DIR = .

# Flags
CPPFLAGS += -isystem $(GTEST_DIR)/include -isystem $(GMOCK_DIR)/include
CXXFLAGS += -g -std=gnu++17 -Wall -Wextra -pthread

# Jobs
run_tests : $(TEST_NAME)
	@echo "Running all tests..."
	./$(TEST_NAME)
	rm -f $(TEST_NAME)

run_tests_docker : $(TEST_NAME_DOCKER)
	@echo "Running all tests..."
	./$(TEST_NAME_DOCKER)
	rm -f $(TEST_NAME_DOCKER)

run_mockplayer : mockplayer
	@echo "Starting MockPlayer..."
	./mockplayer 127.0.0.1 8080
	rm -f mockplayer

run_mockplayer_docker : mockplayer
	@echo "Starting MockPlayer..."
	./mockplayer 172.17.0.2 8080
	rm -f mockplayer

# all : $(mockplayer) $(TEST_NAME)
# 	./mockplayer
# 	./$(TEST_NAME)

clean :
	rm -rf $(TEST_NAME) $(TEST_NAME_DOCKER) gtest.a gtest_main.a *.o gmock.a *.gch .*.gch


# Stuff
# MockPlayer that has to be running in background in order to execute other tests
MockPlayer.o : $(SOURCE_DIR)/Communicator.h $(SOURCE_DIR)/NetworkManager.h $(SOURCE_DIR)/NetworkManager.cpp MockPlayer.cpp
	$(CXX) $(CXXFLAGS) -c MockPlayer.cpp

mockplayer : MockPlayer.o NetworkManager.o Server.o Client.o
	$(CXX) $(CXXFLAGS) $^ -o mockplayer

# Other components
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

$(TEST_NAME_DOCKER).o : $(SOURCE_DIR)/Drawer.h $(SOURCE_DIR)/Communicator.h $(TEST_DIR)/$(TEST_NAME_DOCKER).cpp $(SOURCE_DIR)/Game.cpp $(SOURCE_DIR)/Game.h $(SOURCE_DIR)/Display.cpp $(SOURCE_DIR)/Display.h $(SOURCE_DIR)/Key.cpp $(SOURCE_DIR)/Key.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) -I$(SOURCE_DIR) $(CXXFLAGS) -c $(TEST_DIR)/$(TEST_NAME_DOCKER).cpp

$(TEST_NAME_DOCKER) : Server.o Client.o Renderer.o Display.o Game.o Key.o Keyboard.o NetworkManager.o utils.o $(TEST_NAME_DOCKER).o gtest.a gmock.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -L. -lncurses
