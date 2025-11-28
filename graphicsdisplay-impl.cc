module;
#include <memory>
#include <string>
#include <algorithm>
module graphicsdisplay;
import observer;
import board;
import block;
import xwindow;
import constants;

using namespace GameConstants;

int GraphicsDisplay::getColor(char type) const {
    switch (type) {
        case 'I': return Xwindow::Cyan;
        case 'J': return Xwindow::Blue;
        case 'L': return Xwindow::Orange;
        case 'O': return Xwindow::Yellow;
        case 'S': return Xwindow::Green;
        case 'Z': return Xwindow::Red;
        case 'T': return Xwindow::Magenta;
        case '*': return Xwindow::Brown;
        default: return Xwindow::White;
    }
}

void GraphicsDisplay::draw3DBlock(int row, int col, int color) {
    int x = offsetX + col * blockSize;
    int y = offsetY + row * blockSize;

    window->fillRectangle(x + BLOCK_3D_INSET, y + BLOCK_3D_INSET, 
                        blockSize - BLOCK_3D_INSET * 2, blockSize - BLOCK_3D_INSET * 2, color);
    window->fillRectangle(x, y, blockSize, BLOCK_3D_INSET, Xwindow::White);
    window->fillRectangle(x, y, BLOCK_3D_INSET, blockSize, Xwindow::White);
    window->fillRectangle(x, y + blockSize - BLOCK_3D_SHADOW, blockSize, BLOCK_3D_SHADOW, Xwindow::Black);
    window->fillRectangle(x + blockSize - BLOCK_3D_SHADOW, y, BLOCK_3D_SHADOW, blockSize, Xwindow::Black);
}

void GraphicsDisplay::drawEmptyCell(int row, int col) {
    int x = offsetX + col * blockSize;
    int y = offsetY + row * blockSize;

    window->fillRectangle(x + EMPTY_CELL_INSET, y + EMPTY_CELL_INSET, 
                        blockSize - EMPTY_CELL_INSET * 2, blockSize - EMPTY_CELL_INSET * 2, Xwindow::Black);
    
    if (row >= RESERVE_ROWS) {
        window->fillRectangle(x, y, blockSize, GRID_LINE_THICKNESS, Xwindow::White);
        window->fillRectangle(x, y, GRID_LINE_THICKNESS, blockSize, Xwindow::White);
        window->fillRectangle(x, y + blockSize - GRID_LINE_THICKNESS, blockSize, GRID_LINE_THICKNESS, Xwindow::White);
        window->fillRectangle(x + blockSize - GRID_LINE_THICKNESS, y, GRID_LINE_THICKNESS, blockSize, Xwindow::White);
    }
}

GraphicsDisplay::GraphicsDisplay(Board* b, std::string name, int width, int height)
    : board(b), window(std::make_unique<Xwindow>(width, height)),
      blockSize(GRAPHICS_BLOCK_SIZE), blindMode(false), playerName(name) {
    
    int boardWidth = BOARD_WIDTH * blockSize;
    offsetX = (width - boardWidth) / 2;
    headerHeight = HEADER_HEIGHT;
    offsetY = ARCADE_TOP_BEZEL + PLAYER_NAME_SPACING;
    window->setWindowTitle(playerName);
}

void GraphicsDisplay::update() {
    render();
}

void GraphicsDisplay::setBlindMode(bool blind) {
    blindMode = blind;
}

void GraphicsDisplay::setBoard(Board* b) {
    board = b;
}

void GraphicsDisplay::drawBlock(int row, int col, int color) {
    int x = offsetX + col * blockSize;
    int y = offsetY + row * blockSize;
    window->fillRectangle(x, y, blockSize, blockSize, color);
}

