
export BUILD_DIR ?= cmake-build

default: release

release:
	mkdir -p ./$(BUILD_DIR) && cd ./$(BUILD_DIR) && cmake ../ -DCMAKE_BUILD_TYPE=RELEASE_BUILD && VERBOSE=1 cmake --build .

debug:
	mkdir -p ./$(BUILD_DIR) && cd ./$(BUILD_DIR) && cmake ../ -DCMAKE_BUILD_TYPE=DEBUG_BUILD && VERBOSE=1 cmake --build .

test:
	@if [ -f ./$(BUILD_DIR)/unit-tests ]; then ./$(BUILD_DIR)/unit-tests; else echo "Please run 'make release' or 'make debug' first" && exit 1; fi

bench:
	@if [ -f ./$(BUILD_DIR)/bench-tests ]; then ./$(BUILD_DIR)/bench-tests; else echo "Please run 'make release' or 'make debug' first" && exit 1; fi

clean:
	rm -rf ./$(BUILD_DIR)

distclean: clean
	rm -rf mason_packages

.PHONY: test bench