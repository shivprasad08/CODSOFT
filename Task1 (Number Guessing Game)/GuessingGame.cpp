#include <iostream>
#include <random>
#include <limits>
#include <cmath>
#include <string>
using namespace std;

// Holds settings for a game difficulty level
struct Difficulty {
    string levelName;
    int minNumber;
    int maxNumber;
    int maxGuesses;
};

// Manages random number generation, keeping it separate from game logic
class NumberPicker {
private:
    mt19937 engine; // Mersenne Twister for solid randomness

public:
    NumberPicker() {
        // Seed with random_device for unpredictable results
        engine = mt19937(random_device()());
    }

    // Picks a random number between min and max (inclusive)
    int pick(int min, int max) {
        uniform_int_distribution<int> dist(min, max);
        return dist(engine);
    }
};

// Base class for any guessing game, keeping things flexible
class BaseGame {
protected:
    Difficulty settings;
    int guessesMade;
    NumberPicker& picker; // Use composition for random numbers

public:
    BaseGame(NumberPicker& picker) : picker(picker), guessesMade(0) {}
    virtual ~BaseGame() = default; // Virtual destructor for safe inheritance

    // Pure virtual methods to enforce implementation in derived classes
    virtual void setupDifficulty() = 0;
    virtual bool runGame() = 0;
};

// Implements the number guessing game
class NumberGuesser : public BaseGame {
private:
    int targetNumber;

    // Sets up the random target number for the game
    void setTarget() {
        targetNumber = picker.pick(settings.minNumber, settings.maxNumber);
        // cout << "[DEBUG] Target is: " << targetNumber << endl; // For testing
    }

    // Checks if a guess is within the valid range
    bool isGuessValid(int guess) const {
        return guess >= settings.minNumber && guess <= settings.maxNumber;
    }

    // Gives the player a hint based on how close their guess is
    virtual void provideHint(int guess) const {
        int difference = abs(guess - targetNumber);
        if (difference <= 5) {
            cout << "You're super close!" << endl;
        } else if (difference <= 10) {
            cout << "Getting hot!" << endl;
        } else if (difference <= 20) {
            cout << "Warming up!" << endl;
        } else {
            cout << "Pretty far off!" << endl;
        }
    }

public:
    NumberGuesser(NumberPicker& picker) : BaseGame(picker) {}

    // Lets the player pick a difficulty level
    void setupDifficulty() override {
        int choice;
        cout << "Pick your challenge level:\n";
        cout << "1. Easy (Numbers 1 to 20, 7 guesses)\n";
        cout << "2. Medium (Numbers 1 to 50, 6 guesses)\n";
        cout << "3. Hard (Numbers 1 to 100, 5 guesses)\n";
        cout << "4. Expert (Numbers 1 to 150, 10 guesses)\n";
        cout << "Your choice (1-4): ";
        cin >> choice;

        // Set up the difficulty based on player choice
        switch (choice) {
            case 1:
                settings = {"Easy", 1, 20, 7};
                break;
            case 2:
                settings = {"Medium", 1, 50, 6};
                break;
            case 3:
                settings = {"Hard", 1, 100, 5};
                break;
            case 4:
                settings = {"Expert", 1, 150, 10};
                break;
            default:
                cout << "That’s not a valid choice. Let’s go with Easy.\n";
                settings = {"Easy", 1, 20, 7};
        }

        // Generate the target number and reset guesses
        setTarget();
        guessesMade = 0;

        cout << "\nYou’re playing " << settings.levelName << " mode!\n";
        cout << "I’ve picked a number between " << settings.minNumber << " and " << settings.maxNumber << ".\n";
        cout << "You’ve got " << settings.maxGuesses << " guesses to find it. Let’s go!\n" << endl;
    }

    // Runs the main game loop
    bool runGame() override {
        int guess;

        while (guessesMade < settings.maxGuesses) {
            cout << "What’s your guess? ";
            cin >> guess;

            // Handle invalid input (like letters)
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid number.\n" << endl;
                continue;
            }

            // Check if the guess is in the allowed range
            if (!isGuessValid(guess)) {
                cout << "Your guess should be between " << settings.minNumber
                     << " and " << settings.maxNumber << ".\n" << endl;
                continue;
            }

            guessesMade++;

            // Check if the guess is correct
            if (guess == targetNumber) {
                cout << "\nNailed it! You got it in " << guessesMade << " guess(es)!\n" << endl;
                return true;
            } else {
                if (guess < targetNumber) {
                    cout << "Too low!" << endl;
                } else {
                    cout << "Too high!" << endl;
                }

                provideHint(guess);
                cout << "Guesses left: " << settings.maxGuesses - guessesMade << "\n" << endl;
            }
        }

        // Out of guesses
        cout << "Out of guesses! The number was " << targetNumber << ".\n" << endl;
        return false;
    }
};

int main() {
    cout << "Welcome to the Guessing Number Challenge!\n";

    NumberPicker picker; // One random number generator for the whole game
    char replay;

    do {
        NumberGuesser game(picker); // Create a new game
        game.setupDifficulty();
        game.runGame();

        cout << "Want to try again? (y/n): ";
        cin >> replay;
        cout << endl;
    } while (replay == 'y' || replay == 'Y');

    cout << "Thanks for playing! Come back anytime.\n";
    return 0;
}