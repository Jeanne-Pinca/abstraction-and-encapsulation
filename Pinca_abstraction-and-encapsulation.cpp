#include <iostream>
#include <string>
#include <limits>
using namespace std;

// Abstract class representing a generic Account
class Account {
protected:
    double balance;

public:
    Account(double initialBalance) : balance(initialBalance) {}

    // Abstract methods for deposit, withdraw, and balance check
    virtual void deposit(double amount) = 0;
    virtual void withdraw(double amount) = 0;
    virtual double checkBalance() const = 0;

    virtual ~Account() {} // Virtual destructor for proper cleanup
};

// Class representing savings account
class SavingsAccount : public Account {
    const double MINIMUM_BALANCE = 1000;

public:
    // Constructor with default balance
    SavingsAccount() : Account(MINIMUM_BALANCE) {
        if (balance < MINIMUM_BALANCE) {
            balance = MINIMUM_BALANCE;  // Ensure minimum balance is enforced
        }
    }

    void deposit(double amount) override {
        balance += amount;
        cout << "----------------------------------------" << endl;
        cout << "> Your current savings balance is: " << balance << endl;
    }

    void withdraw(double amount) override {
        if (amount <= 0) {
            cout << "Invalid amount. Please enter a positive value!" << endl;
        } else if (balance - amount < MINIMUM_BALANCE) {
            cout << "Insufficient balance! Withdrawals would reduce your balance below the minimum allowed of " << MINIMUM_BALANCE << "!" << endl;
        } else {
            balance -= amount;
            cout << "----------------------------------------" << endl;
            cout << "> Your current savings balance is: " << balance << endl;
        }
    }

    double checkBalance() const override {
        return balance;
    }
};


// Class representing Current account
class CurrentAccount : public Account {
public:
    CurrentAccount(double initialBalance) : Account(initialBalance) {}

    void deposit(double amount) override {
        balance += amount;
        cout << "----------------------------------------" << endl;
        cout << "> Your current balance is: " << balance << endl;
    }

    void withdraw(double amount) override {
        if (amount <= balance) {
            balance -= amount;
            cout << "----------------------------------------" << endl;
            cout << "> Your current balance is: " << balance << endl;
        } else {
            cout << "Insufficient balance!" << endl;
        }
    }

    double checkBalance() const override {
        return balance;
    }
};

// Utility class for handling input validation 
class InputHandler {
public:
    static double getPositiveAmount() {
        double amount;
        do {
        
            cout << "\n[Amount]: ";
            cin >> amount;
            if (cin.fail() || amount <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n> Invalid input. Please enter a positive number.\n";
                cout << "----------------------------------------" << endl;
            }
        } while (amount <= 0);
        return amount;
    }

    static int getMenuChoice(int min, int max) {
        int choice;
        while (true) {
            cout << "\n[Choice]: ";
            cin >> choice;
            if (cin.fail() || choice < min || choice > max) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "> Invalid choice. Please try again." << endl;
                cout << "----------------------------------------" << endl;
            } else {
                break;
            }
        }
        return choice;
    }

    static void clearScreen() {
        system("cls");
    }
};

// Class for handling menus and user interaction
class Menu {
public:
    void showMainMenu() {
        InputHandler::clearScreen();
        printHeader("Main Menu");
        cout << "1 - Savings Account\n";
        cout << "2 - Current Account\n";
        cout << "3 - Exit\n";
    }

    void showAccountMenu(const string &accountType) {
        InputHandler::clearScreen();
        printHeader(accountType + " Account Menu");
        cout << "1 - Deposit\n";
        cout << "2 - Withdraw\n";
        cout << "3 - Check Balance\n";
        cout << "4 - Back\n";
    }

    void handleAccount(Account &account, const string &accountType) {
        int choice;
        do {
            showAccountMenu(accountType);
            choice = InputHandler::getMenuChoice(1, 4);
            switch (choice) {
                case 1:
                    handleTransaction(account, accountType, "Deposit", &Account::deposit);
                    break;
                case 2:
                    handleTransaction(account, accountType, "Withdrawal", &Account::withdraw);
                    break;
                case 3:
                    handleCheckBalance(account, accountType);
                    break;
                case 4:
                    cout << "Returning to main menu...\n";
                    break;
            }
        } while (choice != 4);
    }

private:
    // Generic transaction handler for both deposit and withdraw
    void handleTransaction(Account &account, const string &accountType, const string &action, void (Account::*transaction)(double)) {
        InputHandler::clearScreen();
        printHeader(action);
        cout << "Currently performing " << action << " in: " << accountType << " Account" << endl;
        cout << "\n> Current Balance: " << account.checkBalance() << endl;

        double amount = InputHandler::getPositiveAmount();
        (account.*transaction)(amount);  

        handleRepeatTransaction(account, accountType, action, transaction);
    }

    // Method to handle repeating transactions
    void handleRepeatTransaction(Account &account, const string &accountType, const string &action, void (Account::*transaction)(double)) {
        int choice;
        do {
            cout << "\n> Choose from the following:\n";
            cout << "1 - Make another " << action << "\n";
            cout << "2 - Go back to " << accountType << " account menu\n";
            choice = InputHandler::getMenuChoice(1, 2);

            if (choice == 1) {
                double amount = InputHandler::getPositiveAmount();
                (account.*transaction)(amount); 
            }
        } while (choice != 2);
    }

    void handleCheckBalance(Account &account, const string &accountType) {
        InputHandler::clearScreen();
        printHeader("Check Balance");
        cout << "> Your recent " << accountType << " account balance is: " << account.checkBalance() << endl;
        cout << "\nPress any key to continue..." << endl;
        cin.ignore();
        cin.get();
    }

    void printHeader(const string &title) {
        int totalWidth = 40;
        int padding = (totalWidth - title.length()) / 2;
        string separator(totalWidth, '=');

        cout << separator << endl;
        cout << string(padding, ' ') << title << endl;
        cout << separator << endl;
    }
};

// Main function 
int main() {
    SavingsAccount savingsAccount; // initialized with minimum balance of 1000
    CurrentAccount currentAccount(0);    

    Menu menu;
    int choice;

    do {
        menu.showMainMenu();
        choice = InputHandler::getMenuChoice(1, 3);

        switch (choice) {
            case 1:
                menu.handleAccount(savingsAccount, "Savings");
                break;
            case 2:
                menu.handleAccount(currentAccount, "Current");
                break;
            case 3:
                cout << "Terminating the program..." << endl;
                break;
        }
    } while (choice != 3);

    return 0;
}
