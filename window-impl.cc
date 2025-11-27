module;
#include <X11/Xlib.h>
#include <X11/Xutil.h>
module xwindow;
import <iostream>;
import <cstdlib>;
import <string>;

using namespace std;

Xwindow::Xwindow(int width, int height)
    : window_width(width), window_height(height) {

  d = XOpenDisplay(NULL);
  if (d == NULL) {
    cerr << "Cannot open display" << endl;
    exit(1);
  }
  s = DefaultScreen(d);
  w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
                          BlackPixel(d, s), WhitePixel(d, s));
  XSelectInput(d, w, ExposureMask | KeyPressMask);
  XMapRaised(d, w);

  // Create double buffer pixmap
  pixmap = XCreatePixmap(d, w, width, height, DefaultDepth(d, s));
  gc = XCreateGC(d, pixmap, 0, (XGCValues *)0);

  XFlush(d);

  // Set up colours.
  XColor xcolour;
  Colormap cmap = DefaultColormap(d,s);
  char color_vals[10][10]={"white", "black", "red", "green", "blue", "cyan", "yellow", "magenta", "orange", "brown"};

  for(int i=0; i < 10; ++i) {
      XParseColor(d,cmap,color_vals[i],&xcolour);
      XAllocColor(d,cmap,&xcolour);
      colours[i]=xcolour.pixel;
  }

  XSetForeground(d,gc,colours[Black]);

  // Make window non-resizeable.
  XSizeHints hints;
  hints.flags = (USPosition | PSize | PMinSize | PMaxSize );
  hints.height = hints.base_height = hints.min_height = hints.max_height = height;
  hints.width = hints.base_width = hints.min_width = hints.max_width = width;
  XSetNormalHints(d, w, &hints);

  XSynchronize(d, False);

  // Clear the pixmap to white
  XSetForeground(d, gc, colours[White]);
  XFillRectangle(d, pixmap, gc, 0, 0, width, height);
  XSetForeground(d, gc, colours[Black]);
}

Xwindow::~Xwindow() {
  XFreePixmap(d, pixmap);
  XFreeGC(d, gc);
  XCloseDisplay(d);
}

void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
  XSetForeground(d, gc, colours[colour]);
  XFillRectangle(d, pixmap, gc, x, y, width, height);
}

void Xwindow::drawString(int x, int y, string msg) {
  // Draw to pixmap using our GC, not DefaultGC
  XSetForeground(d, gc, colours[Black]);
  XDrawString(d, pixmap, gc, x, y, msg.c_str(), msg.length());
}

void Xwindow::present() {
  // Copy the double buffer to the window
  XCopyArea(d, pixmap, w, gc, 0, 0, window_width, window_height, 0, 0);
  XFlush(d);
}
