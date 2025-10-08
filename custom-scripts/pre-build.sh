#!/bin/sh

if [ -d "$BASE_DIR" ]; then
    cp "$BASE_DIR/../custom-scripts/S41network-config" "$BASE_DIR/target/etc/init.d"
    chmod +x "$BASE_DIR/target/etc/init.d/S41network-config"

    # Start modules
    make -C "$BASE_DIR/../modules/simple_driver_original/"
    make -C "$BASE_DIR/../modules/xtea_driver/"

    # Copy thread runner script to kernel
    make -C "$BASE_DIR/../custom-scripts/setpriority-master/"
fi
