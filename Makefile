### Makefile ###

# Usage: 
# make -> compile whole project
# make clean -> clean build folder

### Variables ###
PROJECT := Cidr
CXX := clang++
CXXFLAGS := -std=c++17 -I include
LDFLAGS := -L lib
LDLIBS :=  -l SDL2-2.0.0
BUILDDIR := build
SOURCES := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,$(BUILDDIR)/%.o,$(SOURCES))

### Phony targets ###
.PHONY: all clean
all: $(BUILDDIR)/$(PROJECT)

# Include mostly header dependencies 
deps := $(patsubst %.cpp,%.d,$(SOURCES))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

### Compilation ###
# Compile project
$(BUILDDIR)/$(PROJECT) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@ 

# Compile .o files
$(BUILDDIR)/%.o : %.cpp # ugly, AND DOESN'T WORK
	$(CXX) $(CXXFLAGS) -c $< $(DEPFLAGS) -o $@
	
# clean build
clean: 
	rm $(BUILDDIR)/$(OBJS) $(BUILDDIR)/$(PROJECT) $(BUILDDIR)/$(deps) *.o *.d
