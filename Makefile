### Makefile ###

# Usage: 
# make -> compile whole project
# make clean -> clean build folder

### Variables ###
PROJECT := Cidr
CXX := clang++
CXXFLAGS := -std=c++17 -I include -L lib -l SDL2-2.0.0
BUILDDIR := build
SOURCES := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SOURCES))

### Phony targets ###
.PHONY: all clean
all: $(BUILDDIR)/$(PROJECT)

# Include mostly header dependencies 
deps := $(patsubst %.o,%.d,$(OBJS))
-include $(BUILDDIR)/$(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

### Compilation ###
# Compile project
$(BUILDDIR)/$(PROJECT) : $(BUILDDIR)/$(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ 

# Compile .o files
$(BUILDDIR)/%.o : %.cpp # ugly, but it works
	$(CXX) $(CXXFLAGS) -c $< $(DEPFLAGS) -o $@ 
	
# clean build
clean: 
	rm $(BUILDDIR)/$(OBJS) $(BUILDDIR)/$(PROJECT) $(BUILDDIR)/$(deps)
