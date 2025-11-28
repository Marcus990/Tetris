export module graphicsdisplay;
import <memory>;
import <string>;
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
    int cachedLevel;
    int cachedScore;
    int cachedHighScore;

    int getColor(char type) const;
    void draw3DBlock(int row, int col, int color);
    void drawGhostBlock(int row, int col, int color);
    void drawEmptyCell(int row, int col);

public:
    GraphicsDisplay(Board* b, std::string name = "Player", int width = GRAPHICS_WINDOW_WIDTH, int height = GRAPHICS_WINDOW_HEIGHT);
    void update() override;
    void setBlindMode(bool blind);
    void setBoard(Board* b);
    void setGameInfo(int level, int score, int highScore);
    void render();
    void renderWithInfo(int level, int score, int highScore);
};
