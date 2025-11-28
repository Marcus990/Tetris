module;
#include <memory>
#include <string>
#include <vector>
#include <iostream>
export module game;
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

    int currentPlayer;  // 0 or 1
    bool isRunning;
    bool textOnly;
    unsigned int randomSeed;
    std::string scriptFile1;
    std::string scriptFile2;
    int startLevel;

public:
    Game(unsigned int seed = 0, int level = 0,
         const std::string& script1 = "sequence1.txt",
         const std::string& script2 = "sequence2.txt",
         bool textMode = false)
        : currentPlayer(PLAYER_ONE), isRunning(true), textOnly(textMode),
          randomSeed(seed), scriptFile1(script1), scriptFile2(script2),
          startLevel(level) {

        // Create boards
        board1 = std::make_unique<Board>();
        board2 = std::make_unique<Board>();

        // Create scorekeepers
        score1 = std::make_unique<ScoreKeeper>();
        score2 = std::make_unique<ScoreKeeper>();

        // Set scorekeepers on boards
        board1->setScoreKeeper(score1.get());
        board2->setScoreKeeper(score2.get());

        // Create levels
        createLevels(startLevel);

        // Create displays
        textDisplay1 = std::make_unique<TextDisplay>(board1.get(), std::cout);
        textDisplay2 = std::make_unique<TextDisplay>(board2.get(), std::cout);

        board1->attach(textDisplay1.get());
        board2->attach(textDisplay2.get());

        if (!textOnly) {
            graphicsDisplay1 = std::make_unique<GraphicsDisplay>(board1.get(), "Player 1");
            graphicsDisplay2 = std::make_unique<GraphicsDisplay>(board2.get(), "Player 2");

            board1->attach(graphicsDisplay1.get());
            board2->attach(graphicsDisplay2.get());
        }

        // Spawn initial blocks
        spawnNextBlock(board1.get());
        spawnNextBlock(board1.get());  // Current and next
        spawnNextBlock(board2.get());
        spawnNextBlock(board2.get());
    }

    void createLevels(int levelNum) {
        if (levelNum == 0) {
            level1 = std::make_unique<Level0>(scriptFile1);
            level2 = std::make_unique<Level0>(scriptFile2);
        } else if (levelNum == 1) {
            level1 = std::make_unique<Level1>(randomSeed);
            level2 = std::make_unique<Level1>(randomSeed + 1);
        } else if (levelNum == 2) {
            level1 = std::make_unique<Level2>(randomSeed);
            level2 = std::make_unique<Level2>(randomSeed + 1);
        } else if (levelNum == 3) {
            level1 = std::make_unique<Level3>(randomSeed);
            level2 = std::make_unique<Level3>(randomSeed + 1);
        } else {
            level1 = std::make_unique<Level4>(randomSeed);
            level2 = std::make_unique<Level4>(randomSeed + 1);
        }

        board1->setLevel(level1.get());
        board2->setLevel(level2.get());
    }

    Board* getCurrentBoard() {
        return currentPlayer == PLAYER_ONE ? board1.get() : board2.get();
    }

    Board* getOpponentBoard() {
        return currentPlayer == PLAYER_ONE ? board2.get() : board1.get();
    }

    Level* getCurrentLevel() {
        return currentPlayer == PLAYER_ONE ? level1.get() : level2.get();
    }

    ScoreKeeper* getCurrentScore() {
        return currentPlayer == PLAYER_ONE ? score1.get() : score2.get();
    }



    void spawnNextBlock(Board* board) {
        Level* level = (board == board1.get()) ? level1.get() : level2.get();

        if (!board->getNextBlock()) {
            // First block - create it
            char forcedType = board->getForcedBlockType();
            std::unique_ptr<Block> block;

            if (forcedType != '\0') {
                // Create the forced block type
                block = level->createBlockFromType(forcedType, board->getNextBlockId());
                board->clearForcedBlockType();  // Consume the force effect
            } else {
                block = level->generateBlock(board->getNextBlockId());
            }
            board->setNextBlock(std::move(block));
        } else {
            // Move next to current, generate new next
            board->setCurrentBlock(board->takeNextBlock());

            // Check for forced block type when generating next block
            char forcedType = board->getForcedBlockType();
            std::unique_ptr<Block> newNext;

            if (forcedType != '\0') {
                // Create the forced block type
                newNext = level->createBlockFromType(forcedType, board->getNextBlockId());
                board->clearForcedBlockType();  // Consume the force effect
            } else {
                newNext = level->generateBlock(board->getNextBlockId());
            }
            board->setNextBlock(std::move(newNext));
        }
    }

    void switchPlayer() {
        currentPlayer = (currentPlayer == PLAYER_ONE) ? PLAYER_TWO : PLAYER_ONE;
    }

    bool drop() {
        Board* board = getCurrentBoard();
        board->drop();

        // Clear full rows
        int linesCleared = board->clearRows();

        // Award points for line clears
        if (linesCleared > 0) {
            ScoreKeeper* score = getCurrentScore();
            int points = score->calculateLineClearPoints(
                getCurrentLevel()->getLevelNumber(), linesCleared);
            score->addScore(points);

            // Render board after clearing rows so player can see what happened
            render();

            // Check for special action (2+ lines cleared)
            if (linesCleared >= ROWS_FOR_SPECIAL_ACTION) {
                std::cout << "Special Action! Choose one: blind, heavy, force\n";
                std::string action;
                std::cin >> action;

                // If force, ask for block type
                if (action == "force") {
                    std::cout << "Choose block type (I, J, L, O, S, Z, T): ";
                    std::string blockType;
                    std::cin >> blockType;
                    if (!blockType.empty()) {
                        applySpecialAction(action, blockType[0]);
                    }
                } else {
                    applySpecialAction(action, '\0');
                }

                // Immediately sync displays after applying special action
                bool isBlind1 = board1->hasBlindEffect();
                bool isBlind2 = board2->hasBlindEffect();

                textDisplay1->setBlindMode(isBlind1);
                if (!textOnly) graphicsDisplay1->setBlindMode(isBlind1);

                textDisplay2->setBlindMode(isBlind2);
                if (!textOnly) graphicsDisplay2->setBlindMode(isBlind2);
            }
        }

        // Level 4 feature: Track blocks placed without clearing
        Level* level = getCurrentLevel();
        if (level->getLevelNumber() == MAX_LEVEL) {
            if (linesCleared > 0) {
                // Reset counter if rows were cleared
                board->resetBlocksSinceLastClear();
            } else {
                // Increment counter and check if we need to drop center block
                board->incrementBlocksSinceLastClear();
                if (board->getBlocksSinceLastClear() >= BLOCKS_BEFORE_CENTER_DROP) {
                    // Drop 1x1 center block on THIS board as penalty
                    auto centerBlock = level->createCenterBlock(board->getNextBlockId());
                    board->dropCenterBlock(std::move(centerBlock));
                    board->resetBlocksSinceLastClear();
                }
            }
        }

        // Check for block removals
        board->checkBlockRemoval();

        // Update effects only on the current player's board (whose turn just finished)
        // This ensures effects last for the correct number of opponent turns
        board->updateEffects();

        // Update blind mode displays for both players based on their board state
        bool isBlind1 = board1->hasBlindEffect();
        textDisplay1->setBlindMode(isBlind1);
        if (!textOnly) graphicsDisplay1->setBlindMode(isBlind1);

        bool isBlind2 = board2->hasBlindEffect();
        textDisplay2->setBlindMode(isBlind2);
        if (!textOnly) graphicsDisplay2->setBlindMode(isBlind2);

        // Check game over
        if (board->isGameOver()) {
            isRunning = false;
            return false;
        }

        // Spawn next block
        spawnNextBlock(board);

        // Switch to other player after drop
        switchPlayer();

        return true;
    }

    void render() {
        // Show current player
        std::cout << "╔══════════╦══╦══════════╗\n";
        std::cout << "║ Player 1 ║  ║ Player 2 ║\n";
        std::cout << "╚══════════╩══╩══════════╝\n";
        // Simple side-by-side display
        std::cout << "Level:    " << level1->getLevelNumber() << "    Level:    " << level2->getLevelNumber() << "\n";
        std::cout << "Score:    " << score1->getCurrentScore() << "    Score:    " << score2->getCurrentScore() << "\n";
        std::cout << "High Score: " << score1->getHighScore() << "  High Score:    " << score2->getHighScore() << "\n";
        std::cout << "╔═══════════╗ ╔═══════════╗\n";

        // Get both board grids
        const auto& grid1 = board1->getGrid();
        const auto& grid2 = board2->getGrid();

        // Get current blocks
        Block* current1 = board1->getCurrentBlock();
        Block* current2 = board2->getCurrentBlock();

        // Create display grids with current blocks overlaid
        auto display1 = grid1;
        auto display2 = grid2;

        if (current1) {
            auto cells = current1->getAbsoluteCells();
            for (const auto& cell : cells) {
                int row = cell.first;
                int col = cell.second;
                if (row >= 0 && row < TOTAL_ROWS && col >= 0 && col < BOARD_WIDTH) {
                    display1[row][col].setType(current1->getType());
                    display1[row][col].setFilled(true);
                }
            }
        }

        if (current2) {
            auto cells = current2->getAbsoluteCells();
            for (const auto& cell : cells) {
                int row = cell.first;
                int col = cell.second;
                if (row >= 0 && row < TOTAL_ROWS && col >= 0 && col < BOARD_WIDTH) {
                    display2[row][col].setType(current2->getType());
                    display2[row][col].setFilled(true);
                }
            }
        }

        // Check blind mode for both boards
        bool isBlind1 = board1->hasBlindEffect();
        bool isBlind2 = board2->hasBlindEffect();

        // Print boards side by side (including reserve rows - all 18 rows)
        for (int row = 0; row < TOTAL_ROWS; ++row) {
            std::cout << "║";
            // Print board 1 row
            for (int col = 0; col < BOARD_WIDTH; ++col) {
                // Apply blind effect if active
                if (isBlind1 &&
                    row >= RESERVE_ROWS + BLIND_ROW_START &&
                    row <= RESERVE_ROWS + BLIND_ROW_END &&
                    col >= BLIND_COL_START && col <= BLIND_COL_END) {
                    std::cout << '?';
                } else if (display1[row][col].isFilled()) {
                    std::cout << display1[row][col].getType();
                } else {
                    std::cout << ' ';
                }
            }
            std::cout << "║ ║";

            // Print board 2 row
            for (int col = 0; col < BOARD_WIDTH; ++col) {
                // Apply blind effect if active
                if (isBlind2 &&
                    row >= RESERVE_ROWS + BLIND_ROW_START &&
                    row <= RESERVE_ROWS + BLIND_ROW_END &&
                    col >= BLIND_COL_START && col <= BLIND_COL_END) {
                    std::cout << '?';
                } else if (display2[row][col].isFilled()) {
                    std::cout << display2[row][col].getType();
                } else {
                    std::cout << ' ';
                }
            }

            std::cout << "║";
            std::cout << "\n";
        }

        std::cout << "╚";
        
        // Print separators
        for (int i = 0; i < BOARD_WIDTH; ++i) {
            std::cout << "═";
        }
        std::cout << "╝ ╚";
        
        for (int i = 0; i < BOARD_WIDTH; ++i) {
            std::cout << "═";
        }
        std::cout << "╝" << "\n";

        // Print next blocks header
        std::cout << "Next:       Next:\n";

        Block* next1 = board1->getNextBlock();
        Block* next2 = board2->getNextBlock();

        std::vector<std::string> g1(4, "    ");
        std::vector<std::string> g2(4, "    ");

        if (next1) {
            for (auto& cell : next1->getCells()) {
                int x = cell.first;
                int y = cell.second;
                if (x >= 0 && x < 4 && y >= 0 && y < 4)
                    g1[y][x] = next1->getType();
            }
        }

        if (next2) {
            for (auto& cell : next2->getCells()) {
                int x = cell.first;
                int y = cell.second;
                if (x >= 0 && x < 4 && y >= 0 && y < 4)
                    g2[y][x] = next2->getType();
            }
        }

        // Print side-by-side
        for (int row = 0; row < 4; row++) {
            std::cout << g1[row] << "        " << g2[row] << "\n";
        }

        std::cout << "Current Player: " << (currentPlayer + 1) << "\n";

        if (!textOnly) {
            graphicsDisplay1->renderWithInfo(
                level1->getLevelNumber(),
                score1->getCurrentScore(),
                score1->getHighScore()
            );
            graphicsDisplay2->renderWithInfo(
                level2->getLevelNumber(),
                score2->getCurrentScore(),
                score2->getHighScore()
            );
        }
    }

    void restart() {
        // Reset scores
        score1->reset();
        score2->reset();

        // Recreate boards
        board1 = std::make_unique<Board>();
        board2 = std::make_unique<Board>();

        board1->setScoreKeeper(score1.get());
        board2->setScoreKeeper(score2.get());

        // Recreate levels
        createLevels(startLevel);

        // Update board pointers in displays before reattaching
        textDisplay1->setBoard(board1.get());
        textDisplay2->setBoard(board2.get());

        if (!textOnly) {
            graphicsDisplay1->setBoard(board1.get());
            graphicsDisplay2->setBoard(board2.get());
        }

        // Reattach displays to new boards
        board1->attach(textDisplay1.get());
        board2->attach(textDisplay2.get());

        if (!textOnly) {
            board1->attach(graphicsDisplay1.get());
            board2->attach(graphicsDisplay2.get());
        }

        // Reset current player
        currentPlayer = PLAYER_ONE;
        isRunning = true;

        // Spawn initial blocks
        spawnNextBlock(board1.get());
        spawnNextBlock(board1.get());
        spawnNextBlock(board2.get());
        spawnNextBlock(board2.get());
    }

    bool isGameRunning() const { return isRunning; }

    // Apply special action to opponent
    void applySpecialAction(const std::string& action, char blockType = 'I') {
        Board* opponent = getOpponentBoard();

        if (action == "blind") {
            auto effect = new BlindEffect(1);
            opponent->addEffect(effect);
            // Blind mode will be synchronized automatically in drop()
        } else if (action == "heavy") {
            auto effect = new HeavyEffect();
            opponent->addEffect(effect);
        } else if (action == "force") {
            auto effect = new ForceEffect(blockType);
            opponent->addEffect(effect);
        }
    }

    // Level management
    void levelUp() {
        int currentLevelNum = getCurrentLevel()->getLevelNumber();
        if (currentLevelNum < MAX_LEVEL) {
            if (currentPlayer == PLAYER_ONE) {
                createPlayerLevel(1, currentLevelNum + 1);
            } else {
                createPlayerLevel(2, currentLevelNum + 1);
            }
        }
    }

    void levelDown() {
        int currentLevelNum = getCurrentLevel()->getLevelNumber();
        if (currentLevelNum > MIN_LEVEL) {
            if (currentPlayer == PLAYER_ONE) {
                createPlayerLevel(1, currentLevelNum - 1);
            } else {
                createPlayerLevel(2, currentLevelNum - 1);
            }
        }
    }

    void createPlayerLevel(int player, int levelNum) {
        if (player == 1) {
            if (levelNum == 0) level1 = std::make_unique<Level0>(scriptFile1);
            else if (levelNum == 1) level1 = std::make_unique<Level1>(randomSeed);
            else if (levelNum == 2) level1 = std::make_unique<Level2>(randomSeed);
            else if (levelNum == 3) level1 = std::make_unique<Level3>(randomSeed);
            else level1 = std::make_unique<Level4>(randomSeed);
            board1->setLevel(level1.get());
        } else {
            if (levelNum == 0) level2 = std::make_unique<Level0>(scriptFile2);
            else if (levelNum == 1) level2 = std::make_unique<Level1>(randomSeed + 1);
            else if (levelNum == 2) level2 = std::make_unique<Level2>(randomSeed + 1);
            else if (levelNum == 3) level2 = std::make_unique<Level3>(randomSeed + 1);
            else level2 = std::make_unique<Level4>(randomSeed + 1);
            board2->setLevel(level2.get());
        }
    }
};
