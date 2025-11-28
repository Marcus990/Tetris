export module textdisplay;
import <iostream>;
import <string>;
import observer;
import board;
import block;
import constants;

using namespace GameConstants;

export class TextDisplay : public IObserver {
    Board* board;
    std::ostream& out;
    bool blindMode;

public:
    TextDisplay(Board* b, std::ostream& os = std::cout);
    void update() override;
    void setBlindMode(bool blind);
    void setBoard(Board* b);
    void render();
    void renderWithInfo(int level, int score, int highScore);
};
