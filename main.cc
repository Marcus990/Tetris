#include <iostream>
#include <string>
#include <cstdlib>
import game;
import command;

using namespace std;

int main(int argc, char* argv[]) {
    // Default settings
    bool textOnly = false;
    unsigned int seed = 0;
    string scriptFile1 = "sequence1.txt";
    string scriptFile2 = "sequence2.txt";
    int startLevel = 0;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];

        if (arg == "-text") {
            textOnly = true;
        } else if (arg == "-seed" && i + 1 < argc) {
            seed = stoul(argv[++i]);
        } else if (arg == "-scriptfile1" && i + 1 < argc) {
            scriptFile1 = argv[++i];
        } else if (arg == "-scriptfile2" && i + 1 < argc) {
            scriptFile2 = argv[++i];
        } else if (arg == "-startlevel" && i + 1 < argc) {
            startLevel = stoi(argv[++i]);
            if (startLevel < 0) startLevel = 0;
            if (startLevel > 4) startLevel = 4;
        }
    }

    // Create game
    Game game(seed, startLevel, scriptFile1, scriptFile2, textOnly);

    // Create command interpreter
    CommandInterpreter interpreter(&game);

    // Initial render
    game.render();

    // Main game loop
    string input;
    cout << "> ";
    while (game.isGameRunning() && cin >> input) {
        // Execute command
        interpreter.executeCommand(input);

        // Check if game ended
        if (!game.isGameRunning()) {
            cout << "Game Over!\n";
            break;
        }
        cout << "> ";
    }

    return 0;
}
