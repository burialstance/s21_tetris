CC = gcc
COMPILE_FLAGS = -std=c11 -Wall -Wextra -Werror

# dirs
BUILD_PATH = build
BIN_PATH = bin

# sources
SRC_EXT = c

ENTRYPOINT_SRC = src/tetris.c
ENTRYPOINT_BIN_NAME = tetris

BACKEND_SRC_PATH = src/brick_game
BACKEND_LDFLAGS = 
BACKEND_CFLAGS = -lm
BACKEND_BIN_NAME = lib_backend.a

FRONTEND_SRC_PATH = src/gui
FRONTEND_LDFLAGS = 
FRONTEND_CFLAGS = # -lncurses -lm
FRONTEND_BIN_NAME = lib_frontend.a

# test
TEST_SRC_PATH = tests
TEST_LDFLAGS = -lcheck -lsubunit -lm
TEST_BIN_NAME = test_runner

TEST_GCOV_NAME = test_runner__gcov
GCOVR_PATH = $(shell which gcovr || echo ~/.local/bin/gcovr)
OPEN_BROWSER_CMD = $(shell which firefox || which xdg-open || which open || which x-www-browser)

# misc
DIST_PACKAGE = $(ENTRYPOINT_BIN_NAME).tar.gz

INSTALL = install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644
DESTDIR = /
INSTALL_PREFIX = usr/local


CLANG_FORMAT_CONF = materials/linters/.clang-format
# CLANG_FORMAT_CONF = materials/linters/.clang-format-dev
CLANG_FORMAT_CMD = clang-format --verbose -style=file:$(CLANG_FORMAT_CONF) 
CPPCHECK_ARGS = -q -j4 --enable=performance,portability,warning,style

print-%: ; @echo $*=$($*)

