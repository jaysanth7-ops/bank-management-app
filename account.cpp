#include "Account.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include <algorithm>

// ══════════════════════════════════════════════
//  Transaction helpers
// ══════════════════════════════════════════════
std::string Transaction::serialize() const {
    // Format: TYPE|amount|balanceAfter|timestamp
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << type << "|" << amount << "|" << balanceAfter << "|" << timestamp;
    return oss.str();
}

Transaction Transaction::deserialize(const std::string& line) {
    Transaction t;
    std::istringstream iss(line);
    std::string token;
    std::getline(iss, t.type,        '|');
    std::getline(iss, token,         '|'); t.amount       = std::stod(token);
    std::getline(iss, token,         '|'); t.balanceAfter = std::stod(token);
    std::getline(iss, t.timestamp);
    return t;
}

// ══════════════════════════════════════════════
//  Account – private helpers
// ══════════════════════════════════════════════
std::string Account::hashPin(const std::string& pin) const {
    // Simple obfuscation: XOR each char with account-number-derived key,
    // then encode as hex string.
    unsigned char key = static_cast<unsigned char>(m_accountNumber % 251);
    std::ostringstream oss;
    for (char c : pin) {
        oss << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(static_cast<unsigned char>(c) ^ key);
    }
    return oss.str();
}

std::string Account::currentTimestamp() const {
    auto now  = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm* tm_info = std::localtime(&time);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    return std::string(buf);
}

void Account::recordTransaction(const std::string& type, double amount) {
    Transaction t;
    t.type         = type;
    t.amount       = amount;
    t.balanceAfter = m_balance;
    t.timestamp    = currentTimestamp();
    m_history.push_back(t);
}

// ══════════════════════════════════════════════
//  Constructors
// ══════════════════════════════════════════════
Account::Account()
    : m_accountNumber(0), m_balance(0.0) {}

Account::Account(long long accountNumber,
                 const std::string& holderName,
                 const std::string& pin,
                 double initialBalance)
    : m_accountNumber(accountNumber),
      m_holderName(holderName),
      m_balance(initialBalance)
{
    m_pin = hashPin(pin);
    recordTransaction("CREATED", initialBalance);
}

// ══════════════════════════════════════════════
//  Core Operations
// ══════════════════════════════════════════════
bool Account::deposit(double amount) {
    if (amount <= 0) {
        std::cout << "  [!] Deposit amount must be positive.\n";
        return false;
    }
    m_balance += amount;
    recordTransaction("DEPOSIT", amount);
    return true;
}

bool Account::withdraw(double amount) {
    if (amount <= 0) {
        std::cout << "  [!] Withdrawal amount must be positive.\n";
        return false;
    }
    if (amount > m_balance) {
        std::cout << "  [!] Insufficient funds. Available: $"
                  << std::fixed << std::setprecision(2) << m_balance << "\n";
        return false;
    }
    m_balance -= amount;
    recordTransaction("WITHDRAWAL", amount);
    return true;
}

double Account::getBalance() const { return m_balance; }

// ══════════════════════════════════════════════
//  PIN Operations
// ══════════════════════════════════════════════
bool Account::verifyPin(const std::string& inputPin) const {
    return hashPin(inputPin) == m_pin;
}

bool Account::changePin(const std::string& oldPin, const std::string& newPin) {
    if (!verifyPin(oldPin)) {
        std::cout << "  [!] Incorrect current PIN.\n";
        return false;
    }
    if (newPin.length() < 4) {
        std::cout << "  [!] PIN must be at least 4 digits.\n";
        return false;
    }
    m_pin = hashPin(newPin);
    std::cout << "  [✓] PIN changed successfully.\n";
    return true;
}

// ══════════════════════════════════════════════
//  Getters
// ══════════════════════════════════════════════
long long   Account::getAccountNumber() const { return m_accountNumber; }
std::string Account::getHolderName()    const { return m_holderName; }
std::vector<Transaction> Account::getHistory() const { return m_history; }

// ══════════════════════════════════════════════
//  Persistence (CSV-style line per account)
//  FORMAT: accNum|name|pin|balance|TXN1;TXN2;...
// ══════════════════════════════════════════════
std::string Account::serialize() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << m_accountNumber << "|"
        << m_holderName    << "|"
        << m_pin           << "|"
        << m_balance       << "|";
    for (size_t i = 0; i < m_history.size(); ++i) {
        if (i > 0) oss << ";";
        oss << m_history[i].serialize();
    }
    return oss.str();
}

Account Account::deserialize(const std::string& line) {
    Account a;
    std::istringstream iss(line);
    std::string token;

    std::getline(iss, token, '|'); a.m_accountNumber = std::stoll(token);
    std::getline(iss, a.m_holderName, '|');
    std::getline(iss, a.m_pin,        '|');
    std::getline(iss, token,           '|'); a.m_balance = std::stod(token);

    std::string txnBlock;
    std::getline(iss, txnBlock);
    if (!txnBlock.empty()) {
        std::istringstream txnStream(txnBlock);
        std::string txnLine;
        while (std::getline(txnStream, txnLine, ';')) {
            if (!txnLine.empty())
                a.m_history.push_back(Transaction::deserialize(txnLine));
        }
    }
    return a;
}

// ══════════════════════════════════════════════
//  Display
// ══════════════════════════════════════════════
void Account::printSummary() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Account # : " << m_accountNumber   << "\n"
              << "  Holder    : " << m_holderName       << "\n"
              << "  Balance   : $" << m_balance         << "\n";
}

void Account::printStatement() const {
    std::cout << "\n"
              << "  ╔══════════════════════════════════════════════╗\n"
              << "  ║          ACCOUNT STATEMENT                   ║\n"
              << "  ╠══════════════════════════════════════════════╣\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  ║  Account # : " << std::setw(10) << m_accountNumber
              << "                    ║\n"
              << "  ║  Holder    : " << std::left << std::setw(30) << m_holderName
              << " ║\n" << std::right
              << "  ║  Balance   : $" << std::setw(10) << m_balance
              << "                   ║\n"
              << "  ╠══════════════════════════════════════════════╣\n"
              << "  ║  Date/Time           Type        Amount      ║\n"
              << "  ╠══════════════════════════════════════════════╣\n";

    for (const auto& t : m_history) {
        std::cout << "  ║  " << std::left << std::setw(20) << t.timestamp
                  << std::setw(12) << t.type
                  << "$" << std::right << std::setw(9) << std::fixed
                  << std::setprecision(2) << t.amount << "  ║\n";
    }
    std::cout << "  ╚══════════════════════════════════════════════╝\n\n";
}
