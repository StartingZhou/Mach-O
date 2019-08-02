#import <Foundation/Foundation.h>
#import "Adding.h"
int main(int argc, char **argv) {
    @autoreleasepool {
        NSLog(@"Hello World!");
        Adding *ad = [[Adding alloc] init];
        [ad addX:10 Y:10];
        return 1;
    }
    
}