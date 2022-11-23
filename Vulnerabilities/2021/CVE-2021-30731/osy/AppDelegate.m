//
// Copyright © 2022 osy. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#import "AppDelegate.h"
#import "WVUVCDevice.h"
@import Quartz;

@interface AppDelegate ()

@property (strong) IBOutlet NSWindow *window;
@property (weak) IBOutlet NSPopUpButton *deviceListButton;
@property (weak) IBOutlet NSMenu *deviceListMenu;
@property (weak) IBOutlet IKImageView *imageView;

@property NSArray<WVUVCDevice *> *devices;
@property WVUVCDevice *selectedDevice;

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    [NSNotificationCenter.defaultCenter addObserver:self selector:@selector(dropdownMenuOpened:) name:NSPopUpButtonWillPopUpNotification object:nil];
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    [NSNotificationCenter.defaultCenter removeObserver:self name:NSPopUpButtonWillPopUpNotification object:nil];
    if (self.selectedDevice) {
        [self.selectedDevice stopStream];
        self.selectedDevice = nil;
    }
}

- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
    return YES;
}

- (void)dropdownMenuOpened:(id)sender {
    [self refreshDeviceList];
}

- (void)refreshDeviceList {
    [self.deviceListMenu removeAllItems];
    NSMenuItem *placeholder = [[NSMenuItem alloc] init];
    placeholder.title = NSLocalizedString(@"Select device...", "AppDelegate");
    placeholder.target = self;
    placeholder.action = @selector(selectDeviceListItem:);
    placeholder.tag = -1;
    [self.deviceListMenu addItem:placeholder];
    dispatch_async(dispatch_get_global_queue(QOS_CLASS_BACKGROUND, 0), ^{
        self.devices = WVUVCDevice.allDevices;
        dispatch_async(dispatch_get_main_queue(), ^{
            NSArray<WVUVCDevice *> *devices = self.devices;
            [self.deviceListMenu removeAllItems];
            [self.deviceListMenu addItem:placeholder];
            for (NSInteger i = 0; i < devices.count; i++) {
                NSMenuItem *item = [[NSMenuItem alloc] init];
                item.title = devices[i].name;
                item.target = self;
                item.action = @selector(selectDeviceListItem:);
                item.tag = i;
                [self.deviceListMenu addItem:item];
                if ([self.selectedDevice.name isEqualToString:devices[i].name]) {
                    [self.deviceListButton selectItem:item];
                }
            }
        });
    });
}

- (void)selectDeviceListItem:(NSMenuItem *)sender {
    NSArray<WVUVCDevice *> *devices = self.devices;
    if (self.selectedDevice) {
        [self.selectedDevice stopStream];
        self.selectedDevice = nil;
    }
    if (sender.tag < 0 || sender.tag >= devices.count) {
        return;
    }
    WVUVCDevice *device = devices[sender.tag];
    device.delegate = self;
    self.selectedDevice = device;
    NSError *error;
    if (![device startStreamWithError:&error]) {
        self.selectedDevice = nil;
        NSAlert *alert = [NSAlert alertWithError:error];
        [alert runModal];
    }
}

- (void)uvcDevice:(WVUVCDevice *)device didRecieveFrame:(CGImageRef)frame {
    CGImageRef _frame = CGImageRetain(frame);
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.imageView setImage:_frame imageProperties:nil];
        CGImageRelease(_frame);
    });
}

@end
