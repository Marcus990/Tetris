module;
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
export module board;
import cell;
import block;
import blocks;
import constants;
import observer;
import scorekeeper;
import level;
import effect;  // Import to get the real Effect type

using namespace GameConstants;

export class Board {
    std::vector<std::vector<Cell>> grid;
    std::unique_ptr<Block> currentBlock;
    std::unique_ptr<Block> nextBlock;
    Level* level;
    ScoreKeeper* score;
    std::vector<IObserver*> displays;
    std::vector<Effect*> activeEffects;
    std::map<int, std::unique_ptr<Block>> activeBlocks;
    int nextBlockId;
    int blocksSinceLastClear;  // For Level 4 center block penalty

public:
    Board() : level(nullptr), score(nullptr), nextBlockId(0), blocksSinceLastClear(0) {
        // Initialize grid (18 rows x 11 cols based on constants)
        grid.resize(TOTAL_ROWS, std::vector<Cell>(BOARD_WIDTH));
    }

    // Observer pattern methods
    void attach(IObserver* observer) {
        displays.push_back(observer);
    }

    void detach(IObserver* observer) {
        displays.erase(
            std::remove(displays.begin(), displays.end(), observer),
            displays.end()
        );
    }

    void notifyObservers() {
        for (auto* observer : displays) {
            observer->update();
        }
    }

    // Setters for dependencies
    void setLevel(Level* l) { level = l; }
    void setScoreKeeper(ScoreKeeper* s) { score = s; }

    // Grid access
    const std::vector<std::vector<Cell>>& getGrid() const { return grid; }
    Cell& getCell(int row, int col) { return grid[row][col]; }
    const Cell& getCell(int row, int col) const { return grid[row][col]; }

    // Block access
    Block* getCurrentBlock() { return currentBlock.get(); }
    const Block* getCurrentBlock() const { return currentBlock.get(); }
    Block* getNextBlock() { return nextBlock.get(); }

    void setCurrentBlock(std::unique_ptr<Block> block) {
        currentBlock = std::move(block);
    }

    void setNextBlock(std::unique_ptr<Block> block) {
        nextBlock = std::move(block);
    }

    // Transfer ownership of next block
    std::unique_ptr<Block> takeNextBlock() {
        return std::move(nextBlock);
    }

    // Collision detection
    bool isValidPosition(const Block* block) const {
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

    // Movement methods
    bool moveLeft() {
        if (!currentBlock) return false;

        currentBlock->move(-1, 0);
        if (!isValidPosition(currentBlock.get())) {
            currentBlock->move(1, 0);  // Revert
            return false;
        }
        return true;
    }

    bool moveRight() {
        if (!currentBlock) return false;

        currentBlock->move(1, 0);
        if (!isValidPosition(currentBlock.get())) {
            currentBlock->move(-1, 0);  // Revert
            return false;
        }
        return true;
    }

    bool moveDown() {
        if (!currentBlock) return false;

        currentBlock->move(0, 1);
        if (!isValidPosition(currentBlock.get())) {
            currentBlock->move(0, -1);  // Revert
            return false;
        }
        return true;
    }

    bool rotate(bool clockwise) {
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

    // Lock current block into grid
    void lockBlock() {
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

    // Drop block until it can't move down
    void drop() {
        if (!currentBlock) return;

        while (moveDown()) {
            // Keep moving down
        }
        lockBlock();
    }

    // Drop a 1x1 center block from Level 4
    void dropCenterBlock(std::unique_ptr<Block> block) {
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

    // Clear full rows and return count
    int clearRows() {
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

    // Check if any blocks have been completely removed
    void checkBlockRemoval() {
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

    bool isGameOver() const {
        // Game over if blocks exist in top 3 reserve rows after lock
        for (int row = 0; row < RESERVE_ROWS; ++row) {
            for (int col = 0; col < BOARD_WIDTH; ++col) {
                if (grid[row][col].isFilled()) {
                    return true;
                }
            }
        }
        return false;
    }

    bool replaceCurrentBlock(char type) {
        if (!level) return false;

        // Create new block of the specified type
        int blockId = getNextBlockId();
        std::unique_ptr<Block> newBlock = level->createBlockFromType(type, blockId);

        if (!newBlock) return false;

        // Replace current block
        currentBlock = std::move(newBlock);
        return true;
    }

    void addEffect(Effect* effect) {
        activeEffects.push_back(effect);
    }

    int getNextBlockId() { return nextBlockId++; }

    // Level 4 block tracking methods
    int getBlocksSinceLastClear() const { return blocksSinceLastClear; }
    void incrementBlocksSinceLastClear() { blocksSinceLastClear++; }
    void resetBlocksSinceLastClear() { blocksSinceLastClear = 0; }
};
