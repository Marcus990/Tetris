export module effect;
import constants;

using namespace GameConstants;

// Abstract Effect class
// Effects are markers that Board uses to modify its own state
export class Effect {
public:
    virtual ~Effect() {}
    virtual bool isExpired() const = 0;
    virtual void update() = 0;

    // Type identification for Board to know how to apply/unapply
    enum class Type { Blind, Heavy, Force };
    virtual Type getType() const = 0;
};

// Blind effect - obscures part of opponent's display
export class BlindEffect : public Effect {
    int turnsLeft;

public:
    BlindEffect(int turns = 1) : turnsLeft(turns) {}

    Type getType() const override { return Effect::Type::Blind; }

    bool isExpired() const override {
        return turnsLeft <= 0;
    }

    void update() override {
        if (turnsLeft > 0) {
            turnsLeft--;
        }
    }
};

// Heavy effect - makes blocks drop extra after each move
export class HeavyEffect : public Effect {
    int extraDropAmount;

public:
    HeavyEffect(int drops = HEAVY_EXTRA_DROP) : extraDropAmount(drops) {}

    Type getType() const override { return Effect::Type::Heavy; }

    bool isExpired() const override {
        // Heavy effect never expires once active (permanent penalty)
        return false;
    }

    void update() override {
        // No update needed - permanent effect
    }

    int getExtraDropAmount() const { return extraDropAmount; }
};

// Force effect - forces next block to be a specific type
export class ForceEffect : public Effect {
    char blockType;
    bool used;

public:
    ForceEffect(char type) : blockType(type), used(false) {}

    Type getType() const override { return Effect::Type::Force; }

    bool isExpired() const override {
        return used;
    }

    void update() override {
        // No update needed - expires when used
    }

    void markUsed() { used = true; }
    char getBlockType() const { return blockType; }
};
