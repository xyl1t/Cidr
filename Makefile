### Makefile ###

# Usage: 
# make -> compile whole project
# make clean -> clean build folder

### Variables ###
PROJECTNAME := Cidr
BUILDDIR := build
PROJECT := $(BUILDDIR)/$(PROJECTNAME)
SOURCES := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,$(BUILDDIR)/%.o,$(SOURCES))
CXX := clang++
CXXFLAGS := -std=c++17 -I include -Ofast
LDFLAGS := -L lib
LDLIBS :=  -l SDL2-2.0.0 -lm

### Phony targets ###
.PHONY: all clean
all: $(PROJECT)

# Include mostly header dependencies 
deps := $(patsubst %.o,%.d,$(OBJS))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

### Compilation ###
# Compile project
$(PROJECT) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@ 

# Compile .o files
$(BUILDDIR)/%.o : %.cpp # ugly, AND DOESN'T WORK
	$(CXX) $(CXXFLAGS) -c $< $(DEPFLAGS) -o $@
	
# clean build
clean: 
	@rm -rf build/*
