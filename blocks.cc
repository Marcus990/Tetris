export module blocks;
import block;
import constants;

using namespace GameConstants;

// I-block: ####
export class IBlock : public Block
{
public:
    IBlock(int level, int id, int startX = 3, int startY = RESERVE_ROWS)
        : Block('I', level, id, startX, startY)
    {
        cells.push_back({0, 0});
        cells.push_back({0, 1});
        cells.push_back({0, 2});
        cells.push_back({0, 3});
    }

    void rotateClockwise() override
    {
        rotateCellsClockwise();
    }

    void rotateCounterClockwise() override
    {
        rotateCellsCounterClockwise();
    }
};

// J-block:
// #
// #
// ##
export class JBlock : public Block
{
public:
    JBlock(int level, int id, int startX = 3, int startY = RESERVE_ROWS)
        : Block('J', level, id, startX, startY)
    {
        cells.push_back({0, 0});
        cells.push_back({1, 0});
        cells.push_back({2, 0});
        cells.push_back({2, 1});
    }

    void rotateClockwise() override
    {
        rotateCellsClockwise();
    }

    void rotateCounterClockwise() override
    {
        rotateCellsCounterClockwise();
    }
};

// L-block:
//   #
//   #
//  ##
export class LBlock : public Block
{
public:
    LBlock(int level, int id, int startX = 3, int startY = RESERVE_ROWS)
        : Block('L', level, id, startX, startY)
    {
        cells.push_back({0, 1});
        cells.push_back({1, 1});
        cells.push_back({2, 1});
        cells.push_back({2, 0});
    }

    void rotateClockwise() override
    {
        rotateCellsClockwise();
    }

    void rotateCounterClockwise() override
    {
        rotateCellsCounterClockwise();
    }
};

// O-block:
// ##
// ##
export class OBlock : public Block
{
public:
    OBlock(int level, int id, int startX = 3, int startY = RESERVE_ROWS)
        : Block('O', level, id, startX, startY)
    {
        cells.push_back({0, 0});
        cells.push_back({0, 1});
        cells.push_back({1, 0});
        cells.push_back({1, 1});
    }

    // O-block doesn't rotate
    void rotateClockwise() override {}
    void rotateCounterClockwise() override {}
};

// S-block:
//  ##
// ##
export class SBlock : public Block
{
public:
    SBlock(int level, int id, int startX = 3, int startY = RESERVE_ROWS)
        : Block('S', level, id, startX, startY)
    {
        cells.push_back({0, 1});
        cells.push_back({0, 2});
        cells.push_back({1, 0});
        cells.push_back({1, 1});
    }

    void rotateClockwise() override
    {
        rotateCellsClockwise();
    }

    void rotateCounterClockwise() override
    {
        rotateCellsCounterClockwise();
    }
};

// Z-block:
// ##
//  ##
export class ZBlock : public Block
{
public:
    ZBlock(int level, int id, int startX = 3, int startY = RESERVE_ROWS)
        : Block('Z', level, id, startX, startY)
    {
        cells.push_back({0, 0});
        cells.push_back({0, 1});
        cells.push_back({1, 1});
        cells.push_back({1, 2});
    }

    void rotateClockwise() override
    {
        rotateCellsClockwise();
    }

    void rotateCounterClockwise() override
    {
        rotateCellsCounterClockwise();
    }
};

// T-block:
//  #
// ###
export class TBlock : public Block
{
public:
    TBlock(int level, int id, int startX = 3, int startY = RESERVE_ROWS)
        : Block('T', level, id, startX, startY)
    {
        cells.push_back({0, 1});
        cells.push_back({1, 0});
        cells.push_back({1, 1});
        cells.push_back({1, 2});
    }

    void rotateClockwise() override
    {
        rotateCellsClockwise();
    }

    void rotateCounterClockwise() override
    {
        rotateCellsCounterClockwise();
    }
};

// Single-cell block (for Level 4 center drops)
// *
export class SingleBlock : public Block
{
public:
    SingleBlock(int level, int id, int startX = 5, int startY = RESERVE_ROWS)
        : Block('*', level, id, startX, startY)
    {
        cells.push_back({0, 0});
    }

    // Single block doesn't rotate
    void rotateClockwise() override {}
    void rotateCounterClockwise() override {}
};
