#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>

// ─────────────────────────────────────────────
//  Transaction – records every account activity
// ─────────────────────────────────────────────
struct Transaction {
    std::string type;      // "DEPOSIT" | "WITHDRAWAL" | "CREATED"
    double      amount;
    double      balanceAfter;
    std::string timestamp;

    std::string serialize()   const;
    static Transaction deserialize(const std::string& line);
};

// ─────────────────────────────────────────────
//  Account – core banking entity
// ─────────────────────────────────────────────
class Account {
public:
    // Constructors
    Account();
    Account(long long accountNumber,
            const std::string& holderName,
            const std::string& pin,
            double initialBalance = 0.0);

    // ── Core operations ──────────────────────
    bool   deposit   (double amount);
    bool   withdraw  (double amount);
    double getBalance() const;

    // ── PIN verification ─────────────────────
    bool   verifyPin (const std::string& inputPin) const;
    bool   changePin (const std::string& oldPin, const std::string& newPin);

    // ── Getters ──────────────────────────────
    long long          getAccountNumber() const;
    std::string        getHolderName()    const;
    std::string        getAccountType()   const;
    std::vector<Transaction> getHistory() const;

    // ── Persistence ──────────────────────────
    std::string serialize()   const;
    static Account deserialize(const std::string& line);

    // ── Display ──────────────────────────────
    void printStatement() const;
    void printSummary()   const;

private:
    long long   m_accountNumber;
    std::string m_holderName;
    std::string m_pin;          // stored as simple hash (XOR + base64-like)
    double      m_balance;
    std::vector<Transaction> m_history;

    std::string hashPin(const std::string& pin) const;
    std::string currentTimestamp() const;
    void        recordTransaction(const std::string& type, double amount);
};

#endif // ACCOUNT_H
