#include "Bank.h"
#include <iostream>
#include <fstream>
#include <iomanip>

// ══════════════════════════════════════════════
//  Constructor / Destructor
// ══════════════════════════════════════════════
Bank::Bank(const std::string& dataFile)
    : m_dataFile(dataFile), m_nextAccountNumber(100001)
{
    load();  // hydrate from file on startup
}

Bank::~Bank() {
    save();  // persist on shutdown
}

// ══════════════════════════════════════════════
//  Private: authenticate helper
// ══════════════════════════════════════════════
bool Bank::authenticate(long long accountNumber,
                        const std::string& pin,
                        Account*& outAccount)
{
    auto it = m_accounts.find(accountNumber);
    if (it == m_accounts.end()) {
        std::cout << "  [!] Account " << accountNumber << " not found.\n";
        return false;
    }
    if (!it->second.verifyPin(pin)) {
        std::cout << "  [!] Incorrect PIN.\n";
        return false;
    }
    outAccount = &it->second;
    return true;
}

// ══════════════════════════════════════════════
//  Account Management
// ══════════════════════════════════════════════
long long Bank::createAccount(const std::string& holderName,
                               const std::string& pin,
                               double initialDeposit)
{
    if (pin.length() < 4) {
        std::cout << "  [!] PIN must be at least 4 digits.\n";
        return -1;
    }
    long long accNum = m_nextAccountNumber++;
    m_accounts.emplace(accNum, Account(accNum, holderName, pin, initialDeposit));
    save();
    std::cout << "  [✓] Account created! Your account number: " << accNum << "\n";
    return accNum;
}

bool Bank::deleteAccount(long long accountNumber, const std::string& pin) {
    Account* acc = nullptr;
    if (!authenticate(accountNumber, pin, acc)) return false;
    m_accounts.erase(accountNumber);
    save();
    std::cout << "  [✓] Account " << accountNumber << " closed.\n";
    return true;
}

Account* Bank::findAccount(long long accountNumber) {
    auto it = m_accounts.find(accountNumber);
    return (it != m_accounts.end()) ? &it->second : nullptr;
}

long long Bank::getNextAccountNumber() const { return m_nextAccountNumber; }

// ══════════════════════════════════════════════
//  Authenticated Operations
// ══════════════════════════════════════════════
bool Bank::deposit(long long accountNumber,
                   const std::string& pin, double amount)
{
    Account* acc = nullptr;
    if (!authenticate(accountNumber, pin, acc)) return false;
    bool ok = acc->deposit(amount);
    if (ok) {
        std::cout << "  [✓] Deposited $" << std::fixed << std::setprecision(2)
                  << amount << ". New balance: $" << acc->getBalance() << "\n";
        save();
    }
    return ok;
}

bool Bank::withdraw(long long accountNumber,
                    const std::string& pin, double amount)
{
    Account* acc = nullptr;
    if (!authenticate(accountNumber, pin, acc)) return false;
    bool ok = acc->withdraw(amount);
    if (ok) {
        std::cout << "  [✓] Withdrew $" << std::fixed << std::setprecision(2)
                  << amount << ". New balance: $" << acc->getBalance() << "\n";
        save();
    }
    return ok;
}

bool Bank::transfer(long long fromAcc, const std::string& pin,
                    long long toAcc,   double amount)
{
    Account* src = nullptr;
    if (!authenticate(fromAcc, pin, src)) return false;

    auto it = m_accounts.find(toAcc);
    if (it == m_accounts.end()) {
        std::cout << "  [!] Destination account " << toAcc << " not found.\n";
        return false;
    }
    if (!src->withdraw(amount)) return false;
    it->second.deposit(amount);
    save();
    std::cout << "  [✓] Transferred $" << std::fixed << std::setprecision(2)
              << amount << " from " << fromAcc << " to " << toAcc << ".\n";
    return true;
}

void Bank::showBalance(long long accountNumber, const std::string& pin) {
    Account* acc = nullptr;
    if (!authenticate(accountNumber, pin, acc)) return;
    std::cout << "  [✓] Balance for account " << accountNumber
              << ": $" << std::fixed << std::setprecision(2)
              << acc->getBalance() << "\n";
}

void Bank::showStatement(long long accountNumber, const std::string& pin) {
    Account* acc = nullptr;
    if (!authenticate(accountNumber, pin, acc)) return;
    acc->printStatement();
}

bool Bank::changePin(long long accountNumber,
                     const std::string& oldPin, const std::string& newPin)
{
    Account* acc = nullptr;
    if (!authenticate(accountNumber, oldPin, acc)) return false;
    bool ok = acc->changePin(oldPin, newPin);
    if (ok) save();
    return ok;
}

// ══════════════════════════════════════════════
//  File I/O
// ══════════════════════════════════════════════
void Bank::save() const {
    std::ofstream file(m_dataFile);
    if (!file) {
        std::cerr << "  [!] Could not open " << m_dataFile << " for writing.\n";
        return;
    }
    // First line: next account number
    file << m_nextAccountNumber << "\n";
    for (const auto& [num, acc] : m_accounts) {
        file << acc.serialize() << "\n";
    }
}

void Bank::load() {
    std::ifstream file(m_dataFile);
    if (!file) return;  // no data file yet → fresh start

    std::string line;
    // First line: next account number
    if (std::getline(file, line) && !line.empty())
        m_nextAccountNumber = std::stoll(line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        try {
            Account acc = Account::deserialize(line);
            m_accounts[acc.getAccountNumber()] = acc;
        } catch (...) {
            std::cerr << "  [!] Skipping corrupt record.\n";
        }
    }
    std::cout << "  [✓] Loaded " << m_accounts.size() << " account(s) from file.\n";
}

// ══════════════════════════════════════════════
//  Admin Display
// ══════════════════════════════════════════════
void Bank::listAllAccounts() const {
    if (m_accounts.empty()) {
        std::cout << "  No accounts found.\n";
        return;
    }
    std::cout << "\n  ┌──────────────┬──────────────────────────────┬─────────────┐\n"
              << "  │ Account #    │ Holder Name                  │ Balance     │\n"
              << "  ├──────────────┼──────────────────────────────┼─────────────┤\n";
    for (const auto& [num, acc] : m_accounts) {
        std::cout << "  │ " << std::left  << std::setw(12) << num
                  << " │ " << std::left  << std::setw(28) << acc.getHolderName()
                  << " │ $" << std::right << std::setw(10)
                  << std::fixed << std::setprecision(2) << acc.getBalance()
                  << " │\n";
    }
    std::cout << "  └──────────────┴──────────────────────────────┴─────────────┘\n\n";
}
