export module blocks;
import block;
import constants;

using namespace GameConstants;

// I-block: ####
export class IBlock : public Block
{
public:
    IBlock(int level, int id, int startX = 3, int startY = RESERVE_ROWS);
    void rotateClockwise() override;
    void rotateCounterClockwise() override;
};

// J-block:
// #
// ###
export class JBlock : public Block
{
public:
    JBlock(int level, int id, int startX = SPAWN_X, int startY = SPAWN_Y);
    void rotateClockwise() override;
    void rotateCounterClockwise() override;
};

// L-block:
//   #
// ###
export class LBlock : public Block
{
public:
    LBlock(int level, int id, int startX = SPAWN_X, int startY = SPAWN_Y);
    void rotateClockwise() override;
    void rotateCounterClockwise() override;
};

// O-block:
// ##
// ##
export class OBlock : public Block
{
public:
    OBlock(int level, int id, int startX = SPAWN_X, int startY = SPAWN_Y);
    void rotateClockwise() override;
    void rotateCounterClockwise() override;
};

// S-block:
//  ##
// ##
export class SBlock : public Block
{
public:
    SBlock(int level, int id, int startX = SPAWN_X, int startY = SPAWN_Y);
    void rotateClockwise() override;
    void rotateCounterClockwise() override;
};

// Z-block:
// ##
//  ##
export class ZBlock : public Block
{
public:
    ZBlock(int level, int id, int startX = SPAWN_X, int startY = SPAWN_Y);
    void rotateClockwise() override;
    void rotateCounterClockwise() override;
};

// T-block:
//  #
// ###
export class TBlock : public Block
{
public:
    TBlock(int level, int id, int startX = SPAWN_X, int startY = SPAWN_Y);
    void rotateClockwise() override;
    void rotateCounterClockwise() override;
};

// Single-cell block (for Level 4 center drops)
// *
export class SingleBlock : public Block
{
public:
    SingleBlock(int level, int id, int startX = CENTER_COLUMN, int startY = SPAWN_Y);
    void rotateClockwise() override;
    void rotateCounterClockwise() override;
};
