BUILD_DIR = build
INSTALL_DIR = interpolation-test

.PHONY: all
all: build

build: $(BUILD_DIR)/bin/x11/libinterpolation.so $(BUILD_DIR)/interpolation.gdnlib $(BUILD_DIR)/interpolation.gdns


$(BUILD_DIR)/bin/x11/libinterpolation.so: src/interpolation.cpp src/interpolation.h src/gdlibrary.cpp
	scons platform=linux

$(BUILD_DIR)/interpolation.gdnlib: interpolation.gdnlib
	cp -f interpolation.gdnlib $(BUILD_DIR)/interpolation.gdnlib

$(BUILD_DIR)/interpolation.gdns: interpolation.gdns
	cp -f interpolation.gdns $(BUILD_DIR)/interpolation.gdns

.PHONY: install
install: build 
	rsync -rv $(BUILD_DIR)/* $(INSTALL_DIR)/

.PHONY: clean
clean:
	rm -rf interpolation
