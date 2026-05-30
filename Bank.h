#ifndef BANK_H
#define BANK_H

#include "Account.h"
#include <map>
#include <string>

// ─────────────────────────────────────────────
//  Bank – manages all accounts + file I/O
// ─────────────────────────────────────────────
class Bank {
public:
    explicit Bank(const std::string& dataFile = "bank_data.txt");
    ~Bank();  // auto-saves on exit

    // ── Account management ───────────────────
    long long createAccount(const std::string& holderName,
                            const std::string& pin,
                            double initialDeposit = 0.0);
    bool      deleteAccount(long long accountNumber, const std::string& pin);
    Account*  findAccount  (long long accountNumber);

    // ── Authenticated operations ─────────────
    bool deposit   (long long accountNumber, const std::string& pin, double amount);
    bool withdraw  (long long accountNumber, const std::string& pin, double amount);
    bool transfer  (long long fromAcc, const std::string& pin,
                    long long toAcc,   double amount);
    void showBalance  (long long accountNumber, const std::string& pin);
    void showStatement(long long accountNumber, const std::string& pin);
    bool changePin    (long long accountNumber,
                       const std::string& oldPin, const std::string& newPin);

    // ── File I/O ────────────────────────────
    void save() const;
    void load();

    // ── Admin / display ──────────────────────
    void listAllAccounts() const;
    long long getNextAccountNumber() const;

private:
    std::map<long long, Account> m_accounts;  // accountNumber → Account
    std::string m_dataFile;
    long long   m_nextAccountNumber;

    bool authenticate(long long accountNumber,
                      const std::string& pin,
                      Account*& outAccount);
};

#endif // BANK_H
