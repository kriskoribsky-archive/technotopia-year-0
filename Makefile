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
	EXT			:= exe
else
	RM 			:= rm -f
	MKDIR 		:= mkdir -p
	EXT			:= out
endif

COMMIT 			:= git add -A && git commit



# OPTIONS
# ================================================================================
# Makefile
MAKEFLAGS 				= --no-builtin-variables --no-print-directory --warn-undefined-variables --silent
ifdef verbose
	MAKEFLAGS 			+= --no-silent
endif

ifdef always
	MAKEFLAGS			+= --always-make
endif

ifeq ($(debug), 1)
	MAKEFLAGS			+= --just-print
else ifeq ($(debug), 2)
	MAKEFLAGS			+= --debug
endif


# Files
ifdef main
	MAIN				:= $(main)
endif

ifdef mods
	MODS				:= $(mods)
endif


# Tools
LINK					:= gcc
COMP					:= gcc -c
STATIC					:= cppcheck
DYNAMIC					:= valgrind
DEBUG					:= gdb


# Compile
LIBS 					= -lm
MACRO_COMP				= DEBUG
MACRO_TEST				= TEST UNITY_TEST
PATH_COMP				= 
PATH_TEST				= .
RUN						= 

ifdef libs
	LIBS				:= $(addprefix -, $(libs))
endif

ifdef macro
	MACRO_COMP			:= $(macro)
endif

ifdef tmacro
	MACRO_TEST			:= $(tmacro)
endif

ifdef path
	PATH_COMP			:= $(path)
endif

ifdef tpath
	PATH_TEST			:= $(tpath)
endif

ifdef run
	MAKEFLAGS   		+= --always-make
	RUN					= ./$@
endif


# Flags
FLG_COMP				= -std=c11 -Werror -Wall -Wconversion -ggdb3
FLG_TEST				= 
FLG_DEPEND				= 
FLG_STATIC				= --enable=performance --error-exitcode=1 --language=c --suppress=missingIncludeSystem
FLG_DYNAMIC				= --show-error-list=yes --track-origins=yes	
FLG_DEBUG				= --tui --silent

ifeq ($(w), 1)
	FLG_COMP 			+=  -pedantic -Wextra -Wshadow
	FLG_STATIC 			+= --enable=all --suppress=knownConditionTrueFalse
	FLG_DYNAMIC 		+= --leak-check=full --show-leak-kinds=all
else ifeq ($(w), 2)
	FLG_COMP 			+= -pedantic -Wextra -Wshadow -Wmissing-prototypes -Wstrict-prototypes
	FLG_STATIC 			+= --enable=all --suppress=knownConditionTrueFalse
	FLG_DYNAMIC 		+= --leak-check=full --show-leak-kinds=all
endif

FLG_COMP				+= $(addprefix -I, $(PATH_COMP)) $(addprefix -D, $(MACRO_COMP))
FLG_TEST				+= $(addprefix -I, $(PATH_TEST)) $(addprefix -D, $(MACRO_TEST))
FLG_DEPEND				+= -MT $@ -MMD -MP -MF $(DIR_DEP)$(notdir $(basename $@)).d

ifdef test
	FLG_COMP			+= -DNDEBUG
endif


# INGREDIENTS
# ================================================================================
# Directories
DIR_SRC			:= ./
DIR_SRT			:= tests/

DIR_UNI			:= ../unity/src/
DIR_UTI			:= ../utils/src/

DIR_BLD			:= build/
DIR_BIN			:= build/bin/
DIR_DEP			:= build/dep/
DIR_OBJ			:= build/obj/
DIR_TST			:= build/tests/
DIR_TSC			:= build/tests/testcases/

BUILD_DIRS		:= $(DIR_BLD) $(DIR_BIN) $(DIR_DEP) $(DIR_OBJ) $(DIR_TST) $(DIR_TSC)

# Files
MAIN			?= main.c
MODS			?= $(filter-out $(MAIN), $(patsubst $(DIR_SRC)%.c, %.c, $(wildcard $(DIR_SRC)*.c)))

SRC_MAIN		:= $(addprefix $(DIR_SRC), $(MAIN))
SRC_MODS		:= $(addprefix $(DIR_SRC), $(MODS))

OBJ_MAIN		:= $(patsubst $(DIR_SRC)%.c, $(DIR_OBJ)%.o, $(SRC_MAIN))
OBJ_MODS		:= $(patsubst $(DIR_SRC)%.c, $(DIR_OBJ)%.o, $(SRC_MODS))
OBJ_TST			:= $(patsubst $(DIR_SRC)%.c, $(DIR_TST)%.o, $(SRC_MODS))
OBJ_UTI			:= $(patsubst $(DIR_UTI)%.c, $(DIR_TST)%.o, $(wildcard $(DIR_UTI)*.c))
OBJ_UNI			:= $(patsubst $(DIR_UNI)%.c, $(DIR_TST)%.o, $(wildcard $(DIR_UNI)*.c))

BIN				:= $(addsuffix .$(EXT), $(addprefix $(DIR_BIN), $(patsubst %.c, %, $(MAIN))))
DEP 			:= $(patsubst $(DIR_SRC)%.c, $(DIR_DEP)%.d, $(SRC_MAIN) $(SRC_MODS))
TST				:= $(patsubst $(DIR_SRT)%.c, $(DIR_TST)%.txt, $(wildcard $(DIR_SRT)*.c))



