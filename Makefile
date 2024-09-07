TARGET_EXEC := param-racoon

BUILD_DIR := ./build
SRC_DIRS := ./src

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. Make will incorrectly expand these otherwise.
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')

# String substitution for every C/C++ file.
#    (substitution made with Substitution References)
#    https://www.gnu.org/software/make/manual/html_node/Substitution-Refs.html#Substitution-Refs
# As an example, hello.cpp turns into ./build/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)


INC_DIR = include
CFLAGS=-c -Wall -Werror -I$(INC_DIR)
CXXFLAGS=-Wall -Werror -I$(INC_DIR)

CLANGFORMATARGS=--style=google

# run `make build DEBUG=true` to build with debug symbols
ifeq ($(DEBUG),true)
	override CFLAGS+=-ggdb
	override CXXFLAGS+=-ggdb
endif

# Run
.PHONY: build
build: $(BUILD_DIR)/$(TARGET_EXEC)
	@echo done

# Run
.PHONY: run
run: $(BUILD_DIR)/$(TARGET_EXEC)
	./$(BUILD_DIR)/$(TARGET_EXEC)

# The final build step.
# Note: you may need to use $(CXX) here instead of $(CC)
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

# clang-format targets

.PHONY: format
format:
	clang-format $(CLANGFORMATARGS) -i $(SRCS)

.PHONY: check-format
check-format:
	clang-format $(CLANGFORMATARGS) --dry-run $(SRCS)
