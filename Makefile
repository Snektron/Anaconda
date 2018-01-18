TARGET := build/target/Anaconda
SRC := src
BUILD := build
LIBS := 
FLAGS := -g -O3 -Wall -Wextra -std=c++17 -I$(SRC)

rwildcard = $(foreach d, $(wildcard $1*), $(call rwildcard, $d/, $2) $(filter $(subst *, %, $2), $d))

SRCS := $(patsubst $(SRC)/%, %, $(call rwildcard, $(SRC)/, *.cpp))
OBJECTS := $(SRCS:%.cpp=%.o)

ESC := $(shell echo -e "\033")
RED := $(ESC)[1;31m
WHITE := $(ESC)[1;37m
BLUE := $(ESC)[1;34m
YELLOW := $(ESC)[1;33m
CLEAR := $(ESC)[0m

TOTAL := $(words $(OBJECTS) .)
progress = $(or $(eval PROCESSED := $(PROCESSED) .),$(info $(WHITE)[$(YELLOW)$(words $(PROCESSED))$(WHITE)/$(YELLOW)$(TOTAL)$(WHITE)] $1$(CLEAR)))

vpath %.o $(BUILD)/objects
vpath %.cpp $(SRC)

all: $(TARGET)
	@echo Done!
 
$(TARGET): $(OBJECTS)
	@$(call progress,$(RED)Linking $@)
	@mkdir -p $(BUILD)/target/
	@$(CXX) $(FLAGS) -o $@ $(OBJECTS:%=$(BUILD)/objects/%) $(LIBS:%=-l%)

%.o: %.cpp
	@$(call progress,$(BLUE)Compiling $<)
	@mkdir -p $(BUILD)/objects/$(dir $@)
	@$(CXX) $(FLAGS) -c -o $(BUILD)/objects/$@ $<

clean:
	@echo Cleaning build files
	@rm -rf $(BUILD) $(TARGET)
	
run: all
	@./$(TARGET) test.an
	
force: clean all
	
.PHONY: clean force
