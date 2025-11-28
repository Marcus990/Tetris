module game;
import <memory>;
import <string>;
import <vector>;
import <iostream>;
import board;
import block;
import level;
import scorekeeper;
import effect;
import textdisplay;
import graphicsdisplay;
import constants;

using namespace GameConstants;

Game::Game(unsigned int seed, int level,
     const std::string& script1,
     const std::string& script2,
     bool textMode)
    : currentPlayer(PLAYER_ONE), isRunning(true), textOnly(textMode),
      shouldStopExecution(false), randomSeed(seed), scriptFile1(script1), scriptFile2(script2),
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
    spawnNextBlock(board1.get());
    spawnNextBlock(board2.get());
    spawnNextBlock(board2.get());
}

void Game::createLevels(int levelNum) {
    if (levelNum == 0) {
        level1 = std::make_unique<Level0>(scriptFile1);
        level2 = std::make_unique<Level0>(scriptFile2);
    }
    else if (levelNum == 1) {
        level1 = std::make_unique<Level1>(randomSeed);
        level2 = std::make_unique<Level1>(randomSeed + 1);
    }
    else if (levelNum == 2) {
        level1 = std::make_unique<Level2>(randomSeed);
        level2 = std::make_unique<Level2>(randomSeed + 1);
    }
    else if (levelNum == 3) {
        level1 = std::make_unique<Level3>(randomSeed);
        level2 = std::make_unique<Level3>(randomSeed + 1);
    }
    else {
        level1 = std::make_unique<Level4>(randomSeed);
        level2 = std::make_unique<Level4>(randomSeed + 1);
    }

    board1->setLevel(level1.get());
    board2->setLevel(level2.get());
}

Board* Game::getCurrentBoard() {
    return currentPlayer == PLAYER_ONE ? board1.get() : board2.get();
}

Board* Game::getOpponentBoard() {
    return currentPlayer == PLAYER_ONE ? board2.get() : board1.get();
}

Level* Game::getCurrentLevel() {
    return currentPlayer == PLAYER_ONE ? level1.get() : level2.get();
}

ScoreKeeper* Game::getCurrentScore() {
    return currentPlayer == PLAYER_ONE ? score1.get() : score2.get();
}

void Game::spawnNextBlock(Board* board) {
    Level* level = (board == board1.get()) ? level1.get() : level2.get();

    if (!board->getNextBlock()) {
        // First block - create it
        std::unique_ptr<Block> block = level->generateBlock(board->getNextBlockId());
        board->setNextBlock(std::move(block));
    }
    else {
        // Move next to current, generate new next
        board->setCurrentBlock(board->takeNextBlock());
        std::unique_ptr<Block> newNext = level->generateBlock(board->getNextBlockId());
        board->setNextBlock(std::move(newNext));
    }
}

void Game::switchPlayer() {
    currentPlayer = (currentPlayer == PLAYER_ONE) ? PLAYER_TWO : PLAYER_ONE;
}

