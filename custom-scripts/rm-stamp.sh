#!/bin/sh

if [ -d "$BASE_DIR" ]; then
    rm $BASE_DIR/output/build/linux-custom/.stamp_built
    rm $BASE_DIR/output/build/linux-custom/.stamp_configured
    rm $BASE_DIR/output/build/linux-custom/.stamp_rsynced
    echo "Stamps removed"
fi