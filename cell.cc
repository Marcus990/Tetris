export module cell;

export class Cell {
    char type;      // Block type: ' ' for empty, or block character
    bool filled;    // Whether cell is occupied
    int blockId;    // ID of block this cell belongs to (-1 for none)

public:
    Cell() : type(' '), filled(false), blockId(-1) {}

    char getType() const { return type; }
    bool isFilled() const { return filled; }
    int getBlockId() const { return blockId; }

    void setType(char t) { type = t; }
    void setFilled(bool f) { filled = f; }
    void setBlockId(int id) { blockId = id; }

    void clear() { // Default values for cell
        type = ' ';
        filled = false;
        blockId = -1;
    }
};
