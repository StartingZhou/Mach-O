#import <Foundation/Foundation.h>
#import <mach-o/dyld.h>
#ifdef __LP64__
typedef struct mach_header_64 mach_header_p;
#else
typedef struct mach_header mach_header_p;
#endif
int main(int argc, char **argv) {
    int count = _dyld_image_count();
    for (int i = 0; i < count; i++) {
        const char *name = _dyld_get_image_name(i);
        printf("\t| %s: \n", name);
        const mach_header_p *header = (const mach_header_p *)_dyld_get_image_header(i);
        printf("\t| magic: %02x, \t| cputype: %d, \t| filetype: %d \n", header->magic, header->filetype, header->filetype);
    }
    return 1;
}
