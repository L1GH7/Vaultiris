#line 1 "/Users/tihmstar/dev/itmsBlock/itmsBlock/itmsBlock.xm"


#if TARGET_OS_SIMULATOR
#error Do not support the simulator, please use the real iPhone Device.
#endif

#import <UIKit/UIKit.h>


#include <substrate.h>
#if defined(__clang__)
#if __has_feature(objc_arc)
#define _LOGOS_SELF_TYPE_NORMAL __unsafe_unretained
#define _LOGOS_SELF_TYPE_INIT __attribute__((ns_consumed))
#define _LOGOS_SELF_CONST const
#define _LOGOS_RETURN_RETAINED __attribute__((ns_returns_retained))
#else
#define _LOGOS_SELF_TYPE_NORMAL
#define _LOGOS_SELF_TYPE_INIT
#define _LOGOS_SELF_CONST
#define _LOGOS_RETURN_RETAINED
#endif
#else
#define _LOGOS_SELF_TYPE_NORMAL
#define _LOGOS_SELF_TYPE_INIT
#define _LOGOS_SELF_CONST
#define _LOGOS_RETURN_RETAINED
#endif

@class SKUIURL; @class SUStoreController; 
static id (*_logos_orig$_ungrouped$SUStoreController$handleApplicationURL$)(_LOGOS_SELF_TYPE_NORMAL SUStoreController* _LOGOS_SELF_CONST, SEL, NSURL *); static id _logos_method$_ungrouped$SUStoreController$handleApplicationURL$(_LOGOS_SELF_TYPE_NORMAL SUStoreController* _LOGOS_SELF_CONST, SEL, NSURL *); static id (*_logos_orig$_ungrouped$SUStoreController$_handleAccountURL$)(_LOGOS_SELF_TYPE_NORMAL SUStoreController* _LOGOS_SELF_CONST, SEL, NSURL *); static id _logos_method$_ungrouped$SUStoreController$_handleAccountURL$(_LOGOS_SELF_TYPE_NORMAL SUStoreController* _LOGOS_SELF_CONST, SEL, NSURL *); static SKUIURL* (*_logos_orig$_ungrouped$SKUIURL$initWithUrl$)(_LOGOS_SELF_TYPE_INIT SKUIURL*, SEL, NSURL *) _LOGOS_RETURN_RETAINED; static SKUIURL* _logos_method$_ungrouped$SKUIURL$initWithUrl$(_LOGOS_SELF_TYPE_INIT SKUIURL*, SEL, NSURL *) _LOGOS_RETURN_RETAINED; 

#line 9 "/Users/tihmstar/dev/itmsBlock/itmsBlock/itmsBlock.xm"


static id _logos_method$_ungrouped$SUStoreController$handleApplicationURL$(_LOGOS_SELF_TYPE_NORMAL SUStoreController* _LOGOS_SELF_CONST __unused self, SEL __unused _cmd, NSURL * url){
    if ([[url absoluteString] hasPrefix:@"itms"]) return nil;
	return _logos_orig$_ungrouped$SUStoreController$handleApplicationURL$(self, _cmd, url);
}

static id _logos_method$_ungrouped$SUStoreController$_handleAccountURL$(_LOGOS_SELF_TYPE_NORMAL SUStoreController* _LOGOS_SELF_CONST __unused self, SEL __unused _cmd, NSURL * url){
    if ([[url absoluteString] hasPrefix:@"itms"]) return nil;
    return _logos_orig$_ungrouped$SUStoreController$_handleAccountURL$(self, _cmd, url);
}





static SKUIURL* _logos_method$_ungrouped$SKUIURL$initWithUrl$(_LOGOS_SELF_TYPE_INIT SKUIURL* __unused self, SEL __unused _cmd, NSURL * url) _LOGOS_RETURN_RETAINED{
    if ([[url absoluteString] hasPrefix:@"itms"]) return nil;
    return _logos_orig$_ungrouped$SKUIURL$initWithUrl$(self, _cmd, url);
}


static __attribute__((constructor)) void _logosLocalInit() {
{Class _logos_class$_ungrouped$SUStoreController = objc_getClass("SUStoreController"); { MSHookMessageEx(_logos_class$_ungrouped$SUStoreController, @selector(handleApplicationURL:), (IMP)&_logos_method$_ungrouped$SUStoreController$handleApplicationURL$, (IMP*)&_logos_orig$_ungrouped$SUStoreController$handleApplicationURL$);}{ MSHookMessageEx(_logos_class$_ungrouped$SUStoreController, @selector(_handleAccountURL:), (IMP)&_logos_method$_ungrouped$SUStoreController$_handleAccountURL$, (IMP*)&_logos_orig$_ungrouped$SUStoreController$_handleAccountURL$);}Class _logos_class$_ungrouped$SKUIURL = objc_getClass("SKUIURL"); { MSHookMessageEx(_logos_class$_ungrouped$SKUIURL, @selector(initWithUrl:), (IMP)&_logos_method$_ungrouped$SKUIURL$initWithUrl$, (IMP*)&_logos_orig$_ungrouped$SKUIURL$initWithUrl$);}} }
#line 31 "/Users/tihmstar/dev/itmsBlock/itmsBlock/itmsBlock.xm"
