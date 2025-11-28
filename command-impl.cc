module;
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <cctype>
#include <fstream>
module command;
import game;
import board;
import level;
import constants;

using namespace GameConstants;

// Command base class implementation
bool Command::canMultiply() const { return true; }

// LeftCommand implementation
void LeftCommand::execute(Game* game) {
    Board* board = game->getCurrentBoard();
    board->moveLeft();

    // Apply heavy effect if active (from level or special action)
    Level* level = game->getCurrentLevel();
    if (level->isHeavy() || board->hasHeavyEffect()) {
        board->moveDown();
    }
}

// RightCommand implementation
void RightCommand::execute(Game* game) {
    Board* board = game->getCurrentBoard();
    board->moveRight();

    // Apply heavy effect if active (from level or special action)
    Level* level = game->getCurrentLevel();
    if (level->isHeavy() || board->hasHeavyEffect()) {
        board->moveDown();
    }
}

// DownCommand implementation
void DownCommand::execute(Game* game) {
    Board* board = game->getCurrentBoard();
    board->moveDown();
}

// DropCommand implementation
void DropCommand::execute(Game* game) {
    game->drop();
}

// RotateClockwiseCommand implementation
void RotateClockwiseCommand::execute(Game* game) {
    Board* board = game->getCurrentBoard();
    board->rotate(true);

    // Apply heavy effect if active (from level or special action)
    Level* level = game->getCurrentLevel();
    if (level->isHeavy() || board->hasHeavyEffect()) {
        board->moveDown();
    }
}

// RotateCounterClockwiseCommand implementation
void RotateCounterClockwiseCommand::execute(Game* game) {
    Board* board = game->getCurrentBoard();
    board->rotate(false);

    // Apply heavy effect if active (from level or special action)
    Level* level = game->getCurrentLevel();
    if (level->isHeavy() || board->hasHeavyEffect()) {
        board->moveDown();
    }
}

// LevelUpCommand implementation
void LevelUpCommand::execute(Game* game) {
    game->levelUp();
}

// LevelDownCommand implementation
void LevelDownCommand::execute(Game* game) {
    game->levelDown();
}

// RandomCommand implementation
void RandomCommand::execute(Game* game) {
    game->getCurrentLevel()->setRandom(true);
}

bool RandomCommand::canMultiply() const { return false; }

// NoRandomCommand implementation
void NoRandomCommand::execute(Game* game) {
    std::string filename;
    std::cin >> filename;
    game->getCurrentLevel()->setRandom(false);
    game->getCurrentLevel()->setNonRandom(filename);
}

bool NoRandomCommand::canMultiply() const { return false; }

// RestartCommand implementation
void RestartCommand::execute(Game* game) {
    game->restart();
}

bool RestartCommand::canMultiply() const { return false; }

// SequenceCommand implementation
void SequenceCommand::execute(Game* game) {
    // Actual execution handled in CommandInterpreter
}

bool SequenceCommand::canMultiply() const { return false; }

// IBlockCommand implementation
void IBlockCommand::execute(Game* game) {
    game->getCurrentBoard()->replaceCurrentBlock('I');
}

bool IBlockCommand::canMultiply() const { return false; }

// JBlockCommand implementation
void JBlockCommand::execute(Game* game) {
    game->getCurrentBoard()->replaceCurrentBlock('J');
}

bool JBlockCommand::canMultiply() const { return false; }

// LBlockCommand implementation
void LBlockCommand::execute(Game* game) {
    game->getCurrentBoard()->replaceCurrentBlock('L');
}

bool LBlockCommand::canMultiply() const { return false; }

// OBlockCommand implementation
void OBlockCommand::execute(Game* game) {
    game->getCurrentBoard()->replaceCurrentBlock('O');
}

bool OBlockCommand::canMultiply() const { return false; }

// SBlockCommand implementation
void SBlockCommand::execute(Game* game) {
    game->getCurrentBoard()->replaceCurrentBlock('S');
}

bool SBlockCommand::canMultiply() const { return false; }

// ZBlockCommand implementation
void ZBlockCommand::execute(Game* game) {
    game->getCurrentBoard()->replaceCurrentBlock('Z');
}

bool ZBlockCommand::canMultiply() const { return false; }

// TBlockCommand implementation
void TBlockCommand::execute(Game* game) {
    game->getCurrentBoard()->replaceCurrentBlock('T');
}

bool TBlockCommand::canMultiply() const { return false; }

