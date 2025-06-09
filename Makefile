# Makefile

BUILD_DIR := build
BIN_DIR := bin

CMAKE := cmake
MAKE := make

.PHONY: all clean

all: $(BIN_DIR) $(BUILD_DIR)
	cd $(BUILD_DIR) && $(MAKE)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && $(CMAKE) ..

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)