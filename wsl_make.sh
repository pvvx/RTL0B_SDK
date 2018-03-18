#!/bin/sh
time make -s -j 20 clean
time make -s -j 20 all OTA_IDX=1
time make -s -j 20 all OTA_IDX=2