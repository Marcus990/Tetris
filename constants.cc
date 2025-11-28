// Game constants module interface
export module constants;

export namespace GameConstants {
    // Dimensions of the board
    constexpr int BOARD_WIDTH = 11;
    constexpr int BOARD_HEIGHT = 15;
    constexpr int RESERVE_ROWS = 3;
    constexpr int TOTAL_ROWS = BOARD_HEIGHT + RESERVE_ROWS;  
    
    // Properties of block
    constexpr int CELLS_PER_BLOCK = 4;
    constexpr int NUM_BLOCK_TYPES = 7;
    constexpr int NUM_ROTATION_STATES = 4;
    
    // Level settings
    constexpr int MIN_LEVEL = 0;
    constexpr int MAX_LEVEL = 4;
    
    // Level 4 center block settings
    constexpr int BLOCKS_BEFORE_CENTER_DROP = 5;
    constexpr int CENTER_COLUMN = BOARD_WIDTH / 2;  
    
    // Blind effect boundaries 
    constexpr int BLIND_COL_START = 2;   
    constexpr int BLIND_COL_END = 8;     
    constexpr int BLIND_ROW_START = 2;   
    constexpr int BLIND_ROW_END = 11;    
    
    // Heavy effect settings
    constexpr int HEAVY_EXTRA_DROP = 2;
    constexpr int LEVEL_HEAVY_DROP = 1;
    
    // Special action threshold
    constexpr int ROWS_FOR_SPECIAL_ACTION = 2;
    
    // Block spawn position
    constexpr int SPAWN_X = 3;
    constexpr int SPAWN_Y = RESERVE_ROWS;  
    
    // Display characters
    constexpr char EMPTY_CELL = ' ';
    constexpr char BLIND_CHAR = '?';
    constexpr char CENTER_BLOCK_CHAR = '*';
    
    // Block type characters
    constexpr char I_BLOCK = 'I';
    constexpr char J_BLOCK = 'J';
    constexpr char L_BLOCK = 'L';
    constexpr char O_BLOCK = 'O';
    constexpr char S_BLOCK = 'S';
    constexpr char Z_BLOCK = 'Z';
    constexpr char T_BLOCK = 'T';
    
    // Probability settings for block generation
    constexpr int LEVEL1_TOTAL_PROB = 12;
    constexpr int LEVEL1_SZ_PROB = 1;
    constexpr int LEVEL1_OTHER_PROB = 2;
    
    constexpr int LEVEL2_TOTAL_PROB = 7;
    
    constexpr int LEVEL3_TOTAL_PROB = 9;
    constexpr int LEVEL3_SZ_PROB = 2;
    constexpr int LEVEL3_OTHER_PROB = 1;
    
    // Graphics settings - Arcade Machine Layout
    constexpr int GRAPHICS_BLOCK_SIZE = 25;
    constexpr int GRAPHICS_PADDING = 10;
    constexpr int GRAPHICS_WINDOW_WIDTH = 550;
    constexpr int GRAPHICS_WINDOW_HEIGHT = 800;
    
    // Arcade Machine Layout Constants
    constexpr int ARCADE_TOP_BEZEL = 60;
    constexpr int ARCADE_SCREEN_PADDING = 15;
    constexpr int ARCADE_BOTTOM_PANEL_HEIGHT = 200;
    constexpr int ARCADE_SIDE_MARGIN = 30;
    constexpr int PLAYER_NAME_SPACING = 25;
    constexpr int SCREEN_TO_PANEL_GAP = 30;
    
    // Text Rendering Constants
    constexpr int CHAR_WIDTH_STANDARD = 8;
    constexpr int CHAR_WIDTH_GAMEBOY = 12;
    constexpr int TEXT_BASELINE_OFFSET = 5;
    constexpr int PLAYER_NAME_Y_OFFSET = 20;
    constexpr int GAMEBOY_TEXT_MARGIN = 15;
    
