#!/bin/bash
# Script for the encryption part.

ccrypt -d -k key.txt data.txt
ccrypt -d -k key.txt len_off.txt
./dec $1
