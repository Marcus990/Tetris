module;
#include <memory>
#include <string>
#include <algorithm>
export module graphicsdisplay;
import observer;
import board;
import block;
import xwindow;
import constants;

using namespace GameConstants;

export class GraphicsDisplay : public IObserver {
    Board* board;
    std::unique_ptr<Xwindow> window;
    int blockSize;
    int offsetX;
    int offsetY;
    bool blindMode;

    // Map block types to colors
    int getColor(char type) const {
        switch (type) {
            case 'I': return Xwindow::Cyan;
            case 'J': return Xwindow::Blue;
            case 'L': return Xwindow::Orange;
            case 'O': return Xwindow::Yellow;
            case 'S': return Xwindow::Green;
            case 'Z': return Xwindow::Red;
            case 'T': return Xwindow::Magenta;
            case '*': return Xwindow::Brown;  // Level 4 center block
            default: return Xwindow::White;
        }
    }

    // Draw a 3D beveled block with shading
    void draw3DBlock(int row, int col, int color) {
        int x = offsetX + col * blockSize;
        int y = offsetY + row * blockSize;

        // Main block color
        window->fillRectangle(x + 2, y + 2, blockSize - 4, blockSize - 4, color);

        // Light top-left edges (highlight)
        window->fillRectangle(x, y, blockSize, 2, Xwindow::White);           // Top
        window->fillRectangle(x, y, 2, blockSize, Xwindow::White);           // Left

        // Dark bottom-right edges (shadow)
        window->fillRectangle(x, y + blockSize - 2, blockSize, 2, Xwindow::Black);     // Bottom
        window->fillRectangle(x + blockSize - 2, y, 2, blockSize, Xwindow::Black);     // Right
    }

    // Draw empty cell with grid
    void drawEmptyCell(int row, int col) {
        int x = offsetX + col * blockSize;
        int y = offsetY + row * blockSize;

        // Light gray background for empty cells
        window->fillRectangle(x + 1, y + 1, blockSize - 2, blockSize - 2, Xwindow::White);

        // Grid border
        window->fillRectangle(x, y, blockSize, 1, Xwindow::Black);           // Top
        window->fillRectangle(x, y, 1, blockSize, Xwindow::Black);           // Left
    }

public:
    GraphicsDisplay(Board* b, int width = GRAPHICS_WINDOW_WIDTH, int height = GRAPHICS_WINDOW_HEIGHT)
        : board(b), window(std::make_unique<Xwindow>(width, height)),
          blockSize(GRAPHICS_BLOCK_SIZE), offsetX(GRAPHICS_PADDING), offsetY(GRAPHICS_PADDING), blindMode(false) {}

    void update() override {
        render();
    }

    void setBlindMode(bool blind) {
        blindMode = blind;
    }

    void drawBlock(int row, int col, int color) {
        int x = offsetX + col * blockSize;
        int y = offsetY + row * blockSize;
        window->fillRectangle(x, y, blockSize, blockSize, color);
    }

