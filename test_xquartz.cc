// test_xquartz.cc - Simple test to verify XQuartz is working
#include <X11/Xlib.h>
#include <iostream>
#include <unistd.h>

int main() {
    std::cout << "Testing XQuartz/X11 connection..." << std::endl;
    
    Display *display = XOpenDisplay(NULL);
    
    if (display == NULL) {
        std::cerr << "ERROR: Cannot open display!" << std::endl;
        std::cerr << "Make sure XQuartz is running: open -a XQuartz" << std::endl;
        std::cerr << "And set DISPLAY=:0" << std::endl;
        return 1;
    }
    
    std::cout << "✓ Successfully connected to X11!" << std::endl;
    std::cout << "✓ Display: " << XDisplayName(NULL) << std::endl;
    std::cout << "✓ Screen: " << DefaultScreen(display) << std::endl;
    
    // Create a simple window
    int screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(
        display,
        RootWindow(display, screen),
        100, 100,  // position
        300, 200,  // size
        1,         // border width
        BlackPixel(display, screen),
        WhitePixel(display, screen)
    );
    
    XStoreName(display, window, "XQuartz Test - Close this window");
    XMapWindow(display, window);
    XFlush(display);
    
    std::cout << "✓ Created test window!" << std::endl;
    std::cout << "  A white window should appear." << std::endl;
    std::cout << "  Close it or press Ctrl+C to exit." << std::endl;
    
    // Keep window open for 10 seconds
    sleep(10);
    
    XCloseDisplay(display);
    std::cout << "✓ XQuartz test completed successfully!" << std::endl;
    
    return 0;
}

