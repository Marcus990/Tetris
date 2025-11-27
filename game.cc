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

    int currentPlayer; // 0 or 1
    bool isRunning;
    bool textOnly;
    unsigned int randomSeed;
    std::string scriptFile1;
    std::string scriptFile2;
    int startLevel;

public:
    Game(unsigned int seed = 0, int level = 0,
         const std::string &script1 = "sequence1.txt",
         const std::string &script2 = "sequence2.txt",
         bool textMode = false)
        : currentPlayer(PLAYER_ONE), isRunning(true), textOnly(textMode),
          randomSeed(seed), scriptFile1(script1), scriptFile2(script2),
          startLevel(level)
    {

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

        if (!textOnly)
        {
            graphicsDisplay1 = std::make_unique<GraphicsDisplay>(board1.get());
            graphicsDisplay2 = std::make_unique<GraphicsDisplay>(board2.get());

            board1->attach(graphicsDisplay1.get());
            board2->attach(graphicsDisplay2.get());
        }

        // Spawn initial blocks
        spawnNextBlock(board1.get());
        spawnNextBlock(board1.get()); // Current and next
        spawnNextBlock(board2.get());
        spawnNextBlock(board2.get());
    }

    void createLevels(int levelNum)
    {
        if (levelNum == 0)
        {
            level1 = std::make_unique<Level0>(scriptFile1);
            level2 = std::make_unique<Level0>(scriptFile2);
        }
        else if (levelNum == 1)
        {
            level1 = std::make_unique<Level1>(randomSeed);
            level2 = std::make_unique<Level1>(randomSeed + 1);
        }
        else if (levelNum == 2)
        {
            level1 = std::make_unique<Level2>(randomSeed);
            level2 = std::make_unique<Level2>(randomSeed + 1);
        }
        else if (levelNum == 3)
        {
            level1 = std::make_unique<Level3>(randomSeed);
            level2 = std::make_unique<Level3>(randomSeed + 1);
        }
        else
        {
            level1 = std::make_unique<Level4>(randomSeed);
            level2 = std::make_unique<Level4>(randomSeed + 1);
        }

        board1->setLevel(level1.get());
        board2->setLevel(level2.get());
    }

    Board *getCurrentBoard()
    {
        return currentPlayer == PLAYER_ONE ? board1.get() : board2.get();
    }

    Board *getOpponentBoard()
    {
        return currentPlayer == PLAYER_ONE ? board2.get() : board1.get();
    }

    Level *getCurrentLevel()
    {
        return currentPlayer == PLAYER_ONE ? level1.get() : level2.get();
    }

    ScoreKeeper *getCurrentScore()
    {
        return currentPlayer == PLAYER_ONE ? score1.get() : score2.get();
    }

    void spawnNextBlock(Board *board)
    {
        Level *level = (board == board1.get()) ? level1.get() : level2.get();

        if (!board->getNextBlock())
        {
            // First block - create it
            std::unique_ptr<Block> block = level->generateBlock(board->getNextBlockId());
            board->setNextBlock(std::move(block));
        }
        else
        {
            // Move next to current, generate new next
            board->setCurrentBlock(board->takeNextBlock());
            std::unique_ptr<Block> newNext = level->generateBlock(board->getNextBlockId());
            board->setNextBlock(std::move(newNext));
        }
    }

    void switchPlayer()
    {
        currentPlayer = (currentPlayer == PLAYER_ONE) ? PLAYER_TWO : PLAYER_ONE;
    }

    bool drop()
    {
        Board *board = getCurrentBoard();
        board->drop();

        // Clear full rows
        int linesCleared = board->clearRows();

        // Award points for line clears
        if (linesCleared > 0)
        {
            ScoreKeeper *score = getCurrentScore();
            int points = score->calculateLineClearPoints(
                getCurrentLevel()->getLevelNumber(), linesCleared);
            score->addScore(points);

            // Render board after clearing rows so player can see what happened
            render();

            // Check for special action (2+ lines cleared)
            if (linesCleared >= ROWS_FOR_SPECIAL_ACTION)
            {
                std::cout << "Special Action! Choose one: blind, heavy, force\n";
                std::string action;
                std::cin >> action;

                // If force, ask for block type
                if (action == "force")
                {
                    std::cout << "Choose block type (I, J, L, O, S, Z, T): ";
                    std::string blockType;
                    std::cin >> blockType;
                    if (!blockType.empty())
                    {
                        applySpecialAction(action, blockType[0]);
                    }
                }
                else
                {
                    applySpecialAction(action, '\0');
                }

                // Immediately sync displays after applying special action
                bool isBlind1 = board1->hasBlindEffect();
                bool isBlind2 = board2->hasBlindEffect();

                textDisplay1->setBlindMode(isBlind1);
                if (!textOnly)
                    graphicsDisplay1->setBlindMode(isBlind1);

                textDisplay2->setBlindMode(isBlind2);
                if (!textOnly)
                    graphicsDisplay2->setBlindMode(isBlind2);
            }
        }

        // Level 4 feature: Track blocks placed without clearing
        Level *level = getCurrentLevel();
        if (level->getLevelNumber() == MAX_LEVEL)
        {
            if (linesCleared > 0)
            {
                // Reset counter if rows were cleared
                board->resetBlocksSinceLastClear();
            }
            else
            {
                // Increment counter and check if we need to drop center block
                board->incrementBlocksSinceLastClear();
                if (board->getBlocksSinceLastClear() >= BLOCKS_BEFORE_CENTER_DROP)
                {
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
        if (!textOnly)
            graphicsDisplay1->setBlindMode(isBlind1);

        bool isBlind2 = board2->hasBlindEffect();
        textDisplay2->setBlindMode(isBlind2);
        if (!textOnly)
            graphicsDisplay2->setBlindMode(isBlind2);

        // Check game over
        if (board->isGameOver())
        {
            isRunning = false;
            return false;
        }

        // Spawn next block
        spawnNextBlock(board);

        // Switch to other player after drop
        switchPlayer();

        return true;
    }

    void render()
    {
        // ANSI color codes
        const std::string RESET = "\033[0m";
        const std::string BOLD = "\033[1m";
        const std::string RED = "\033[31m";
        const std::string GREEN = "\033[32m";
        const std::string YELLOW = "\033[33m";
        const std::string BLUE = "\033[34m";
        const std::string MAGENTA = "\033[35m";
        const std::string CYAN = "\033[36m";
        const std::string WHITE = "\033[37m";
        const std::string BG_BLUE = "\033[44m";
        const std::string BG_RED = "\033[41m";

        // Clear screen for cleaner display
        std::cout << "\033[2J\033[H";

        // Board is 11 columns wide, each cell is 2 chars = 22 chars per board
        // Plus 1 space padding on each side = 24 chars per board section

        // Header
        std::cout << BOLD << CYAN;
        std::cout << "╔═════════════════════════════════════════════════╗\n";
        std::cout << "║              " << YELLOW << "✦ B I Q U A D R I S ✦" << CYAN << "              ║\n";
        std::cout << "╠════════════════════════╦════════════════════════╣\n";
        std::cout << RESET;

        // Player headers with current player highlight
        std::cout << BOLD;
        if (currentPlayer == PLAYER_ONE)
        {
            std::cout << BG_BLUE << WHITE << "║     ► PLAYER 1 ◄       " << RESET << BOLD << CYAN << "║" << RESET;
            std::cout << BOLD << CYAN << "       PLAYER 2         ║\n"
                      << RESET;
        }
        else
        {
            std::cout << CYAN << "║       PLAYER 1         ║" << RESET;
            std::cout << BOLD << BG_BLUE << WHITE << "     ► PLAYER 2 ◄       " << RESET << BOLD << CYAN << "║\n"
                      << RESET;
        }

        std::cout << BOLD << CYAN << "╠════════════════════════╬════════════════════════╣\n"
                  << RESET;

        // Stats - pad to 24 chars per section
        std::cout << BOLD << CYAN << "║" << RESET;
        std::cout << YELLOW << " Level: " << BOLD << WHITE << level1->getLevelNumber() << RESET;
        std::string lvlStr1 = std::to_string(level1->getLevelNumber());
        for (size_t i = lvlStr1.length(); i < 16; ++i)
            std::cout << " ";
        std::cout << BOLD << CYAN << "║" << RESET;
        std::cout << YELLOW << " Level: " << BOLD << WHITE << level2->getLevelNumber() << RESET;
        std::string lvlStr2 = std::to_string(level2->getLevelNumber());
        for (size_t i = lvlStr2.length(); i < 16; ++i)
            std::cout << " ";
        std::cout << BOLD << CYAN << "║\n"
                  << RESET;

        std::cout << BOLD << CYAN << "║" << RESET;
        std::cout << GREEN << " Score: " << BOLD << WHITE << score1->getCurrentScore() << RESET;
        std::string scoreStr1 = std::to_string(score1->getCurrentScore());
        for (size_t i = scoreStr1.length(); i < 16; ++i)
            std::cout << " ";
        std::cout << BOLD << CYAN << "║" << RESET;
        std::cout << GREEN << " Score: " << BOLD << WHITE << score2->getCurrentScore() << RESET;
        std::string scoreStr2 = std::to_string(score2->getCurrentScore());
        for (size_t i = scoreStr2.length(); i < 16; ++i)
            std::cout << " ";
        std::cout << BOLD << CYAN << "║\n"
                  << RESET;

        std::cout << BOLD << CYAN << "║" << RESET;
        std::cout << MAGENTA << " High:  " << BOLD << WHITE << score1->getHighScore() << RESET;
        std::string highStr1 = std::to_string(score1->getHighScore());
        for (size_t i = highStr1.length(); i < 16; ++i)
            std::cout << " ";
        std::cout << BOLD << CYAN << "║" << RESET;
        std::cout << MAGENTA << " High:  " << BOLD << WHITE << score2->getHighScore() << RESET;
        std::string highStr2 = std::to_string(score2->getHighScore());
        for (size_t i = highStr2.length(); i < 16; ++i)
            std::cout << " ";
        std::cout << BOLD << CYAN << "║\n"
                  << RESET;

        std::cout << BOLD << CYAN << "╠════════════════════════╬════════════════════════╣\n"
                  << RESET;

        // Get both board grids
        const auto &grid1 = board1->getGrid();
        const auto &grid2 = board2->getGrid();

        // Get current blocks
        Block *current1 = board1->getCurrentBlock();
        Block *current2 = board2->getCurrentBlock();

        // Create display grids with current blocks overlaid
        auto display1 = grid1;
        auto display2 = grid2;

        if (current1)
        {
            auto cells = current1->getAbsoluteCells();
            for (const auto &cell : cells)
            {
                int row = cell.first;
                int col = cell.second;
                if (row >= 0 && row < TOTAL_ROWS && col >= 0 && col < BOARD_WIDTH)
                {
                    display1[row][col].setType(current1->getType());
                    display1[row][col].setFilled(true);
                }
            }
        }

        if (current2)
        {
            auto cells = current2->getAbsoluteCells();
            for (const auto &cell : cells)
            {
                int row = cell.first;
                int col = cell.second;
                if (row >= 0 && row < TOTAL_ROWS && col >= 0 && col < BOARD_WIDTH)
                {
                    display2[row][col].setType(current2->getType());
                    display2[row][col].setFilled(true);
                }
            }
        }

        // Check blind mode for both boards
        bool isBlind1 = board1->hasBlindEffect();
        bool isBlind2 = board2->hasBlindEffect();

        // Helper lambda to get block color
        auto getBlockColor = [&](char type) -> std::string
        {
            switch (type)
            {
            case 'I':
                return CYAN;
            case 'J':
                return BLUE;
            case 'L':
                return "\033[38;5;208m"; // Orange
            case 'O':
                return YELLOW;
            case 'S':
                return GREEN;
            case 'Z':
                return RED;
            case 'T':
                return MAGENTA;
            case '*':
                return WHITE;
            default:
                return RESET;
            }
        };

        // Print boards side by side (including reserve rows - all 18 rows)
        // Each cell is 2 chars, 11 columns = 22 chars, plus 1 space padding each side = 24
        for (int row = 0; row < TOTAL_ROWS; ++row)
        {
            std::cout << BOLD << CYAN << "║ " << RESET;

            // Print board 1 row (11 columns × 2 chars = 22 chars)
            for (int col = 0; col < BOARD_WIDTH; ++col)
            {
                if (isBlind1 &&
                    row >= RESERVE_ROWS + BLIND_ROW_START &&
                    row <= RESERVE_ROWS + BLIND_ROW_END &&
                    col >= BLIND_COL_START && col <= BLIND_COL_END)
                {
                    std::cout << BOLD << RED << "? " << RESET;
                }
                else if (display1[row][col].isFilled())
                {
                    char type = display1[row][col].getType();
                    std::cout << BOLD << getBlockColor(type) << "█ " << RESET;
                }
                else
                {
                    std::cout << "· ";
                }
            }

            std::cout << BOLD << CYAN << " ║ " << RESET;

            // Print board 2 row
            for (int col = 0; col < BOARD_WIDTH; ++col)
            {
                if (isBlind2 &&
                    row >= RESERVE_ROWS + BLIND_ROW_START &&
                    row <= RESERVE_ROWS + BLIND_ROW_END &&
                    col >= BLIND_COL_START && col <= BLIND_COL_END)
                {
                    std::cout << BOLD << RED << "? " << RESET;
                }
                else if (display2[row][col].isFilled())
                {
                    char type = display2[row][col].getType();
                    std::cout << BOLD << getBlockColor(type) << "█ " << RESET;
                }
                else
                {
                    std::cout << "· ";
                }
            }

            std::cout << BOLD << CYAN << " ║" << RESET << "\n";
        }

        std::cout << BOLD << CYAN << "╠════════════════════════╬════════════════════════╣\n"
                  << RESET;

        // Print next blocks
        std::cout << BOLD << CYAN << "║" << YELLOW << " Next:                  " << CYAN << "║" << YELLOW << " Next:                  " << CYAN << "║\n"
                  << RESET;

        Block *next1 = board1->getNextBlock();
        Block *next2 = board2->getNextBlock();

        std::vector<std::string> g1(3, std::string(4, ' '));
        std::vector<std::string> g2(3, std::string(4, ' '));

        if (next1)
        {
            for (auto &cell : next1->getCells())
            {
                int r = cell.first;
                int c = cell.second;
                if (r >= 0 && r < 3 && c >= 0 && c < 4)
                    g1[r][c] = next1->getType();
            }
        }

        if (next2)
        {
            for (auto &cell : next2->getCells())
            {
                int r = cell.first;
                int c = cell.second;
                if (r >= 0 && r < 3 && c >= 0 && c < 4)
                    g2[r][c] = next2->getType();
            }
        }

        // Print next blocks side-by-side (pad to 24 chars per section)
        for (int row = 0; row < 3; row++)
        {
            std::cout << BOLD << CYAN << "║ " << RESET;
            for (int col = 0; col < 4; col++)
            {
                if (g1[row][col] != ' ')
                {
                    std::cout << BOLD << getBlockColor(g1[row][col]) << "█ " << RESET;
                }
                else
                {
                    std::cout << "  ";
                }
            }
            std::cout << "               " << BOLD << CYAN << "║ " << RESET;
            for (int col = 0; col < 4; col++)
            {
                if (g2[row][col] != ' ')
                {
                    std::cout << BOLD << getBlockColor(g2[row][col]) << "█ " << RESET;
                }
                else
                {
                    std::cout << "  ";
                }
            }
            std::cout << "               " << BOLD << CYAN << "║\n"
                      << RESET;
        }

        std::cout << BOLD << CYAN << "╚════════════════════════╩════════════════════════╝\n"
                  << RESET;

        // Command prompt
        std::cout << "\n"
                  << BOLD << WHITE << "Enter command: > " << RESET;
    }

    void restart()
    {
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

        if (!textOnly)
        {
            graphicsDisplay1->setBoard(board1.get());
            graphicsDisplay2->setBoard(board2.get());
        }

        // Reattach displays to new boards
        board1->attach(textDisplay1.get());
        board2->attach(textDisplay2.get());

        if (!textOnly)
        {
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
    void applySpecialAction(const std::string &action, char blockType = 'I')
    {
        Board *opponent = getOpponentBoard();

        if (action == "blind")
        {
            auto effect = new BlindEffect(1);
            opponent->addEffect(effect);
            // Blind mode will be synchronized automatically in drop()
        }
        else if (action == "heavy")
        {
            auto effect = new HeavyEffect();
            opponent->addEffect(effect);
        }
        else if (action == "force")
        {
            // Force effect replaces opponent's current block immediately
            opponent->replaceCurrentBlock(blockType);
        }
    }

    // Level management
    void levelUp()
    {
        int currentLevelNum = getCurrentLevel()->getLevelNumber();
        if (currentLevelNum < MAX_LEVEL)
        {
            if (currentPlayer == PLAYER_ONE)
            {
                createPlayerLevel(1, currentLevelNum + 1);
            }
            else
            {
                createPlayerLevel(2, currentLevelNum + 1);
            }
        }
    }

    void levelDown()
    {
        int currentLevelNum = getCurrentLevel()->getLevelNumber();
        if (currentLevelNum > MIN_LEVEL)
        {
            if (currentPlayer == PLAYER_ONE)
            {
                createPlayerLevel(1, currentLevelNum - 1);
            }
            else
            {
                createPlayerLevel(2, currentLevelNum - 1);
            }
        }
    }

    void createPlayerLevel(int player, int levelNum)
    {
        if (player == 1)
        {
            if (levelNum == 0)
                level1 = std::make_unique<Level0>(scriptFile1);
            else if (levelNum == 1)
                level1 = std::make_unique<Level1>(randomSeed);
            else if (levelNum == 2)
                level1 = std::make_unique<Level2>(randomSeed);
            else if (levelNum == 3)
                level1 = std::make_unique<Level3>(randomSeed);
            else
                level1 = std::make_unique<Level4>(randomSeed);
            board1->setLevel(level1.get());
        }
        else
        {
            if (levelNum == 0)
                level2 = std::make_unique<Level0>(scriptFile2);
            else if (levelNum == 1)
                level2 = std::make_unique<Level1>(randomSeed + 1);
            else if (levelNum == 2)
                level2 = std::make_unique<Level2>(randomSeed + 1);
            else if (levelNum == 3)
                level2 = std::make_unique<Level3>(randomSeed + 1);
            else
                level2 = std::make_unique<Level4>(randomSeed + 1);
            board2->setLevel(level2.get());
        }
    }
};
