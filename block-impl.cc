module;
#include <vector>
#include <utility>
#include <algorithm>
module block;

// Block constructor
Block::Block(char t, int level, int id, int startX, int startY)
    : posX(startX), posY(startY), type(t), levelGenerated(level),
      blockId(id), rotationState(0) {}

Block::~Block() {}

// Getters
char Block::getType() const { return type; }
int Block::getX() const { return posX; }
int Block::getY() const { return posY; }
int Block::getBlockId() const { return blockId; }
int Block::getLevelGenerated() const { return levelGenerated; }
const std::vector<std::pair<int, int>> &Block::getCells() const { return cells; }

// Get absolute cell positions
std::vector<std::pair<int, int>> Block::getAbsoluteCells() const
{
    std::vector<std::pair<int, int>> absolute;
    for (const auto &cell : cells)
    {
        absolute.push_back({posY + cell.first, posX + cell.second});
    }
    return absolute;
}

// Movement
void Block::move(int dx, int dy)
{
    posX += dx;
    posY += dy;
}

void Block::setPosition(int x, int y)
{
    posX = x;
    posY = y;
}

// Helper for 90-degree clockwise rotation
// Lower-right of rotated block goes to lower-left of original
void Block::rotateCellsClockwise()
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
void Block::rotateCellsCounterClockwise()
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
