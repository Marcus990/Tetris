module;
#include <vector>
#include <utility>
export module block;

export class Block {
protected:
    std::vector<std::pair<int, int>> cells;  // Relative positions (row, col)
    int posX;           // Current X position (column)
    int posY;           // Current Y position (row)
    char type;          // Block type character
    int levelGenerated; // Level this block was generated in
    int blockId;        // Unique block identifier
    int rotationState;  // Current rotation (0-3)

public:
    Block(char t, int level, int id, int startX = 3, int startY = 0)
        : posX(startX), posY(startY), type(t), levelGenerated(level),
          blockId(id), rotationState(0) {}

    virtual ~Block() {}

    // Getters
    char getType() const { return type; }
    int getX() const { return posX; }
    int getY() const { return posY; }
    int getBlockId() const { return blockId; }
    int getLevelGenerated() const { return levelGenerated; }
    const std::vector<std::pair<int, int>>& getCells() const { return cells; }

    // Get absolute cell positions
    std::vector<std::pair<int, int>> getAbsoluteCells() const {
        std::vector<std::pair<int, int>> absolute;
        for (const auto& cell : cells) {
            absolute.push_back({posY + cell.first, posX + cell.second});
        }
        return absolute;
    }

    // Movement
    void move(int dx, int dy) {
        posX += dx;
        posY += dy;
    }

    void setPosition(int x, int y) {
        posX = x;
        posY = y;
    }

    // Rotation - to be implemented by derived classes
    virtual void rotateClockwise() = 0;
    virtual void rotateCounterClockwise() = 0;

protected:
    // Helper for 90-degree clockwise rotation
    void rotateCellsClockwise() {
        for (auto& cell : cells) {
            int oldRow = cell.first;
            int oldCol = cell.second;
            cell.first = oldCol;
            cell.second = -oldRow;
        }
        rotationState = (rotationState + 1) % 4;
    }

    // Helper for 90-degree counter-clockwise rotation
    void rotateCellsCounterClockwise() {
        for (auto& cell : cells) {
            int oldRow = cell.first;
            int oldCol = cell.second;
            cell.first = -oldCol;
            cell.second = oldRow;
        }
        rotationState = (rotationState + 3) % 4;
    }
};
