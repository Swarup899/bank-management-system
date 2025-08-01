#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class Account {
    int accountNumber;
    char name[50];
    char type;
    double balance;

public:
    void createAccount();
    void showAccount() const;
    void modify();
    void deposit(double);
    void withdraw(double);
    void report() const;
    int getAccountNumber() const;
    double getBalance() const;
    char getType() const;
    void applyInterest();
};

void Account::createAccount() {
    cout << "\nEnter Account No.: ";
    cin >> accountNumber;
    cout << "Enter Account Holder Name: ";
    cin.ignore();
    cin.getline(name, 50);

    do {
        cout << "Enter Account Type (C/S): ";
        cin >> type;
        type = toupper(type);
    } while (type != 'C' && type != 'S');

    do {
        cout << "Enter Initial Deposit (>=500 for Saving, >=1000 for Current): ";
        cin >> balance;
        if ((type == 'S' && balance < 500) || (type == 'C' && balance < 1000))
            cout << "Invalid initial deposit amount. Please try again.\n";
    } while ((type == 'S' && balance < 500) || (type == 'C' && balance < 1000));

    cout << "\nAccount Created Successfully!";
}

void Account::showAccount() const {
    cout << "\nAccount No.: " << accountNumber;
    cout << "\nAccount Holder Name: " << name;
    cout << "\nType of Account: " << type;
    cout << "\nBalance Amount: " << balance;
}

void Account::modify() {
    cout << "\nModify Account Holder Name: ";
    cin.ignore();
    cin.getline(name, 50);
    do {
        cout << "Modify Account Type (C/S): ";
        cin >> type;
        type = toupper(type);
    } while (type != 'C' && type != 'S');

    cout << "Modify Balance Amount: ";
    cin >> balance;
}

void Account::deposit(double amount) {
    balance += amount;
}

void Account::withdraw(double amount) {
    balance -= amount;
}

void Account::report() const {
    cout << setw(10) << accountNumber << setw(20) << name << setw(10)
         << type << setw(10) << balance << endl;
}

int Account::getAccountNumber() const {
    return accountNumber;
}

double Account::getBalance() const {
    return balance;
}

char Account::getType() const {
    return type;
}

void Account::applyInterest() {
    double rate = (type == 'S') ? 0.05 : 0.03;
    balance += balance * rate;
}

// Function declarations
void writeAccount();
void displayAccount(int);
void modifyAccount(int);
void deleteAccount(int);
void displayAll();
void depositWithdraw(int, int);
void applyInterestToAll();

int main() {
    int choice;
    int accNo;
    do {
        cout << "\n\n\tBank Management System";
        cout << "\n\n1. New Account";
        cout << "\n2. Deposit Amount";
        cout << "\n3. Withdraw Amount";
        cout << "\n4. Balance Enquiry";
        cout << "\n5. All Account Holder List";
        cout << "\n6. Close Account";
        cout << "\n7. Modify Account";
        cout << "\n8. Apply Interest to All Accounts";
        cout << "\n9. Exit";
        cout << "\n\nSelect Your Option (1-9): ";
        cin >> choice;

        switch (choice) {
        case 1:
            writeAccount();
            break;
        case 2:
            cout << "\nEnter Account No.: ";
            cin >> accNo;
            depositWithdraw(accNo, 1);
            break;
        case 3:
            cout << "\nEnter Account No.: ";
            cin >> accNo;
            depositWithdraw(accNo, 2);
            break;
        case 4:
            cout << "\nEnter Account No.: ";
            cin >> accNo;
            displayAccount(accNo);
            break;
        case 5:
            displayAll();
            break;
        case 6:
            cout << "\nEnter Account No.: ";
            cin >> accNo;
            deleteAccount(accNo);
            break;
        case 7:
            cout << "\nEnter Account No.: ";
            cin >> accNo;
            modifyAccount(accNo);
            break;
        case 8:
            applyInterestToAll();
            break;
        case 9:
            cout << "\nThanks for using Bank Management System!\n";
            break;
        default:
            cout << "\nInvalid Option!";
        }
    } while (choice != 9);
    return 0;
}

