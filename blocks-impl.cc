module blocks;
import <vector>;
import <utility>;
import block;
import constants;

using namespace GameConstants;

// I-block: ####
IBlock::IBlock(int level, int id, int startX, int startY)
    : Block('I', level, id, startX, startY)
{
    cells.push_back({0, 0});
    cells.push_back({0, 1});
    cells.push_back({0, 2});
    cells.push_back({0, 3});
}

void IBlock::rotateClockwise()
{
    rotateCellsClockwise();
}

void IBlock::rotateCounterClockwise()
{
    rotateCellsCounterClockwise();
}

// J-block:
// #
// ###
JBlock::JBlock(int level, int id, int startX, int startY)
    : Block('J', level, id, startX, startY)
{
    cells.push_back({0, 0});
    cells.push_back({1, 0});
    cells.push_back({1, 1});
    cells.push_back({1, 2});
}

void JBlock::rotateClockwise()
{
    rotateCellsClockwise();
}

void JBlock::rotateCounterClockwise()
{
    rotateCellsCounterClockwise();
}

// L-block:
//   #
// ###
LBlock::LBlock(int level, int id, int startX, int startY)
    : Block('L', level, id, startX, startY)
{
    cells.push_back({1, 0});
    cells.push_back({1, 1});
    cells.push_back({0, 2});
    cells.push_back({1, 2});
}

void LBlock::rotateClockwise()
{
    rotateCellsClockwise();
}

void LBlock::rotateCounterClockwise()
{
    rotateCellsCounterClockwise();
}

// O-block:
// ##
// ##
OBlock::OBlock(int level, int id, int startX, int startY)
    : Block('O', level, id, startX, startY)
{
    cells.push_back({0, 0});
    cells.push_back({0, 1});
    cells.push_back({1, 0});
    cells.push_back({1, 1});
}

// O-block doesn't rotate
void OBlock::rotateClockwise() {}
void OBlock::rotateCounterClockwise() {}

// S-block:
//  ##
// ##
SBlock::SBlock(int level, int id, int startX, int startY)
    : Block('S', level, id, startX, startY)
{
    cells.push_back({0, 1});
    cells.push_back({0, 2});
    cells.push_back({1, 0});
    cells.push_back({1, 1});
}

void SBlock::rotateClockwise()
{
    rotateCellsClockwise();
}

void SBlock::rotateCounterClockwise()
{
    rotateCellsCounterClockwise();
}

// Z-block:
// ##
//  ##
ZBlock::ZBlock(int level, int id, int startX, int startY)
    : Block('Z', level, id, startX, startY)
{
    cells.push_back({0, 0});
    cells.push_back({0, 1});
    cells.push_back({1, 1});
    cells.push_back({1, 2});
}

void ZBlock::rotateClockwise()
{
    rotateCellsClockwise();
}

void ZBlock::rotateCounterClockwise()
{
    rotateCellsCounterClockwise();
}

// T-block:
//  #
// ###
TBlock::TBlock(int level, int id, int startX, int startY)
    : Block('T', level, id, startX, startY)
{
    cells.push_back({0, 1});
    cells.push_back({0, 0});
    cells.push_back({1, 1});
    cells.push_back({0, 2});
}

void TBlock::rotateClockwise()
{
    rotateCellsClockwise();
}

void TBlock::rotateCounterClockwise()
{
    rotateCellsCounterClockwise();
}

// Single-cell block (for Level 4 center drops)
// *
SingleBlock::SingleBlock(int level, int id, int startX, int startY)
    : Block('*', level, id, startX, startY)
{
    cells.push_back({0, 0});
}

// Single block doesn't rotate
void SingleBlock::rotateClockwise() {}
void SingleBlock::rotateCounterClockwise() {}
