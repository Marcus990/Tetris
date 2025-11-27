module;
#include <X11/Xlib.h>
export module xwindow;
import <iostream>;
import <string>;

export class Xwindow {
  Display *d;
  Window w;
  int s;
  GC gc;
  unsigned long colours[10];
  Pixmap pixmap;  // Double buffer
  int window_width, window_height;

 public:
  Xwindow(int width=500, int height=500);  // Constructor; displays the window.
  ~Xwindow();                              // Destructor; destroys the window.
  Xwindow(const Xwindow&) = delete;
  Xwindow &operator=(const Xwindow&) = delete;

  // Available colours.
  enum {White=0, Black, Red, Green, Blue, Cyan, Yellow, Magenta, Orange, Brown};

  // Draws a rectangle (to double buffer)
  void fillRectangle(int x, int y, int width, int height, int colour=Black);

  // Draws a string (to double buffer)
  void drawString(int x, int y, std::string msg);

  // Flips the double buffer to screen (call after all drawing)
  void present();

};

