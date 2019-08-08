//
//  mheader.c
//  Math-OTest
//
//  Created by developer on 2019/8/2.
//  Copyright © 2019 Atomic. All rights reserved.
//

#include "mheader.h"
mach_header_p mheader(const char * path, bool verbose)
{
    ssize_t sizeOfHeader = sizeof(mach_header_p);
    ssize_t position = 0;
    uint8_t buf[sizeOfHeader];
    //声明一个buffer数组，大小为mach_header结构体的大小，用来保存main.o目标文件的header数据
    int mach_fd = open(path, O_RDONLY);
    if (mach_fd <= 0) {
        printf("Error no. is %d \n", errno);
        exit(errno);
    }
    while (position < sizeOfHeader - 1) {
        ssize_t read_no;
        if ((read_no = read(mach_fd, buf + position, sizeOfHeader - position)) <= 0) {
            // 从我们创建的main.o目标文件中读取字节
            printf("Error no. is %d \n", errno);
            exit(errno);
        }
        position += read_no;
    }
    close(mach_fd);
    mach_header_p *header = (mach_header_p *)buf;
    if (verbose) {
        // 指向buf，并且将类型转换为mach_header的类型
        printf("Mach-O Header: \n");
        // 开始打印mach_headerd中的内容
        printf("\t| magic: %02x, \n\t| cputype: 0x%0x, \n\t| cpusubtype: %u, \n\t| filetype: %d, \n\t| ncmds: %d \n\t| sizeofcmds: %d  \n\n",header->magic, header->cputype, header->cpusubtype, header->filetype, header->ncmds, header->sizeofcmds);
        printf("Mach-O Header Raw Data: \n");
        // 打印buf中的字节数据
        uint8_t *by = (uint8_t *)buf;
        for (int i = 0 ; i < sizeOfHeader; i++) {
            if (i % 4 == 0) {
                printf("\t");
            }
            printf(" %02x", *(by+i));
            if ((i+1) % 4 == 0) {
                printf("\n");
            }
        }
    }
    return *header;
}
