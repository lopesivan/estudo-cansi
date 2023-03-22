#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    Display *display;
    Window window;
    XEvent event;
    GC gc;
    XImage *image;
    int screen;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, 500, 300, 1,
                                  BlackPixel(display, screen), WhitePixel(display, screen));

    XSelectInput(display, window, ExposureMask | KeyPressMask | ButtonPressMask);
    XMapWindow(display, window);

    gc = XCreateGC(display, window, 0, NULL);

    // Load image data here using XCreateImage, XReadBitmapFile, or similar functions
    // image = ...

    while (1) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            int button_width = 80;
            int button_height = 30;
            int img_width = 200; // Replace with image width
            int img_height = 200; // Replace with image height

            // Draw buttons
            XDrawRectangle(display, window, gc, 10, 10, button_width, button_height);
            XDrawString(display, window, gc, 20, 30, "Button 1", 7);
            XDrawRectangle(display, window, gc, 410, 10, button_width, button_height);
            XDrawString(display, window, gc, 420, 30, "Button 2", 7);

            // Draw image
            // XPutImage(display, window, gc, image, 0, 0, (500 - img_width) / 2, (300 - img_height) / 2, img_width, img_height);
        }
        
        if (event.type == ButtonPress) {
            printf("Mouse button pressed\n");
        }

        if (event.type == KeyPress) {
            break;
        }
    }

    // Destroy image
    // XDestroyImage(image);

    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