    // NEXT Panel Layout Constants
    constexpr int NEXT_TEXT_WIDTH = 40;
    constexpr int NEXT_PREVIEW_PADDING = 10;
    constexpr int NEXT_STATS_OFFSET = 15;
    constexpr int NEXT_STATS_Y_START = 25;
    constexpr int NEXT_STATS_LINE_SPACING = 25;
    constexpr int NEXT_PREVIEW_HEIGHT_REDUCTION = 15;
    
    // Preview Block Constants
    constexpr int PREVIEW_GRID_SIZE = 4;
    constexpr int PREVIEW_BLOCK_SCALE = 2;
    constexpr int PREVIEW_CENTERING_MAX = 100;
    constexpr int PREVIEW_CENTERING_MIN = -100;
    
    constexpr int ARROW_Y_OFFSET = 20;
    constexpr int ARROW_KEY_GAP = 5;
    constexpr int ARROW_TRIANGLE_INSET = 10;
    constexpr int ARROW_TRIANGLE_WIDTH = 8;
    constexpr int ARROW_TRIANGLE_LENGTH_SHORT = 12;
    constexpr int ARROW_TRIANGLE_LENGTH_LONG = 15;
    constexpr int ARROW_TEXT_CENTER_OFFSET = 4;
    constexpr int ARROW_TEXT_VERTICAL_OFFSET = 5;
    
    // Border Constants
    constexpr int PANEL_OUTER_BORDER = 4;
    constexpr int PANEL_MIDDLE_BORDER = 3;
    constexpr int PREVIEW_BOX_BORDER = 1;
    constexpr int SCREEN_INNER_BEZEL = 2;
    constexpr int EMPTY_CELL_INSET = 1;
    constexpr int GRID_LINE_THICKNESS = 1;
    
    constexpr int HEADER_HEIGHT = 50;
    constexpr int PLAYER_NAME_BOX_WIDTH = 120;
    constexpr int PLAYER_NAME_BOX_HEIGHT = 35;
    constexpr int PLAYER_NAME_BOX_Y = 12;
    
    constexpr int SIDE_PANEL_WIDTH = 240;
    constexpr int SIDE_PANEL_GAP = 20;
    constexpr int PANEL_HEADER_HEIGHT = 30;
    constexpr int PANEL_PREVIEW_SECTION_HEIGHT = 100;
    constexpr int PANEL_INFO_SECTION_HEIGHT = 100;
    
    constexpr int CONTROL_PANEL_WIDTH = 180;
    constexpr int CONTROL_PANEL_HEIGHT = 170;
    constexpr int CONTROL_KEY_SIZE = 35;
    constexpr int CONTROL_KEY_GAP = 5;
    
    constexpr int BOARD_BORDER_THICKNESS = 3;
    constexpr int PANEL_BORDER_THICKNESS = 2;
    constexpr int CORNER_ACCENT_SIZE = 5;
    constexpr int ARCADE_BEZEL_THICKNESS = 8;
    
    constexpr int BLOCK_3D_INSET = 2;
    constexpr int BLOCK_3D_SHADOW = 2;
    
    constexpr int LOGO_WIDTH = 180;
    constexpr int LOGO_HEIGHT = 25;
    constexpr int LOGO_MARGIN = 10;
    constexpr int LOGO_NUM_LETTERS = 9;
    constexpr int LOGO_BLOCK_BORDER = 2;
    constexpr int LOGO_TEXT_X_OFFSET = 4;
    constexpr int LOGO_TEXT_Y_OFFSET = 5;
    
    constexpr int PLAYER_ONE = 0;
    constexpr int PLAYER_TWO = 1;
    constexpr int NUM_PLAYERS = 2;
    
    constexpr int INVALID_BLOCK_ID = -1;
    constexpr int INITIAL_BLOCK_ID = 0;
    constexpr int INITIAL_SCORE = 0;
    constexpr int NEXT_PIECE_PADDING = 8;
}