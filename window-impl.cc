module;
#include <X11/Xlib.h>
#include <X11/Xutil.h>
module xwindow;
import <iostream>;
import <cstdlib>;
import <string>;
import constants;

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

  pixmap = XCreatePixmap(d, w, width, height, DefaultDepth(d, s));
  gc = XCreateGC(d, pixmap, 0, (XGCValues *)0);
  XFlush(d);

  XColor xcolour;
  Colormap cmap = DefaultColormap(d,s);
  char color_vals[12][12]={"white", "black", "red", "green", "blue", "cyan", "yellow", "magenta", "orange", "brown", "darkgreen", "darkcyan"};

  for(int i=0; i < 12; ++i) {
      XParseColor(d,cmap,color_vals[i],&xcolour);
      XAllocColor(d,cmap,&xcolour);
      colours[i]=xcolour.pixel;
  }

  XSetForeground(d,gc,colours[Black]);

  XSizeHints hints;
  hints.flags = (USPosition | PSize | PMinSize | PMaxSize );
  hints.height = hints.base_height = hints.min_height = hints.max_height = height;
  hints.width = hints.base_width = hints.min_width = hints.max_width = width;
  XSetNormalHints(d, w, &hints);

  XSynchronize(d, False);

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
  XSetForeground(d, gc, colours[White]);
  XDrawString(d, pixmap, gc, x, y, msg.c_str(), msg.length());
}

void Xwindow::drawStringBlack(int x, int y, string msg) {
  XSetForeground(d, gc, colours[Black]);
  XDrawString(d, pixmap, gc, x, y, msg.c_str(), msg.length());
  XSetForeground(d, gc, colours[White]);
}

void Xwindow::drawStringBlackBold(int x, int y, string msg) {
  XSetForeground(d, gc, colours[Black]);
  XDrawString(d, pixmap, gc, x, y, msg.c_str(), msg.length());
  XDrawString(d, pixmap, gc, x + 1, y, msg.c_str(), msg.length());
  XDrawString(d, pixmap, gc, x, y + 1, msg.c_str(), msg.length());
  XDrawString(d, pixmap, gc, x + 1, y + 1, msg.c_str(), msg.length());
  XSetForeground(d, gc, colours[White]);
}

void Xwindow::present() {
  XCopyArea(d, pixmap, w, gc, 0, 0, window_width, window_height, 0, 0);
  XFlush(d);
}

void Xwindow::setWindowTitle(string title) {
  XStoreName(d, w, title.c_str());
  XFlush(d);
}

void Xwindow::drawLogo(int x, int y, int width, int height) {
  using namespace GameConstants;
  
  int blockWidth = width / LOGO_NUM_LETTERS;
  
  for (int i = 0; i < LOGO_NUM_LETTERS; i++) {
    int blockX = x + i * blockWidth;
    
    XSetForeground(d, gc, colours[LOGO_COLORS[i]]);
    XFillRectangle(d, pixmap, gc, blockX, y, blockWidth - LOGO_BLOCK_BORDER, height);
    
    XSetForeground(d, gc, colours[White]);
    XDrawRectangle(d, pixmap, gc, blockX, y, blockWidth - LOGO_BLOCK_BORDER, height);
    
    string letter(1, LOGO_TEXT[i]);
    int textX = blockX + blockWidth / 2 - LOGO_TEXT_X_OFFSET;
    int textY = y + height / 2 + LOGO_TEXT_Y_OFFSET;
    XDrawString(d, pixmap, gc, textX, textY, letter.c_str(), 1);
  }
  
  XSetForeground(d, gc, colours[White]);
}

void Xwindow::drawArrowKeys(int x, int y, int keySize) {
  int gap = 5;
  
  int centerX = x + keySize + gap;
  XSetForeground(d, gc, colours[Black]);
  XFillArc(d, pixmap, gc, centerX, y, keySize, keySize, 0, 360 * 64);
  XSetForeground(d, gc, colours[White]);
  XDrawArc(d, pixmap, gc, centerX, y, keySize, keySize, 0, 360 * 64);
  
  XPoint upTriangle[3];
  upTriangle[0].x = centerX + keySize/2;
  upTriangle[0].y = y + 10;
  upTriangle[1].x = centerX + keySize/2 - 8;
  upTriangle[1].y = y + 22;
  upTriangle[2].x = centerX + keySize/2 + 8;
  upTriangle[2].y = y + 22;
  XFillPolygon(d, pixmap, gc, upTriangle, 3, Convex, CoordModeOrigin);
  
  int centerY = y + keySize + gap;
  XSetForeground(d, gc, colours[Black]);
  XFillArc(d, pixmap, gc, centerX, centerY + keySize + gap, keySize, keySize, 0, 360 * 64);
  XSetForeground(d, gc, colours[White]);
  XDrawArc(d, pixmap, gc, centerX, centerY + keySize + gap, keySize, keySize, 0, 360 * 64);
  
  int downY = centerY + keySize + gap;
  XPoint downTriangle[3];
  downTriangle[0].x = centerX + keySize/2;
  downTriangle[0].y = downY + 25;
  downTriangle[1].x = centerX + keySize/2 - 8;
  downTriangle[1].y = downY + 13;
  downTriangle[2].x = centerX + keySize/2 + 8;
  downTriangle[2].y = downY + 13;
  XFillPolygon(d, pixmap, gc, downTriangle, 3, Convex, CoordModeOrigin);
  
  XSetForeground(d, gc, colours[Black]);
  XFillArc(d, pixmap, gc, x, centerY, keySize, keySize, 0, 360 * 64);
  XSetForeground(d, gc, colours[White]);
  XDrawArc(d, pixmap, gc, x, centerY, keySize, keySize, 0, 360 * 64);
  
  XPoint leftTriangle[3];
  leftTriangle[0].x = x + 10;
  leftTriangle[0].y = centerY + keySize/2;
  leftTriangle[1].x = x + 22;
  leftTriangle[1].y = centerY + keySize/2 - 8;
  leftTriangle[2].x = x + 22;
  leftTriangle[2].y = centerY + keySize/2 + 8;
  XFillPolygon(d, pixmap, gc, leftTriangle, 3, Convex, CoordModeOrigin);
  
  int rightX = centerX + keySize + gap;
  XSetForeground(d, gc, colours[Black]);
  XFillArc(d, pixmap, gc, rightX, centerY, keySize, keySize, 0, 360 * 64);
  XSetForeground(d, gc, colours[White]);
  XDrawArc(d, pixmap, gc, rightX, centerY, keySize, keySize, 0, 360 * 64);
  
  XPoint rightTriangle[3];
  rightTriangle[0].x = rightX + 25;
  rightTriangle[0].y = centerY + keySize/2;
  rightTriangle[1].x = rightX + 13;
  rightTriangle[1].y = centerY + keySize/2 - 8;
  rightTriangle[2].x = rightX + 13;
  rightTriangle[2].y = centerY + keySize/2 + 8;
  XFillPolygon(d, pixmap, gc, rightTriangle, 3, Convex, CoordModeOrigin);
  
  XSetForeground(d, gc, colours[White]);
}
