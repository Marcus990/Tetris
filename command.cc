module;
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <cctype>
#include <fstream>
export module command;
import game;
import board;
import level;
import constants;

using namespace GameConstants;

// Abstract Command class
export class Command {
public:
    virtual ~Command() = default;
    virtual void execute(Game* game) = 0;
    virtual bool canMultiply() const { return true; }
};

// Movement commands
export class LeftCommand : public Command {
public:
    void execute(Game* game) override {
        Board* board = game->getCurrentBoard();
        board->moveLeft();

        // Apply heavy effect if active
        Level* level = game->getCurrentLevel();
        if (level->isHeavy()) {
            board->moveDown();
        }
    }
};

export class RightCommand : public Command {
public:
    void execute(Game* game) override {
        Board* board = game->getCurrentBoard();
        board->moveRight();

        // Apply heavy effect if active
        Level* level = game->getCurrentLevel();
        if (level->isHeavy()) {
            board->moveDown();
        }
    }
};

export class DownCommand : public Command {
public:
    void execute(Game* game) override {
        Board* board = game->getCurrentBoard();
        board->moveDown();
    }
};

export class DropCommand : public Command {
public:
    void execute(Game* game) override {
        game->drop();
    }
};

// Rotation commands
export class RotateClockwiseCommand : public Command {
public:
    void execute(Game* game) override {
        Board* board = game->getCurrentBoard();
        board->rotate(true);

        // Apply heavy effect if active
        Level* level = game->getCurrentLevel();
        if (level->isHeavy()) {
            board->moveDown();
        }
    }
};

export class RotateCounterClockwiseCommand : public Command {
public:
    void execute(Game* game) override {
        Board* board = game->getCurrentBoard();
        board->rotate(false);

        // Apply heavy effect if active
        Level* level = game->getCurrentLevel();
        if (level->isHeavy()) {
            board->moveDown();
        }
    }
};

// Level commands
export class LevelUpCommand : public Command {
public:
    void execute(Game* game) override {
        game->levelUp();
    }
};

export class LevelDownCommand : public Command {
public:
    void execute(Game* game) override {
        game->levelDown();
    }
};

// Random mode commands
export class RandomCommand : public Command {
public:
    void execute(Game* game) override {
        game->getCurrentLevel()->setRandom(true);
    }

    bool canMultiply() const override { return false; }
};

export class NoRandomCommand : public Command {
public:
    void execute(Game* game) override {
        std::string filename;
        std::cin >> filename;
        game->getCurrentLevel()->setRandom(false);
        game->getCurrentLevel()->setNonRandom(filename);
    }

    bool canMultiply() const override { return false; }
};

// Restart command
export class RestartCommand : public Command {
public:
    void execute(Game* game) override {
        game->restart();
    }

    bool canMultiply() const override { return false; }
};

// Sequence command - execute commands from file
// Note: This is handled specially in CommandInterpreter
export class SequenceCommand : public Command {
public:
    void execute(Game* game) override {
        // Actual execution handled in CommandInterpreter
    }

    bool canMultiply() const override { return false; }
};

// Test commands - replace current block with specified type
export class IBlockCommand : public Command {
public:
    void execute(Game* game) override {
        game->getCurrentBoard()->replaceCurrentBlock('I');
    }

    bool canMultiply() const override { return false; }
};

export class JBlockCommand : public Command {
public:
    void execute(Game* game) override {
        game->getCurrentBoard()->replaceCurrentBlock('J');
    }

    bool canMultiply() const override { return false; }
};

export class LBlockCommand : public Command {
public:
    void execute(Game* game) override {
        game->getCurrentBoard()->replaceCurrentBlock('L');
    }

    bool canMultiply() const override { return false; }
};

export class OBlockCommand : public Command {
public:
    void execute(Game* game) override {
        game->getCurrentBoard()->replaceCurrentBlock('O');
    }

    bool canMultiply() const override { return false; }
};

export class SBlockCommand : public Command {
public:
    void execute(Game* game) override {
        game->getCurrentBoard()->replaceCurrentBlock('S');
    }

    bool canMultiply() const override { return false; }
};

export class ZBlockCommand : public Command {
public:
    void execute(Game* game) override {
        game->getCurrentBoard()->replaceCurrentBlock('Z');
    }

    bool canMultiply() const override { return false; }
};

export class TBlockCommand : public Command {
public:
    void execute(Game* game) override {
        game->getCurrentBoard()->replaceCurrentBlock('T');
    }

    bool canMultiply() const override { return false; }
};

// Help command
export class HelpCommand : public Command {
public:
    void execute(Game*) override {
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

    bool canMultiply() const override { return false; }
};

// Command Interpreter
export class CommandInterpreter {
    std::map<std::string, std::unique_ptr<Command>> commands;
    Game* game;

public:
    CommandInterpreter(Game* g) : game(g) {
        registerCommands();
    }

    void registerCommands() {
        commands["left"] = std::make_unique<LeftCommand>();
        commands["right"] = std::make_unique<RightCommand>();
        commands["down"] = std::make_unique<DownCommand>();
        commands["drop"] = std::make_unique<DropCommand>();
        commands["clockwise"] = std::make_unique<RotateClockwiseCommand>();
        commands["counterclockwise"] = std::make_unique<RotateCounterClockwiseCommand>();
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

    // Match command prefix to full command name
    std::string matchCommand(const std::string& prefix) {
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

    void executeCommand(const std::string& input) {
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
        // Note: Only drop command switches players (done inside drop() method)
        // Render after command execution
        game->render();
    }

    void executeSequenceFile(const std::string& filename) {
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
};