void GraphicsDisplay::render() {
    const auto& grid = board->getGrid();
    Block* current = board->getCurrentBlock();

    window->drawTetrisBackground(GRAPHICS_WINDOW_WIDTH, GRAPHICS_WINDOW_HEIGHT);

    std::string gameboyText = "Nintendo";
    int gameboyTextWidth = gameboyText.length() * CHAR_WIDTH_GAMEBOY;
    int pillWidth = gameboyTextWidth + NINTENDO_PILL_PADDING_X * 2;
    int pillHeight = CHAR_WIDTH_GAMEBOY + NINTENDO_PILL_PADDING_Y * 2;
    int pillX = GRAPHICS_WINDOW_WIDTH - pillWidth - GAMEBOY_TEXT_MARGIN;
    int pillY = GRAPHICS_WINDOW_HEIGHT - pillHeight - GAMEBOY_TEXT_MARGIN;
    
    window->drawRoundedRectangle(pillX, pillY, pillWidth, pillHeight, NINTENDO_PILL_RADIUS, Xwindow::White);
    
    int gameboyTextX = pillX + (pillWidth - gameboyTextWidth) / 2 + NINTENDO_TEXT_X_OFFSET;
    int gameboyTextY = pillY + NINTENDO_PILL_PADDING_Y + CHAR_WIDTH_GAMEBOY;
    
    for (size_t i = 0; i < gameboyText.length(); i++) {
        std::string ch(1, gameboyText[i]);
        int charX = gameboyTextX + i * CHAR_WIDTH_GAMEBOY;
        window->drawStringBlackBold(charX, gameboyTextY, ch);
        window->drawStringBlackBold(charX + 1, gameboyTextY, ch);
    }

    int screenX = offsetX - ARCADE_SCREEN_PADDING;
    int screenY = ARCADE_TOP_BEZEL - ARCADE_SCREEN_PADDING;
    int screenW = BOARD_WIDTH * blockSize + ARCADE_SCREEN_PADDING * 2;
    int screenH = TOTAL_ROWS * blockSize + ARCADE_SCREEN_PADDING * 2 + PLAYER_NAME_SPACING;
    
    window->fillRectangle(screenX - ARCADE_BEZEL_THICKNESS, screenY - ARCADE_BEZEL_THICKNESS,
                        screenW + ARCADE_BEZEL_THICKNESS * 2, ARCADE_BEZEL_THICKNESS, Xwindow::Orange);
    window->fillRectangle(screenX - ARCADE_BEZEL_THICKNESS, screenY - ARCADE_BEZEL_THICKNESS,
                        ARCADE_BEZEL_THICKNESS, screenH + ARCADE_BEZEL_THICKNESS * 2, Xwindow::Orange);
    window->fillRectangle(screenX - ARCADE_BEZEL_THICKNESS, screenY + screenH,
                        screenW + ARCADE_BEZEL_THICKNESS * 2, ARCADE_BEZEL_THICKNESS, Xwindow::Orange);
    window->fillRectangle(screenX + screenW, screenY - ARCADE_BEZEL_THICKNESS,
                        ARCADE_BEZEL_THICKNESS, screenH + ARCADE_BEZEL_THICKNESS * 2, Xwindow::Orange);
    
    window->fillRectangle(screenX - SCREEN_INNER_BEZEL, screenY - SCREEN_INNER_BEZEL, 
                        screenW + SCREEN_INNER_BEZEL * 2, SCREEN_INNER_BEZEL, Xwindow::White);
    window->fillRectangle(screenX - SCREEN_INNER_BEZEL, screenY - SCREEN_INNER_BEZEL, 
                        SCREEN_INNER_BEZEL, screenH + SCREEN_INNER_BEZEL * 2, Xwindow::White);
    window->fillRectangle(screenX - SCREEN_INNER_BEZEL, screenY + screenH, 
                        screenW + SCREEN_INNER_BEZEL * 2, SCREEN_INNER_BEZEL, Xwindow::White);
    window->fillRectangle(screenX + screenW, screenY - SCREEN_INNER_BEZEL, 
                        SCREEN_INNER_BEZEL, screenH + SCREEN_INNER_BEZEL * 2, Xwindow::White);
    
    window->fillRectangle(screenX, screenY, screenW, screenH, Xwindow::Black);

    int textWidth = playerName.length() * CHAR_WIDTH_STANDARD;
    int nameX = (GRAPHICS_WINDOW_WIDTH - textWidth) / 2 + TEXT_BASELINE_OFFSET;
    int nameY = screenY + PLAYER_NAME_Y_OFFSET;
    window->drawString(nameX, nameY, playerName);

    auto display = grid;

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

    for (int row = 0; row < TOTAL_ROWS; ++row) {
        for (int col = 0; col < BOARD_WIDTH; ++col) {
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

void GraphicsDisplay::renderWithInfo(int level, int score, int highScore) {
    render();

    int boardHeight = TOTAL_ROWS * blockSize;
    int bottomPanelY = ARCADE_TOP_BEZEL + boardHeight + ARCADE_SCREEN_PADDING * 2 + PLAYER_NAME_SPACING + SCREEN_TO_PANEL_GAP;

    int leftPanelX = ARCADE_SIDE_MARGIN;
    int previewSize = blockSize * PREVIEW_GRID_SIZE;
    int totalPanelHeight = PANEL_HEADER_HEIGHT + PANEL_PREVIEW_SECTION_HEIGHT;

    window->fillRectangle(leftPanelX - PANEL_OUTER_BORDER, bottomPanelY - PANEL_OUTER_BORDER, 
                        SIDE_PANEL_WIDTH + PANEL_OUTER_BORDER * 2, totalPanelHeight + PANEL_OUTER_BORDER * 2, Xwindow::Orange);
    window->fillRectangle(leftPanelX - PANEL_MIDDLE_BORDER, bottomPanelY - PANEL_MIDDLE_BORDER, 
                        SIDE_PANEL_WIDTH + PANEL_MIDDLE_BORDER * 2, totalPanelHeight + PANEL_MIDDLE_BORDER * 2, Xwindow::DarkCyan);
    window->fillRectangle(leftPanelX - PANEL_BORDER_THICKNESS, bottomPanelY - PANEL_BORDER_THICKNESS,
                        SIDE_PANEL_WIDTH + PANEL_BORDER_THICKNESS * 2, totalPanelHeight + PANEL_BORDER_THICKNESS * 2, Xwindow::Black);

    window->fillRectangle(leftPanelX, bottomPanelY, SIDE_PANEL_WIDTH, PANEL_HEADER_HEIGHT, Xwindow::DarkCyan);
    int nextTextX = leftPanelX + (SIDE_PANEL_WIDTH - NEXT_TEXT_WIDTH) / 2 + TEXT_BASELINE_OFFSET;
    window->drawString(nextTextX, bottomPanelY + PANEL_HEADER_HEIGHT / 2 + TEXT_BASELINE_OFFSET, "NEXT");

    int contentSectionY = bottomPanelY + PANEL_HEADER_HEIGHT;
    window->fillRectangle(leftPanelX, contentSectionY, SIDE_PANEL_WIDTH, PANEL_BORDER_THICKNESS, Xwindow::White);
    
    Block* next = board->getNextBlock();
    if (next) {
        int previewX = leftPanelX + NEXT_PREVIEW_PADDING;
        int previewY = contentSectionY + NEXT_PREVIEW_PADDING;
        int previewHeight = previewSize - NEXT_PREVIEW_HEIGHT_REDUCTION;

        window->fillRectangle(previewX - PREVIEW_BOX_BORDER, previewY - PREVIEW_BOX_BORDER, 
                            previewSize + PREVIEW_BOX_BORDER * 2, previewHeight + PREVIEW_BOX_BORDER * 2, Xwindow::White);
        window->fillRectangle(previewX, previewY, previewSize, previewHeight, Xwindow::Black);

        auto cells = next->getCells();
        int color = getColor(next->getType());

        int minCol = PREVIEW_CENTERING_MAX, maxCol = PREVIEW_CENTERING_MIN;
        int minRow = PREVIEW_CENTERING_MAX, maxRow = PREVIEW_CENTERING_MIN;
        for (const auto& cell : cells) {
            minRow = std::min(minRow, cell.first);
            maxRow = std::max(maxRow, cell.first);
            minCol = std::min(minCol, cell.second);
            maxCol = std::max(maxCol, cell.second);
        }
        int blockWidth = maxCol - minCol + 1;
        int blockHeight = maxRow - minRow + 1;
        int centerOffsetX = (PREVIEW_GRID_SIZE - blockWidth) * (blockSize / PREVIEW_BLOCK_SCALE) / PREVIEW_BLOCK_SCALE;
        int centerOffsetY = (PREVIEW_GRID_SIZE - blockHeight) * (blockSize / PREVIEW_BLOCK_SCALE) / PREVIEW_BLOCK_SCALE;

        for (const auto& cell : cells) {
            int relRow = cell.first - minRow;
            int relCol = cell.second - minCol;
            int x = previewX + centerOffsetX + relCol * (blockSize / PREVIEW_BLOCK_SCALE);
            int y = previewY + centerOffsetY + relRow * (blockSize / PREVIEW_BLOCK_SCALE);

            window->fillRectangle(x + PREVIEW_BOX_BORDER, y + PREVIEW_BOX_BORDER, 
                                blockSize / PREVIEW_BLOCK_SCALE - PREVIEW_BOX_BORDER * 2, 
                                blockSize / PREVIEW_BLOCK_SCALE - PREVIEW_BOX_BORDER * 2, color);
            window->fillRectangle(x, y, blockSize / PREVIEW_BLOCK_SCALE, PREVIEW_BOX_BORDER, Xwindow::White);
            window->fillRectangle(x, y, PREVIEW_BOX_BORDER, blockSize / PREVIEW_BLOCK_SCALE, Xwindow::White);
            window->fillRectangle(x, y + blockSize / PREVIEW_BLOCK_SCALE - PREVIEW_BOX_BORDER, 
                                blockSize / PREVIEW_BLOCK_SCALE, PREVIEW_BOX_BORDER, Xwindow::Black);
            window->fillRectangle(x + blockSize / PREVIEW_BLOCK_SCALE - PREVIEW_BOX_BORDER, y, 
                                PREVIEW_BOX_BORDER, blockSize / PREVIEW_BLOCK_SCALE, Xwindow::Black);
        }
        
        int statsX = previewX + previewSize + NEXT_STATS_OFFSET;
        int statsStartY = contentSectionY + NEXT_STATS_Y_START;
        
        window->drawString(statsX, statsStartY, "Level: " + std::to_string(level));
        window->drawString(statsX, statsStartY + NEXT_STATS_LINE_SPACING, "Score: " + std::to_string(score));
        window->drawString(statsX, statsStartY + NEXT_STATS_LINE_SPACING * 2, "Hi Score: " + std::to_string(highScore));
    }

    window->fillRectangle(leftPanelX - PANEL_BORDER_THICKNESS, bottomPanelY - PANEL_BORDER_THICKNESS,
                        CORNER_ACCENT_SIZE, PREVIEW_BOX_BORDER, Xwindow::Yellow);
    window->fillRectangle(leftPanelX - PANEL_BORDER_THICKNESS, bottomPanelY - PANEL_BORDER_THICKNESS,
                        PREVIEW_BOX_BORDER, CORNER_ACCENT_SIZE, Xwindow::Yellow);

    int rightPanelX = GRAPHICS_WINDOW_WIDTH - ARCADE_SIDE_MARGIN - CONTROL_PANEL_WIDTH;
    int arrowX = rightPanelX + (CONTROL_PANEL_WIDTH - (CONTROL_KEY_SIZE * 3 + CONTROL_KEY_GAP * 2)) / 2;
    int arrowY = bottomPanelY + ARROW_Y_OFFSET;
    window->drawArrowKeys(arrowX, arrowY, CONTROL_KEY_SIZE);

    int logoX = LOGO_MARGIN;
    int logoY = GRAPHICS_WINDOW_HEIGHT - LOGO_HEIGHT - LOGO_MARGIN;
    window->drawLogo(logoX, logoY, LOGO_WIDTH, LOGO_HEIGHT);

    window->present();
}

