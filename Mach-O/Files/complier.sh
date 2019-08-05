# /bin/sh
clang -c main.m
file main.o

otool -lv main.o
otool -hv main.o

xcrun clang -c main.m 
xcurn clang -c Adding.m
xcrun clang main.o Adding.o -framework Foundation
xcrun clang -E main.m > main.e

nm -m main.o
otool -rv main.o