# RECIPES
# ================================================================================
build: prepare-build $(BUILD_DIRS) $(BIN)
test: prepare-test $(BUILD_DIRS) $(TST)
	$(MSG_TEST_RUN)
	
	for test in $(TST) ; do \
		file="$$(basename $$test .txt).c" ; \
		[[ $$(tail -n 1 $$test) = "OK" ]] && res="OK" clr="$(GREEN)" || res="FAIL" clr="$(RED)" ; \
		printf "%-20s %s\n" "[ $$clr$$res$(RESET) ]" "$$file" ; \
	done

	$(MSG_DONE)

static: $(SRC_MAIN) $(SRC_MODS)
	$(MSG_STATIC)
	$(STATIC) $(FLG_STATIC) $(SRC_MAIN) $(SRC_MODS)

dynamic: build
	$(MSG_DYNAMIC)
	$(DYNAMIC) $(FLG_DYNAMIC) ./$(BIN)

debug: build
	$(MSG_DEBUG)
	$(DEBUG) $(FLG_DEBUG) ./$(BIN)

full: build static dynamic test

prepare-build:
	$(MSG_BUILD)

prepare-test:
	$(MSG_TEST)

clean:
	$(MSG_CLEAN)
	$(RM) -r $(BUILD_DIRS)
	$(MSG_DONE)

commit: clean
	$(MSG_COMMIT)
	$(COMMIT)
	$(MSG_DONE)

help:
	$(MSG_HELP)

$(DIR_BIN)%.$(EXT): $(OBJ_MODS) $(DIR_OBJ)%.o
	$(MSG_LINK)
	$(LINK) $^ -o $@ $(LIBS)
	$(MSG_DONE)
	$(RUN)

$(DIR_OBJ)%.o:: $(DIR_SRC)%.c
	$(MSG_COMPILE)
	$(COMP) $(FLG_DEPEND) $(FLG_COMP) $< -o $@
	$(MSG_DONE)

$(DIR_TST)test_%.txt: $(OBJ_UNI) $(OBJ_UTI) $(OBJ_TST) $(DIR_TST)test_%.o
	$(LINK) $^ -o $@.$(EXT) $(LIBS)
	-./$@.$(EXT) > $@ 2>&1
	$(RM) $@.$(EXT)

$(DIR_TST)%.o:: $(DIR_SRC)%.c
	$(COMP) $(FLG_TEST) $< -o $@

$(DIR_TST)%.o:: $(DIR_SRT)%.c
	$(COMP) $(FLG_TEST) $< -o $@

$(DIR_TST)%.o:: $(DIR_UNI)%.c $(DIR_UNI)%.h
	$(COMP) $(FLG_TEST) $< -o $@

$(DIR_TST)%.o:: $(DIR_UTI)%.c $(DIR_UTI)%.h
	$(COMP) $(FLG_TEST) $< -o $@

$(BUILD_DIRS):
	$(MKDIR) $@

-include $(DEP)



# MESSAGES
# ================================================================================
MSG_BUILD		= echo -e "$(BOLD)$(YELLOW)MAKEFILE: BUILD$(RESET)"
MSG_STATIC		= echo -e "$(BOLD)$(YELLOW)MAKEFILE: STATIC ANALYSIS$(RESET)"
MSG_DYNAMIC		= echo -e "$(BOLD)$(YELLOW)MAKEFILE: DYNAMIC ANALYSIS$(RESET)"
MSG_TEST		= echo -e "$(BOLD)$(YELLOW)MAKEFILE: TEST$(RESET)"
MSG_DEBUG		= echo -e "$(BOLD)$(YELLOW)MAKEFILE: DEBUG$(RESET)"

MSG_COMPILE		= echo -e "$(GREEN)Compiling $*.o ...$(RESET)"
MSG_LINK		= echo -e "$(GREEN)Linking into $(BOLD)$@$(RESET)"

MSG_TEST_RUN	= echo -e "$(GREEN)Running tests ...$(RESET)"
MSG_CLEAN		= echo -e "$(GREEN)Removing $(DIR_BLD) ...$(RESET)"
MSG_COMMIT		= echo -e "$(GREEN)Committing ...$(RESET)"
MSG_DONE		= echo -e "$(BLUE)done$(RESET)"

define msg_help
$(BOLD)$(YELLOW)MAKEFILE: HELP$(RESET)
$(GREEN)Usage: make [targets] [options]$(RESET)

$(BLUE)Targets:$(RESET)
  build			Build the project ($(COMP))
  static		Static analysis   ($(STATIC))
  dynamic		Dynamic analysis  ($(DYNAMIC))
  debug			Debug executable  ($(DEBUG))
  test			Run tests
  full			Complete build, static analysis, dynamic analysis and tests
  clean			Clean build artifacts
  commit		Clean and commit changes to git
  help			Display this help message

$(BLUE)Options:$(RESET)
  main=''		Specify the source files with main() function
  mods=''		Specify module source files 
  libs=''		Specify additional external libraries
  macro=''		Specify macros to include for source files
  tmacro=''		Specify macros to include for test files
  path=''		Specify relative paths to include for source files
  tpath=''		Specify relative paths to include for test files
  w=2			Strictness of warning flags  (default is 0)
  debug=2		Level of makefile debug info (default is 0)
  run=y			Run the compiled binary after building
  always=y		Always build, even if files are up to date
  verbose=y		Enable verbose output

$(BLUE)Examples:$(RESET)
  make
  make test
  make clean
  make commit
  make debug test debug=2
  make debug main="dir/file.c"
  make static dynamic test w=1 always=y
  make build libs="lm lcurses" debug=1 run=y
  make test macro=NDEBUG tmacro=TEST path=dir/ tpath=.
  make full main="main1.c main2.c" mods="file1.c file2.c"
endef

export msg_help
MSG_HELP		:= echo -e "$$msg_help"



# SPEC
# ================================================================================
.PHONY: all build test static dynamic debug full prepare-build prepare-test clean commit help