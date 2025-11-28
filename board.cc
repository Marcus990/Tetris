export module board;
import <vector>;
import <map>;
import <memory>;
import cell;
import block;
import constants;
import observer;
import scorekeeper;
import level;
import effect;

using namespace GameConstants;

export class Board {
    std::vector<std::vector<Cell>> grid;
    std::unique_ptr<Block> currentBlock;
    std::unique_ptr<Block> nextBlock;
    Level* level;
    ScoreKeeper* score;
    std::vector<IObserver*> displays;
    std::vector<Effect*> activeEffects;
    std::map<int, std::unique_ptr<Block>> activeBlocks;
    int nextBlockId;
    int blocksSinceLastClear;

    // Effect state (managed by effects via apply/unapply)
    bool blindActive;
    int heavyCount;

public:
    Board();

    // Observer pattern methods
    void attach(IObserver* observer);
    void detach(IObserver* observer);
    void notifyObservers();

    // Setters for dependencies
    void setLevel(Level* l);
    void setScoreKeeper(ScoreKeeper* s);

    // Grid access
    const std::vector<std::vector<Cell>>& getGrid() const;
    Cell& getCell(int row, int col);
    const Cell& getCell(int row, int col) const;

    // Block access
    Block* getCurrentBlock();
    const Block* getCurrentBlock() const;
    Block* getNextBlock();
    void setCurrentBlock(std::unique_ptr<Block> block);
    void setNextBlock(std::unique_ptr<Block> block);
    std::unique_ptr<Block> takeNextBlock();

    // Collision detection
    bool isValidPosition(const Block* block) const;

    // Movement methods
    bool moveLeft();
    bool moveRight();
    bool moveDown();
    bool rotate(bool clockwise);

    // Lock current block into grid
    void lockBlock();

    // Drop block until it can't move down
    void drop();

    // Drop a 1x1 center block from Level 4
    void dropCenterBlock(std::unique_ptr<Block> block);

    // Clear full rows and return count
    int clearRows();

    // Check if any blocks have been completely removed
    void checkBlockRemoval();

    // Game over check
    bool isGameOver() const;

    // Replace current block with specified type
    bool replaceCurrentBlock(char type);

    // Effect state management
    void setBlindActive(bool active);
    void incrementHeavy();
    void decrementHeavy();

    // Effect state queries
    bool hasBlindEffect() const;
    bool hasHeavyEffect() const;

    // Effect management
    void addEffect(Effect* effect);
    void updateEffects();

    // Block ID management
    int getNextBlockId();

    // Level 4 block tracking methods
    int getBlocksSinceLastClear() const;
    void incrementBlocksSinceLastClear();
    void resetBlocksSinceLastClear();
};
