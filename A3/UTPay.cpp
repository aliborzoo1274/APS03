#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cmath>
using namespace std;

const char FIRST_DELIMITER = ',';
const char SECOND_DELIMITER = ';';
const char THIRD_DELIMITER = ':';
const string FIRST_SIGN = "->";
const char SECOND_SIGN = ':';

typedef pair<string, double> Pair; //<name:cost>

struct Expenses
{
    string type;
    vector<Pair> payers;
    vector<Pair> borrowers;
};

struct Checkout
{
    string debtor;
    string creditor;
    double amount;
};

vector<string> readUsersFile(char *argv[]);
vector<Expenses> readExpensesFile(char *argv[]);
vector<Pair> calcPayOrRec(const vector<string> &usersList, const vector<Expenses> &expensesList);
void calcEqually(vector<Pair> &payOrRecList, const Expenses &expenses);
void calcUnequally(vector<Pair> &payOrRecList, const Expenses &expenses);
void calcAdjustment(vector<Pair> &payOrRecList, const Expenses &expenses);
vector<Pair> sortCreditors(vector<Pair> payOrRecList);
vector<Pair> sortDebtors(vector<Pair> payOrRecList);
vector<Checkout> checkout(vector<Pair> creditorsList, vector<Pair> debtorsList);
vector<Checkout> sortCheckout(vector<Checkout> checkoutList);
void printResult(const vector<Checkout> &sortedCheckoutList);

int main(int argc, char *argv[])
{
    vector<string> usersList = readUsersFile(argv);
    vector<Expenses> expensesList = readExpensesFile(argv);
    vector<Pair> payOrRecList = calcPayOrRec(usersList, expensesList);
    vector<Pair> creditorsList = sortCreditors(payOrRecList);
    vector<Pair> debtorsList = sortDebtors(payOrRecList);
    vector<Checkout> checkoutList = checkout(creditorsList, debtorsList);
    vector<Checkout> sortedCheckoutList = sortCheckout(checkoutList);
    printResult(sortedCheckoutList);
    return 0;
}

vector<string> readUsersFile(char *argv[])
{
    ifstream users(argv[1]);
    string name;
    vector<string> usersList;
    bool firstLine = true;
    while (getline(users, name))
    {
        if (firstLine)
        {
            firstLine = false;
            continue;
        }
        usersList.push_back(name);
    }
    users.close();
    return usersList;
}

vector<Expenses> readExpensesFile(char *argv[])
{
    ifstream expenses(argv[2]);
    int numOfLines = 0;
    int lineNum = 0;
    int dataKindCounter;
    int payerAndCostDelimiter, borrowerAndCostDelimiter;
    string line, lineParameters, payerAndCost, borrowerAndCost;
    string payerOrCost, borrowerOrCost, payer, borrower;
    double cost;
    while (getline(expenses, line))
    {
        numOfLines++;
    }
    numOfLines--;
    vector<Expenses> expensesList(numOfLines);
    expenses.clear();
    expenses.seekg(0);
    bool firstLine = true;
    while (getline(expenses, line))
    {
        if (firstLine)
        {
            firstLine = false;
            continue;
        }
        stringstream lineStream(line);
        dataKindCounter = 0;
        while (getline(lineStream, lineParameters, FIRST_DELIMITER))
        {
            if (dataKindCounter == 0)
                expensesList[lineNum].type = lineParameters;
            if (dataKindCounter == 1)
            {
                stringstream payersStream(lineParameters);
                while (getline(payersStream, payerAndCost, SECOND_DELIMITER))
                {
                    stringstream payerAndCostStream(payerAndCost);
                    payerAndCostDelimiter = 0;
                    while (getline(payerAndCostStream, payerOrCost, THIRD_DELIMITER))
                    {
                        if (payerAndCostDelimiter == 0)
                            payer = payerOrCost;
                        if (payerAndCostDelimiter == 1)
                            cost = stof(payerOrCost);
                        payerAndCostDelimiter++;
                    }
                    expensesList[lineNum].payers.push_back({payer, cost});
                    cost = 0;
                }
            }
            if (dataKindCounter == 2)
            {
                stringstream borrowersStream(lineParameters);
                while (getline(borrowersStream, borrowerAndCost, SECOND_DELIMITER))
                {
                    stringstream borrowerAndCostStream(borrowerAndCost);
                    borrowerAndCostDelimiter = 0;
                    while (getline(borrowerAndCostStream, borrowerOrCost, THIRD_DELIMITER))
                    {
                        if (borrowerAndCostDelimiter == 0)
                            borrower = borrowerOrCost;
                        if (borrowerAndCostDelimiter == 1)
                            cost = stof(borrowerOrCost);
                        borrowerAndCostDelimiter++;
                    }
                    expensesList[lineNum].borrowers.push_back({borrower, cost});
                    cost = 0;
                }
            }
            dataKindCounter++;
        }
        lineNum++;
    }
    expenses.close();
    return expensesList;
}

