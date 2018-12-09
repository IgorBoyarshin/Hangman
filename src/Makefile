# The name of the main file
mainFileName = main
# The name of the executable
executableName = Hangman
# Files that have .h and .cpp versions
classFiles = Client Server Display Game Log Renderer utils NetworkManager Key Keyboard
# Files that only have the .h version
justHeaderFiles = Drawer Communicator
# Compilation flags
OPTIMIZATION_FLAG = -O0
LANGUAGE_LEVEL = -std=c++17
COMPILER_FLAGS = -Wall -Wextra -Wno-unused-parameter
LINKER_FLAGS = -lncurses


# Auxiliary
filesObj = $(addsuffix .o, $(mainFileName) $(classFiles))
filesH = $(addsuffix .h, $(classFiles) $(justHeaderFiles))


all: cleanExe describe $(mainFileName)


# Compiler
# %.o: %.cpp $(filesH)
# 	$(CXX) $(COMPILER_FLAGS) $(OPTIMIZATION_FLAG) $(LANGUAGE_LEVEL) -c $<
Client.o: Client.cpp Client.h
	$(CXX) $(COMPILER_FLAGS) $(OPTIMIZATION_FLAG) $(LANGUAGE_LEVEL) -c $<
Server.o: Server.cpp Server.h
	$(CXX) $(COMPILER_FLAGS) $(OPTIMIZATION_FLAG) $(LANGUAGE_LEVEL) -c $<
NetworkManager.o: NetworkManager.cpp NetworkManager.h Communicator.h
	$(CXX) $(COMPILER_FLAGS) $(OPTIMIZATION_FLAG) $(LANGUAGE_LEVEL) -c $<
Display.o: Display.cpp Display.h utils.h Drawer.h Key.h Keyboard.h Log.h
	$(CXX) $(COMPILER_FLAGS) $(OPTIMIZATION_FLAG) $(LANGUAGE_LEVEL) -c $<
Drawer.o: Drawer.cpp Drawer.h Log.h utils.h
	$(CXX) $(COMPILER_FLAGS) $(OPTIMIZATION_FLAG) $(LANGUAGE_LEVEL) -c $<
Game.o: Game.cpp Game.h Display.h Renderer.h Communicator.h Keyboard.h Key.h Drawer.h utils.h Log.h
	$(CXX) $(COMPILER_FLAGS) $(OPTIMIZATION_FLAG) $(LANGUAGE_LEVEL) -c $<
Key.o: Key.cpp Key.h Log.h
	$(CXX) $(COMPILER_FLAGS) $(OPTIMIZATION_FLAG) $(LANGUAGE_LEVEL) -c $<
Keyboard.o: Keyboard.cpp Keyboard.h Key.h Log.h
	$(CXX) $(COMPILER_FLAGS) $(OPTIMIZATION_FLAG) $(LANGUAGE_LEVEL) -c $<
Log.o: Log.cpp Log.h
	$(CXX) $(COMPILER_FLAGS) $(OPTIMIZATION_FLAG) $(LANGUAGE_LEVEL) -c $<
utils.o: utils.cpp utils.h
	$(CXX) $(COMPILER_FLAGS) $(OPTIMIZATION_FLAG) $(LANGUAGE_LEVEL) -c $<
Renderer.o: Renderer.cpp Renderer.h Log.h Drawer.h utils.h
	$(CXX) $(COMPILER_FLAGS) $(OPTIMIZATION_FLAG) $(LANGUAGE_LEVEL) -c $<
main.o: main.cpp Game.h NetworkManager.h Log.h
	$(CXX) $(COMPILER_FLAGS) $(OPTIMIZATION_FLAG) $(LANGUAGE_LEVEL) -c $<

# Linker
$(mainFileName): $(filesObj)
	$(CXX) $(COMPILER_FLAGS) $(OPTIMIZATION_FLAG) $(LANGUAGE_LEVEL) $^ -o $(executableName) $(LINKER_FLAGS)


# Generate a piece of code that has accessible constant that holds
# the current project version (tag)
describe:
	@echo ":> Generating version.h"
	@echo "#ifndef VERSION_H" > version.h
	@echo "#define VERSION_H" >> version.h
	@echo "#include <string>" >> version.h
	@echo "const std::string GLOBAL_VERSION{\"$(shell git describe)\"};" >> version.h
	@echo "#endif" >> version.h


# Utils
clean:
	rm -f *.o *.gch .*.gch $(mainFileName) version.h

cleanExe:
	rm -f $(mainFileName)
