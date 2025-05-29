#include <iostream>
#include <limits>
#include <string>
using namespace std;

// This class handles user input and makes sure the input is valid (1 to 9).
class MoveValidator {
public:
    int getMove(const string& prompt) {
        int move;
        while (true) {
            cout << prompt;
            cin >> move;

            // If the input is invalid (non-number or out of range), ask again
            if (cin.fail() || move < 1 || move > 9) {
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore bad input
                cout << "Please enter a number between 1 and 9.\n";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return move;
            }
        }
    }
};

// Base class for any game (currently just Tic-Tac-Toe)
class BaseGame {
protected:
    MoveValidator validator; // Responsible for input validation
public:
    virtual ~BaseGame() = default; // Ensures cleanup if derived class is deleted via base pointer
    virtual void play() = 0; // Every game must define how to play
};

// The main Tic-Tac-Toe game class
class TicTacToe : public BaseGame {
private:
    char board[3][3];        // 3x3 grid
    char currentPlayer;      // 'X' or 'O'
    bool gameEnded;          // True when the game ends

    // Initializes the board with numbers 1 to 9
    void resetBoard() {
        int num = 1;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                board[i][j] = '0' + num++;
        currentPlayer = 'X';
        gameEnded = false;
    }

    // Displays the current state of the board
    void showBoard() const {
        cout << "\n";
        for (int i = 0; i < 3; ++i) {
            cout << " " << board[i][0] << " | " << board[i][1] << " | " << board[i][2] << "\n";
            if (i < 2) cout << "---+---+---\n";
        }
        cout << "\n";
    }

    // Converts move number to board indices
    bool getMoveCoordinates(int move, int& row, int& col) const {
        if (move < 1 || move > 9) return false;
        row = (move - 1) / 3;
        col = (move - 1) % 3;
        return true;
    }

    // Checks if a move is valid (not already taken)
    bool isMoveValid(int move) const {
        int row, col;
        if (!getMoveCoordinates(move, row, col)) return false;
        return board[row][col] != 'X' && board[row][col] != 'O';
    }

    // Updates the board with the current player's move
    bool makeMove(int move) {
        int row, col;
        if (!getMoveCoordinates(move, row, col)) return false;
        if (!isMoveValid(move)) {
            cout << "That spot’s already taken or invalid. Try again!\n";
            return false;
        }
        board[row][col] = currentPlayer;
        return true;
    }

    // Checks for winning conditions
    bool checkWin() const {
        for (int i = 0; i < 3; ++i)
            if (board[i][0] == currentPlayer && board[i][1] == currentPlayer && board[i][2] == currentPlayer)
                return true;

        for (int j = 0; j < 3; ++j)
            if (board[0][j] == currentPlayer && board[1][j] == currentPlayer && board[2][j] == currentPlayer)
                return true;

        if (board[0][0] == currentPlayer && board[1][1] == currentPlayer && board[2][2] == currentPlayer)
            return true;

        if (board[0][2] == currentPlayer && board[1][1] == currentPlayer && board[2][0] == currentPlayer)
            return true;

        return false;
    }

    // Checks if the board is completely filled (draw)
    bool checkDraw() const {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (board[i][j] != 'X' && board[i][j] != 'O')
                    return false;
        return true;
    }

    // Switches turn between players
    void switchPlayer() {
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    // Displays the outcome of the game
    virtual void showResult() const {
        if (checkWin())
            cout << "Player " << currentPlayer << " wins! Great game!\n";
        else if (checkDraw())
            cout << "It’s a tie! Well played, both of you!\n";
    }

public:
    TicTacToe() {
        resetBoard(); // Start fresh
    }

    // Main game loop
    void play() override {
        while (!gameEnded) {
            string prompt = "Player " + string(1, currentPlayer) + ", enter your move (1-9): ";
            int move = validator.getMove(prompt);

            if (makeMove(move)) {
                cout << "Nice move!\n";
                showBoard();

                if (checkWin() || checkDraw()) {
                    showResult();
                    gameEnded = true;
                } else {
                    switchPlayer();
                }
            } else {
                // Show the board again for reference if move was invalid
                showBoard();
            }
        }
    }
};

int main() {
    cout << "Welcome to Tic-Tac-Toe! Player 1 is X, Player 2 is O.\n";
    cout << "Use numbers (1-9) to choose a position:\n";
    cout << " 1 | 2 | 3 \n";
    cout << "---+---+---\n";
    cout << " 4 | 5 | 6 \n";
    cout << "---+---+---\n";
    cout << " 7 | 8 | 9 \n";

    char playAgain;
    do {
        TicTacToe game;
        game.play();
        cout << "Want to play another round? (y/n): ";
        cin >> playAgain;
        cout << endl;
    } while (playAgain == 'y' || playAgain == 'Y');

    cout << "Thanks for playing Tic-Tac-Toe! See you next time!\n";
    return 0;
}
