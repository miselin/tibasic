PROJDIRS := src
SRCFILES := $(shell find $(PROJDIRS) -type f -name "*.cpp")
OBJFILES := $(patsubst %.cpp,%.o,$(SRCFILES))
DEPFILES := $(patsubst %.cpp,%.d,$(SRCFILES))

.PHONY: all clean

CXXFLAGS := -g -Wall -pedantic -Werror

CXX := g++

all: tibasic

tibasic: $(OBJFILES)
	g++ -o tibasic $?

-include $(DEPFILES)

%.o: %.c
	@$(CXX) $(CFLAGS) -MMD -MP -c $< -o $@
