#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

class BaseAccount
{
protected:
    int uniqueAccountNumber;
    string ownerFullName;
    double accountBalanceValue;
    unsigned int accessPermissionBits;
    vector<double> accountTransactionList;
    vector<unsigned int> encodedTransactionData;

public:
    BaseAccount(int id, string name, double balance, unsigned int permissions)
    {
        uniqueAccountNumber = id;
        ownerFullName = name;
        accountBalanceValue = balance;
        accessPermissionBits = permissions;
    }

    virtual string fetchAccountCategory() = 0;

    int getAccountNumber()
    {
        return uniqueAccountNumber;
    }

    double getBalanceAmount()
    {
        return accountBalanceValue;
    }

    unsigned int encodeTransactionData(unsigned int type, unsigned int amount)
    {
        unsigned int encodedValue = (type << 28) | (amount & 0x0FFFFFFF);
        return encodedValue;
    }

    void addTransactionRecord(double value, unsigned int type)
    {
        accountTransactionList.push_back(value);
        encodedTransactionData.push_back(encodeTransactionData(type, (unsigned int)value));
    }

    virtual void performDeposit(double amountValue) = 0;
    virtual void performWithdrawal(double amountValue) = 0;

    void printAccountInfo()
    {
        cout << "\nAccount Number: " << uniqueAccountNumber << endl;
        cout << "Owner: " << ownerFullName << endl;
        cout << "Balance: " << accountBalanceValue << endl;
        cout << "Permissions: " << accessPermissionBits << endl;

        cout << "Transaction History: ";
        for (int i = 0; i < accountTransactionList.size(); i++)
            cout << accountTransactionList[i] << " ";
        cout << endl;
    }

    void writeAccountToFile(ofstream &fileWriter)
    {
        fileWriter << "ACCOUNT " << fetchAccountCategory() << endl;

        fileWriter << uniqueAccountNumber << " "
                   << ownerFullName << " "
                   << accountBalanceValue << " "
                   << accessPermissionBits << endl;

        fileWriter << "TRANSACTIONS" << endl;

        for (int i = 0; i < accountTransactionList.size(); i++)
            fileWriter << accountTransactionList[i] << endl;
    }

    virtual ~BaseAccount() {}
};

class SavingsTypeAccount : public BaseAccount
{
public:
    SavingsTypeAccount(int id, string name, double balance, unsigned int permissions)
        : BaseAccount(id, name, balance, permissions) {}

    string fetchAccountCategory()
    {
        return "Savings";
    }

    void performDeposit(double amountValue)
    {
        if (!(accessPermissionBits & 2))
        {
            cout << "Deposit not allowed.\n";
            return;
        }

        accountBalanceValue += amountValue;
        addTransactionRecord(amountValue, 1);
    }

    void performWithdrawal(double amountValue)
    {
        if (!(accessPermissionBits & 1))
        {
            cout << "Withdraw not allowed.\n";
            return;
        }

        if (amountValue > accountBalanceValue)
        {
            cout << "Not enough balance.\n";
            return;
        }

        accountBalanceValue -= amountValue;
        addTransactionRecord(-amountValue, 2);
    }
};

class CurrentTypeAccount : public BaseAccount
{
public:
    CurrentTypeAccount(int id, string name, double balance, unsigned int permissions)
        : BaseAccount(id, name, balance, permissions) {}

    string fetchAccountCategory()
    {
        return "Current";
    }

    void performDeposit(double amountValue)
    {
        if (!(accessPermissionBits & 2))
        {
            cout << "Deposit not allowed.\n";
            return;
        }

        accountBalanceValue += amountValue;
        addTransactionRecord(amountValue, 1);
    }

    void performWithdrawal(double amountValue)
    {
        if (!(accessPermissionBits & 1))
        {
            cout << "Withdraw not allowed.\n";
            return;
        }

        accountBalanceValue -= amountValue;
        addTransactionRecord(-amountValue, 2);
    }
};

