module;
#include <memory>
#include <string>
export module graphicsdisplay;
import observer;
import board;
import block;
import xwindow;
import constants;

using namespace GameConstants;

export class GraphicsDisplay : public IObserver {
    Board* board;
    std::unique_ptr<Xwindow> window;
    int blockSize;
    int offsetX;
    int offsetY;
    int headerHeight;
    bool blindMode;
    std::string playerName;

    int getColor(char type) const;
    void draw3DBlock(int row, int col, int color);
    void drawEmptyCell(int row, int col);

public:
    GraphicsDisplay(Board* b, std::string name = "Player", int width = GRAPHICS_WINDOW_WIDTH, int height = GRAPHICS_WINDOW_HEIGHT);
    void update() override;
    void setBlindMode(bool blind);
    void setBoard(Board* b);
    void drawBlock(int row, int col, int color);
    void render();
    void renderWithInfo(int level, int score, int highScore);
};
