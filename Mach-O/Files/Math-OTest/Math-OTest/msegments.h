//
//  msegments.h
//  Math-OTest
//
//  Created by developer on 2019/8/6.
//  Copyright © 2019 Atomic. All rights reserved.
//

#ifndef msegments_h
#define msegments_h
#import <mach-o/dyld.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "mheader.h"
struct segment_section {
    segment_command_p segment;
    section_p section;
};
char * CMDNAME(int cmd);
struct load_command* cmds(char *path, bool verbose, int *count);
void segments(char *path, bool verbose, int *count);
#endif /* msegments_h */
