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
  unsigned long colours[12];
  Pixmap pixmap;
  int window_width, window_height;

  static constexpr const char* LOGO_TEXT = "BIQUADRIS";
  static constexpr int LOGO_NUM_LETTERS = 9;

 public:
  Xwindow(int width=500, int height=500);
  ~Xwindow();
  Xwindow(const Xwindow&) = delete;
  Xwindow &operator=(const Xwindow&) = delete;

  enum {White=0, Black, Red, Green, Blue, Cyan, Yellow, Magenta, Orange, Brown, DarkGreen, DarkCyan};
  
  static constexpr int LOGO_COLORS[9] = {Red, Orange, Brown, DarkGreen, DarkCyan, Blue, Magenta, Magenta, Red};

  void fillRectangle(int x, int y, int width, int height, int colour=Black);
  void drawString(int x, int y, std::string msg);
  void drawStringBlack(int x, int y, std::string msg);
  void drawStringBlackBold(int x, int y, std::string msg);
  void present();
  void setWindowTitle(std::string title);
  void drawLogo(int x, int y, int width, int height);
  void drawArrowKeys(int x, int y, int keySize);
};

