#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <cmath>
using namespace std;

// This class helps us safely get numbers and choices from the user
class InputHelper {
public:
    // Ask the user to enter a number and make sure it's valid
    double getNumber(const string& prompt) {
        double number;
        while (true) {
            cout << prompt;
            cin >> number;
            if (cin.fail()) {
                cin.clear(); // Clear the error so we can read input again
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Throw away the bad input
                cout << "Invalid input. Please enter a valid number.\n";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clean up the input buffer
                return number;
            }
        }
    }

    // Show a list of operations and ask the user to choose one
    int getOperationChoice() {
        int choice;
        while (true) {
            cout << "Choose an operation:\n";
            cout << "1. Add\n2. Subtract\n3. Multiply\n4. Divide\n5. Modulus\nYour choice (1-5): ";
            cin >> choice;
            if (cin.fail() || choice < 1 || choice > 5) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Get rid of bad input
                cout << "Please enter a number between 1 and 5.\n";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return choice;
            }
        }
    }
};

// A general blueprint for doing math operations
class Operation {
public:
    virtual ~Operation() = default;
    virtual double execute(double a, double b) const = 0; // Perform the operation
    virtual string getName() const = 0; // Return the name of the operation
};

// This one adds two numbers
class Addition : public Operation {
public:
    double execute(double a, double b) const override {
        return a + b;
    }
    string getName() const override {
        return "Addition";
    }
};

// This one subtracts the second number from the first
class Subtraction : public Operation {
public:
    double execute(double a, double b) const override {
        return a - b;
    }
    string getName() const override {
        return "Subtraction";
    }
};

// This multiplies two numbers together
class Multiplication : public Operation {
public:
    double execute(double a, double b) const override {
        return a * b;
    }
    string getName() const override {
        return "Multiplication";
    }
};

// This divides one number by another, carefully avoiding division by zero
class Division : public Operation {
public:
    double execute(double a, double b) const override {
        if (b == 0) throw runtime_error("Division by zero is not allowed.");
        return a / b;
    }
    string getName() const override {
        return "Division";
    }
};

// This finds the remainder when one whole number is divided by another
class Modulus : public Operation {
public:
    double execute(double a, double b) const override {
        if (floor(a) != a || floor(b) != b)
            throw runtime_error("Modulus only works with whole numbers.");
        if (b == 0)
            throw runtime_error("Modulus by zero is not allowed.");
        return static_cast<long long>(a) % static_cast<long long>(b);
    }
    string getName() const override {
        return "Modulus";
    }
};

// This is the heart of the calculator — it pulls everything together
class Calculator {
private:
    InputHelper input;     // Helper for getting user input
    double num1, num2;     // The two numbers for operations
    Operation* operation;  // Pointer to the chosen operation

    // Based on the user's choice, create the correct operation object
    void setOperation(int choice) {
        delete operation; // Clean up old operation
        switch (choice) {
            case 1: operation = new Addition(); break;
            case 2: operation = new Subtraction(); break;
            case 3: operation = new Multiplication(); break;
            case 4: operation = new Division(); break;
            case 5: operation = new Modulus(); break;
        }
    }

public:
    Calculator() : operation(nullptr) {} // Start with no operation
    ~Calculator() { delete operation; }  // Free memory when done

    // This function handles one calculation session
    void runCalculation() {
        num1 = input.getNumber("Enter the first number: ");
        num2 = input.getNumber("Enter the second number: ");
        int choice = input.getOperationChoice();
        setOperation(choice); // Set the right operation

        try {
            double result = operation->execute(num1, num2); // Try to calculate
            cout << fixed << setprecision(0);
            cout << "\nResult of " << operation->getName() << ":\n";
            cout << num1 << " " << getOperatorSymbol(choice) << " " << num2
                 << " = " << result << "\n" << endl;
            cout << resetiosflags(ios::fixed);
        } catch (const runtime_error& e) {
            cout << "\nError: " << e.what() << "\n" << endl;
        }
    }

private:
    // Show the right symbol for the operation
    string getOperatorSymbol(int choice) const {
        switch (choice) {
            case 1: return "+";
            case 2: return "-";
            case 3: return "*";
            case 4: return "/";
            case 5: return "%";
            default: return "";
        }
    }
};

// This is where the program starts
int main() {
    cout << "Welcome to the Calculator\n";

    Calculator calc;
    char tryAgain;

    // Allow multiple calculations
    do {
        calc.runCalculation();
        cout << "Would you like to calculate again? (y/n): ";
        cin >> tryAgain;
        cout << endl;
    } while (tryAgain == 'y' || tryAgain == 'Y');

    cout << "Thank you for using the calculator.\n";
    return 0;
}
