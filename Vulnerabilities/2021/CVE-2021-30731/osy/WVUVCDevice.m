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

#import "WVUVCDevice.h"
#import <libusb.h>
#import <libuvc.h>

extern uvc_context_t *uvc_ctx;

@interface WVUVCDevice ()

@property uvc_device_t *device;
@property uvc_device_handle_t *handle;
@property (readwrite) NSString *name;

- (nullable instancetype)initWithDevice:(uvc_device_t *)device;

@end

@implementation WVUVCDevice

+ (NSArray<WVUVCDevice *> *)allDevices {
    NSMutableArray<WVUVCDevice *> *devices = [NSMutableArray array];
    uvc_device_t **devs;
    
    if (uvc_get_device_list(uvc_ctx, &devs) < 0) {
        return devices;
    }
    for (ssize_t i = 0; devs[i] != NULL; i++) {
        WVUVCDevice *device = [[WVUVCDevice alloc] initWithDevice:devs[i]];
        if (device) {
            [devices addObject:device];
        }
    }
    uvc_free_device_list(devs, 1);
    return devices;
}

- (instancetype)initWithDevice:(uvc_device_t *)device {
    if (self = [super init]) {
        self.device = device;
        self.name = [self createDescription];
        if (!self.name) {
            return nil;
        }
        uvc_ref_device(device);
    }
    return self;
}

- (void)dealloc {
    [self stopStream];
    uvc_unref_device(self.device);
}

- (nullable NSString *)createDescription {
    uvc_device_descriptor_t *desc;
    NSString *product;
    NSString *manufacturer;
    NSString *path;
    if (uvc_get_device_descriptor(self.device, &desc) < 0) {
        return nil;
    }
    path = [NSString stringWithFormat:@"%04X:%04X (bus %u, device %u)", desc->idVendor, desc->idProduct, uvc_get_bus_number(self.device), uvc_get_device_address(self.device)];
    if (desc->product) {
        product = [NSString stringWithCString:desc->product encoding:NSASCIIStringEncoding];
    }
    if (desc->manufacturer) {
        manufacturer = [NSString stringWithCString:desc->manufacturer encoding:NSASCIIStringEncoding];
    }
    uvc_free_device_descriptor(desc);
    if (product && manufacturer) {
        return [NSString stringWithFormat:@"%@ - %@ - %@", manufacturer, product, path];
    } else if (product) {
        return [NSString stringWithFormat:@"%@ - %@", product, path];
    } else if (manufacturer) {
        return [NSString stringWithFormat:@"%@ - %@", manufacturer, path];
    } else {
        return path;
    }
}

static NSError *uvcErrorToNSError(uvc_error_t res) {
    const char *errstr = uvc_strerror(res);
    if (errstr) {
        return [NSError errorWithDomain:@"com.osy86.WebcamViewer" code:-1 userInfo:@{NSLocalizedDescriptionKey: [NSString stringWithFormat:NSLocalizedString(@"libuvc error: %s", "WVUVCDevice"), errstr]}];
    } else {
        return [NSError errorWithDomain:@"com.osy86.WebcamViewer" code:-1 userInfo:@{NSLocalizedDescriptionKey: NSLocalizedString(@"Unknown error.", "WVUVCDevice")}];
    }
}

static void streamCallback(struct uvc_frame *frame, void *user_ptr) {
    WVUVCDevice *self = (__bridge WVUVCDevice *)user_ptr;
    uvc_frame_t *rgb;
    uvc_error_t ret;
    
    /* We'll convert the image from YUV/JPEG to BGR, so allocate space */
    rgb = uvc_allocate_frame(frame->width * frame->height * 3);
    if (!rgb) {
        return;
    }
    
    /* Do the BGR conversion */
    // TODO: use CoreVideo to process the frame
    ret = uvc_any2rgb(frame, rgb);
    if (ret) {
        uvc_perror(ret, "uvc_any2rgb");
        uvc_free_frame(rgb);
        return;
    }
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, rgb->data, rgb->data_bytes, NULL);
    CGColorSpaceRef colorspace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
    CGImageRef cgImage = CGImageCreate(/* width              */ rgb->width,
                                       /* height             */ rgb->height,
                                       /* bitsPerComponent   */ 8,
                                       /* bitsPerPixel       */ 24,
                                       /* bytesPerRow        */ rgb->width * 3,
                                       /* colorspace         */ colorspace,
                                       /* bitmapInfo         */ kCGBitmapByteOrderDefault,
                                       /* provider           */ provider,
                                       /* decode             */ NULL,
                                       /* shouldInterpolate  */ YES,
                                       /* intent             */ kCGRenderingIntentDefault);
    CGColorSpaceRelease(colorspace);
    CGDataProviderRelease(provider);
    CGImageRef copy = CGImageCreateCopy(cgImage);
    CGImageRelease(cgImage);
    [self.delegate uvcDevice:self didRecieveFrame:copy];
    CGImageRelease(copy);
    
    uvc_free_frame(rgb);
}

