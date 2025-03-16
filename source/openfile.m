#import <CoreServices/CoreServices.h>

int openFileInApp(const char* filepath, const char* app) {
    CFStringRef path = CFStringCreateWithCString (NULL, app, kCFStringEncodingUTF8);
    CFStringRef filePath = CFStringCreateWithCString (NULL, filepath, kCFStringEncodingUTF8);
    CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, path, kCFURLPOSIXPathStyle, false);
    CFURLRef fileURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, filePath, kCFURLPOSIXPathStyle, false);
    CFArrayRef array = CFArrayCreate(kCFAllocatorDefault, (const void **)&fileURL, 1, NULL);
    LSLaunchURLSpec spec = { url,array,NULL, kLSLaunchAsync | kLSLaunchNewInstance, nullptr };
    OSStatus err = LSOpenFromURLSpec(&spec,nullptr);
    return err;
}

int openFile(const char* filepath) {
    CFStringRef filePath = CFStringCreateWithCString (NULL, filepath, kCFStringEncodingUTF8);
    CFURLRef fileURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, filePath, kCFURLPOSIXPathStyle, false);
    CFArrayRef array = CFArrayCreate(kCFAllocatorDefault, (const void **)&fileURL, 1, NULL);
    LSLaunchURLSpec spec = { NULL,array,NULL, kLSLaunchAsync | kLSLaunchNewInstance, nullptr };
    OSStatus err = LSOpenFromURLSpec(&spec,nullptr);
    return err;
}
