#include <X11/Xlib.h>
#include <iostream>
#include <string>

int main() {
  Display *display;
  Window window;
  XEvent event;
  std::string msg = "Hello, X Window in C++!";
  int screen;

  // Abrir conexão com o servidor X
  display = XOpenDisplay(nullptr);
  if (display == nullptr) {
    std::cerr << "Não foi possível abrir o display" << std::endl;
    return 1;
  }

  screen = DefaultScreen(display);

  // Criar janela
  window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10,
                               300, 150, 1, BlackPixel(display, screen),
                               WhitePixel(display, screen));

  XSelectInput(display, window, ExposureMask | KeyPressMask);
  XMapWindow(display, window);

  // Loop de eventos
  while (true) {
    XNextEvent(display, &event);
    if (event.type == Expose) {
      XDrawString(display, window, DefaultGC(display, screen), 50, 50,
                  msg.c_str(), msg.length());
    }
    if (event.type == KeyPress)
      break;
  }

  XCloseDisplay(display);
  return 0;
}
