module block;
import <vector>;
import <utility>;
import <algorithm>;

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
void Block::move(int dx, int dy) {
    posX += dx;
    posY += dy;
}

void Block::setPosition(int x, int y)
{
    posX = x;
    posY = y;
}

void Block::rotateCellsClockwise() {
    int minRow = cells[0].first, maxRow = cells[0].first;
    int minCol = cells[0].second, maxCol = cells[0].second;
    for (auto &cell : cells) {
        minRow = std::min(minRow, cell.first);
        maxRow = std::max(maxRow, cell.first);
        minCol = std::min(minCol, cell.second);
        maxCol = std::max(maxCol, cell.second);
    }
    
    int width  = maxCol - minCol + 1;
    int height = maxRow - minRow + 1; // not used here, but fine to keep

    for (auto &cell : cells) {
        int row = cell.first;
        int col = cell.second;

        int x = col - minCol;      // right
        int y = maxRow - row;      // up

        // CLOCKWISE: (x, y) -> (y, width - 1 - x)
        int xp = y;
        int yp = width - 1 - x;

        cell.second = minCol + xp; // new col
        cell.first  = maxRow - yp; // new row
    }

    rotationState = (rotationState + 1) % 4;
}

void Block::rotateCellsCounterClockwise() {
    int minRow = cells[0].first, maxRow = cells[0].first;
    int minCol = cells[0].second, maxCol = cells[0].second;
    for (auto &cell : cells) {
        minRow = std::min(minRow, cell.first);
        maxRow = std::max(maxRow, cell.first);
        minCol = std::min(minCol, cell.second);
        maxCol = std::max(maxCol, cell.second);
    }

    int width  = maxCol - minCol + 1;
    int height = maxRow - minRow + 1;

    for (auto &cell : cells) {
        int row = cell.first;
        int col = cell.second;

        int x = col - minCol;
        int y = maxRow - row;

        // COUNTER-CLOCKWISE: (x, y) -> (height - 1 - y, x)
        int xp = height - 1 - y;
        int yp = x;

        cell.second = minCol + xp;
        cell.first  = maxRow - yp;
    }

    rotationState = (rotationState + 3) % 4; // -1 mod 4
}
