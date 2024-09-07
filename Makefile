TARGET_EXEC := param-racoon

BUILD_DIR := ./build
SRC_DIRS := ./src
INC_DIR = include

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. Make will incorrectly expand these otherwise.
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
INCS := $(shell find $(INC_DIR) -name '*.h')

# String substitution for every C/C++ file.
#    (substitution made with Substitution References)
#    https://www.gnu.org/software/make/manual/html_node/Substitution-Refs.html#Substitution-Refs
# As an example, hello.cpp turns into ./build/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)


CFLAGS=-c -Wall -Werror -I$(INC_DIR)
CXXFLAGS=-Wall -Werror -std=c++17 -I$(INC_DIR)
LDFLAGS=-lcurl -lpthread -pthread -lboost_thread -lboost_program_options

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

# Run tests
.PHONY: run-tests
run-tests: $(BUILD_DIR)/$(TARGET_EXEC)
	@echo "\n---------- Running basic.php test ----------\n"
	./$(BUILD_DIR)/$(TARGET_EXEC) -w test/wordlist/test.txt 'http://localhost:8888/basic.php'
	@echo "\n---------- Running reflecting.php test ----------\n"
	./$(BUILD_DIR)/$(TARGET_EXEC) -w test/wordlist/test.txt 'http://localhost:8888/reflecting.php'
	@echo "\n---------- Running headers.php test ----------\n"
	./$(BUILD_DIR)/$(TARGET_EXEC) -w test/wordlist/test.txt -H 'X-Required-Header: someval' -H 'X-Other-Required-Header: otherval' 'http://localhost:8888/headers.php'
	@echo "\n---------- Running cookies.php test ----------\n"
	./$(BUILD_DIR)/$(TARGET_EXEC) -w test/wordlist/test.txt -c 'PHPSESSID=dc12e575ca; LANG=en' 'http://localhost:8888/cookies.php'
	@echo "\n---------- Running match.php test ----------\n"
	./$(BUILD_DIR)/$(TARGET_EXEC) -w test/wordlist/test.txt -m 'found a secret' 'http://localhost:8888/match.php'
	@echo "\n---------- Running match.php test with filtering ----------\n"
	./$(BUILD_DIR)/$(TARGET_EXEC) -w test/wordlist/test.txt -f 'found a nothing' 'http://localhost:8888/match.php'
	@echo "\n---------- Running post.php test ----------\n"
	./$(BUILD_DIR)/$(TARGET_EXEC) -w test/wordlist/test.txt -X POST 'http://localhost:8888/post.php'
	@echo "\n---------- Running post_with_required_params.php test ----------\n"
	./$(BUILD_DIR)/$(TARGET_EXEC) -w test/wordlist/test.txt -X POST -d 'required=foo&otherrequired=bar' 'http://localhost:8888/post_with_required_params.php'
	@echo "\n---------- Running random_len.php test ----------\n"
	./$(BUILD_DIR)/$(TARGET_EXEC) -w test/wordlist/test.txt -D 5 'http://localhost:8888/random_len.php'

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
	clang-format $(CLANGFORMATARGS) -i $(SRCS) $(INCS)

.PHONY: check-format
check-format:
	clang-format $(CLANGFORMATARGS) --dry-run $(SRCS) $(INCS)

# clang-tidy targets

.PHONY: tidy
tidy:
	clang-tidy $(SRCS) -- $(CXXFLAGS)

.PHONY: tidy-fix
tidy-fix:
	clang-tidy -fix $(SRCS) -- $(CXXFLAGS)