BaseAccount* locateAccount(vector<BaseAccount*> &accountStorageVector, int accountIdValue)
{
    for (int i = 0; i < accountStorageVector.size(); i++)
    {
        if (accountStorageVector[i]->getAccountNumber() == accountIdValue)
            return accountStorageVector[i];
    }
    return NULL;
}

void storeAccountsInFile(vector<BaseAccount*> &accountStorageVector)
{
    ofstream bankFileWriter("bankdata.txt");

    for (int i = 0; i < accountStorageVector.size(); i++)
        accountStorageVector[i]->writeAccountToFile(bankFileWriter);

    bankFileWriter.close();
}

void calculateYearlyMonthlySummary(vector<BaseAccount*> &accountStorageVector)
{
    double monthlyBalanceChange[12] = {0};

    for (int i = 0; i < accountStorageVector.size(); i++)
    {
        double currentBalance = accountStorageVector[i]->getBalanceAmount();
        monthlyBalanceChange[0] += currentBalance;
    }

    cout << "\nMonthly Summary\n";
    for (int i = 0; i < 12; i++)
        cout << "Month " << i + 1 << " : " << monthlyBalanceChange[i] << endl;
}

void createNewAccount(vector<BaseAccount*> &accountStorageVector)
{
    int accountTypeChoice;
    int id;
    string name;
    double balance;
    unsigned int permissions;

    cout << "1 Savings  2 Current: ";
    cin >> accountTypeChoice;

    cout << "Enter ID Name Balance Permissions: ";
    cin >> id >> name >> balance >> permissions;

    BaseAccount* createdAccountPointer;

    if (accountTypeChoice == 1)
        createdAccountPointer = new SavingsTypeAccount(id, name, balance, permissions);
    else
        createdAccountPointer = new CurrentTypeAccount(id, name, balance, permissions);

    accountStorageVector.push_back(createdAccountPointer);
}

void depositMoney(vector<BaseAccount*> &accountStorageVector)
{
    int id;
    double amount;

    cout << "Enter ID and amount: ";
    cin >> id >> amount;

    BaseAccount* accountPointer = locateAccount(accountStorageVector, id);

    if (accountPointer)
        accountPointer->performDeposit(amount);
}

void withdrawMoney(vector<BaseAccount*> &accountStorageVector)
{
    int id;
    double amount;

    cout << "Enter ID and amount: ";
    cin >> id >> amount;

    BaseAccount* accountPointer = locateAccount(accountStorageVector, id);

    if (accountPointer)
        accountPointer->performWithdrawal(amount);
}

void showAllAccounts(vector<BaseAccount*> &accountStorageVector)
{
    for (int i = 0; i < accountStorageVector.size(); i++)
        accountStorageVector[i]->printAccountInfo();
}

int main()
{
    vector<BaseAccount*> accountStorageVector;
    int menuSelection;

    while (true)
    {
        cout << "\n1 Create Account\n";
        cout << "2 Deposit\n";
        cout << "3 Withdraw\n";
        cout << "4 Show Accounts\n";
        cout << "5 Save File\n";
        cout << "6 Monthly Summary\n";
        cout << "7 Exit\n";
        cout << "Choice: ";

        cin >> menuSelection;

        if (menuSelection == 1)
            createNewAccount(accountStorageVector);

        else if (menuSelection == 2)
            depositMoney(accountStorageVector);

        else if (menuSelection == 3)
            withdrawMoney(accountStorageVector);

        else if (menuSelection == 4)
            showAllAccounts(accountStorageVector);

        else if (menuSelection == 5)
            storeAccountsInFile(accountStorageVector);

        else if (menuSelection == 6)
            calculateYearlyMonthlySummary(accountStorageVector);

        else if (menuSelection == 7)
            break;
    }

    for (int i = 0; i < accountStorageVector.size(); i++)
        delete accountStorageVector[i];

    accountStorageVector.clear();

    return 0;
}