vector<Pair> calcPayOrRec(const vector<string> &usersList, const vector<Expenses> &expensesList)
{
    vector<Pair> payOrRecList(usersList.size());
    for (int i = 0; i < usersList.size(); i++)
    {
        payOrRecList[i].first = usersList[i];
        payOrRecList[i].second = 0;
    }
    for (int i = 0; i < expensesList.size(); i++)
    {
        if (expensesList[i].type == "equally")
            calcEqually(payOrRecList, expensesList[i]);
        else if (expensesList[i].type == "unequally")
            calcUnequally(payOrRecList, expensesList[i]);
        else if (expensesList[i].type == "adjustment")
            calcAdjustment(payOrRecList, expensesList[i]);
    }
    return payOrRecList;
}

void calcEqually(vector<Pair> &payOrRecList, const Expenses &expenses)
{
    double totalPay = 0;
    for (int i = 0; i < expenses.payers.size(); i++)
    {
        for (int j = 0; j < payOrRecList.size(); j++)
        {
            if (payOrRecList[j].first == expenses.payers[i].first)
            {
                payOrRecList[j].second += expenses.payers[i].second;
                totalPay += expenses.payers[i].second;
            }
        }
    }
    if (expenses.borrowers.empty())
    {
        for (int i = 0; i < payOrRecList.size() - 1; i++)
            payOrRecList[i].second -= floor(totalPay / payOrRecList.size() * 100) / 100;
        payOrRecList[payOrRecList.size() - 1].second -= totalPay -
        floor(totalPay / payOrRecList.size() * 100) / 100 * (payOrRecList.size() - 1);
    }
    else
    {
        for (int i = 0; i < expenses.borrowers.size() - 1; i++)
        {
            for (int j = 0; j < payOrRecList.size(); j++)
            {
                if (payOrRecList[j].first == expenses.borrowers[i].first)
                {
                    payOrRecList[j].second -= floor(totalPay /
                    expenses.borrowers.size() * 100) / 100;
                }
            }
        }
        for (int i = 0; i < payOrRecList.size(); i++)
        {
            if (payOrRecList[i].first == expenses.borrowers[expenses.borrowers.size() - 1].first)
                payOrRecList[i].second -= totalPay -
                floor(totalPay / expenses.borrowers.size() * 100) / 100 * (expenses.borrowers.size() - 1);
        }
    }
}

void calcUnequally(vector<Pair> &payOrRecList, const Expenses &expenses)
{
    for (int i = 0; i < expenses.payers.size(); i++)
    {
        for (int j = 0; j < payOrRecList.size(); j++)
        {
            if (payOrRecList[j].first == expenses.payers[i].first)
                payOrRecList[j].second += expenses.payers[i].second;
        }
    }
    for (int i = 0; i < expenses.borrowers.size(); i++)
    {
        for (int j = 0; j < payOrRecList.size(); j++)
        {
            if (payOrRecList[j].first == expenses.borrowers[i].first)
                payOrRecList[j].second -= expenses.borrowers[i].second;
        }
    }
}

