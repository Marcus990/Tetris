module board;
import <vector>;
import <map>;
import <memory>;
import <algorithm>;
import cell;
import block;
import blocks;
import constants;
import observer;
import scorekeeper;
import level;
import effect;

using namespace GameConstants;

Board::Board() : level(nullptr), score(nullptr), nextBlockId(0), blocksSinceLastClear(0),
          blindActive(false), heavyCount(0) {
    // Initialize grid (18 rows x 11 cols based on constants)
    grid.resize(TOTAL_ROWS, std::vector<Cell>(BOARD_WIDTH));
}

void Board::attach(IObserver* observer) {
    displays.push_back(observer);
}

void Board::detach(IObserver* observer) {
    displays.erase(
        std::remove(displays.begin(), displays.end(), observer),
        displays.end()
    );
}

void Board::notifyObservers() {
    for (auto* observer : displays) {
        observer->update();
    }
}

void Board::setLevel(Level* l) { level = l; }

void Board::setScoreKeeper(ScoreKeeper* s) { score = s; }

const std::vector<std::vector<Cell>>& Board::getGrid() const { return grid; }

Cell& Board::getCell(int row, int col) { return grid[row][col]; }

const Cell& Board::getCell(int row, int col) const { return grid[row][col]; }

Block* Board::getCurrentBlock() { return currentBlock.get(); }

const Block* Board::getCurrentBlock() const { return currentBlock.get(); }

Block* Board::getNextBlock() { return nextBlock.get(); }

void Board::setCurrentBlock(std::unique_ptr<Block> block) {
    currentBlock = std::move(block);
}

void Board::setNextBlock(std::unique_ptr<Block> block) {
    nextBlock = std::move(block);
}

std::unique_ptr<Block> Board::takeNextBlock() {
    return std::move(nextBlock);
}

bool Board::isValidPosition(const Block* block) const {
    if (!block) return false;

    auto cells = block->getAbsoluteCells();
    for (const auto& cell : cells) {
        int row = cell.first;
        int col = cell.second;

        // Check boundaries
        if (row < 0 || row >= TOTAL_ROWS || col < 0 || col >= BOARD_WIDTH) {
            return false;
        }

        // Check if cell is already occupied by a different block
        if (grid[row][col].isFilled() &&
            grid[row][col].getBlockId() != block->getBlockId()) {
            return false;
        }
    }
    return true;
}

bool Board::moveLeft() {
    if (!currentBlock) return false;

    currentBlock->move(-1, 0);
    if (!isValidPosition(currentBlock.get())) {
        currentBlock->move(1, 0);  // Revert
        return false;
    }
    return true;
}

bool Board::moveRight() {
    if (!currentBlock) return false;

    currentBlock->move(1, 0);
    if (!isValidPosition(currentBlock.get())) {
        currentBlock->move(-1, 0);  // Revert
        return false;
    }
    return true;
}

bool Board::moveDown() {
    if (!currentBlock) return false;

    currentBlock->move(0, 1);
    if (!isValidPosition(currentBlock.get())) {
        currentBlock->move(0, -1);  // Revert
        return false;
    }
    return true;
}

bool Board::rotate(bool clockwise) {
    if (!currentBlock) return false;

    if (clockwise) {
        currentBlock->rotateClockwise();
    } else {
        currentBlock->rotateCounterClockwise();
    }

    if (!isValidPosition(currentBlock.get())) {
        // Revert rotation
        if (clockwise) {
            currentBlock->rotateCounterClockwise();
        } else {
            currentBlock->rotateClockwise();
        }
        return false;
    }
    return true;
}

void Board::lockBlock() {
    if (!currentBlock) return;

    auto cells = currentBlock->getAbsoluteCells();
    for (const auto& cell : cells) {
        int row = cell.first;
        int col = cell.second;
        grid[row][col].setFilled(true);
        grid[row][col].setType(currentBlock->getType());
        grid[row][col].setBlockId(currentBlock->getBlockId());
    }

    // Store in active blocks
    activeBlocks[currentBlock->getBlockId()] = std::move(currentBlock);
}

void Board::drop() {
    if (!currentBlock) return;

    while (moveDown()) {
        // Keep moving down
    }
    lockBlock();
}

