module;
#include <vector>
#include <utility>
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
    Block(char t, int level, int id, int startX = 3, int startY = 0);
    virtual ~Block();

    // Getters
    char getType() const;
    int getX() const;
    int getY() const;
    int getBlockId() const;
    int getLevelGenerated() const;
    const std::vector<std::pair<int, int>> &getCells() const;

    // Get absolute cell positions
    std::vector<std::pair<int, int>> getAbsoluteCells() const;

    // Movement
    void move(int dx, int dy);
    void setPosition(int x, int y);

    // Rotation - to be implemented by derived classes
    virtual void rotateClockwise() = 0;
    virtual void rotateCounterClockwise() = 0;

protected:
    // Helper for 90-degree clockwise rotation
    void rotateCellsClockwise();

    // Helper for 90-degree counter-clockwise rotation
    void rotateCellsCounterClockwise();
};
