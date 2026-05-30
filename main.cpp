#include "Bank.h"
#include <iostream>
#include <limits>
#include <string>
#include <iomanip>

// ─────────────────────────────────────────────
//  Input helpers
// ─────────────────────────────────────────────
void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

long long readAccountNumber() {
    long long n;
    std::cout << "  Enter account number: ";
    std::cin >> n;
    clearInput();
    return n;
}

std::string readPin(const std::string& prompt = "  Enter PIN: ") {
    std::string pin;
    std::cout << prompt;
    std::cin >> pin;
    clearInput();
    return pin;
}

double readAmount(const std::string& prompt = "  Enter amount: $") {
    double amt;
    std::cout << prompt;
    std::cin >> amt;
    clearInput();
    return amt;
}

// ─────────────────────────────────────────────
//  Banner
// ─────────────────────────────────────────────
void printBanner() {
    std::cout << R"(
  ╔═══════════════════════════════════════════════════╗
  ║        C++ BANK MANAGEMENT SYSTEM  v1.0           ║
  ║        Secure · Reliable · OOP-Powered            ║
  ╚═══════════════════════════════════════════════════╝
)";
}

void printMainMenu() {
    std::cout << R"(
  ════════════════ MAIN MENU ════════════════
   1. Create New Account
   2. Deposit
   3. Withdraw
   4. Check Balance
   5. Transfer Funds
   6. View Account Statement
   7. Change PIN
   8. Close Account
   9. List All Accounts  (Admin)
   0. Exit
  ═══════════════════════════════════════════
  Choice: )";
}

// ─────────────────────────────────────────────
//  Menu Handlers
// ─────────────────────────────────────────────
void handleCreateAccount(Bank& bank) {
    std::cout << "\n  ── Create New Account ──\n";
    std::string name, pin;
    double initial = 0.0;

    std::cout << "  Full name: ";
    std::getline(std::cin, name);

    pin = readPin("  Set 4-digit PIN: ");

    std::cout << "  Initial deposit (0 for none): $";
    std::cin >> initial;
    clearInput();

    bank.createAccount(name, pin, initial);
}

void handleDeposit(Bank& bank) {
    std::cout << "\n  ── Deposit ──\n";
    long long acc = readAccountNumber();
    std::string pin = readPin();
    double amt      = readAmount();
    bank.deposit(acc, pin, amt);
}

void handleWithdraw(Bank& bank) {
    std::cout << "\n  ── Withdrawal ──\n";
    long long acc   = readAccountNumber();
    std::string pin = readPin();
    double amt      = readAmount();
    bank.withdraw(acc, pin, amt);
}

void handleBalance(Bank& bank) {
    std::cout << "\n  ── Balance Inquiry ──\n";
    long long acc   = readAccountNumber();
    std::string pin = readPin();
    bank.showBalance(acc, pin);
}

void handleTransfer(Bank& bank) {
    std::cout << "\n  ── Fund Transfer ──\n";
    std::cout << "  FROM ";
    long long from  = readAccountNumber();
    std::string pin = readPin();
    std::cout << "  TO ";
    long long to    = readAccountNumber();
    double amt      = readAmount();
    bank.transfer(from, pin, to, amt);
}

void handleStatement(Bank& bank) {
    std::cout << "\n  ── Account Statement ──\n";
    long long acc   = readAccountNumber();
    std::string pin = readPin();
    bank.showStatement(acc, pin);
}

void handleChangePin(Bank& bank) {
    std::cout << "\n  ── Change PIN ──\n";
    long long acc      = readAccountNumber();
    std::string oldPin = readPin("  Current PIN: ");
    std::string newPin = readPin("  New PIN    : ");
    bank.changePin(acc, oldPin, newPin);
}

void handleCloseAccount(Bank& bank) {
    std::cout << "\n  ── Close Account ──\n";
    std::cout << "  WARNING: This is irreversible!\n";
    long long acc   = readAccountNumber();
    std::string pin = readPin("  Confirm PIN: ");
    bank.deleteAccount(acc, pin);
}

// ─────────────────────────────────────────────
//  Main
// ─────────────────────────────────────────────
int main() {
    printBanner();

    Bank bank("bank_data.txt");   // loads existing data automatically

    int choice = -1;
    while (choice != 0) {
        printMainMenu();
        std::cin >> choice;
        clearInput();

        switch (choice) {
            case 1: handleCreateAccount(bank); break;
            case 2: handleDeposit(bank);       break;
            case 3: handleWithdraw(bank);      break;
            case 4: handleBalance(bank);       break;
            case 5: handleTransfer(bank);      break;
            case 6: handleStatement(bank);     break;
            case 7: handleChangePin(bank);     break;
            case 8: handleCloseAccount(bank);  break;
            case 9: bank.listAllAccounts();    break;
            case 0:
                std::cout << "\n  [✓] Data saved. Goodbye!\n\n";
                break;
            default:
                std::cout << "  [!] Invalid option. Try again.\n";
        }
    }
    return 0;
}