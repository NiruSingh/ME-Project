#!/bin/bash
# Script for the encryption part.

./enc $1
ccrypt -e -k key.txt data.txt
ccrypt -e -k key.txt len_off.txt
