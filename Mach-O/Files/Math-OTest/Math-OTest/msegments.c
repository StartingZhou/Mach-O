//
//  msegments.c
//  Math-OTest
//
//  Created by developer on 2019/8/6.
//  Copyright © 2019 Atomic. All rights reserved.
//

#include "msegments.h"
void createSection(FILE *file, segment_command_p *scp, section_p **rtnValue);
void createSegment(FILE *file, struct load_command lcp, segment_command_p **rtnValue, section_p **rtnSection);
char * CMDNAME(int cmd) {
    switch (cmd) {
        case LC_VERSION_MIN_WATCHOS:            return "LC_VERSION_MIN_WATCHOS";
        case LC_NOTE:                           return "LC_NOTE";
        case LC_BUILD_VERSION:                  return "LC_BUILD_VERSION";
        case LC_DATA_IN_CODE:                   return "LC_DATA_IN_CODE";
        case LC_SOURCE_VERSION:                 return "LC_SOURCE_VERSION";
        case LC_DYLIB_CODE_SIGN_DRS:            return "LC_DYLIB_CODE_SIGN_DRS";
        case LC_ENCRYPTION_INFO_64:             return "LC_ENCRYPTION_INFO_64";
        case LC_LINKER_OPTION:                  return "LC_LINKER_OPTION";
        case LC_LINKER_OPTIMIZATION_HINT:       return "LC_LINKER_OPTIMIZATION_HINT";
        case LC_VERSION_MIN_TVOS:               return "LC_VERSION_MIN_TVOS";
        case LC_DYLD_INFO:                      return "LC_DYLD_INFO";
        case LC_DYLD_INFO_ONLY:                 return "LC_DYLD_INFO_ONLY";
        case LC_LOAD_UPWARD_DYLIB:              return "LC_LOAD_UPWARD_DYLIB";
        case LC_VERSION_MIN_MACOSX:             return "LC_VERSION_MIN_MACOSX";
        case LC_VERSION_MIN_IPHONEOS:           return "LC_VERSION_MIN_IPHONEOS";
        case LC_FUNCTION_STARTS:                return "LC_FUNCTION_STARTS";
        case LC_DYLD_ENVIRONMENT:               return "LC_DYLD_ENVIRONMENT";
        case LC_MAIN:                           return "LC_MAIN";
        case LC_LOAD_WEAK_DYLIB:                return "LC_LOAD_WEAK_DYLIB";
        case LC_SEGMENT_64:                     return "LC_SEGMENT_64";
        case LC_ROUTINES_64:                    return "LC_ROUTINES_64";
        case LC_UUID:                           return "LC_UUID";
        case LC_RPATH:                          return "LC_RPATH";
        case LC_CODE_SIGNATURE:                 return "LC_CODE_SIGNATURE";
        case LC_SEGMENT_SPLIT_INFO:             return "LC_SEGMENT_SPLIT_INFO";
        case LC_REEXPORT_DYLIB:                 return "LC_REEXPORT_DYLIB";
        case LC_LAZY_LOAD_DYLIB:                return "LC_LAZY_LOAD_DYLIB";
        case LC_ENCRYPTION_INFO:                return "LC_ENCRYPTION_INFO";
        case LC_ID_DYLINKER:                    return "LC_ID_DYLINKER";
        case LC_PREBOUND_DYLIB:                 return "LC_PREBOUND_DYLIB";
        case LC_ROUTINES:                       return "LC_ROUTINES";
        case LC_SUB_FRAMEWORK:                  return "LC_SUB_FRAMEWORK";
        case LC_SUB_UMBRELLA:                   return "LC_SUB_UMBRELLA";
        case LC_SUB_CLIENT:                     return "LC_SUB_CLIENT";
        case LC_SUB_LIBRARY:                    return "LC_SUB_LIBRARY";
        case LC_TWOLEVEL_HINTS:                 return "LC_TWOLEVEL_HINTS";
        case LC_PREBIND_CKSUM:                  return "LC_PREBIND_CKSUM";
        case LC_SEGMENT:                        return "LC_SEGMENT";
        case LC_SYMTAB:                         return "LC_SYMTAB";
        case LC_SYMSEG:                         return "LC_SYMSEG";
        case LC_THREAD:                         return "LC_THREAD";
        case LC_UNIXTHREAD:                     return "LC_UNIXTHREAD";
        case LC_LOADFVMLIB:                     return "LC_LOADFVMLIB";
        case LC_IDFVMLIB:                       return "LC_IDFVMLIB";
        case LC_IDENT:                          return "LC_IDENT";
        case LC_FVMFILE:                        return "LC_FVMFILE";
        case LC_PREPAGE:                        return "LC_PREPAGE";
        case LC_DYSYMTAB:                       return "LC_DYSYMTAB";
        case LC_LOAD_DYLIB:                     return "LC_LOAD_DYLIB";
        case LC_ID_DYLIB:                       return "LC_ID_DYLIB";
        case LC_LOAD_DYLINKER:                  return "LC_LOAD_DYLINKER";
        default:                                return NULL;
    }
    return NULL;
}