# Macros for timing compilation
UNAME_S:=$(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	CUR_TIME = awk 'BEGIN{srand(); print srand()}'
	TIME_FILE = $(dir $@).$(notdir $@)_time
	START_TIME = $(CUR_TIME) > $(TIME_FILE)
	END_TIME = read st < $(TIME_FILE) ; \
		$(RM) $(TIME_FILE) ; \
		st=$$((`$(CUR_TIME)` - $$st)) ; \
		echo $$st
else
	TIME_FILE = $(dir $@).$(notdir $@)_time
	START_TIME = date '+%s' > $(TIME_FILE)
	END_TIME = read st < $(TIME_FILE) ; \
		$(RM) $(TIME_FILE) ; \
		st=$$((`date '+%s'` - $$st - 86400)) ; \
		echo `date -u -d @$$st '+%H:%M:%S'`
endif



# collect sources 
ifeq ($(BACKEND_SOURCES),)
	BACKEND_SOURCES :=  $(shell find $(BACKEND_SRC_PATH) -type f -name "*.c")
endif

ifeq ($(FRONTEND_SOURCES),)
	FRONTEND_SOURCES := $(shell find $(FRONTEND_SRC_PATH) -type f -name "*.c")
endif

ifeq ($(TEST_SOURCES),)
	TEST_SOURCES := $(shell find $(TEST_SRC_PATH) -type f -name "*.c")
endif
# /////////////////



# prepare objects 
BACKEND_OBJECTS = $(BACKEND_SOURCES:$(BACKEND_SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
FRONTEND_OBJECTS = $(FRONTEND_SOURCES:$(FRONTEND_SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/tests/%.o)
# /////////////////


# logs
RED    = $(shell printf "\33[31m")
GREEN  = $(shell printf "\33[32m")
WHITE  = $(shell printf "\33[37m")
YELLOW = $(shell printf "\33[33m")
RESET  = $(shell printf "\33[0m")

define _log_message
	@if [ $(1) = INFO ]; then \
		echo "$(WHITE)"; \
	elif [ $(1) = SUCCESS ]; then \
		echo "$(GREEN)"; \
	elif [ $(1) = WARNING ]; then \
		echo "$(RED)"; \
	fi; \
	echo "$(2)"; \
	echo "$(RESET)"
endef

log_info = $(call _log_message, INFO, $(1))
log_success = $(call _log_message, SUCCESS, $(1))
log_warning = $(call _log_message, WARNING, $(1))
log_t = $(echo -e $(1))
# /////////////////




# targets 
.PHONY: all
all: $(ENTRYPOINT_BIN_NAME)
	@ln -f -s $(BIN_PATH)/$(ENTRYPOINT_BIN_NAME) $(ENTRYPOINT_BIN_NAME)
	$(call log_info, "Created symlink ./$(ENTRYPOINT_BIN_NAME)")

# tetris builder
$(ENTRYPOINT_BIN_NAME): dirs backend frontend
	@$(CC) $(COMPILE_FLAGS) $(ENTRYPOINT_SRC) -o $(BIN_PATH)/$(ENTRYPOINT_BIN_NAME) \
	$(BIN_PATH)/$(BACKEND_BIN_NAME) \
	$(BIN_PATH)/$(FRONTEND_BIN_NAME) -lncurses 



.PHONY: dirs
dirs:
	@mkdir -p $(dir $(BACKEND_OBJECTS))
	@mkdir -p $(dir $(FRONTEND_OBJECTS))
	@mkdir -p $(dir $(TEST_OBJECTS))
	@mkdir -p $(BIN_PATH)



.PHONY: backend
backend: dirs $(BIN_PATH)/$(BACKEND_BIN_NAME)

$(BIN_PATH)/$(BACKEND_BIN_NAME): $(BACKEND_OBJECTS)
	@ar rcs $@ $(BACKEND_OBJECTS)
	$(call log_success, "Success created $@")

$(BUILD_PATH)/%.o: $(BACKEND_SRC_PATH)/%.$(SRC_EXT)
	@echo "$(GREEN)Compiling:$(RESET) $< -> $@"
	@$(CC) $(COMPILE_FLAGS) $(BACKEND_CFLAGS) -c $< -o $@	



.PHONY: frontend
frontend: dirs $(BIN_PATH)/$(FRONTEND_BIN_NAME)
	
$(BIN_PATH)/$(FRONTEND_BIN_NAME): $(FRONTEND_OBJECTS)
	@ar rcs $@ $(FRONTEND_OBJECTS)
	$(call log_success, "Success created $@")

$(BUILD_PATH)/%.o: $(FRONTEND_SRC_PATH)/%.$(SRC_EXT)
	@$(CC) $(COMPILE_FLAGS) $(FRONTEND_CFLAGS) -c $< -o $@
	@echo "$(GREEN)Compiling:$(RESET) $< -> $@"



.PHONY: test
test: backend clean_test $(BIN_PATH)/$(TEST_BIN_NAME)
	@$(BIN_PATH)/$(TEST_BIN_NAME)

$(BIN_PATH)/$(TEST_BIN_NAME): $(TEST_OBJECTS)
	@$(CC) $(COMPILE_FLAGS) $(TEST_OBJECTS) $(TEST_LDFLAGS) -o $@ $(BIN_PATH)/$(BACKEND_BIN_NAME)
	@echo "$(GREEN)Compiling:$(RESET) $< -> $@"
	$(call log_success, "Success created $@")

$(BUILD_PATH)/tests/%.o: $(TEST_SRC_PATH)/%.$(SRC_EXT)
	@$(CC) $(COMPILE_FLAGS) -c $< -o $@
	@echo "$(GREEN)Compiling:$(RESET) $< -> $@"



.PHONY: clean_test
clean_test:
	@$(RM) $(BIN_PATH)/$(TEST_BIN_NAME)



.PHONY: gcov_report
gcov_report: backend clean_gcov clean_report $(TEST_GCOV_NAME)
	@./$(TEST_GCOV_NAME)
	@mkdir report
	@$(GCOVR_PATH) -r . --html --html-details -o report/coverage.html --exclude=$(TEST_SRC_PATH)
	@make clean_gcov
	@$(OPEN_BROWSER_CMD) report/coverage.html

$(TEST_GCOV_NAME): $(TEST_SOURCES) $(BACKEND_SOURCES)
	@$(CC) $(COMPILE_FLAGS) -g -fprofile-arcs -ftest-coverage $^ -o $@ $(TEST_LDFLAGS) 
	@echo "$(GREEN)Compiling:$(RESET) $< -> $@"



.PHONY: clean
clean: clean_gcov clean_report
	@$(RM) $(ENTRYPOINT_BIN_NAME)
	@$(RM) $(DIST_PACKAGE)
	@$(RM) -r build
	@$(RM) -r bin
	@$(RM) -r docs
	$(call log_warning, "Files and directories was removed")

.PHONY: clean_gcov
clean_gcov:
	@$(RM) $(TEST_GCOV_NAME)
	@$(RM) *.gcda *.gcno

.PHONY: clean_report
clean_report:
	@$(RM) -r report



.PHONY: dvi
dvi:
	@doxygen
	$(OPEN_BROWSER_CMD) docs/html/index.html

.PHONY: dist
dist:
	tar czvf $(DIST_PACKAGE) --ignore-failed-read \
		src $(TEST_SRC_PATH) $(BIN_PATH) $(BUILD_PATH) Makefile Doxyfile README.md 
	$(call log_info, "created tar with dist")

.PHONY: install
install: all
	$(call log_info, "Install to $(DESTDIR)$(INSTALL_PREFIX)/bin")
	@$(INSTALL_PROGRAM) $(BIN_PATH)/$(ENTRYPOINT_BIN_NAME) $(DESTDIR)$(INSTALL_PREFIX)/bin


.PHONY: uninstall
uninstall:
	$(call log_info, "Removing $(DESTDIR)$(INSTALL_PREFIX)/bin/$(ENTRYPOINT_BIN_NAME)")
	@$(RM) $(DESTDIR)$(INSTALL_PREFIX)/bin/$(ENTRYPOINT_BIN_NAME)

.PHONY: lines
lines:
	git ls-files src | xargs wc -l
	
.PHONY: clang
clang:
	@$(CLANG_FORMAT_CMD) -n $(shell find $(ENTRYPOINT_SRC) -type f \( -name "*.c" -o -name "*.h" \)) 
	@$(CLANG_FORMAT_CMD) -n $(shell find $(BACKEND_SRC_PATH) -type f \( -name "*.c" -o -name "*.h" \)) 
	@$(CLANG_FORMAT_CMD) -n $(shell find $(FRONTSRC) -type f \( -name "*.c" -o -name "*.h" \)) 
	$(call log_info, "$(CLANG_FORMAT_CMD)")

.PHONY: clang-format
clang-format:
	@$(CLANG_FORMAT_CMD) -i $(shell find $(ENTRYPOINT_SRC) -type f \( -name "*.c" -o -name "*.h" \)) 
	@$(CLANG_FORMAT_CMD) -i $(shell find $(BACKEND_SRC_PATH) -type f \( -name "*.c" -o -name "*.h" \)) 
	@$(CLANG_FORMAT_CMD) -i $(shell find $(FRONTSRC) -type f \( -name "*.c" -o -name "*.h" \)) 
	$(call log_success, "$(CLANG_FORMAT_CMD)")

# /////////////////