- (BOOL)startStreamWithError:(NSError *__autoreleasing  _Nullable *)error {
    uvc_error_t res = UVC_SUCCESS;
    uvc_device_handle_t *devh = NULL;
    uvc_stream_ctrl_t ctrl;
    NSLog(@"Opening device (%@)...", self.name);
    if ((res = uvc_open(self.device, &devh)) < 0) {
        goto err;
    }
    NSLog(@"Device opened.");
    uvc_print_diag(devh, stderr);
    enum uvc_frame_format frame_format = UVC_FRAME_FORMAT_ANY;
    int width = 640;
    int height = 480;
    int fps = 30;
    for (const uvc_format_desc_t *format_desc = uvc_get_format_descs(devh);
         format_desc;
         format_desc = format_desc->next) {
        if (format_desc->bDescriptorSubtype != UVC_VS_FORMAT_UNCOMPRESSED) {
            continue;
        }
        for (const uvc_frame_desc_t *frame_desc = format_desc->frame_descs;
             frame_desc;
             frame_desc = frame_desc->next) {
            if (frame_desc->wWidth > 640) {
                continue;
            }
            if (frame_desc->wHeight > 480) {
                continue;
            }
            frame_format = UVC_FRAME_FORMAT_UNCOMPRESSED;
            width = frame_desc->wWidth;
            height = frame_desc->wHeight;
            fps = 10000000 / frame_desc->dwDefaultFrameInterval;
            break;
        }
        if (frame_format != UVC_FRAME_FORMAT_ANY) {
            break;
        }
    }
    if (frame_format == UVC_FRAME_FORMAT_ANY) {
        NSLog(@"Cannot find supported format");
        res = UVC_ERROR_NOT_SUPPORTED;
        goto err;
    }
    NSLog(@"Found format: (%x) %dx%d %dfps", frame_format, width, height, fps);
    NSLog(@"Negotiate stream profile...");
    if ((res = uvc_get_stream_ctrl_format_size(devh, &ctrl, frame_format, width, height, fps)) < 0) {
        goto err;
    }
    uvc_print_stream_ctrl(&ctrl, stderr);
    NSLog(@"Starting stream...");
    libusb_set_option(NULL, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_ERROR);
    if ((res = uvc_start_streaming(devh, &ctrl, streamCallback, (__bridge void *)self, 0)) < 0) {
        goto err;
    }
    NSLog(@"Stream started.");
    self.handle = devh;
    [self setDefaultOptions];
    return YES;
err:
    if (error) {
        *error = uvcErrorToNSError(res);
    }
    if (devh) {
        uvc_close(devh);
    }
    return NO;
}

- (void)stopStream {
    if (self.handle) {
        NSLog(@"Stopping stream...");
        uvc_stop_streaming(self.handle);
        NSLog(@"Stream stopped.");
        libusb_set_option(NULL, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_DEBUG);
        uvc_close(self.handle);
        self.handle = NULL;
    }
}

- (void)setDefaultOptions {
    uvc_device_handle_t *devh = self.handle;
    uvc_error_t res = UVC_SUCCESS;
    /* enable auto exposure - see uvc_set_ae_mode documentation */
    NSLog(@"Enabling auto exposure ...");
    const uint8_t UVC_AUTO_EXPOSURE_MODE_AUTO = 2;
    res = uvc_set_ae_mode(devh, UVC_AUTO_EXPOSURE_MODE_AUTO);
    if (res == UVC_SUCCESS) {
        NSLog(@" ... enabled auto exposure");
    } else if (res == UVC_ERROR_PIPE) {
        /* this error indicates that the camera does not support the full AE mode;
         * try again, using aperture priority mode (fixed aperture, variable exposure time) */
        NSLog(@" ... full AE not supported, trying aperture priority mode");
        const uint8_t UVC_AUTO_EXPOSURE_MODE_APERTURE_PRIORITY = 8;
        res = uvc_set_ae_mode(devh, UVC_AUTO_EXPOSURE_MODE_APERTURE_PRIORITY);
        if (res < 0) {
            uvc_perror(res, " ... uvc_set_ae_mode failed to enable aperture priority mode");
        } else {
            NSLog(@" ... enabled aperture priority auto exposure mode");
        }
    } else {
        uvc_perror(res, " ... uvc_set_ae_mode failed to enable auto exposure mode");
    }
}

@end
