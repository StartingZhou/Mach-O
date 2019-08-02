# /bin/sh
clang -c main.m
file main.o
otool -lv main.o
otool -hv main.o