export module scorekeeper;

export class ScoreKeeper {
    int currentScore;
    int highScore;

public:
    ScoreKeeper() : currentScore(0), highScore(0) {}

    int getCurrentScore() const { return currentScore; }
    int getHighScore() const { return highScore; }

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

    void reset() {
        currentScore = 0;
        // High score persists
    }
};
