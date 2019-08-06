#import <Foundation/Foundation.h>
#import "Adding.h"
#import <mach-o/dyld.h>
#import <mach-o/loader.h>

struct mach_header_64 *mh_64;
struct mach_header *mh;
struct load_command *lc;
struct segment_command *scmd;
struct segment_command_64 *scmd_64;
struct section *sc;
struct section_64 *sc_64;

int main(int argc, char **argv) {
    @autoreleasepool {
        NSLog(@"Hello World!");
        Adding *ad = [[Adding alloc] init];
        [ad addX:10 Y:10];
        return 1;
    }
    
}