#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  Display *d;
  Window w;
  XEvent e;
  const char *msg = "Hello, X Window!";
  int s;

  // Abrir conexão com o servidor X
  d = XOpenDisplay(NULL);
  if (d == NULL) {
    fprintf(stderr, "Não foi possível abrir o display\n");
    exit(1);
  }

  s = DefaultScreen(d);

  // Criar janela
  w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 200, 100, 1,
                          BlackPixel(d, s), WhitePixel(d, s));

  // Selecionar tipos de eventos
  XSelectInput(d, w, ExposureMask | KeyPressMask);

  // Mostrar a janela
  XMapWindow(d, w);

  // Loop de eventos
  while (1) {
    XNextEvent(d, &e);
    if (e.type == Expose) {
      XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
      XDrawString(d, w, DefaultGC(d, s), 50, 50, msg, strlen(msg));
    }
    if (e.type == KeyPress)
      break;
  }

  // Fechar conexão com o servidor X
  XCloseDisplay(d);
  return 0;
}
