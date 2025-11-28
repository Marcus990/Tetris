export module level;
import <memory>;
import <random>;
import <string>;
import <vector>;
import block;
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
    // Constructor
    Level(int num);
    
    // Virtual destructor
    virtual ~Level() = default;

    // Getter for level number
    int getLevelNumber() const;

    // Pure virtual methods to be implemented by derived classes
    virtual std::unique_ptr<Block> generateBlock(int blockId) = 0;
    virtual bool isHeavy() const;
    virtual std::unique_ptr<Block> createCenterBlock(int blockId);

    // Set random or non-random mode
    void setRandom(bool random);
    void setNonRandom(const std::string& filename);

    // Load non-random sequence from file
    void loadNonRandomSequence();

    // Get next block type from non-random sequence
    char getNextNonRandomBlock();

    // Factory method to create block from type
    std::unique_ptr<Block> createBlockFromType(char type, int blockId);
};

// Level 0: Reads from sequence file (non-random only)
export class Level0 : public Level {
public:
    Level0(const std::string& filename);
    std::unique_ptr<Block> generateBlock(int blockId) override;
    bool isHeavy() const override;
};

// Level 1: Random with S,Z prob 1/12, others 2/12
export class Level1 : public Level {
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;

public:
    Level1(unsigned int seed = std::random_device{}());
    std::unique_ptr<Block> generateBlock(int blockId) override;
    bool isHeavy() const override;
};

// Level 2: Equal probability for all blocks
export class Level2 : public Level {
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;

public:
    Level2(unsigned int seed = std::random_device{}());
    std::unique_ptr<Block> generateBlock(int blockId) override;
    bool isHeavy() const override;
};

// Level 3: S,Z prob 2/9, others 1/9, blocks are heavy
export class Level3 : public Level {
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;

public:
    Level3(unsigned int seed = std::random_device{}());
    std::unique_ptr<Block> generateBlock(int blockId) override;
    bool isHeavy() const override;
};

// Level 4: Like Level 3, plus center block every 5 drops without clearing
export class Level4 : public Level {
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;

public:
    Level4(unsigned int seed = std::random_device{}());
    std::unique_ptr<Block> generateBlock(int blockId) override;
    bool isHeavy() const override;
    std::unique_ptr<Block> createCenterBlock(int blockId) override;
};
