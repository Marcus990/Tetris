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

    // Overlay ghost piece first (use '~' as marker)
    if (current) {
        auto ghostCells = board->getGhostPosition();
        for (const auto& cell : ghostCells) {
            int row = cell.first;
            int col = cell.second;
            if (row >= 0 && row < TOTAL_ROWS && col >= 0 && col < BOARD_WIDTH) {
                display[row][col].setType('~');
                display[row][col].setFilled(true);
            }
        }
    }

    // Overlay current block (overwrites ghost if at same position)
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
                char cellType = display[row][col].getType();
                if (cellType == '~') {
                    out << "☐";  // Empty box for ghost piece
                } else {
                    out << cellType;
                }
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

std::string TextDisplay::getBlockColor(char type) const {
    const std::string CYAN = "\033[36m";
    const std::string BLUE = "\033[34m";
    const std::string ORANGE = "\033[38;5;208m";
    const std::string YELLOW = "\033[33m";
    const std::string GREEN = "\033[32m";
    const std::string RED = "\033[31m";
    const std::string MAGENTA = "\033[35m";
    const std::string WHITE = "\033[37m";
    const std::string RESET = "\033[0m";

    switch (type) {
        case 'I': return CYAN;
        case 'J': return BLUE;
        case 'L': return ORANGE;
        case 'O': return YELLOW;
        case 'S': return GREEN;
        case 'Z': return RED;
        case 'T': return MAGENTA;
        case '*': return WHITE;
        default: return RESET;
    }
}

std::string TextDisplay::renderBoardRow(int row) const {
    const std::string BOLD = "\033[1m";
    const std::string RED = "\033[31m";
    const std::string RESET = "\033[0m";
    const std::string DIM = "\033[2m";

    const auto& grid = board->getGrid();
    Block* current = board->getCurrentBlock();

    // Create display grid with current block overlaid
    auto display = grid;

    // Overlay ghost piece first (use '~' as marker)
    if (current) {
        auto ghostCells = board->getGhostPosition();
        for (const auto& cell : ghostCells) {
            int r = cell.first;
            int c = cell.second;
            if (r >= 0 && r < TOTAL_ROWS && c >= 0 && c < BOARD_WIDTH) {
                display[r][c].setType('~');
                display[r][c].setFilled(true);
            }
        }
    }

    // Overlay current block (overwrites ghost if at same position)
    if (current) {
        auto cells = current->getAbsoluteCells();
        for (const auto& cell : cells) {
            int r = cell.first;
            int c = cell.second;
            if (r >= 0 && r < TOTAL_ROWS && c >= 0 && c < BOARD_WIDTH) {
                display[r][c].setType(current->getType());
                display[r][c].setFilled(true);
            }
        }
    }

    std::string result;
    for (int col = 0; col < BOARD_WIDTH; ++col) {
        if (blindMode &&
            row >= RESERVE_ROWS + BLIND_ROW_START &&
            row <= RESERVE_ROWS + BLIND_ROW_END &&
            col >= BLIND_COL_START && col <= BLIND_COL_END) {
            result += BOLD + RED + "? " + RESET;
        }
        else if (display[row][col].isFilled()) {
            char type = display[row][col].getType();
            if (type == '~') {
                // Ghost piece - render as dimmed outline
                result += DIM + "□ " + RESET;
            } else {
                result += BOLD + getBlockColor(type) + "█ " + RESET;
            }
        }
        else {
            result += "· ";
        }
    }

    return result;
}

std::vector<std::string> TextDisplay::renderNextBlockPreview() const {
    const std::string BOLD = "\033[1m";
    const std::string RESET = "\033[0m";

    std::vector<std::string> preview(3);
    Block* next = board->getNextBlock();

    std::vector<std::string> grid(3, std::string(4, ' '));

    if (next) {
        for (auto& cell : next->getCells()) {
            int r = cell.first;
            int c = cell.second;
            if (r >= 0 && r < 3 && c >= 0 && c < 4)
                grid[r][c] = next->getType();
        }
    }

    for (int row = 0; row < 3; row++) {
        std::string line;
        for (int col = 0; col < 4; col++) {
            if (grid[row][col] != ' ') {
                line += BOLD + getBlockColor(grid[row][col]) + "█ " + RESET;
            }
            else {
                line += "  ";
            }
        }
        preview[row] = line;
    }

    return preview;
}
