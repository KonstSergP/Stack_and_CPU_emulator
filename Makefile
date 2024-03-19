#-----------------------
# Compiler/linker flags
#-----------------------

CXX = g++

# Compiler flags:
CXXFLAGS = \
	-std=c++17 \
	-Wall      \
	-Wextra    \
	-Werror

# Select build mode:
ifeq ($(DEBUG),1)
	CXXFLAGS += -g
else
	CXXFLAGS  += -flto
	LDFLAGS += -flto
endif

#--------
# Colors
#--------

# Use ANSI color codes:
BRED    = \033[1;31m
BGREEN  = \033[1;32m
BYELLOW = \033[1;33m
GREEN   = \033[1;35m
BCYAN   = \033[1;36m
RESET   = \033[0m

#-------
# Files
#-------

# Add "include" folder to header search path:
#CXXFLAGS += -I $(abspath include)

INCLUDES = $(shell find ./ -name \*.hpp)

SOURCES := $(shell find ./src -name \*.cpp)

VPATH := $(dir $(SOURCES))

OBJDIR := ./build
OBJECTS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(notdir $(SOURCES)))

EXECUTABLE := $(OBJDIR)/main

#---------------
# Build process
#---------------

$(OBJDIR)/%: examples/%.cpp $(OBJECTS)
	@printf "$(BYELLOW)Building object file $(BCYAN)$@$(RESET)\n"
	@mkdir -p build
	$(CXX) -c $< $(CXXFLAGS) -o $@.o
	@printf "$(BYELLOW)Linking executable $(BCYAN)$@$(RESET)\n"
	$(CXX) $(LDFLAGS) $(OBJECTS) $@.o -o $@

default: $(EXECUTABLE)

$(OBJDIR)/%.o: %.cpp $(INCLUDES) Makefile
	@printf "$(BYELLOW)Building object file $(BCYAN)$@$(RESET)\n"
	@mkdir -p build
	$(CXX) -c $< $(CXXFLAGS) -o $@

#--------------
# Run scripts
#--------------

# Run program:
run: $(EXECUTABLE)
	@./$(EXECUTABLE)

phibonacci: $(EXECUTABLE)
	@./$(EXECUTABLE) phibonacci.test

factorial: $(EXECUTABLE)
	@./$(EXECUTABLE) factorial.test

factorial_rec: $(EXECUTABLE)
	@./$(EXECUTABLE) factorial_rec.test

%: $(OBJDIR)/%
	@./$<

#---------------
# Other tools
#---------------

clean:
	@printf "$(BYELLOW)Cleaning build directory $(RESET)\n"
	rm -rf build

# List of non-file targets:
.PHONY: run clean default
.PRECIOUS: $(OBJDIR)/%.o % $(OBJDIR)/%