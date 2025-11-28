export module scorekeeper;

export class ScoreKeeper {
    int currentScore;
    int highScore;
    int wins;

public:
    ScoreKeeper() : currentScore(0), highScore(0), wins(0) {}

    // Getter methods
    int getCurrentScore() const { return currentScore; }
    int getHighScore() const { return highScore; }
    int getWins() const { return wins; }

    // Add points to current score and update high score if needed
    void addScore(int points) {
        currentScore += points;
        if (currentScore > highScore) {
            highScore = currentScore;
        }
    }

    // Calculate points for clearing lines
    int calculateLineClearPoints(int level, int linesCleared) const {
        int points = (level + linesCleared);
        return points * points;
    }

    // Calculate points for completely removing a block
    int calculateBlockRemovalPoints(int levelGenerated) const {
        int points = levelGenerated + 1;
        return points * points;
    }

    // Reset current score (high score and wins remain)
    void reset() {
        currentScore = 0;
    }

    // Increment win counter
    void incrementWins() {
        wins++;
    }
};