void calcAdjustment(vector<Pair> &payOrRecList, const Expenses &expenses)
{
    double totalPay = 0;
    calcUnequally(payOrRecList, expenses);
    for (int i = 0; i < expenses.payers.size(); i++)
        totalPay += expenses.payers[i].second;
    for (int i = 0; i < expenses.borrowers.size(); i++)
        totalPay -= expenses.borrowers[i].second;
    for (int i = 0; i < payOrRecList.size(); i++)
    {
        if (payOrRecList[i].first != expenses.borrowers[expenses.borrowers.size() - 1].first)
            payOrRecList[i].second -= floor(totalPay / payOrRecList.size() * 100) / 100;
    }
    for (int i = 0; i < payOrRecList.size(); i++)
    {
        if (payOrRecList[i].first == expenses.borrowers[expenses.borrowers.size() - 1].first)
            payOrRecList[i].second -= totalPay -
            floor(totalPay / payOrRecList.size() * 100) / 100 * (payOrRecList.size() - 1);
    }
}

vector<Pair> sortCreditors(vector<Pair> payOrRecList)
{
    double credit = 0;
    int numOfCreditors = 0;
    string user, longerName;
    vector<Pair> creditorsList;
    for (int i = 0; i < payOrRecList.size(); i++)
    {
        if (payOrRecList[i].second > 0)
            numOfCreditors++;
    }
    for (int i = 0; i < numOfCreditors; i++)
    {
        for (int j = 0; j < payOrRecList.size(); j++)
        {
            if (payOrRecList[j].second > credit)
            {
                credit = payOrRecList[j].second;
                user = payOrRecList[j].first;
            }
            else if (payOrRecList[j].second == credit)
            {
                longerName = user;
                if (user.size() < payOrRecList[j].first.size())
                    longerName = payOrRecList[j].first;
                for (int z = 0; z < longerName.size(); z++)
                {
                    if (user[z] == payOrRecList[j].first[z]) continue;
                    else if (payOrRecList[j].first[z] < user[z])
                    {
                        user = payOrRecList[j].first;
                        break;
                    }
                    else break;
                }
            }
        }
        creditorsList.push_back({user, credit});
        for (int j = 0; j < payOrRecList.size(); j++)
        {
            if (payOrRecList[j].first == user)
                payOrRecList.erase(payOrRecList.begin() + j);
        }
        credit = 0;
    }
    return creditorsList;
}

vector<Pair> sortDebtors(vector<Pair> payOrRecList)
{
    double debt = 0;
    int numOfDebtors = 0;
    string user, longerName;
    vector<Pair> debtorsList;
    for (int i = 0; i < payOrRecList.size(); i++)
    {
        if (payOrRecList[i].second < 0)
            numOfDebtors++;
    }
    for (int i = 0; i < numOfDebtors; i++)
    {
        for (int j = 0; j < payOrRecList.size(); j++)
        {
            if (payOrRecList[j].second < debt)
            {
                debt = payOrRecList[j].second;
                user = payOrRecList[j].first;
            }
            else if (payOrRecList[j].second == debt)
            {
                longerName = user;
                if (user.size() < payOrRecList[j].first.size())
                    longerName = payOrRecList[j].first;
                for (int z = 0; z < longerName.size(); z++)
                {
                    if (user[z] == payOrRecList[j].first[z]) continue;
                    else if (payOrRecList[j].first[z] < user[z])
                    {
                        user = payOrRecList[j].first;
                        break;
                    }
                    else break;
                }
            }
        }
        debtorsList.push_back({user, abs(debt)});
        for (int j = 0; j < payOrRecList.size(); j++)
        {
            if (payOrRecList[j].first == user)
                payOrRecList.erase(payOrRecList.begin() + j);
        }
        debt = 0;
    }
    return debtorsList;
}

