# 🏦 Bank Management App

A simple banking system built with C++ where you can create accounts, deposit money, withdraw money, transfer funds, and check your balance.

---

## 💻 What You Need Before Starting

- A computer (Windows, Mac, or Linux)
- G++ compiler installed

### How to install G++ on Windows:
1. Download MinGW from https://www.mingw-w64.org
2. Install it and add it to your PATH
3. Open Command Prompt and type `g++ --version` to confirm

### How to install G++ on Mac:
1. Open Terminal
2. Type `xcode-select --install` and press Enter

### How to install G++ on Linux:
1. Open Terminal
2. Type `sudo apt install g++` and press Enter

---

## 📥 How to Download This Project

1. Click the green **Code** button on this page
2. Click **Download ZIP**
3. Extract the ZIP file on your computer
4. Open the extracted folder

---

## ▶️ How to Run the App

1. Open Terminal (Mac/Linux) or Command Prompt (Windows)
2. Navigate to the project folder. For example:
   cd Downloads/bank-management-app
3. Compile the code by typing:
   g++ -std=c++17 -o bank_app main.cpp Account.cpp Bank.cpp
4. Run the app:
   On Mac/Linux: ./bank_app
   On Windows: bank_app.exe

---

## 📋 How to Use the App

When you run the app you will see a menu like this:

1. Create New Account
2. Deposit
3. Withdraw
4. Check Balance
5. Transfer Funds
6. View Account Statement
7. Change PIN
8. Close Account
9. List All Accounts
0. Exit

### Creating your first account:
1. Press 1 and hit Enter
2. Type your full name and hit Enter
3. Set a 4 digit PIN like 1234 and hit Enter
4. Enter an opening amount or 0 if none and hit Enter
5. You will get an account number — write it down!

### Depositing money:
1. Press 2 and hit Enter
2. Enter your account number
3. Enter your PIN
4. Enter the amount to deposit

### Withdrawing money:
1. Press 3 and hit Enter
2. Enter your account number
3. Enter your PIN
4. Enter the amount to withdraw

### Checking your balance:
1. Press 4 and hit Enter
2. Enter your account number
3. Enter your PIN
4. Your balance will be shown

### Transferring money to another account:
1. Press 5 and hit Enter
2. Enter YOUR account number and PIN
3. Enter the DESTINATION account number
4. Enter the amount to transfer

### Viewing full transaction history:
1. Press 6 and hit Enter
2. Enter your account number and PIN
3. All your transactions will be displayed

### Exiting the app:
1. Press 0 and hit Enter
2. Your data is automatically saved!

---

## 💾 Where is my data saved?

All your account data is automatically saved in a file called bank_data.txt in the same folder. Next time you run the app your accounts will still be there!

---

## 🔒 Is my PIN safe?

Yes! Your PIN is never stored as plain text. It is scrambled using encryption so nobody can read it directly from the file.

---

## 👨‍💻 Built With

- C++ (C++17)
- Object Oriented Programming (OOP)
- File Handling

---

## 🙋 Need Help?

Open an Issue on this GitHub page and describe your problem. We will help you!
