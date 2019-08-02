#include "mheader.h"
int main(int argc, char **argv) {
//    mheader("./main.o", true);
    uint32_t count = _dyld_image_count();
    for (uint32_t i = 0; i < count; i++) {
        const char* image = _dyld_get_image_name(i);
        const struct mach_header *header = _dyld_get_image_header(i);
        printf("%s Header: \n", image);
        // 开始打印mach_headerd中的内容
        printf("\t| magic: %02x, \n\t| cputype: 0x%0x, \n\t| cpusubtype: %u, \n\t| filetype: %d, \n\t| ncmds: %d \n\t| sizeofcmds: %d  \n\n",header->magic, header->cputype, header->cpusubtype, header->filetype, header->ncmds, header->sizeofcmds);
    }
    return 1;
}
