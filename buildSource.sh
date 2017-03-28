#!/bin/sh
# A build script to build GFrame's source code into a static library (stored in lib/).

# Clean before the build.
function clean() {
    # Remove previous build folder
    if [ -d lib/build ]; then
        rm -rf lib/build
        echo "[INFO] Previous build foler removed."
    fi
}

# Validate the command exit with 0.
function validate() {
    if [ $? -eq 0 ]; then
        echo "[INFO] Command executed successfully."
    else
        echo "[ERROR] Command failed."
    fi
}

clean

mkdir -p lib/build
cd lib/build
cmake ../.. -DBUILD_SRC=ON
validate
make
validate
make install
validate