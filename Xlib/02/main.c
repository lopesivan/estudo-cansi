#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include <stdlib.h>

#include <png.h>

XImage *load_png_image (Display *display, const char *filename);

int main()
{
    Display *display;
    Window window;
    XEvent event;
    GC gc;
    XImage *image;
    int screen;

    display = XOpenDisplay (NULL);
    if (display == NULL)
    {
        fprintf (stderr, "Cannot open display\n");
        exit (1);
    }

    screen = DefaultScreen (display);
    window = XCreateSimpleWindow (display, RootWindow (display, screen), 10, 10,
                                  500, 300, 1,
                                  BlackPixel (display, screen), WhitePixel (display, screen));

    XSelectInput (display, window, ExposureMask | KeyPressMask | ButtonPressMask);
    XMapWindow (display, window);

    gc = XCreateGC (display, window, 0, NULL);

    // Load image data
    image = load_png_image (display, "foto.png");

    while (1)
    {
        XNextEvent (display, &event);

        if (event.type == Expose)
        {
            int button_width = 80;
            int button_height = 30;
            int img_width = image->width;
            int img_height = image->height;

            // Draw buttons
            XDrawRectangle (display, window, gc, 10, 10, button_width, button_height);
            XDrawString (display, window, gc, 20, 30, "Button 1", 7);
            XDrawRectangle (display, window, gc, 410, 10, button_width, button_height);
            XDrawString (display, window, gc, 420, 30, "Button 2", 7);

            // Draw image
            XPutImage (display, window, gc, image, 0, 0, (500 - img_width) / 2,
                       (300 - img_height) / 2, img_width, img_height);
        }

        if (event.type == ButtonPress)
        {
            printf ("Mouse button pressed\n");
        }

        if (event.type == KeyPress)
        {
            break;
        }
    }

    // Destroy image
    XDestroyImage (image);

    XDestroyWindow (display, window);
    XCloseDisplay (display);

    return 0;
}

XImage *load_png_image (Display *display, const char *filename)
{
    FILE *file = fopen (filename, "rb");
    if (!file)
    {
        fprintf (stderr, "Cannot open file: %s\n", filename);
        return NULL;
    }

    png_structp png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL,
                          NULL);
    if (!png_ptr)
    {
        fclose (file);
        return NULL;
    }

    png_infop info_ptr = png_create_info_struct (png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct (&png_ptr, NULL, NULL);
        fclose (file);
        return NULL;
    }

    if (setjmp (png_jmpbuf (png_ptr)))
    {
        png_destroy_read_struct (&png_ptr, &info_ptr, NULL);
        fclose (file);
        return NULL;
    }

    png_init_io (png_ptr, file);
    png_read_info (png_ptr, info_ptr);

    int width = png_get_image_width (png_ptr, info_ptr);
    int height = png_get_image_height (png_ptr, info_ptr);
    png_byte color_type = png_get_color_type (png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth (png_ptr, info_ptr);

    if (bit_depth == 16) png_set_strip_16 (png_ptr);
    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb (png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY &&
        bit_depth < 8) png_set_expand_gray_1_2_4_to_8 (png_ptr);
    if (png_get_valid (png_ptr, info_ptr,
                       PNG_INFO_tRNS)) png_set_tRNS_to_alpha (png_ptr);
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE) png_set_filler (png_ptr, 0xff,
                    PNG_FILLER_AFTER);
    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb (png_ptr);

    png_read_update_info (png_ptr, info_ptr);

    png_bytep *row_pointers = (png_bytep *)malloc (sizeof (png_bytep) * height);
    for (int y = 0; y < height; y++)
    {
        row_pointers[y] = (png_byte *)malloc (png_get_rowbytes (png_ptr, info_ptr));
    }

    png_read_image (png_ptr, row_pointers);

    fclose (file);

    char *image_data = (char *)malloc (width * height * 4);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            png_bytep pixel = & (row_pointers[y][x * 4]);
            image_data[ (y * width + x) * 4 + 0] = pixel[0];
            image_data[ (y * width + x) * 4 + 1] = pixel[1];
            image_data[ (y * width + x) * 4 + 2] = pixel[2];
            image_data[ (y * width + x) * 4 + 3] = pixel[3];
        }
    }

    for (int y = 0; y < height; y++)
    {
        free (row_pointers[y]);
    }
    free (row_pointers);

    png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

    int depth = DefaultDepth (display, DefaultScreen (display));
    Visual *visual = DefaultVisual (display, DefaultScreen (display));
    XImage *ximage = XCreateImage (display, visual, depth, ZPixmap, 0, image_data,
                                   width, height, 32, 0);

    if (!ximage)
    {
        fprintf (stderr, "Failed to create XImage\n");
        free (image_data);
        return NULL;
    }

    return ximage;
}
