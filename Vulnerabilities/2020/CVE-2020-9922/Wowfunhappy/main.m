#import <Foundation/Foundation.h>
#import "ZKSwizzle.h"




@interface myMCMimePart : NSObject

@end


@implementation myMCMimePart

-(BOOL)isAutoArchivePart {
    return false;
}

@end




@interface main : NSObject

+ (void)registerBundle;

@end


@implementation main

+ (void)initialize {
    Class mvMailBundleClass = NSClassFromString(@"MVMailBundle");
    if (!mvMailBundleClass) {
        NSLog(@"Mail.app doesn't appear to support bundles.");
        return;
    }
    
    // Register plugin in Mail.app
    NSLog(@"Plugin loaded successfully.");
    [mvMailBundleClass registerBundle];
    
    ZKSwizzle(myMCMimePart, MCMimePart);
}

@end