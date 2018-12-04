# The name of the main file and executable
mainFileName = main
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
%.o: %.cpp $(filesH)
	$(CXX) $(COMPILER_FLAGS) $(OPTIMIZATION_FLAG) $(LANGUAGE_LEVEL) -c $<

# Linker
$(mainFileName): $(filesObj)
	$(CXX) $(COMPILER_FLAGS) $(OPTIMIZATION_FLAG) $(LANGUAGE_LEVEL) $^ -o $@ $(LINKER_FLAGS)


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
