
module;
#include <memory>
#include <random>
#include <fstream>
#include <vector>
#include <string>
export module level;
import block;
import blocks;
import constants;

using namespace GameConstants;

// Abstract Level class
export class Level {
protected:
    int levelNumber;
    bool randomMode;
    std::string nonRandomFile;
    std::vector<char> nonRandomSequence;
    int nonRandomIndex;

public:
    Level(int num) : levelNumber(num), randomMode(true), nonRandomIndex(0) {}
    virtual ~Level() = default;

    int getLevelNumber() const { return levelNumber; }

    virtual std::unique_ptr<Block> generateBlock(int blockId) = 0;
    virtual bool isHeavy() const { return false; }

    // Level 4 specific method - overridden in Level4
    virtual std::unique_ptr<Block> createCenterBlock(int blockId) { return nullptr; }

    void setRandom(bool random) {
        randomMode = random;
    }

    void setNonRandom(const std::string& filename) {
        nonRandomFile = filename;
        randomMode = false;
        loadNonRandomSequence();
    }

    void loadNonRandomSequence() {
        std::ifstream file(nonRandomFile);
        nonRandomSequence.clear();
        nonRandomIndex = 0;

        char blockType;
        while (file >> blockType) {
            nonRandomSequence.push_back(blockType);
        }
    }

    char getNextNonRandomBlock() {
        if (nonRandomSequence.empty()) {
            nonRandomIndex = 0;
            return 'I';  // Default fallback
        }
        char block = nonRandomSequence[nonRandomIndex];
        nonRandomIndex = (nonRandomIndex + 1) % nonRandomSequence.size();
        return block;
    }

    std::unique_ptr<Block> createBlockFromType(char type, int blockId) {
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
};

// Level 0: Reads from sequence file (non-random only)
export class Level0 : public Level {
public:
    Level0(const std::string& filename) : Level(0) {
        setNonRandom(filename);
    }

    std::unique_ptr<Block> generateBlock(int blockId) override {
        char type = getNextNonRandomBlock();
        return createBlockFromType(type, blockId);
    }

    bool isHeavy() const override { return false; }
};

// Level 1: Random with S,Z prob 1/12, others 2/12
export class Level1 : public Level {
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;

public:
    Level1(unsigned int seed = std::random_device{}()) : Level(1), rng(seed), dist(0, 11) {}

    std::unique_ptr<Block> generateBlock(int blockId) override {
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

    bool isHeavy() const override { return false; }
};

// Level 2: Equal probability for all blocks
export class Level2 : public Level {
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;

public:
    Level2(unsigned int seed = std::random_device{}()) : Level(2), rng(seed), dist(0, 6) {}

    std::unique_ptr<Block> generateBlock(int blockId) override {
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

    bool isHeavy() const override { return false; }
};

// Level 3: S,Z prob 2/9, others 1/9, blocks are heavy
export class Level3 : public Level {
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;

public:
    Level3(unsigned int seed = std::random_device{}()) : Level(3), rng(seed), dist(0, 8) {}

    std::unique_ptr<Block> generateBlock(int blockId) override {
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

    bool isHeavy() const override { return true; }
};

// Level 4: Like Level 3, plus center block every 5 blocks without clearing
export class Level4 : public Level {
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;

public:
    Level4(unsigned int seed = std::random_device{}())
        : Level(4), rng(seed), dist(0, 8) {}

    std::unique_ptr<Block> generateBlock(int blockId) override {
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

    bool isHeavy() const override { return true; }

    // Create a 1x1 center block at column 5
    std::unique_ptr<Block> createCenterBlock(int blockId) override {
        // Create a single cell block at the center column
        auto block = std::make_unique<SingleBlock>(levelNumber, blockId, BOARD_WIDTH / 2, 0);
        return block;
    }
};
