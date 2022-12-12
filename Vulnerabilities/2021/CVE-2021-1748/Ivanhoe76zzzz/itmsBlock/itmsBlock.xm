// See http://iphonedevwiki.net/index.php/Logos

#if TARGET_OS_SIMULATOR
#error Do not support the simulator, please use the real iPhone Device.
#endif

#import <UIKit/UIKit.h>

%hook SUStoreController

-(id)handleApplicationURL:(NSURL *)url{
    if ([[url absoluteString] hasPrefix:@"itms"]) return nil;
	return %orig;
}

-(id)_handleAccountURL:(NSURL *)url{
    if ([[url absoluteString] hasPrefix:@"itms"]) return nil;
    return %orig;
}

%end

%hook SKUIURL

-(id)initWithUrl:(NSURL *)url{
    if ([[url absoluteString] hasPrefix:@"itms"]) return nil;
    return %orig;
}

%end
