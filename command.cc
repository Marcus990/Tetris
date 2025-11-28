export module command;
import <string>;
import <map>;
import <vector>;
import <memory>;
import game;

// Abstract Command class
export class Command {
public:
    virtual ~Command() = default;
    virtual void execute(Game* game) = 0;
    virtual bool canMultiply() const;
};

// Movement commands
export class LeftCommand : public Command {
public:
    void execute(Game* game) override;
};

export class RightCommand : public Command {
public:
    void execute(Game* game) override;
};

export class DownCommand : public Command {
public:
    void execute(Game* game) override;
};

export class DropCommand : public Command {
public:
    void execute(Game* game) override;
};

// Rotation commands
export class RotateClockwiseCommand : public Command {
public:
    void execute(Game* game) override;
};

export class RotateCounterClockwiseCommand : public Command {
public:
    void execute(Game* game) override;
};

// Level commands
export class LevelUpCommand : public Command {
public:
    void execute(Game* game) override;
};

export class LevelDownCommand : public Command {
public:
    void execute(Game* game) override;
};

// Random mode commands
export class RandomCommand : public Command {
public:
    void execute(Game* game) override;
    bool canMultiply() const override;
};

export class NoRandomCommand : public Command {
public:
    void execute(Game* game) override;
    bool canMultiply() const override;
};

// Restart command
export class RestartCommand : public Command {
public:
    void execute(Game* game) override;
    bool canMultiply() const override;
};

// Sequence command - execute commands from file
export class SequenceCommand : public Command {
public:
    void execute(Game* game) override;
    bool canMultiply() const override;
};

// Test commands - replace current block with specified type
export class IBlockCommand : public Command {
public:
    void execute(Game* game) override;
    bool canMultiply() const override;
};

export class JBlockCommand : public Command {
public:
    void execute(Game* game) override;
    bool canMultiply() const override;
};

export class LBlockCommand : public Command {
public:
    void execute(Game* game) override;
    bool canMultiply() const override;
};

export class OBlockCommand : public Command {
public:
    void execute(Game* game) override;
    bool canMultiply() const override;
};

export class SBlockCommand : public Command {
public:
    void execute(Game* game) override;
    bool canMultiply() const override;
};

export class ZBlockCommand : public Command {
public:
    void execute(Game* game) override;
    bool canMultiply() const override;
};

export class TBlockCommand : public Command {
public:
    void execute(Game* game) override;
    bool canMultiply() const override;
};

// Help command
export class HelpCommand : public Command {
public:
    void execute(Game*) override;
    bool canMultiply() const override;
};

// Command Interpreter
export class CommandInterpreter {
    std::map<std::string, std::unique_ptr<Command>> commands;
    Game* game;

public:
    CommandInterpreter(Game* g);
    void registerCommands();
    std::string matchCommand(const std::string& prefix);
    void executeCommand(const std::string& input);
    void executeSequenceFile(const std::string& filename);
};
