CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++17
GCOVFLAGS = -fprofile-arcs -ftest-coverage

BUILD_DIR = bin
BIN_NAME = 3DViewer
PRO_FILE = view/view.pro

MODEL_DIR = model
CONTR_DIR = controller
VIEW_DIR = view
TEST_DIR = tests/*.cc
LSRC = $(MODEL_DIR)/*.cc $(MODEL_DIR)/parser/*.cc $(MODEL_DIR)/affine_transform/*.cc libs/*.cc
INCLUDES = -I$(MODEL_DIR) -I$(MODEL_DIR)/parser -I$(MODEL_DIR)/affine_transform -Ilibs
DIST_DIR = s21_3DViewer_v2_0

SYSTEM := $(shell uname -s)

ifeq ($(SYSTEM), Linux)
		OPEN_CMD = xdg-open
		LTEST = -lgtest -lsubunit -lm -lrt -pthread
		SETTINGS = ~/.config/s21/3DViewer_v2.0.conf
else ifeq ($(SYSTEM), Darwin)
		OPEN_CMD = open
		LTEST = -lgtest -lgtest_main
		SETTINGS = ~/Library/Preferences/com.s21.3DViewer_v2.0.plist
else
		$(error Unsupported system: $(SYSTEM))
endif

.PHONY: all install gcov_report dvi dist uninstall clean

all: install gcov_report dvi dist

install:
		@echo "Installing 3DViewer v2.0"
		@mkdir -p $(BUILD_DIR)
		@qmake $(PRO_FILE) -o $(BUILD_DIR)/Makefile
		@cd $(BUILD_DIR) && make
		@echo "Installed $(BIN_NAME) in $(BUILD_DIR)"

uninstall: clean
		@echo "Uninstalling 3DViewer v2.0..."
		@rm -rf $(BUILD_DIR)
		@echo "Removed /src/$(BUILD_DIR)/$(BIN_NAME)"
		rm -f $(SETTINGS)

test: clean
		$(CC) $(CFLAGS) $(INCLUDES) $(LSRC) $(TEST_DIR) $(LTEST) -o test -pthread
		./test

gcov_flag:
		$(eval CFLAGS += --coverage $(GCOVFLAGS))

gcov_report: clean gcov_flag test
ifeq ($(SYSTEM), Linux)
		lcov --no-external -c -d . -o line_coverage.info
		lcov --remove line_coverage.info "*/libs/*" -o coverage.info
		genhtml coverage.info -o report
		open report/index.html
else
		lcov --no-external --ignore-errors inconsistent -c -d . -o line_coverage.info
		lcov --remove line_coverage.info "*/libs/*" --ignore-errors inconsistent -o coverage.info
		genhtml --ignore-errors inconsistent coverage.info -o report
		open report/index.html
endif

dist: install clean
		mkdir -p $(DIST_DIR)
		cp -r $(BUILD_DIR) $(DIST_DIR)/bin/
		doxygen Doxyfile
		cp -r html $(DIST_DIR)
		tar -czvf $(DIST_DIR).tar.gz $(DIST_DIR)
		rm -rf $(DIST_DIR)
		@echo "Distribution package created: $(DIST_DIR).tar.gz"

dvi:
		doxygen Doxyfile
		open html/index.html

clean: 
		rm -rf *.gcda *.gcno *.info
		rm -rf $(BUILD_DIR)/*.o
		rm -rf test
		rm -rf report
		rm -rf s21_3DViewer_v2_0.tar.gz
		rm -rf html/
		rm -rf latex/

style:
		@find $(MODEL_DIR) \( -name "*.c*" -or -name "*.cpp*" -or -name "*.h" \) -exec clang-format --style=google -i {} +
		@find $(MODEL_DIR)/parser \( -name "*.c*" -or -name "*.cpp*" -or -name "*.h" \) -exec clang-format --style=google -i {} +
		@find $(MODEL_DIR)/affine_transform \( -name "*.c*" -or -name "*.cpp*" -or -name "*.h" \) -exec clang-format --style=google -i {} +
		@find $(CONTR_DIR) \( -name "*.c*" -or -name "*.cpp*" -or -name "*.h" \) -exec clang-format --style=google -i {} +
		@find $(VIEW_DIR) \( -name "*.c*" -or -name "*.cpp*" -or -name "*.h" \) -exec clang-format --style=google -i {} +
		@find tests \( -name "*.c*" -or -name "*.cpp*" -or -name "*.h" \) -exec clang-format --style=google -i {} +
		@find libs \( -name "*.c*" -or -name "*.cpp*" -or -name "*.h" \) -exec clang-format --style=google -i {} +

style_check:
		@find $(MODEL_DIR) \( -name "*.c*" -or -name "*.cpp*" -or -name "*.h" \) -exec clang-format --style=google -n {} +
		@find $(MODEL_DIR)/parser \( -name "*.c*" -or -name "*.cpp*" -or -name "*.h" \) -exec clang-format --style=google -n {} +
		@find $(MODEL_DIR)/affine_transform \( -name "*.c*" -or -name "*.cpp*" -or -name "*.h" \) -exec clang-format --style=google -n {} +
		@find $(CONTR_DIR) \( -name "*.c*" -or -name "*.cpp*" -or -name "*.h" \) -exec clang-format --style=google -n {} +
		@find $(VIEW_DIR) \( -name "*.c*" -or -name "*.cpp*" -or -name "*.h" \) -exec clang-format --style=google -n {} +
		@find tests \( -name "*.c*" -or -name "*.cpp*" -or -name "*.h" \) -exec clang-format --style=google -n {} +
		@find libs \( -name "*.c*" -or -name "*.cpp*" -or -name "*.h" \) -exec clang-format --style=google -n {} +
