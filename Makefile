# ================================================================================
# Generic Makefile template for TUKE prog-2023 course in C programming language
# Suitable for small to medium projects
# author: kristian.koribsky@student.tuke.sk
# ================================================================================



# UTILS	
# ================================================================================
ifneq ($(findstring xterm,${TERM}), )
	RED         := $(shell tput -Txterm setaf 1)
	GREEN       := $(shell tput -Txterm setaf 2)
	YELLOW      := $(shell tput -Txterm setaf 3)
	BLUE        := $(shell tput -Txterm setaf 4)
	BOLD		:= $(shell tput -Txterm bold)
	RESET		:= $(shell tput -Txterm sgr0)
else
	RED         := ""
	GREEN       := ""
	YELLOW      := ""
	BLUE        := ""
	BOLD		:= ""
	RESET       := ""
endif

ifeq ($(OS),Windows_NT)
	ifeq ($(shell uname -s),)
		RM		:= del /F /Q
		MKDIR	:= mkdir
	else
	RM			:= rm -f
	MKDIR		:= mkdir -p
	endif
	EXT			:= .exe
else
	RM 			:= rm -f
	MKDIR 		:= mkdir -p
	EXT			:= .out
endif

COMMIT 			:= git add -A && git commit



# OPTIONS
# ================================================================================
MAKEFLAGS 				+= --no-print-directory --warn-undefined-variables

ifneq ($(debug), 1)
	MAKEFLAGS 			+= --silent
endif

ifeq ($(always), 1)
	MAKEFLAGS			+= --always-make
endif

ifeq ($(run), 1)
	MAKEFLAGS   		+= --always-make
	RUN					+= ./$(BIN)
endif

ifeq ($(w), 1)
	FLG_COMPILE 		= $(FLG_COMPILE_0) $(FLG_COMPILE_1) 
	FLG_STATIC 			= $(FLG_STATIC_0) $(FLG_STATIC_1)
	FLG_DYNAMIC 		= $(FLG_DYNAMIC_0) $(FLG_DYNAMIC_1)
else
	FLG_COMPILE 		= $(FLG_COMPILE_0)
	FLG_STATIC 			= $(FLG_STATIC_0)
	FLG_DYNAMIC 		= $(FLG_DYNAMIC_0)
endif

ifdef bin
	BIN					= $(DIR_BIN)$(bin)
endif

ifdef src
	SRC					= $(addprefix $(DIR_SRC), $(src))
endif

ifdef libs
	LIBS				= $(addprefix -, $(libs))
endif



# INGREDIENTS
# ================================================================================
# Directories
DIR_SRC			:= ./
DIR_TST			:= tests/

DIR_UNI			:= ../unity/src/
DIR_UTI			:= ../utils/src/

DIR_BLD			:= build/
DIR_BIN			:= build/bin/
DIR_DEP			:= build/depends/
DIR_OBJ			:= build/objs/
DIR_RES			:= build/results/
DIR_OUT			:= build/results/out

BUILD_DIRS		:= $(DIR_BLD) $(DIR_BIN) $(DIR_DEP) $(DIR_OBJ) $(DIR_RES) $(DIR_OUT)

# Files
BIN				?= $(DIR_BIN)build$(EXT)
SRC				?= $(wildcard $(DIR_SRC)*.c)

OBJ				:= $(patsubst $(DIR_SRC)%.c, $(DIR_OBJ)%.o, $(SRC))
OBJ_UTI			:= $(patsubst $(DIR_UTI)%.c, $(DIR_OBJ)%.o, $(wildcard $(DIR_UTI)*.c))
DEP 			:= $(patsubst $(DIR_SRC)%.c, $(DIR_DEP)%.d, $(SRC))

TESTS			:= $(patsubst $(DIR_TST)%.c, $(DIR_RES)%.txt, $(wildcard $(DIR_TST)*.c))

# Compilation
PATHS			?= 
MACRO			?= DEBUG
LIBS 			?= -lm
RUN				?= 

LINK			:= gcc
COMPILE			:= gcc -c
DEPEND			:= -MMD -MF

FLG_COMPILE_0	:= -std=c11 -Werror -Wall -Wconversion -ggdb3 $(addprefix -I, $(PATHS)) $(addprefix -D, $(MACRO))
FLG_COMPILE_1	:= -pedantic -Wextra -Wshadow 


# Static analysis
STATIC			:= cppcheck

FLG_STATIC_0	:= --enable=performance --error-exitcode=1 --language=c
FLG_STATIC_1	:= --enable=all --suppress=missingIncludeSystem

# Dynamic analysis
DYNAMIC			:= valgrind

FLG_DYNAMIC_0	:= --show-error-list=yes --track-origins=yes
FLG_DYNAMIC_1	:= --leak-check=full --show-leak-kinds=all

# Test
TEST_PATHS		?= .
TEST_MACRO		?= TEST UNITY_TEST

FLG_TEST		:= $(addprefix -I, $(TEST_PATHS)) $(addprefix -D, $(TEST_MACRO))

# Debug
DEBUG			:= gdb
FLG_DEBUG		:= --tui --silent



# RECIPES
# ================================================================================
all: build