struct load_command* cmds(char *path, bool verbose, int *count) {
    ssize_t header_size = sizeof(mach_header_p);
    uint8_t buf_header[header_size];
    ssize_t position = 0;
    int fd = open(path, O_RDONLY);
    if (fd <= 0) {
        printf("Error occurs when open file %s, errno is %d", path, errno);
        exit(errno);
    }
    while (position < header_size - 1) {
        ssize_t read_size = read(fd, buf_header + position, header_size - position);
        if(read_size <= 0) {
            printf("Error occurs when read file %s, errno is %d", path, errno);
            exit(errno);
        }
        position += read_size;
    }
    mach_header_p *header = (mach_header_p *)buf_header;
    *count = header->ncmds;
    FILE *file = fdopen(fd, "r");
    struct load_command *commands = calloc(*count, sizeof(struct load_command));
    for (uint32_t index = 0; index < *count; index++) {
        ssize_t command_size = sizeof(struct load_command);
        uint8_t buf_command[command_size];
        ssize_t origin_pos = position;
        while (position - origin_pos < command_size - 1) {
            ssize_t read_size = fread((void *)buf_command, 1, command_size, file);
            if(read_size <= 0) {
                printf("Error occurs when read file %s, errno is %d", path, errno);
                exit(errno);
            }
            position += read_size;
        }
        struct load_command *command = (struct load_command *)buf_command;
        commands[index] = *command;
        uint32_t cmdsize = command->cmdsize;
        char *description = CMDNAME(command->cmd);
        printf("%s\n", description);
        fseek(file, origin_pos + cmdsize, 0);
        position = origin_pos + cmdsize;
    }
    fclose(file);
    return commands;
}

void segments(char *path, bool verbose, int *count) {
    mach_header_p header = mheader(path, false);
    uint32_t ncmds = header.ncmds;
    ssize_t size_command = sizeof(struct load_command);
    FILE *file = fopen(path, "r");
    
    if(file == NULL) {
        printf("Error occurs when open file %s, errno is %d", path, errno);
        exit(errno);
    }
    if(fseek(file, sizeof(mach_header_p), 0) != 0) {
        printf("Error occurs when seek file %s, errno is %d", path, errno);
        exit(errno);
    }
    for (int index = 0; index < ncmds; index++) {
        uint8_t command_buf[size_command];
        ssize_t read_size = fread((void *)command_buf, size_command, 1, file);
        if(read_size != 1) {
            printf("Error occurs when fread in segments, errno is %d", errno);
            exit(errno);
        }
        struct load_command *command_current = (struct load_command *)command_buf;
        segment_command_p *segment = NULL; section_p *section = NULL;
        createSegment(file, *command_current, &segment, &section);
        if (segment != NULL) {
            printf("|Segment");
            printf("\t|name: %s\tnsects: %d\n", segment->segname, segment->nsects);
        }
        if(section != NULL) {
            printf("\t|--> Sections:\n");
            for (int i = 0; i < segment->nsects; i++) {
                printf("\t\t name: %s\n", section[i].sectname);
                ;
            }
            
        }
        free(segment);
    }
}

void createSegment(FILE *file, struct load_command lcp, segment_command_p **rtnValue, section_p **rtnSection) {
    if (lcp.cmd == LC_SEGMENT || lcp.cmd == LC_SEGMENT_64) {
        fseek(file, -sizeof(lcp), 1);
        *rtnValue = malloc(sizeof(segment_command_p));
        ssize_t read_size = fread(*rtnValue, sizeof(segment_command_p), 1, file);
        if(read_size != 1) {
            printf("Error occurs when fread in createSegment, errno is %d", errno);
            exit(errno);
        }
        createSection(file, *rtnValue, rtnSection);
    } else {
        *rtnValue = NULL;
    }
}

void createSection(FILE *file, segment_command_p *scp, section_p **rtnValue) {
    if(scp != NULL && (scp->cmd == LC_SEGMENT_64 || scp->cmd == LC_SEGMENT) && scp->nsects > 0) {
        *rtnValue = malloc(scp->nsects * sizeof(segment_command_p));
        size_t read_size = fread(*rtnValue , sizeof(section_p), scp->nsects, file);
        if(read_size != scp->nsects) {
            printf("Error occurs when fread in createSection, errno is %d", errno);
            exit(errno);
        }
    } else {
        *rtnValue = NULL;
    }
}