// HelpCommand implementation
void HelpCommand::execute(Game*) {
    std::cout << "╔════════════════════════════════════════╗\n";
    std::cout << "║      BIQUADRIS COMMANDS HELP           ║\n";
    std::cout << "╠════════════════════════════════════════╣\n";
    std::cout << "║ MOVEMENT:                              ║\n";
    std::cout << "║  left/lef      - Move block left       ║\n";
    std::cout << "║  right/ri      - Move block right      ║\n";
    std::cout << "║  down/do       - Move block down       ║\n";
    std::cout << "║  drop/dr       - Drop block            ║\n";
    std::cout << "║                                        ║\n";
    std::cout << "║ ROTATION:                              ║\n";
    std::cout << "║  clockwise/cw  - Rotate clockwise      ║\n";
    std::cout << "║  counterclockwise/cc - Rotate CCW      ║\n";
    std::cout << "║                                        ║\n";
    std::cout << "║ LEVEL:                                 ║\n";
    std::cout << "║  levelup/levelu   - Increase level     ║\n";
    std::cout << "║  leveldown/leveld - Decrease level     ║\n";
    std::cout << "║                                        ║\n";
    std::cout << "║ MODE:                                  ║\n";
    std::cout << "║  random/r     - Random blocks          ║\n";
    std::cout << "║  norandom/nor - Use sequence file      ║\n";
    std::cout << "║                                        ║\n";
    std::cout << "║ TESTING:                               ║\n";
    std::cout << "║  I,J,L,O,S,Z,T - Replace block         ║\n";
    std::cout << "║  sequence file - Run commands from file║\n";
    std::cout << "║                                        ║\n";
    std::cout << "║ GAME:                                  ║\n";
    std::cout << "║  restart       - Restart game          ║\n";
    std::cout << "║  help/h        - Show this help        ║\n";
    std::cout << "║                                        ║\n";
    std::cout << "║ TIP: Use numbers before commands!      ║\n";
    std::cout << "║      Example: 3left, 2down             ║\n";
    std::cout << "╚════════════════════════════════════════╝\n\n";
}

bool HelpCommand::canMultiply() const { return false; }

// CommandInterpreter implementation
CommandInterpreter::CommandInterpreter(Game* g) : game(g) {
    registerCommands();
}

void CommandInterpreter::registerCommands() {
    commands["left"] = std::make_unique<LeftCommand>();
    commands["right"] = std::make_unique<RightCommand>();
    commands["down"] = std::make_unique<DownCommand>();
    commands["drop"] = std::make_unique<DropCommand>();
    commands["clockwise"] = std::make_unique<RotateClockwiseCommand>();
    commands["cw"] = std::make_unique<RotateClockwiseCommand>();
    commands["counterclockwise"] = std::make_unique<RotateCounterClockwiseCommand>();
    commands["ccw"] = std::make_unique<RotateCounterClockwiseCommand>();
    commands["levelup"] = std::make_unique<LevelUpCommand>();
    commands["leveldown"] = std::make_unique<LevelDownCommand>();
    commands["random"] = std::make_unique<RandomCommand>();
    commands["norandom"] = std::make_unique<NoRandomCommand>();
    commands["restart"] = std::make_unique<RestartCommand>();
    commands["sequence"] = std::make_unique<SequenceCommand>();
    commands["help"] = std::make_unique<HelpCommand>();
    commands["I"] = std::make_unique<IBlockCommand>();
    commands["J"] = std::make_unique<JBlockCommand>();
    commands["L"] = std::make_unique<LBlockCommand>();
    commands["O"] = std::make_unique<OBlockCommand>();
    commands["S"] = std::make_unique<SBlockCommand>();
    commands["Z"] = std::make_unique<ZBlockCommand>();
    commands["T"] = std::make_unique<TBlockCommand>();
}

std::string CommandInterpreter::matchCommand(const std::string& prefix) {
    std::vector<std::string> matches;

    for (const auto& [name, cmd] : commands) {
        if (name.substr(0, prefix.size()) == prefix) {
            matches.push_back(name);
        }
    }

    // Return the match if it's unique
    if (matches.size() == 1) {
        return matches[0];
    }

    // If exact match exists, use it
    if (commands.count(prefix)) {
        return prefix;
    }

    return "";  // No unique match
}

void CommandInterpreter::executeCommand(const std::string& input) {
    // Parse multiplier and command
    int multiplier = 1;
    std::string commandStr = input;

    // Check for multiplier prefix
    size_t i = 0;
    while (i < input.size() && std::isdigit(input[i])) {
        i++;
    }

    if (i > 0) {
        multiplier = std::stoi(input.substr(0, i));
        commandStr = input.substr(i);
    }

    // Match command
    std::string fullCommand = matchCommand(commandStr);

    if (fullCommand.empty() || !commands.count(fullCommand)) {
        // Invalid command - silently ignore
        return;
    }

    Command* cmd = commands[fullCommand].get();

    // Special handling for sequence command
    if (fullCommand == "sequence") {
        std::string filename;
        std::cin >> filename;
        executeSequenceFile(filename);
        return;  // Don't render yet, sequence commands will render
    }

    // Execute command (respecting multiplier if allowed)
    if (cmd->canMultiply()) {
        for (int j = 0; j < multiplier; ++j) {
            cmd->execute(game);
        }
    } else {
        cmd->execute(game);
    }
    // Note: Drop command switches players internally (done inside drop() method)

    // Render after command execution
    game->render();
}

void CommandInterpreter::executeSequenceFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open sequence file: " << filename << "\n";
        return;
    }

    std::string command;
    while (file >> command) {
        executeCommand(command);
    }
}
