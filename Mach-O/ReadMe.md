# Mach-O 文件浅析
&nbsp;&nbsp;&nbsp;&nbsp;Mac和iOS系统的可执行文件被称作是Mach-O格式的文件，一般叫做Mach-O文件，该文件描述了目标文件或可执行程序文件的信息以及各个段的信息、机器码和符号表、重定向信息等，程序在启动前，Unix系统会通过 `fork` 创建一个进程，之后通过 `exec` 调用 `dyld`，`dyld`将可执行文件映射进内存，根据可执行文件的信息加载相应的动态库并绑定地址、调用C++的静态构造函数和OC的 `+load` 方法等，最后执行程序文件的入口函数 `main`，本篇文章只对Mach-O文件的结构做简要分析。

## 生成 Mach-O 文件
首先变出一个 Mach-O 文件，最简单的方法就是通过Xcode创建一个控制台项目，根据所选择使用的编程语言默认会带一个main.c或者main.m的文件，但是为了让事情看起来麻烦一点，通过创建一个 OC 文件然后使用 `clang` 编译成 Mach-O文件可能会更有意思一些，我们直接创建一个文件并命名为`main.m`，其中的代码如下：
```objc
#import <Foundation/Foundation.h>
int main(int argc, char **argv) {
    NSLog(@"Hello World!");
    return 1;
}
```
该段代码简单的打印了`Hello World!`，然后打开 `Terminal` 终端，`cd` 到该文件的目录下执行以下命令：
```
clang -c main.m
# -E 预处理，-S 汇编文件 -c 生成目标文件
```
之后该目录下会生成 `main.o`的文件。这是我们的第一个 Mach-O文件，我们平时在Xcode中进行 `build` 或者 `run`时，Xcode会采用相似的方式对每一个`.c、.m、.swift`文件生成目标文件(object file)，只是 Xcode在调用 clang命令时会根据`build-setting`设置项里的参数进行编译。让我们使用file命令查看该文件的类型和CPU架构：
```
file main.o
# main.o: Mach-O 64-bit object x86_64
```
结果根据使用的机器的CUP架构可能会不同，我的是64位的 Mach-O文件。

## 查看 Mach-O 文件
查看 Mach-O文件可以使用`otool`命令或者使用[MachOView工具](https://github.com/gdbinit/MachOView)，下图为在MachOView中打开我们上一步生成的Mach-O文件

![MachOView](./Images/MachOView.png "MachOView")

可以看到，Mach-O文件包含Header信息（Mach64 Header）、加载的段信息（LoadCommands）、和节信息（Section64），由于我们生成的是目标文件（没有进行最后的链接阶段），所以会有重定位信息（Relocations），以及最后的符号表信息。
使用`otool`命令查看Mach-O文件：

1. 查看Mach-O Header信息
```shell
otool -hv main.o
```
输出结果如下图所示

![Header](./Images/MachOHeader.png "Mach-O Header")

Mach-header对应的结构体定义位于`<mach-o/loader.h>`，有对应的64位版本，我把这两个定义的结构体拷贝出来：
```c
struct mach_header {
	uint32_t	magic;		/* mach magic number identifier */
	cpu_type_t	cputype;	/* cpu specifier */
	cpu_subtype_t	cpusubtype;	/* machine specifier */
	uint32_t	filetype;	/* type of file */
	uint32_t	ncmds;		/* number of load commands */
	uint32_t	sizeofcmds;	/* the size of all the load commands */
	uint32_t	flags;		/* flags */
};

struct mach_header_64 {
	uint32_t	magic;		/* mach magic number identifier */
	cpu_type_t	cputype;	/* cpu specifier */
	cpu_subtype_t	cpusubtype;	/* machine specifier */
	uint32_t	filetype;	/* type of file */
	uint32_t	ncmds;		/* number of load commands */
	uint32_t	sizeofcmds;	/* the size of all the load commands */
	uint32_t	flags;		/* flags */
	uint32_t	reserved;	/* reserved */
};
```
* magic：对应的是魔数，用来标识是否是Mach-O文件，取值如下：
```c
...
#define CPU_ARCH_ABI64		0x01000000	/* 64 bit ABI */
...
#define	MH_MAGIC    0xfeedface	/* the mach magic number */
#define MH_CIGAM    0xcefaedfe	/* NXSwapInt(MH_MAGIC) */
#define MH_MAGIC_64 0xfeedfacf  /* the 64-bit mach magic number */
#define MH_CIGAM_64 0xcffaedfe  /* NXSwapInt(MH_MAGIC_64) */
```
* cputype、cpusubtype：该Mach-O文件所支持的CPU架构，定义位于`<mach/machine.h>`，取值如下：
```c
...
#define CPU_TYPE_X86		((cpu_type_t) 7)
#define CPU_TYPE_I386		CPU_TYPE_X86		
#define	CPU_TYPE_X86_64		(CPU_TYPE_X86 | CPU_ARCH_ABI64)
...
#define CPU_TYPE_ARM		((cpu_type_t) 12)
#define CPU_TYPE_ARM64		(CPU_TYPE_ARM | CPU_ARCH_ABI64)
...
```
* filetype: 该Mach-O文件的文件类型，数值定义位于`<mach-o/loader.h>`，`MH_OBJECT`为目标文件，`MH_EXECUTE`为可执行文件。
* ncmds：Load Commands中段的个数。
* sizeofcmds：所有段占用的存储空间。
* flags: 一些掩码，表示Mach-O文件的一些可选的特性，例如：
    * NOUNDEFS：当该目标文件构建时，不存在未定义的引用。
    * DYLDLINK：该文件是作为动态链接器的输入文件，不能用于静态链接
    * SUBSECTIONS_VIA_SYMBOLS：该目标文件的节可以被分为各自的块，如果一些块没有被其它代码使用，链接时则会剥离出去。
* reserved：保留字段。
那么程序中如何获取Mach-O文件的Header信息呢？有两种方式：
1. 直接分析可执行文件，例如将我们的`main.o`文件直接加载进内存中（或者部分加载）按照字节流进行读取，下面所展示的就是Mach-O文件的开始部分的二进制表示（整型是小端法表示）：
```
    cffa edfe 0700 0001 0300 0000 0100 0000
    0400 0000 a802 0000 0020 0000 0000 0000
    1900 0000 2802 0000 0000 0000 0000 0000
    0000 0000 0000 0000 0000 0000 0000 0000
    c800 0000 0000 0000 c802 0000 0000 0000
    c800 0000 0000 0000 0700 0000 0700 0000
    0600 0000 0000 0000 5f5f 7465 7874 0000
    0000 0000 0000 0000 5f5f 5445 5854 0000
    0000 0000 0000 0000 0000 0000 0000 0000
    3200 0000 0000 0000 c802 0000 0400 0000
    9003 0000 0200 0000 0004 0080 0000 0000
    0000 0000 0000 0000 5f5f 6373 7472 696e
```
可以看到第一个字节的内容为0xfeedafacf对应的是`MH_MAGIC_64`，第二个字节内容为0x01000007对应的是`(CPU_TYPE_X86 | CPU_ARCH_ABI64) = 0000 0001 0000 0000 0000 0000 0000 | 0000 0111`，第三个字节为0x00000003对应的是
```
#define CPU_SUBTYPE_X86_ALL        ((cpu_subtype_t)3)
#define CPU_SUBTYPE_X86_64_ALL     ((cpu_subtype_t)3)
```
第四个字节为0x00000001对应的是`MH_OBJECT`表示的是一个目标文件，`ncmds`对应着0x00000004，`sizeofcmds`为0x000002a8( = 680)
