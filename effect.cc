export module effect;
import constants;

using namespace GameConstants;

// Forward declaration - Board is defined in board module
class Board;

// Abstract Effect class
export class Effect {
public:
    virtual ~Effect() {}
    virtual void apply(Board* board) = 0;
    virtual bool isExpired() const = 0;
    virtual void update() = 0;
};

// Blind effect - obscures part of opponent's display
export class BlindEffect : public Effect {
    int turnsLeft;

public:
    BlindEffect(int turns = 1) : turnsLeft(turns) {}

    void apply(Board* board) override {
        // Applied by the display classes checking if blind mode is active
        // Board doesn't need to do anything special
    }

    bool isExpired() const override {
        return turnsLeft <= 0;
    }

    void update() override {
        if (turnsLeft > 0) {
            turnsLeft--;
        }
    }

    int getTurnsLeft() const { return turnsLeft; }
};

// Heavy effect - makes blocks drop extra after each move
export class HeavyEffect : public Effect {
    int extraDropAmount;

public:
    HeavyEffect(int drops = HEAVY_EXTRA_DROP) : extraDropAmount(drops) {}

    void apply(Board*) override {
        // Heavy effect is applied by checking if it exists
        // The actual extra drops are handled in game logic
    }

    bool isExpired() const override {
        // Heavy effect never expires once active (controlled by level)
        return false;
    }

    void update() override {
        // No update needed
    }

    int getExtraDropAmount() const { return extraDropAmount; }
};

// Force effect - forces next block to be a specific type
export class ForceEffect : public Effect {
    char blockType;
    bool used;

public:
    ForceEffect(char type) : blockType(type), used(false) {}

    void apply(Board*) override {
        // Force effect is checked by game logic when spawning blocks
        // Mark as used after game applies it
    }

    bool isExpired() const override {
        return used;
    }

    void update() override {
        // No update needed
    }

    void markUsed() { used = true; }
    char getBlockType() const { return blockType; }
};
