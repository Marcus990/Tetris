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
    
    // Graphics settings
    constexpr int GRAPHICS_BLOCK_SIZE = 25;
    constexpr int GRAPHICS_PADDING = 10;
    constexpr int GRAPHICS_WINDOW_WIDTH = 600;
    constexpr int GRAPHICS_WINDOW_HEIGHT = 550;
    
    // Player indices
    constexpr int PLAYER_ONE = 0;
    constexpr int PLAYER_TWO = 1;
    constexpr int NUM_PLAYERS = 2;
    
    // Invalid/default values
    constexpr int INVALID_BLOCK_ID = -1;
    constexpr int INITIAL_BLOCK_ID = 0;
    constexpr int INITIAL_SCORE = 0;

    // Padding values for text display

    constexpr int NEXT_PIECE_PADDING = 8;
}