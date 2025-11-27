module;
#include <vector>
#include <utility>
#include <algorithm>
export module block;

export class Block
{
protected:
    std::vector<std::pair<int, int>> cells; // Relative positions (row, col)
    int posX;                               // Current X position (column)
    int posY;                               // Current Y position (row)
    char type;                              // Block type character
    int levelGenerated;                     // Level this block was generated in
    int blockId;                            // Unique block identifier
    int rotationState;                      // Current rotation (0-3)

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
    const std::vector<std::pair<int, int>> &getCells() const { return cells; }

    // Get absolute cell positions
    std::vector<std::pair<int, int>> getAbsoluteCells() const
    {
        std::vector<std::pair<int, int>> absolute;
        for (const auto &cell : cells)
        {
            absolute.push_back({posY + cell.first, posX + cell.second});
        }
        return absolute;
    }

    // Movement
    void move(int dx, int dy)
    {
        posX += dx;
        posY += dy;
    }

    void setPosition(int x, int y)
    {
        posX = x;
        posY = y;
    }

    // Rotation - to be implemented by derived classes
    virtual void rotateClockwise() = 0;
    virtual void rotateCounterClockwise() = 0;

protected:
    // Helper for 90-degree clockwise rotation
    // Lower-right of rotated block goes to lower-left of original
    void rotateCellsClockwise()
    {
        // Find bounding box
        int minRow = cells[0].first, maxRow = cells[0].first;
        int minCol = cells[0].second, maxCol = cells[0].second;
        for (const auto &cell : cells)
        {
            minRow = std::min(minRow, cell.first);
            maxRow = std::max(maxRow, cell.first);
            minCol = std::min(minCol, cell.second);
            maxCol = std::max(maxCol, cell.second);
        }
        int height = maxRow - minRow;

        // Rotate: (row, col) -> (col, height - row)
        for (auto &cell : cells)
        {
            int oldRow = cell.first - minRow;
            int oldCol = cell.second - minCol;
            cell.first = oldCol;
            cell.second = height - oldRow;
        }

        // Find new bounding box
        int newMinRow = cells[0].first, newMaxRow = cells[0].first;
        int newMinCol = cells[0].second, newMaxCol = cells[0].second;
        for (const auto &cell : cells)
        {
            newMinRow = std::min(newMinRow, cell.first);
            newMaxRow = std::max(newMaxRow, cell.first);
            newMinCol = std::min(newMinCol, cell.second);
            newMaxCol = std::max(newMaxCol, cell.second);
        }

        // Shift so lower-left is preserved (normalize to minRow=0, and shift right edge to col 0)
        int newWidth = newMaxCol - newMinCol;
        for (auto &cell : cells)
        {
            cell.first -= newMinRow;
            cell.second -= (newMinCol + newWidth); // Shift so right edge is at col 0
        }

        // Now shift to make all coords non-negative starting from 0
        newMinCol = cells[0].second;
        for (const auto &cell : cells)
        {
            newMinCol = std::min(newMinCol, cell.second);
        }
        for (auto &cell : cells)
        {
            cell.second -= newMinCol;
        }

        rotationState = (rotationState + 1) % 4;
    }

    // Helper for 90-degree counter-clockwise rotation
    // Top-left of rotated block goes to lower-left of original
    void rotateCellsCounterClockwise()
    {
        // Find bounding box
        int minRow = cells[0].first, maxRow = cells[0].first;
        int minCol = cells[0].second, maxCol = cells[0].second;
        for (const auto &cell : cells)
        {
            minRow = std::min(minRow, cell.first);
            maxRow = std::max(maxRow, cell.first);
            minCol = std::min(minCol, cell.second);
            maxCol = std::max(maxCol, cell.second);
        }
        int width = maxCol - minCol;

        // Rotate: (row, col) -> (width - col, row)
        for (auto &cell : cells)
        {
            int oldRow = cell.first - minRow;
            int oldCol = cell.second - minCol;
            cell.first = width - oldCol;
            cell.second = oldRow;
        }

        // Find new bounding box
        int newMinRow = cells[0].first, newMaxRow = cells[0].first;
        int newMinCol = cells[0].second, newMaxCol = cells[0].second;
        for (const auto &cell : cells)
        {
            newMinRow = std::min(newMinRow, cell.first);
            newMaxRow = std::max(newMaxRow, cell.first);
            newMinCol = std::min(newMinCol, cell.second);
            newMaxCol = std::max(newMaxCol, cell.second);
        }

        // Shift so lower-left is preserved (top of rotated goes to bottom)
        int newHeight = newMaxRow - newMinRow;
        for (auto &cell : cells)
        {
            cell.first -= (newMinRow + newHeight); // Shift so top edge is at row 0
            cell.second -= newMinCol;
        }

        // Now shift to make all coords non-negative starting from 0
        newMinRow = cells[0].first;
        for (const auto &cell : cells)
        {
            newMinRow = std::min(newMinRow, cell.first);
        }
        for (auto &cell : cells)
        {
            cell.first -= newMinRow;
        }

        rotationState = (rotationState + 3) % 4;
    }
};