// Definitions

void writeAccount() {
    Account acc;
    ofstream outFile("bank.dat", ios::binary | ios::app);
    acc.createAccount();
    outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
    outFile.close();
}

void displayAccount(int n) {
    Account acc;
    bool found = false;
    ifstream inFile("bank.dat", ios::binary);

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccountNumber() == n) {
            acc.showAccount();
            found = true;
        }
    }

    inFile.close();
    if (!found)
        cout << "\nAccount Not Found!";
}

void modifyAccount(int n) {
    Account acc;
    bool found = false;
    fstream file("bank.dat", ios::binary | ios::in | ios::out);

    while (!file.eof() && !found) {
        int pos = file.tellg();
        if (file.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
            if (acc.getAccountNumber() == n) {
                acc.showAccount();
                cout << "\nEnter New Details:\n";
                acc.modify();
                file.seekp(pos);
                file.write(reinterpret_cast<char*>(&acc), sizeof(Account));
                cout << "\nRecord Updated!";
                found = true;
            }
        }
    }

    file.close();
    if (!found)
        cout << "\nRecord Not Found!";
}

void deleteAccount(int n) {
    Account acc;
    ifstream inFile("bank.dat", ios::binary);
    ofstream outFile("Temp.dat", ios::binary);
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccountNumber() != n) {
            outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
        } else {
            found = true;
        }
    }

    inFile.close();
    outFile.close();
    remove("bank.dat");
    rename("Temp.dat", "bank.dat");

    if (found)
        cout << "\nRecord Deleted Successfully!";
    else
        cout << "\nRecord Not Found!";
}

void displayAll() {
    Account acc;
    ifstream inFile("bank.dat", ios::binary);
    cout << "\n\n\tACCOUNT HOLDER LIST\n\n";
    cout << "=====================================================\n";
    cout << setw(10) << "Acc No." << setw(20) << "Name" << setw(10)
         << "Type" << setw(10) << "Balance\n";
    cout << "=====================================================\n";

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        acc.report();
    }

    inFile.close();
}

void depositWithdraw(int accNo, int option) {
    Account acc;
    fstream file("bank.dat", ios::binary | ios::in | ios::out);
    bool found = false;
    double amount;

    if (!file) {
        cout << "\nError opening file!";
        return;
    }

    while (!file.eof() && !found) {
        int pos = file.tellg();
        if (file.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
            if (acc.getAccountNumber() == accNo) {
                acc.showAccount();

                if (option == 1) {
                    do {
                        cout << "\nEnter Amount to Deposit: ";
                        cin >> amount;
                        if (amount <= 0)
                            cout << "Amount must be positive. Try again.\n";
                    } while (amount <= 0);
                    acc.deposit(amount);
                    cout << "\nDeposit Successful!";
                } else {
                    do {
                        cout << "\nEnter Amount to Withdraw: ";
                        cin >> amount;
                        if (amount <= 0)
                            cout << "Amount must be positive. Try again.\n";
                        else if (amount > acc.getBalance())
                            cout << "Insufficient Balance! Try a smaller amount.\n";
                    } while (amount <= 0 || amount > acc.getBalance());
                    acc.withdraw(amount);
                    cout << "\nWithdrawal Successful!";
                }

                file.seekp(pos);
                file.write(reinterpret_cast<char*>(&acc), sizeof(Account));
                found = true;
            }
        }
    }

    file.close();
    if (!found)
        cout << "\nAccount Not Found!";
}

void applyInterestToAll() {
    Account acc;
    fstream file("bank.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "Could not open file.\n";
        return;
    }

    while (!file.eof()) {
        int pos = file.tellg();
        if (file.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
            acc.applyInterest();
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&acc), sizeof(Account));
        }
    }

    file.close();
    cout << "\nInterest applied to all accounts.";
}