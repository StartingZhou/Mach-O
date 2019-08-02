//
//  mheader.h
//  Math-OTest
//
//  Created by developer on 2019/8/2.
//  Copyright © 2019 Atomic. All rights reserved.
//

#ifndef mheader_h
#define mheader_h
#import <mach-o/dyld.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
// 根据是否是64位或者32位机器选择正确的mach_header结构体
#ifdef __LP64__
typedef struct mach_header_64 mach_header_p;
#else
typedef struct mach_header mach_header_p;
#endif
mach_header_p mheader(const char * path, bool verbose);
#endif /* mheader_h */
