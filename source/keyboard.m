#import <CoreGraphics/CoreGraphics.h>
#include <stdbool.h>

bool getkeystate(unsigned short key) {
    if (key == 0x39) return false; // CapsLock key pressed means CapsLock is toggled so we pretend it's never pressed
    return CGEventSourceKeyState(kCGEventSourceStateHIDSystemState, key);
}

bool capslocktoggled(void) {
    return CGEventSourceKeyState(kCGEventSourceStateHIDSystemState, 0x39); // CapsLock key pressed means CapsLock is toggled
}
