module textdisplay;
import <iostream>;
import <vector>;
import <string>;
import observer;
import board;
import block;
import constants;

using namespace GameConstants;

TextDisplay::TextDisplay(Board* b, std::ostream& os)
    : board(b), out(os), blindMode(false) {}

void TextDisplay::update() {
    // Update is called when board changes, but we render on demand
}

void TextDisplay::setBlindMode(bool blind) {
    blindMode = blind;
}

void TextDisplay::setBoard(Board* b) {
    board = b;
}

void TextDisplay::render() {
    const auto& grid = board->getGrid();
    Block* current = board->getCurrentBlock();

    // Create a copy of the grid to overlay current block
    auto display = grid;

    // Overlay current block if it exists
    if (current) {
        auto cells = current->getAbsoluteCells();
        for (const auto& cell : cells) {
            int row = cell.first;
            int col = cell.second;
            if (row >= 0 && row < TOTAL_ROWS && col >= 0 && col < BOARD_WIDTH) {
                display[row][col].setType(current->getType());
                display[row][col].setFilled(true);
            }
        }
    }

    // Print the entire board including reserve rows (all 18 rows)
    for (int row = 0; row < TOTAL_ROWS; ++row) {
        for (int col = 0; col < BOARD_WIDTH; ++col) {
            // Apply blind effect if active (only on visible rows)
            // Blind boundaries are 0-indexed from visible area, so add RESERVE_ROWS offset
            if (blindMode &&
                row >= RESERVE_ROWS + BLIND_ROW_START &&
                row <= RESERVE_ROWS + BLIND_ROW_END &&
                col >= BLIND_COL_START && col <= BLIND_COL_END) {
                out << '?';
            } else if (display[row][col].isFilled()) {
                out << display[row][col].getType();
            } else {
                out << ' ';
            }
        }
        out << '\n';
    }
    out << std::string(BOARD_WIDTH, '-') << '\n';
}

void TextDisplay::renderWithInfo(int level, int score, int highScore) {
    out << "Level: " << level << '\n';
    out << "Score: " << score << '\n';
    out << "Hi Score: " << highScore << '\n';
    out << std::string(BOARD_WIDTH, '-') << '\n';

    render();

    // Show next block
    out << "Next:\n";
    Block* next = board->getNextBlock();
    if (next) {
        // Simple representation of next block
        auto cells = next->getCells();
        for (const auto& cell : cells) {
            out << next->getType();
        }
        out << '\n';
    }
}