vector<Checkout> checkout(vector<Pair> creditorsList, vector<Pair> debtorsList)
{
    vector<Checkout> checkoutList;
    while (!debtorsList.empty() && !creditorsList.empty())
    {
        if (creditorsList[0].second > debtorsList[0].second)
        {
            creditorsList[0].second -= debtorsList[0].second;
            checkoutList.push_back({debtorsList[0].first, creditorsList[0].first,
            round(debtorsList[0].second * 100) / 100});
            debtorsList.erase(debtorsList.begin());
        }
        else if (creditorsList[0].second == debtorsList[0].second)
        {
            creditorsList[0].second -= debtorsList[0].second;
            checkoutList.push_back({debtorsList[0].first, creditorsList[0].first,
            round(debtorsList[0].second * 100) / 100});
            debtorsList.erase(debtorsList.begin());
            creditorsList.erase(creditorsList.begin());
        }
        else
        {
            debtorsList[0].second -= creditorsList[0].second;
            checkoutList.push_back({debtorsList[0].first, creditorsList[0].first,
            round(creditorsList[0].second * 100) / 100});
            creditorsList.erase(creditorsList.begin());
        }
    }
    return checkoutList;
}

vector<Checkout> sortCheckout(vector<Checkout> checkoutList)
{
    double amount = 0;
    int numOfPayments = checkoutList.size();
    string debtor, creditor, longerName;
    vector<Checkout> sortedCheckoutList;
    for (int i = 0; i < numOfPayments; i++)
    {
        for (int j = 0; j < checkoutList.size(); j++)
        {
            if (checkoutList[j].amount > amount)
            {
                amount = checkoutList[j].amount;
                debtor = checkoutList[j].debtor;
                creditor = checkoutList[j].creditor;
            }
            else if (checkoutList[j].amount == amount)
            {
                if (checkoutList[j].debtor != debtor)
                {
                    longerName = debtor;
                    if (debtor.size() < checkoutList[j].debtor.size())
                        longerName = checkoutList[j].debtor;
                    for (int z = 0; z < longerName.size(); z++)
                    {
                        if (debtor[z] == checkoutList[j].debtor[z]) continue;
                        else if (checkoutList[j].debtor[z] < debtor[z])
                        {
                            debtor = checkoutList[j].debtor;
                            creditor = checkoutList[j].creditor;
                            break;
                        }
                        else break;
                    }
                }
                else
                {
                    longerName = creditor;
                    if (creditor.size() < checkoutList[j].creditor.size())
                        longerName = checkoutList[j].creditor;
                    for (int z = 0; z < longerName.size(); z++)
                    {
                        if (creditor[z] == checkoutList[j].creditor[z]) continue;
                        else if (checkoutList[j].creditor[z] < creditor[z])
                        {
                            debtor = checkoutList[j].debtor;
                            creditor = checkoutList[j].creditor;
                            break;
                        }
                        else break;
                    }
                }
            }
        }
        sortedCheckoutList.push_back({debtor, creditor, amount});
        for (int j = 0; j < checkoutList.size(); j++)
        {
            if (checkoutList[j].debtor == debtor &&
            checkoutList[j].creditor == creditor)
                checkoutList.erase(checkoutList.begin() + j);
        }
        amount = 0;
    }
    return sortedCheckoutList;
}

void printResult(const vector<Checkout> &sortedCheckoutList)
{
    for (int i = 0; i < sortedCheckoutList.size(); i++)
    {
        cout << sortedCheckoutList[i].debtor << ' ' <<
        FIRST_SIGN << ' ' << sortedCheckoutList[i].creditor <<
        SECOND_SIGN << ' ' << sortedCheckoutList[i].amount << endl;
    }
}