void Board::dropCenterBlock(std::unique_ptr<Block> block) {
    if (!block) return;

    int centerCol = BOARD_WIDTH / 2;  // Column 5 for 11-wide board

    // Start from the top and drop down until we hit something
    int dropRow = RESERVE_ROWS;  // Start at top of visible area

    // Find where the block should land (first filled cell from top)
    for (int row = RESERVE_ROWS; row < TOTAL_ROWS; ++row) {
        if (grid[row][centerCol].isFilled()) {
            // Land on top of this filled cell
            dropRow = row - 1;
            break;
        }
        dropRow = row;  // Can fall to this row
    }

    // Set block position to where it landed
    block->setPosition(centerCol, dropRow);

    // Place all cells of the block (should be just 1 cell for SingleBlock)
    for (const auto& [relRow, relCol] : block->getCells()) {
        int absRow = block->getY() + relRow;
        int absCol = block->getX() + relCol;

        if (absRow >= 0 && absRow < TOTAL_ROWS &&
            absCol >= 0 && absCol < BOARD_WIDTH) {
            grid[absRow][absCol].setFilled(true);
            grid[absRow][absCol].setType(block->getType());
            grid[absRow][absCol].setBlockId(block->getBlockId());
        }
    }

    // Store in active blocks
    activeBlocks[block->getBlockId()] = std::move(block);
}

int Board::clearRows() {
    int cleared = 0;

    for (int row = TOTAL_ROWS - 1; row >= 0; --row) {
        bool full = true;
        for (int col = 0; col < BOARD_WIDTH; ++col) {
            if (!grid[row][col].isFilled()) {
                full = false;
                break;
            }
        }

        if (full) {
            // Remove this row
            grid.erase(grid.begin() + row);
            // Add new empty row at top
            grid.insert(grid.begin(), std::vector<Cell>(BOARD_WIDTH));
            cleared++;
            row++;  // Check same row again since rows shifted
        }
    }

    return cleared;
}

void Board::checkBlockRemoval() {
    std::vector<int> blocksToRemove;

    for (auto& [blockId, block] : activeBlocks) {
        bool stillExists = false;
        for (int row = 0; row < TOTAL_ROWS; ++row) {
            for (int col = 0; col < BOARD_WIDTH; ++col) {
                if (grid[row][col].getBlockId() == blockId) {
                    stillExists = true;
                    break;
                }
            }
            if (stillExists) break;
        }

        if (!stillExists) {
            blocksToRemove.push_back(blockId);
        }
    }

    // Remove blocks and award points
    for (int blockId : blocksToRemove) {
        if (score && activeBlocks.count(blockId)) {
            int levelGen = activeBlocks[blockId]->getLevelGenerated();
            int points = score->calculateBlockRemovalPoints(levelGen);
            score->addScore(points);
        }
        activeBlocks.erase(blockId);
    }
}

bool Board::isGameOver() const {

    for (const auto& [posX, posY] : nextBlock->getAbsoluteCells()) {
        if (grid[posX][posY].isFilled()) {
            return true;
        }
    }
    return false;
}

bool Board::replaceCurrentBlock(char type) {
    if (!level) return false;

    // Create new block of the specified type
    int blockId = getNextBlockId();
    std::unique_ptr<Block> newBlock = level->createBlockFromType(type, blockId);

    if (!newBlock) return false;

    // Replace current block
    currentBlock = std::move(newBlock);
    return true;
}

void Board::setBlindActive(bool active) { blindActive = active; }

void Board::incrementHeavy() { heavyCount++; }

void Board::decrementHeavy() { if (heavyCount > 0) heavyCount--; }

bool Board::hasBlindEffect() const { return blindActive; }

bool Board::hasHeavyEffect() const { return heavyCount > 0; }

void Board::addEffect(Effect* effect) {
    activeEffects.push_back(effect);

    // Apply effect immediately based on type
    auto effectType = effect->getType();

    if (effectType == Effect::Type::Blind) {
        setBlindActive(true);
    } else if (effectType == Effect::Type::Heavy) {
        incrementHeavy();
    }
    // Force is not handled as an ongoing effect - it immediately replaces current block
}

void Board::updateEffects() {
    for (auto* effect : activeEffects) {
        effect->update();
    }

    // Remove expired effects
    auto it = activeEffects.begin();
    while (it != activeEffects.end()) {
        if ((*it)->isExpired()) {
            // Unapply effect before deletion
            auto effectType = (*it)->getType();

            if (effectType == Effect::Type::Blind) {
                setBlindActive(false);
            } else if (effectType == Effect::Type::Heavy) {
                decrementHeavy();
            }
            // Force is not handled as an ongoing effect

            delete *it;
            it = activeEffects.erase(it);
        } else {
            ++it;
        }
    }
}

int Board::getNextBlockId() { return nextBlockId++; }

int Board::getBlocksSinceLastClear() const { return blocksSinceLastClear; }

void Board::incrementBlocksSinceLastClear() { blocksSinceLastClear++; }

void Board::resetBlocksSinceLastClear() { blocksSinceLastClear = 0; }
