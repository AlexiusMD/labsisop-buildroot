#!/bin/sh

cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41network-config

cp $BASE_DIR/../custom-scripts/systeminfo.py $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/systeminfo.py

cp $BASE_DIR/../custom-scripts/S50systeminfo $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S50systeminfo