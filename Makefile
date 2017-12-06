TARGET = Anaconda
SRC = src
BUILD = build
LIBS = 
FLAGS = -g -O3 -Wall -Wextra -std=c++17 -I$(SRC)

rwildcard = $(foreach d, $(wildcard $1*), $(call rwildcard, $d/, $2) $(filter $(subst *, %, $2), $d))

FMT = $(call rwildcard, $(SRC)/, *.cpp *.h *.inl)
SRCS = $(patsubst $(SRC)/%, %, $(call rwildcard, $(SRC)/, *.cpp))
OBJECTS = $(SRCS:%.cpp=%.o)

vpath %.o $(BUILD)/objects
vpath %.cpp $(SRC)

all: $(TARGET)
	@echo Done!
 
$(TARGET): $(OBJECTS)
	@echo Linking $@
	@mkdir -p $(BUILD)/target/
	@$(CXX) $(FLAGS) -o $@ $(OBJECTS:%=$(BUILD)/objects/%) $(LIBS:%=-l%)

%.o: %.cpp
	@echo Compiling $<
	@mkdir -p $(BUILD)/objects/$(dir $@)
	@$(CXX) $(FLAGS) -c -o $(BUILD)/objects/$@ $<

clean:
	@echo Cleaning build files
	@rm -rf $(BUILD) $(TARGET)
	
format:
	@$(foreach file, $(FMT), $(shell E:\\Programs\\msys2\\usr\\bin\\expand -i -t 4 $(file) > $(file)))
	
run: all
	@./$(TARGET) test.an
	
force: clean all
	
.PHONY: clean force
