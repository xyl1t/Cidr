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
CXXFLAGS := -std=c++17 -I include -O2 -Wall
LDFLAGS := -L lib
LDLIBS :=  -l SDL2-2.0.0 -lm

### Phony targets ###
.PHONY: all multi clean
multi: 
	$(MAKE) -j8 all
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
$(BUILDDIR)/%.o : %.cpp # ugly
	$(CXX) $(CXXFLAGS) -c $< $(DEPFLAGS) -o $@
	
# clean build
clean: 
	@rm -rf build/*
