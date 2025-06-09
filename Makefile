# Makefile

BUILD_DIR := build
BIN_DIR := bin

CMAKE := cmake
MAKE := make

.PHONY: all clean install install-manual

all: $(BIN_DIR) $(BUILD_DIR)
	cd $(BUILD_DIR) && $(MAKE)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && $(CMAKE) ..

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

install:
	cmake --install build

install-manual:
	install -Dm755 bin/diff_numerics /usr/local/bin/diff_numerics
	install -Dm644 diff_numerics.1 /usr/local/share/man/man1/diff_numerics.1