bool Game::drop() {
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
            std::string action;
            bool validAction = false;

            while (!validAction) {
                std::cout << "Special Action! Choose one: blind, heavy, force\n";
                std::cin >> action;

                if (action == "blind" || action == "heavy" || action == "force") {
                    validAction = true;
                } else {
                    std::cout << "Invalid action. Please choose: blind, heavy, or force\n";
                }
            }

            // If force, ask for block type
            if (action == "force") {
                std::string blockType;
                bool validBlock = false;

                while (!validBlock) {
                    std::cout << "Choose block type (I, J, L, O, S, Z, T): ";
                    std::cin >> blockType;

                    if (!blockType.empty() &&
                        (blockType[0] == 'I' || blockType[0] == 'J' || blockType[0] == 'L' ||
                         blockType[0] == 'O' || blockType[0] == 'S' || blockType[0] == 'Z' ||
                         blockType[0] == 'T')) {
                        validBlock = true;
                        applySpecialAction(action, blockType[0]);
                    } else {
                        std::cout << "Invalid block type. Please choose: I, J, L, O, S, Z, or T\n";
                    }
                }
            }
            else {
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
    Level* level = getCurrentLevel();
    if (level->getLevelNumber() == MAX_LEVEL) {
        if (linesCleared > 0) {
            // Reset counter if rows were cleared
            board->resetBlocksSinceLastClear();
        }
        else {
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
    
    if (!textOnly)
        graphicsDisplay1->setBlindMode(isBlind1);

    bool isBlind2 = board2->hasBlindEffect();
    textDisplay2->setBlindMode(isBlind2);

    if (!textOnly)
        graphicsDisplay2->setBlindMode(isBlind2);

    // Beep sound
    std::cout << '\a';
    
    // Check game over
    if (board->isGameOver()) {
        // Determine winner (the player who didn't lose)
        ScoreKeeper* winnerScore = (currentPlayer == PLAYER_ONE) ? score2.get() : score1.get();

        // Increment winner's win count
        winnerScore->incrementWins();

        // Set flag to stop executing remaining multiplied commands
        shouldStopExecution = true;

        // Automatically restart the game
        restart();
        return true;
    }

    // Spawn next block
    spawnNextBlock(board);

    return true;
}

void Game::render() {
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

    // Header
    std::cout << BOLD << CYAN;
    std::cout << "╔═════════════════════════════════════════════════╗\n";
    std::cout << "║              " << YELLOW << "✦ B I Q U A D R I S ✦" << CYAN << "              ║\n";
    std::cout << "╠════════════════════════╦════════════════════════╣\n";
    std::cout << RESET;

    // Player headers with current player highlight
    std::cout << BOLD;
    if (currentPlayer == PLAYER_ONE) {
        std::cout << BG_BLUE << WHITE << "║     ► PLAYER 1 ◄       " << RESET << BOLD << CYAN << "║" << RESET;
        std::cout << BOLD << CYAN << "       PLAYER 2         ║\n" << RESET;
    }
    else {
        std::cout << CYAN << "║       PLAYER 1         ║" << RESET;
        std::cout << BOLD << BG_BLUE << WHITE << "     ► PLAYER 2 ◄       " << RESET << BOLD << CYAN << "║\n" << RESET;
    }

    std::cout << BOLD << CYAN << "╠════════════════════════╬════════════════════════╣\n" << RESET;

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
    std::cout << BOLD << CYAN << "║\n" << RESET;

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
    std::cout << BOLD << CYAN << "║\n" << RESET;

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
    std::cout << BOLD << CYAN << "║\n" << RESET;

    std::cout << BOLD << CYAN << "║" << RESET;
    std::cout << BLUE << " Wins:  " << BOLD << WHITE << score1->getWins() << RESET;
    std::string winsStr1 = std::to_string(score1->getWins());
    for (size_t i = winsStr1.length(); i < 16; ++i)
        std::cout << " ";
    std::cout << BOLD << CYAN << "║" << RESET;
    std::cout << BLUE << " Wins:  " << BOLD << WHITE << score2->getWins() << RESET;
    std::string winsStr2 = std::to_string(score2->getWins());
    for (size_t i = winsStr2.length(); i < 16; ++i)
        std::cout << " ";
    std::cout << BOLD << CYAN << "║\n" << RESET;

    std::cout << BOLD << CYAN << "╠════════════════════════╬════════════════════════╣\n" << RESET;

    // Print boards side by side using TextDisplay methods
    for (int row = 0; row < TOTAL_ROWS; ++row) {
        std::cout << BOLD << CYAN << "║ " << RESET;
        std::cout << textDisplay1->renderBoardRow(row);
        std::cout << BOLD << CYAN << " ║ " << RESET;
        std::cout << textDisplay2->renderBoardRow(row);
        std::cout << BOLD << CYAN << " ║" << RESET << "\n";
    }

    std::cout << BOLD << CYAN << "╠════════════════════════╬════════════════════════╣\n" << RESET;

    // Print next blocks using TextDisplay methods
    std::cout << BOLD << CYAN << "║" << YELLOW << " Next:                  " << CYAN << "║" << YELLOW << " Next:                  " << CYAN << "║\n" << RESET;

    auto nextPreview1 = textDisplay1->renderNextBlockPreview();
    auto nextPreview2 = textDisplay2->renderNextBlockPreview();

    for (int row = 0; row < 3; row++) {
        std::cout << BOLD << CYAN << "║ " << RESET;
        std::cout << nextPreview1[row];
        std::cout << "               " << BOLD << CYAN << "║ " << RESET;
        std::cout << nextPreview2[row];
        std::cout << "               " << BOLD << CYAN << "║\n" << RESET;
    }

    std::cout << BOLD << CYAN << "╚════════════════════════╩════════════════════════╝\n" << RESET;

    // Command prompt
    std::cout << "\n" << BOLD << WHITE << "Enter command: > " << RESET;

    if (!textOnly) {
        // Set game info on graphics displays before notifying
        graphicsDisplay1->setGameInfo(
            level1->getLevelNumber(),
            score1->getCurrentScore(),
            score1->getHighScore()
        );
        graphicsDisplay2->setGameInfo(
            level2->getLevelNumber(),
            score2->getCurrentScore(),
            score2->getHighScore()
        );

        // Notify observers to trigger rendering
        board1->notifyObservers();
        board2->notifyObservers();
    }
}

void Game::restart() {
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

bool Game::isGameRunning() const { return isRunning; }

void Game::applySpecialAction(const std::string& action, char blockType) {
    Board* opponent = getOpponentBoard();
    int opponentNum = (currentPlayer == PLAYER_ONE) ? 2 : 1;

    if (action == "blind") {
        auto effect = new BlindEffect(1);
        opponent->addEffect(effect);
        std::cout << "Blind effect activated on Player " << opponentNum << "!\n";
        // Blind mode will be synchronized automatically in drop()
    }
    else if (action == "heavy") {
        auto effect = new HeavyEffect();
        opponent->addEffect(effect);
        std::cout << "Heavy effect activated on Player " << opponentNum << "!\n";
    }
    else if (action == "force") {
        // Force effect replaces opponent's current block immediately
        opponent->replaceCurrentBlock(blockType);
        std::cout << "Force effect activated on Player " << opponentNum << "! Block type: " << blockType << "\n";
    }
}

void Game::levelUp() {
    int currentLevelNum = getCurrentLevel()->getLevelNumber();
    if (currentLevelNum < MAX_LEVEL) {
        if (currentPlayer == PLAYER_ONE) {
            createPlayerLevel(1, currentLevelNum + 1);
        }
        else {
            createPlayerLevel(2, currentLevelNum + 1);
        }
    }
}

void Game::levelDown() {
    int currentLevelNum = getCurrentLevel()->getLevelNumber();
    if (currentLevelNum > MIN_LEVEL) {
        if (currentPlayer == PLAYER_ONE) {
            createPlayerLevel(1, currentLevelNum - 1);
        }
        else {
            createPlayerLevel(2, currentLevelNum - 1);
        }
    }
}

void Game::createPlayerLevel(int player, int levelNum) {
    if (player == 1) {
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
    else {
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

bool Game::shouldStopExecutingCommands() const {
    return shouldStopExecution;
}

void Game::clearStopExecutionFlag() {
    shouldStopExecution = false;
}
