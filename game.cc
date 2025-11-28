export module game;
import <memory>;
import <string>;
import board;
import block;
import level;
import scorekeeper;
import effect;
import textdisplay;
import graphicsdisplay;
import constants;

using namespace GameConstants;

export class Game {
    std::unique_ptr<Board> board1;
    std::unique_ptr<Board> board2;
    std::unique_ptr<Level> level1;
    std::unique_ptr<Level> level2;
    std::unique_ptr<ScoreKeeper> score1;
    std::unique_ptr<ScoreKeeper> score2;
    std::unique_ptr<TextDisplay> textDisplay1;
    std::unique_ptr<TextDisplay> textDisplay2;
    std::unique_ptr<GraphicsDisplay> graphicsDisplay1;
    std::unique_ptr<GraphicsDisplay> graphicsDisplay2;

    int currentPlayer;
    bool isRunning;
    bool textOnly;
    unsigned int randomSeed;
    std::string scriptFile1;
    std::string scriptFile2;
    int startLevel;

public:
    Game(unsigned int seed = 0, int level = 0,
         const std::string& script1 = "biquadris_sequence1.txt",
         const std::string& script2 = "biquadris_sequence2.txt",
         bool textMode = false);

    void createLevels(int levelNum);
    Board* getCurrentBoard();
    Board* getOpponentBoard();
    Level* getCurrentLevel();
    ScoreKeeper* getCurrentScore();
    void spawnNextBlock(Board* board);
    void switchPlayer();
    bool drop();
    void render();
    void restart();
    bool isGameRunning() const;
    void applySpecialAction(const std::string& action, char blockType = 'I');
    void levelUp();
    void levelDown();
    void createPlayerLevel(int player, int levelNum);
};
