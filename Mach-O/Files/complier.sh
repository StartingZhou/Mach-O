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

nm -mp main.o

otool -tVj main.o

clang -g -framework Foundation main.m Adding.m

lldb ./a.out
(lldb) breakpoint set -f main.m -l 16
(lldb) process launch
(lldb) thread step-in
(lldb) type format add -f hex long
(lldb) expr 
const struct mach_header_64 *mh_64 = (const struct mach_header_64 *) _dyld_get_image_header(0);
long magic = mh_64->magic;
magic;
      
(lldb) type format clear
