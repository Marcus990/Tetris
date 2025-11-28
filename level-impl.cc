module level;
import <memory>;
import <random>;
import <fstream>;
import <vector>;
import <string>;
import block;
import blocks;
import constants;

using namespace GameConstants;

// Level base class implementations
Level::Level(int num) : levelNumber(num), randomMode(true), nonRandomIndex(0) {}

int Level::getLevelNumber() const { return levelNumber; }

bool Level::isHeavy() const { return false; }

std::unique_ptr<Block> Level::createCenterBlock(int blockId) { return nullptr; }

void Level::setRandom(bool random) {
    randomMode = random;
}

void Level::setNonRandom(const std::string& filename) {
    nonRandomFile = filename;
    randomMode = false;
    loadNonRandomSequence();
}

void Level::loadNonRandomSequence() {
    std::ifstream file(nonRandomFile);
    nonRandomSequence.clear();
    nonRandomIndex = 0;

    char blockType;
    while (file >> blockType) {
        nonRandomSequence.push_back(blockType);
    }
}

char Level::getNextNonRandomBlock() {
    if (nonRandomSequence.empty()) {
        nonRandomIndex = 0;
        return 'I';  // Default fallback
    }
    char block = nonRandomSequence[nonRandomIndex];
    nonRandomIndex = (nonRandomIndex + 1) % nonRandomSequence.size();
    return block;
}

std::unique_ptr<Block> Level::createBlockFromType(char type, int blockId) {
    switch (type) {
        case 'I': return std::make_unique<IBlock>(levelNumber, blockId);
        case 'J': return std::make_unique<JBlock>(levelNumber, blockId);
        case 'L': return std::make_unique<LBlock>(levelNumber, blockId);
        case 'O': return std::make_unique<OBlock>(levelNumber, blockId);
        case 'S': return std::make_unique<SBlock>(levelNumber, blockId);
        case 'Z': return std::make_unique<ZBlock>(levelNumber, blockId);
        case 'T': return std::make_unique<TBlock>(levelNumber, blockId);
        default: return std::make_unique<IBlock>(levelNumber, blockId);
    }
}

// Level0 implementations
Level0::Level0(const std::string& filename) : Level(0) {
    setNonRandom(filename);
}

std::unique_ptr<Block> Level0::generateBlock(int blockId) {
    char type = getNextNonRandomBlock();
    return createBlockFromType(type, blockId);
}

bool Level0::isHeavy() const { return false; }

// Level1 implementations
Level1::Level1(unsigned int seed) : Level(1), rng(seed), dist(0, 11) {}

std::unique_ptr<Block> Level1::generateBlock(int blockId) {
    if (!randomMode) {
        char type = getNextNonRandomBlock();
        return createBlockFromType(type, blockId);
    }

    int rand = dist(rng);
    char type;

    // S: 0, Z: 1, Others: 2 each (I:2-3, J:4-5, L:6-7, O:8-9, T:10-11)
    if (rand == 0) type = 'S';
    else if (rand == 1) type = 'Z';
    else if (rand <= 3) type = 'I';
    else if (rand <= 5) type = 'J';
    else if (rand <= 7) type = 'L';
    else if (rand <= 9) type = 'O';
    else type = 'T';

    return createBlockFromType(type, blockId);
}

bool Level1::isHeavy() const { return false; }

// Level2 implementations
Level2::Level2(unsigned int seed) : Level(2), rng(seed), dist(0, 6) {}

std::unique_ptr<Block> Level2::generateBlock(int blockId) {
    if (!randomMode) {
        char type = getNextNonRandomBlock();
        return createBlockFromType(type, blockId);
    }

    int rand = dist(rng);
    char type;

    switch (rand) {
        case 0: type = 'I'; break;
        case 1: type = 'J'; break;
        case 2: type = 'L'; break;
        case 3: type = 'O'; break;
        case 4: type = 'S'; break;
        case 5: type = 'Z'; break;
        case 6: type = 'T'; break;
        default: type = 'I';
    }

    return createBlockFromType(type, blockId);
}

bool Level2::isHeavy() const { return false; }

// Level3 implementations
Level3::Level3(unsigned int seed) : Level(3), rng(seed), dist(0, 8) {}

std::unique_ptr<Block> Level3::generateBlock(int blockId) {
    if (!randomMode) {
        char type = getNextNonRandomBlock();
        return createBlockFromType(type, blockId);
    }

    int rand = dist(rng);
    char type;

    // S: 0-1, Z: 2-3, I: 4, J: 5, L: 6, O: 7, T: 8
    if (rand <= 1) type = 'S';
    else if (rand <= 3) type = 'Z';
    else if (rand == 4) type = 'I';
    else if (rand == 5) type = 'J';
    else if (rand == 6) type = 'L';
    else if (rand == 7) type = 'O';
    else type = 'T';

    return createBlockFromType(type, blockId);
}

bool Level3::isHeavy() const { return true; }

// Level4 implementations
Level4::Level4(unsigned int seed)
    : Level(4), rng(seed), dist(0, 8) {}

std::unique_ptr<Block> Level4::generateBlock(int blockId) {
    if (!randomMode) {
        char type = getNextNonRandomBlock();
        return createBlockFromType(type, blockId);
    }

    int rand = dist(rng);
    char type;

    // Same probabilities as Level 3
    if (rand <= 1) type = 'S';
    else if (rand <= 3) type = 'Z';
    else if (rand == 4) type = 'I';
    else if (rand == 5) type = 'J';
    else if (rand == 6) type = 'L';
    else if (rand == 7) type = 'O';
    else type = 'T';

    return createBlockFromType(type, blockId);
}

bool Level4::isHeavy() const { return true; }

// Create a 1x1 center block at column 5
std::unique_ptr<Block> Level4::createCenterBlock(int blockId) {
    // Create a single cell block at the center column
    auto block = std::make_unique<SingleBlock>(levelNumber, blockId, BOARD_WIDTH / 2, 0);
    return block;
}