build: announce-build $(BUILD_DIRS) $(BIN)
test: announce-test $(BUILD_DIRS) $(TESTS)
	$(MSG_TEST_RUN)
	
	for test in $(TESTS) ; do \
		file="$$(basename $$test .txt).c" ; \
		[[ $$(tail -n 1 $$test) = "OK" ]] && res="OK" clr="$(GREEN)" || res="FAIL" clr="$(RED)" ; \
		printf "%-20s %s\n" "[ $$clr$$res$(RESET) ]" "$$file" ; \
	done

	$(MSG_DONE)

static: $(SRC)
	$(MSG_STATIC)
	$(STATIC) $(FLG_STATIC) $(SRC)

dynamic: build
	$(MSG_DYNAMIC)
	$(DYNAMIC) $(FLG_DYNAMIC) ./$(BIN)

debug: build
	$(MSG_DEBUG)
	$(DEBUG) $(FLG_DEBUG) ./$(BIN)

full: build static dynamic test

announce-build:
	$(MSG_BUILD)

announce-test:
	$(MSG_TEST)

clean:
	$(MSG_CLEAN)
	$(RM) -r $(DIR_BLD)
	$(MSG_DONE)

commit: clean
	$(MSG_COMMIT)
	$(COMMIT)
	$(MSG_DONE)

help:
	$(MSG_HELP)

-include $(DEP)

$(BIN): $(OBJ)
	echo -e "$(GREEN)Linking into $(BOLD)$@$(RESET)"
	$(LINK) $^ -o $@ $(LIBS)
	$(MSG_DONE)
	$(RUN)
	
$(DIR_OBJ)%.o:: $(DIR_SRC)%.c
	echo -e "$(GREEN)Compiling $*.o ...$(RESET)"
	$(COMPILE) $(DEPEND) $(DIR_DEP)$*.d $(FLG_COMPILE) $< -o $@
	$(MSG_DONE)

$(DIR_OBJ)%.o:: $(DIR_TST)%.c
	$(COMPILE) $(FLG_TEST) $< -o $@

$(DIR_OBJ)%.o:: $(DIR_UNI)%.c $(DIR_UNI)%.h
	$(COMPILE) $(FLG_TEST) $< -o $@

$(DIR_OBJ)%.o:: $(DIR_UTI)%.c $(DIR_UTI)%.h
	$(COMPILE) $(FLG_TEST) $< -o $@

$(DIR_RES)%.txt: $(DIR_BIN)%$(EXT)
	-./$< > $@ 2>&1

$(DIR_BIN)testh_%$(EXT): $(DIR_OBJ)testh_%.o $(DIR_OBJ)unity.o $(DIR_OBJ)%.o $(OBJ_UTI)
	$(LINK) $^ -o $@ $(LIBS)

$(DIR_BIN)testc_%$(EXT): $(DIR_OBJ)testc_%.o $(DIR_OBJ)unity.o $(OBJ_UTI)
	$(LINK) $^ -o $@ $(LIBS)

$(BUILD_DIRS):
	$(MKDIR) $@



# MESSAGES
# ================================================================================
MSG_BUILD		:= echo -e "$(BOLD)$(YELLOW)MAKEFILE: BUILD$(RESET)"
MSG_STATIC		:= echo -e "$(BOLD)$(YELLOW)MAKEFILE: STATIC ANALYSIS$(RESET)"
MSG_DYNAMIC		:= echo -e "$(BOLD)$(YELLOW)MAKEFILE: DYNAMIC ANALYSIS$(RESET)"
MSG_TEST		:= echo -e "$(BOLD)$(YELLOW)MAKEFILE: TEST$(RESET)"
MSG_DEBUG		:= echo -e "$(BOLD)$(YELLOW)MAKEFILE: DEBUG$(RESET)"

MSG_TEST_RUN	:= echo -e "$(GREEN)Running tests ...$(RESET)"
MSG_CLEAN		:= echo -e "$(GREEN)Removing $(DIR_BLD) ...$(RESET)"
MSG_COMMIT		:= echo -e "$(GREEN)Committing ...$(RESET)"
MSG_DONE		:= echo -e "$(BLUE)done$(RESET)"

define msg_help
$(BOLD)$(YELLOW)MAKEFILE: HELP$(RESET)
$(GREEN)Usage: make [targets] [options]$(RESET)

$(BLUE)Targets:$(RESET)
  build			Build the project
  static		Perform static analysis
  dynamic		Perform dynamic analysis
  test			Run tests
  debug			Debug compiled executable
  full			Complete build, static analysis, dynamic analysis and testing
  clean			Clean build artifacts
  commit		Clean and commit changes to git
  help			Display this help message

$(BLUE)Options:$(RESET)
  debug=1		Enable verbose output
  always=1		Always build, even if files are up to date
  run=1			Run the compiled binary after building
  w=1			Level of warning flags (default is 0)
  bin=''		Specify the name of the generated executable
  src=''		Specify source files to build
  libs=''		Specify additional libraries

$(BLUE)Examples:$(RESET)
  make
  make test
  make clean
  make debug src="dir/file1.c"
  make build static test w=1 always=1
  make build libs="lm lcurses" debug=1 run=1
  make full src="file1.c file2.c" bin="build.out" w=1
endef

export msg_help
MSG_HELP		:= echo -e "$$msg_help"



# SPEC
# ================================================================================
.PHONY: all build test static dynamic full announce-build announce-test clean commit help