    void render() {
        const auto& grid = board->getGrid();
        Block* current = board->getCurrentBlock();

        // Clear window with light background
        window->fillRectangle(0, 0, GRAPHICS_WINDOW_WIDTH, GRAPHICS_WINDOW_HEIGHT, Xwindow::White);

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

        // Draw board border (thick frame) - includes reserve rows
        int boardX = offsetX - 3;
        int boardY = offsetY - 3;
        int boardW = BOARD_WIDTH * blockSize + 6;
        int boardH = TOTAL_ROWS * blockSize + 6;  // All 18 rows
        window->fillRectangle(boardX, boardY, boardW, 3, Xwindow::Black);                    // Top
        window->fillRectangle(boardX, boardY, 3, boardH, Xwindow::Black);                    // Left
        window->fillRectangle(boardX, boardY + boardH - 3, boardW, 3, Xwindow::Black);       // Bottom
        window->fillRectangle(boardX + boardW - 3, boardY, 3, boardH, Xwindow::Black);       // Right

        // Draw the entire board including reserve rows (all 18 rows)
        for (int row = 0; row < TOTAL_ROWS; ++row) {
            for (int col = 0; col < BOARD_WIDTH; ++col) {
                // Apply blind effect if active (only on visible rows)
                // Blind boundaries are 0-indexed from visible area, so add RESERVE_ROWS offset
                if (blindMode &&
                    row >= RESERVE_ROWS + BLIND_ROW_START &&
                    row <= RESERVE_ROWS + BLIND_ROW_END &&
                    col >= BLIND_COL_START && col <= BLIND_COL_END) {
                    draw3DBlock(row, col, Xwindow::Black);
                } else if (display[row][col].isFilled()) {
                    int color = getColor(display[row][col].getType());
                    draw3DBlock(row, col, color);
                } else {
                    drawEmptyCell(row, col);
                }
            }
        }
    }

    void renderWithInfo(int level, int score, int highScore) {
        // Clear and render board first (to buffer, not presented yet)
        render();

        // Next block and info panel position
        int nextX = offsetX + (BOARD_WIDTH + 2) * blockSize;
        int nextY = offsetY;
        int previewSize = blockSize * 4;

        // Draw next block preview with border
        Block* next = board->getNextBlock();
        if (next) {
            // Draw "Next:" label
            window->drawString(nextX, nextY - 10, "NEXT:");

            int previewX = nextX;
            int previewY = nextY + 10;

            // Draw preview box border
            window->fillRectangle(previewX - 2, previewY - 2, previewSize + 4, previewSize + 4, Xwindow::Black);
            window->fillRectangle(previewX, previewY, previewSize, previewSize, Xwindow::White);

            // Draw the next block centered in preview
            auto cells = next->getCells();
            int color = getColor(next->getType());

            // Calculate centering offset
            int minCol = 100, maxCol = -100, minRow = 100, maxRow = -100;
            for (const auto& cell : cells) {
                minRow = std::min(minRow, cell.first);
                maxRow = std::max(maxRow, cell.first);
                minCol = std::min(minCol, cell.second);
                maxCol = std::max(maxCol, cell.second);
            }
            int blockWidth = maxCol - minCol + 1;
            int blockHeight = maxRow - minRow + 1;
            int centerOffsetX = (4 - blockWidth) * (blockSize / 2) / 2;
            int centerOffsetY = (4 - blockHeight) * (blockSize / 2) / 2;

            for (const auto& cell : cells) {
                int relRow = cell.first - minRow;
                int relCol = cell.second - minCol;
                int x = previewX + centerOffsetX + relCol * (blockSize / 2);
                int y = previewY + centerOffsetY + relRow * (blockSize / 2);

                // Draw mini 3D block
                window->fillRectangle(x + 1, y + 1, blockSize / 2 - 2, blockSize / 2 - 2, color);
                window->fillRectangle(x, y, blockSize / 2, 1, Xwindow::White);
                window->fillRectangle(x, y, 1, blockSize / 2, Xwindow::White);
                window->fillRectangle(x, y + blockSize / 2 - 1, blockSize / 2, 1, Xwindow::Black);
                window->fillRectangle(x + blockSize / 2 - 1, y, 1, blockSize / 2, Xwindow::Black);
            }
        }

        // Draw info text below next block preview area
        int infoTextX = nextX;
        int infoTextY = nextY + 10 + previewSize + 30;  // Below preview box

        window->drawString(infoTextX, infoTextY, "Level:    " + std::to_string(level));
        window->drawString(infoTextX, infoTextY + 20, "Score:    " + std::to_string(score));
        window->drawString(infoTextX, infoTextY + 40, "Hi Score: " + std::to_string(highScore));

        // Present the complete frame to screen
        window->present();
    }
};
