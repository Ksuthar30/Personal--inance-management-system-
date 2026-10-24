#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

const string DATA_DIR = "";

int readInt()
{
    int value;
    while (!(cin >> value))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter a number: ";
    }
    return value;
}

double readDouble()
{
    double value;
    while (!(cin >> value))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter a number: ";
    }
    return value;
}

double calculateEMI(double loanAmount, double annualRate, int months)
{
    double r = annualRate / 12.0 / 100.0;
    if (r == 0)
        return loanAmount / months;

    return (loanAmount * r * pow(1 + r, months)) /
           (pow(1 + r, months) - 1);
}

bool getMonthYear(string date, int &month, int &year)
{
    if (date.length() != 10 || date[2] != '/' || date[5] != '/')
        return false;

    for (int i = 0; i < 10; i++)
    {
        if (i == 2 || i == 5)
            continue;

        if (date[i] < '0' || date[i] > '9')
            return false;
    }

    month = (date[3] - '0') * 10 + (date[4] - '0');
    year = (date[6] - '0') * 1000 +
           (date[7] - '0') * 100 +
           (date[8] - '0') * 10 +
           (date[9] - '0');

    if (month < 1 || month > 12 || year < 1)
        return false;

    return true;
}

class Hello
{
private:
    string user, pass, old;
    double income;
    int noOfLoans;
    double loanAmount[20];
    double annualInterest[20];
    int loanMonths[20];
    double monthlyEMI[20];
    double totalEMI;
    bool profileLoaded;

    void saveProfile()
    {
        ofstream file(DATA_DIR + "profile.txt");
        if (!file)
        {
            cout << "Error saving profile!\n";
            return;
        }

        file << user << endl;
        file << pass << endl;
        file << income << endl;
        file << noOfLoans << endl;

        for (int i = 0; i < noOfLoans; i++)
        {
            file << loanAmount[i] << " "
                 << annualInterest[i] << " "
                 << loanMonths[i] << " "
                 << monthlyEMI[i] << endl;
        }

        file.close();
    }

    bool loadProfile()
    {
        ifstream file(DATA_DIR + "profile.txt");
        if (!file)
            return false;

        if (!(file >> user >> pass >> income >> noOfLoans))
            return false;

        if (noOfLoans < 0 || noOfLoans > 20)
            return false;

        totalEMI = 0;

        for (int i = 0; i < noOfLoans; i++)
        {
            if (!(file >> loanAmount[i] >>
                  annualInterest[i] >>
                  loanMonths[i] >>
                  monthlyEMI[i]))
                return false;

            totalEMI += monthlyEMI[i];
        }

        file.close();
        return true;
    }

public:
    Hello()
    {
        user = "";
        pass = "";
        old = "";
        income = 0;
        noOfLoans = 0;
        totalEMI = 0;
        profileLoaded = false;

        for (int i = 0; i < 20; i++)
        {
            loanAmount[i] = 0;
            annualInterest[i] = 0;
            loanMonths[i] = 0;
            monthlyEMI[i] = 0;
        }

        profileLoaded = loadProfile();
    }

    void hi();
    void profile();

    double getIncome()
    {
        return income;
    }

    double getTotalEMI()
    {
        return totalEMI;
    }
};

void Hello::hi()
{
    if (profileLoaded)
    {
        int choice;

        cout << "\n=========================================\n";
        cout << "                FINMATE\n";
        cout << "=========================================\n";
        cout << "\n1. Login";
        cout << "\n2. Register New User";
        cout << "\n3. Exit";
        cout << "\n\nEnter Choice: ";

        choice = readInt();

        while (choice < 1 || choice > 3)
        {
            cout << "Invalid Choice. Enter again: ";
            choice = readInt();
        }

        if (choice == 3)
        {
            exit(0);
        }

        if (choice == 1)
        {
            string u, p;

            cout << "\n=========================================\n";
            cout << "                 LOGIN\n";
            cout << "=========================================\n\n";

            while (true)
            {
                cout << "Enter Username : ";
                cin >> u;

                cout << "Enter Password : ";
                cin >> p;

                if (u == user && p == pass)
                {
                    cout << "\n=========================================\n";
                    cout << "            LOGIN SUCCESSFUL!\n";
                    cout << "=========================================\n\n";
                    return;
                }

                cout << "\nInvalid username or password!\n";
                cout << "Please try again.\n\n";
            }
        }

        if (choice == 2)
        {
            cout << "\n=========================================\n";
            cout << "          NEW USER REGISTRATION\n";
            cout << "=========================================\n\n";

            cout << "Enter Username       : ";
            cin >> user;

            cout << "Enter Password       : ";
            cin >> pass;

            cout << "Enter Monthly Income : Rs. ";
            income = readDouble();

            while (income <= 0)
            {
                cout << "Income must be greater than 0: Rs. ";
                income = readDouble();
            }

            char loanChoice;

            cout << "\nDo you have any loan? (y/n): ";
            cin >> loanChoice;

            if (loanChoice == 'Y' || loanChoice == 'y')
            {
                cout << "\nHow many loans do you have? (1-20): ";
                noOfLoans = readInt();

                while (noOfLoans < 1 || noOfLoans > 20)
                {
                    cout << "Invalid number. Enter between 1 and 20: ";
                    noOfLoans = readInt();
                }

                totalEMI = 0;

                for (int i = 0; i < noOfLoans; i++)
                {
                    cout << "\n-----------------------------------------\n";
                    cout << "Loan " << i + 1 << endl;
                    cout << "-----------------------------------------\n";

                    cout << "Enter Loan Amount: Rs. ";
                    loanAmount[i] = readDouble();

                    while (loanAmount[i] <= 0)
                    {
                        cout << "Loan amount must be greater than 0: Rs. ";
                        loanAmount[i] = readDouble();
                    }

                    cout << "Enter Annual Interest Rate (%): ";
                    annualInterest[i] = readDouble();

                    while (annualInterest[i] < 0)
                    {
                        cout << "Interest rate cannot be negative: ";
                        annualInterest[i] = readDouble();
                    }

                    int years;

                    cout << "Enter Loan Duration (Years): ";
                    years = readInt();

                    while (years <= 0)
                    {
                        cout << "Duration must be greater than 0: ";
                        years = readInt();
                    }

                    loanMonths[i] = years * 12;

                    monthlyEMI[i] = calculateEMI(
                        loanAmount[i],
                        annualInterest[i],
                        loanMonths[i]
                    );

                    totalEMI += monthlyEMI[i];

                    cout << fixed << setprecision(2);
                    cout << "Monthly EMI: Rs. "
                         << monthlyEMI[i]
                         << endl;
                }

                cout << "\nTotal Monthly EMI: Rs. "
                     << totalEMI
                     << endl;
            }
            else
            {
                noOfLoans = 0;
                totalEMI = 0;
            }

            saveProfile();
            profileLoaded = true;

            cout << "\n=========================================\n";
            cout << "       REGISTRATION SUCCESSFUL!\n";
            cout << "=========================================\n\n";

            return;
        }
    }

    cout << "\n=========================================\n";
    cout << "              REGISTRATION\n";
    cout << "=========================================\n\n";

    cout << "Enter Username       : ";
    cin >> user;

    cout << "Enter Password       : ";
    cin >> pass;

    cout << "Enter Monthly Income : Rs. ";
    income = readDouble();

    while (income <= 0)
    {
        cout << "Income must be greater than 0: Rs. ";
        income = readDouble();
    }

    char loanChoice;

    cout << "\nDo you have any loan? (y/n): ";
    cin >> loanChoice;

    if (loanChoice == 'Y' || loanChoice == 'y')
    {
        cout << "\nHow many loans do you have? (1-20): ";
        noOfLoans = readInt();

        while (noOfLoans < 1 || noOfLoans > 20)
        {
            cout << "Invalid number. Enter between 1 and 20: ";
            noOfLoans = readInt();
        }

        totalEMI = 0;

        for (int i = 0; i < noOfLoans; i++)
        {
            cout << "\n-----------------------------------------\n";
            cout << "Loan " << i + 1 << endl;
            cout << "-----------------------------------------\n";

            cout << "Enter Loan Amount: Rs. ";
            loanAmount[i] = readDouble();

            while (loanAmount[i] <= 0)
            {
                cout << "Loan amount must be greater than 0: Rs. ";
                loanAmount[i] = readDouble();
            }

            cout << "Enter Annual Interest Rate (%): ";
            annualInterest[i] = readDouble();

            while (annualInterest[i] < 0)
            {
                cout << "Interest rate cannot be negative: ";
                annualInterest[i] = readDouble();
            }

            int years;

            cout << "Enter Loan Duration (Years): ";
            years = readInt();

            while (years <= 0)
            {
                cout << "Duration must be greater than 0: ";
                years = readInt();
            }

            loanMonths[i] = years * 12;

            monthlyEMI[i] = calculateEMI(
                loanAmount[i],
                annualInterest[i],
                loanMonths[i]
            );

            totalEMI += monthlyEMI[i];

            cout << fixed << setprecision(2);
            cout << "Monthly EMI: Rs. "
                 << monthlyEMI[i]
                 << endl;
        }

        cout << "\nTotal Monthly EMI: Rs. "
             << totalEMI
             << endl;
    }
    else
    {
        noOfLoans = 0;
        totalEMI = 0;
    }

    saveProfile();
    profileLoaded = true;

    cout << "\n=========================================\n";
    cout << "       REGISTRATION SUCCESSFUL!\n";
    cout << "=========================================\n\n";
}

void Hello::profile()
{
    int select;

    while (true)
    {
        cout << "\n=========================================\n";
        cout << "           EDIT & VIEW PROFILE\n";
        cout << "=========================================\n";

        cout << "\n1. Username       : " << user << endl;
        cout << "2. Password       : ********" << endl;

        cout << fixed << setprecision(2);
        cout << "3. Monthly Income : Rs. " << income << endl;

        if (noOfLoans > 0)
        {
            cout << "4. Loans          : " << noOfLoans << endl;

            for (int i = 0; i < noOfLoans; i++)
            {
                cout << "\nLoan " << i + 1 << endl;
                cout << "Amount        : Rs. " << loanAmount[i] << endl;
                cout << "Interest      : " << annualInterest[i] << "%" << endl;
                cout << "Duration      : " << loanMonths[i] << " Months" << endl;
                cout << "Monthly EMI   : Rs. " << monthlyEMI[i] << endl;
            }

            cout << "\nTotal Monthly EMI     : Rs. " << totalEMI << endl;
            cout << "Income Left After EMI : Rs. " << income - totalEMI << endl;
        }
        else
        {
            cout << "4. Loans          : No active loans." << endl;
        }

        cout << "\n-----------------------------------------\n";
        cout << "5. Edit Username\n";
        cout << "6. Edit Password\n";
        cout << "7. Edit Monthly Income\n";
        cout << "8. Return to Home\n";
        cout << "-----------------------------------------\n";

        cout << "Enter Choice: ";
        select = readInt();

        switch (select)
        {
        case 5:
            cout << "Enter New Username: ";
            cin >> user;
            saveProfile();
            cout << "Username Updated Successfully!\n";
            break;

        case 6:
            cout << "Enter old Password: ";
            cin >> old;

            if (old == pass)
            {
                cout << "Enter New Password: ";
                cin >> pass;
                saveProfile();
                cout << "Password Updated Successfully!\n";
            }
            else
            {
                cout << "Wrong Password!\n";
            }
            break;

        case 7:
            cout << "Enter New Monthly Income: Rs. ";
            income = readDouble();

            while (income <= 0)
            {
                cout << "Income must be greater than 0: Rs. ";
                income = readDouble();
            }

            saveProfile();
            cout << "Income Updated Successfully!\n";
            break;

        case 8:
            return;

        default:
            cout << "Invalid Choice!\n";
        }
    }
}

class Expense
{
private:
    float amount;
    int category;
    string date;

public:
    Expense()
    {
        amount = 0;
        category = 0;
        date = "";
    }

    Expense(float a, int c, string d)
    {
        amount = a;
        category = c;
        date = d;
    }

    float getAmount()
    {
        return amount;
    }

    int getCategory()
    {
        return category;
    }

    string getDate()
    {
        return date;
    }
};

class ExpenseManager
{
private:
    Expense expenses[100];
    int count;
    float savings;

public:
    ExpenseManager()
    {
        count = 0;
        savings = 0;
        loadExpenses();
    }

    void addExpense();
    void viewExpenses();
    void totalExpense();
    void monthlyReport(float existingEMI, float calculatorEMI, double income);
    void saveExpenses();
    void loadExpenses();

    float getSavings()
    {
        return savings;
    }
};

void expenseTrackerMenu(ExpenseManager &manager)
{
    int choice;

    while (true)
    {
        cout << "\n=====================================\n";
        cout << "||         EXPENSE TRACKER         ||\n";
        cout << "=====================================\n";
        cout << "||     1. Add Expense              ||\n";
        cout << "||     2. View Expenses            ||\n";
        cout << "||     3. Total Expenses           ||\n";
        cout << "||     4. Back                     ||\n";
        cout << "=====================================\n";

        cout << "Enter Choice: ";
        choice = readInt();

        switch (choice)
        {
        case 1:
            manager.addExpense();
            break;

        case 2:
            manager.viewExpenses();
            break;

        case 3:
            manager.totalExpense();
            break;

        case 4:
            return;

        default:
            cout << "\nInvalid Choice!\n";
        }
    }
}

void ExpenseManager::addExpense()
{
    if (count >= 100)
    {
        cout << "\nExpense limit reached!\n";
        return;
    }

    float amount;
    int category;
    string date;

    cout << "\n======== ADD EXPENSE ==========\n";

    cout << "Enter Amount: ";
    amount = readDouble();

    while (amount <= 0)
    {
        cout << "Amount must be greater than 0: ";
        amount = readDouble();
    }

    cout << "\nSelect Category\n";
    cout << "1. Food\n";
    cout << "2. Travel\n";
    cout << "3. Shopping\n";
    cout << "4. Rent\n";
    cout << "5. Bills\n";
    cout << "6. Medical\n";
    cout << "7. Entertainment\n";
    cout << "8. Other\n";

    cout << "Enter Choice: ";
    category = readInt();

    while (category < 1 || category > 8)
    {
        cout << "Invalid Category! Enter Again: ";
        category = readInt();
    }

    cout << "Enter Date (DD/MM/YYYY): ";
    cin >> date;

    int month, year;

    while (!getMonthYear(date, month, year))
    {
        cout << "Invalid date format. Use DD/MM/YYYY: ";
        cin >> date;
    }

    expenses[count] = Expense(amount, category, date);
    count++;

    saveExpenses();

    cout << "\nExpense Added Successfully!\n";
}

void ExpenseManager::saveExpenses()
{
    ofstream file(DATA_DIR + "expense.txt");

    if (!file)
    {
        cout << "Error opening expense file!\n";
        return;
    }

    for (int i = 0; i < count; i++)
    {
        file << expenses[i].getAmount() << " "
             << expenses[i].getCategory() << " "
             << expenses[i].getDate() << endl;
    }

    file.close();
}

void ExpenseManager::loadExpenses()
{
    ifstream file(DATA_DIR + "expense.txt");

    if (!file)
        return;

    float amount;
    int category;
    string date;

    while (count < 100 && file >> amount >> category >> date)
    {
        expenses[count] = Expense(amount, category, date);
        count++;
    }

    file.close();
}

void ExpenseManager::viewExpenses()
{
    if (count == 0)
    {
        cout << "\nNo Expenses Found!\n";
        return;
    }

    cout << "\n===============================================\n";
    cout << "                 ALL EXPENSES\n";
    cout << "===============================================\n";

    cout << fixed << setprecision(2);
    cout << "Amount\t\tCategory\tDate\n";
    cout << "-----------------------------------------------\n";

    for (int i = 0; i < count; i++)
    {
        cout << expenses[i].getAmount() << "\t\t";

        switch (expenses[i].getCategory())
        {
        case 1:
            cout << "Food";
            break;

        case 2:
            cout << "Travel";
            break;

        case 3:
            cout << "Shopping";
            break;

        case 4:
            cout << "Rent";
            break;

        case 5:
            cout << "Bills";
            break;

        case 6:
            cout << "Medical";
            break;

        case 7:
            cout << "Entertainment";
            break;

        case 8:
            cout << "Other";
            break;

        default:
            cout << "Unknown";
        }

        cout << "\t\t" << expenses[i].getDate() << endl;
    }

    cout << "-----------------------------------------------\n";
    cout << "Total Records : " << count << endl;
}

void ExpenseManager::totalExpense()
{
    if (count == 0)
    {
        cout << "\nNo Expenses Found!\n";
        return;
    }

    float food = 0, travel = 0, shopping = 0, rent = 0;
    float bills = 0, medical = 0, entertainment = 0, other = 0;

    for (int i = 0; i < count; i++)
    {
        switch (expenses[i].getCategory())
        {
        case 1:
            food += expenses[i].getAmount();
            break;

        case 2:
            travel += expenses[i].getAmount();
            break;

        case 3:
            shopping += expenses[i].getAmount();
            break;

        case 4:
            rent += expenses[i].getAmount();
            break;

        case 5:
            bills += expenses[i].getAmount();
            break;

        case 6:
            medical += expenses[i].getAmount();
            break;

        case 7:
            entertainment += expenses[i].getAmount();
            break;

        case 8:
            other += expenses[i].getAmount();
            break;
        }
    }

    float total = food + travel + shopping + rent +
                  bills + medical + entertainment + other;

    cout << "\n=============================================\n";
    cout << "              EXPENSE SUMMARY\n";
    cout << "=============================================\n";

    cout << fixed << setprecision(2);
    cout << "Category\t\tAmount\n";
    cout << "---------------------------------------------\n";

    cout << "Food\t\t\t" << food << endl;
    cout << "Travel\t\t\t" << travel << endl;
    cout << "Shopping\t\t" << shopping << endl;
    cout << "Rent\t\t\t" << rent << endl;
    cout << "Bills\t\t\t" << bills << endl;
    cout << "Medical\t\t\t" << medical << endl;
    cout << "Entertainment\t\t" << entertainment << endl;
    cout << "Other\t\t\t" << other << endl;

    cout << "---------------------------------------------\n";
    cout << "TOTAL\t\t\t" << total << endl;
    cout << "=============================================\n";
}

class FinanceCalculator
{
private:
    float savedEMI;

public:
    FinanceCalculator()
    {
        savedEMI = 0;
    }

    void emiCalculator();
    void savingsGoal(float savings);

    float getEMI()
    {
        return savedEMI;
    }
};

void FinanceCalculator::savingsGoal(float savings)
{
    if (savings <= 0)
    {
        cout << "\nNo positive savings available.\n";
        cout << "Please run Monthly Report first.\n";
        return;
    }

    string goalName;
    float goalAmount;
    int months;

    cout << "\n=====================================\n";
    cout << "             SAVINGS GOAL\n";
    cout << "=====================================\n";

    cout << "Enter Goal Name: ";
    cin >> goalName;

    cout << "Enter Goal Amount: ";
    goalAmount = readDouble();

    while (goalAmount <= 0)
    {
        cout << "Goal amount must be greater than 0: ";
        goalAmount = readDouble();
    }

    cout << fixed << setprecision(2);
    cout << "Monthly Saving (from report): Rs. " << savings << endl;

    months = static_cast<int>(ceil(goalAmount / savings));

    cout << "\n=====================================\n";
    cout << "            SAVINGS RESULT\n";
    cout << "=====================================\n";

    cout << "Goal Name       : " << goalName << endl;
    cout << "Goal Amount     : Rs. " << goalAmount << endl;
    cout << "Monthly Saving  : Rs. " << savings << endl;
    cout << "Months Required : " << months << endl;

    cout << "\nKeep saving consistently every month!\n";
}

void FinanceCalculator::emiCalculator()
{
    float loanAmount, annualRate;
    float emi, totalPayment, totalInterest;
    int years, months;

    cout << "\n=====================================\n";
    cout << "          EMI CALCULATOR\n";
    cout << "=====================================\n";

    cout << "Enter Loan Amount : ";
    loanAmount = readDouble();

    while (loanAmount <= 0)
    {
        cout << "Loan amount must be greater than 0: ";
        loanAmount = readDouble();
    }

    cout << "Enter Annual Interest Rate (%) : ";
    annualRate = readDouble();

    while (annualRate < 0)
    {
        cout << "Interest rate cannot be negative: ";
        annualRate = readDouble();
    }

    cout << "Enter Loan Duration (Years) : ";
    years = readInt();

    while (years <= 0)
    {
        cout << "Duration must be greater than 0: ";
        years = readInt();
    }

    months = years * 12;

    emi = calculateEMI(loanAmount, annualRate, months);
    totalPayment = emi * months;
    totalInterest = totalPayment - loanAmount;

    cout << "\n=====================================\n";
    cout << "            EMI DETAILS\n";
    cout << "=====================================\n";

    cout << fixed << setprecision(2);
    cout << "Monthly EMI     : Rs. " << emi << endl;
    cout << "Total Payment   : Rs. " << totalPayment << endl;
    cout << "Total Interest  : Rs. " << totalInterest << endl;

    int choice;

    cout << "\nAdd this EMI to Monthly Report?\n";
    cout << "1. YES\n";
    cout << "2. NO\n";
    cout << "Enter Choice: ";
    choice = readInt();

    if (choice == 1)
    {
        savedEMI = emi;
        cout << "\nThis EMI will be included in the Monthly Report.\n";
    }
    else
    {
        savedEMI = 0;
        cout << "\nThis EMI will not be included in the Monthly Report.\n";
    }
}

void ExpenseManager::monthlyReport(
    float existingEMI,
    float calculatorEMI,
    double income)
{
    if (income <= 0)
    {
        cout << "\nInvalid income.\n";
        return;
    }

    int selectedMonth, selectedYear;

    cout << "\n=====================================\n";
    cout << "          MONTHLY REPORT\n";
    cout << "=====================================\n";

    cout << "Enter Month (1-12): ";
    selectedMonth = readInt();

    while (selectedMonth < 1 || selectedMonth > 12)
    {
        cout << "Invalid month. Enter 1-12: ";
        selectedMonth = readInt();
    }

    cout << "Enter Year: ";
    selectedYear = readInt();

    while (selectedYear < 1)
    {
        cout << "Invalid year. Enter again: ";
        selectedYear = readInt();
    }

    float food = 0, travel = 0, shopping = 0, rent = 0;
    float bills = 0, medical = 0, entertainment = 0, other = 0;

    int matchingExpenses = 0;

    for (int i = 0; i < count; i++)
    {
        int month, year;

        if (!getMonthYear(expenses[i].getDate(), month, year))
            continue;

        if (month != selectedMonth || year != selectedYear)
            continue;

        matchingExpenses++;

        switch (expenses[i].getCategory())
        {
        case 1:
            food += expenses[i].getAmount();
            break;

        case 2:
            travel += expenses[i].getAmount();
            break;

        case 3:
            shopping += expenses[i].getAmount();
            break;

        case 4:
            rent += expenses[i].getAmount();
            break;

        case 5:
            bills += expenses[i].getAmount();
            break;

        case 6:
            medical += expenses[i].getAmount();
            break;

        case 7:
            entertainment += expenses[i].getAmount();
            break;

        case 8:
            other += expenses[i].getAmount();
            break;
        }
    }

    float totalExpense =
        food + travel + shopping + rent +
        bills + medical + entertainment + other;

    float totalEMI = existingEMI + calculatorEMI;
    float totalOutflow = totalExpense + totalEMI;

    savings = income - totalOutflow;

    float savingsRate = (savings / income) * 100;

    float needs = food + rent + bills + medical;
    float wants = shopping + travel + entertainment + other;

    float needsPercent = (needs / income) * 100;
    float wantsPercent = (wants / income) * 100;

    cout << "\n=========================================================\n";
    cout << "                FINMATE MONTHLY REPORT\n";
    cout << "=========================================================\n";

    cout << fixed << setprecision(2);

    cout << "\nReport Month : "
         << selectedMonth << "/" << selectedYear;

    cout << "\nExpenses Found : " << matchingExpenses;
    cout << "\nMonthly Income : Rs. " << income;
    cout << "\nTotal Expense  : Rs. " << totalExpense;
    cout << "\nExisting Loan EMI : Rs. " << existingEMI;
    cout << "\nAdditional EMI    : Rs. " << calculatorEMI;
    cout << "\nTotal EMI      : Rs. " << totalEMI;
    cout << "\nTotal Outflow  : Rs. " << totalOutflow;
    cout << "\nTotal Savings  : Rs. " << savings;
    cout << "\nSavings Rate   : " << savingsRate << "%";

    cout << "\n\n--------------- 50 / 30 / 20 ANALYSIS ------------------";

    cout << "\nNEEDS : "
         << needsPercent << "%  (Target <= 50%)";

    cout << "\nWANTS: "
         << wantsPercent << "%  (Target <= 30%)";

    cout << "\nSavings : "
         << savingsRate << "%  (Target >= 20%)";

    cout << "\n\n---------------- CATEGORY BREAKDOWN --------------------";
    cout<<"\n=== NEEDS ===\n";
    cout << "Food           : Rs. " << food;
    cout << "\nRent           : Rs. " << rent;
     cout << "\nBills          : Rs. " << bills;
     cout << "\nMedical        : Rs. " << medical;
     cout<<"\n";
     cout<<"\n=== WANTS===\n";
    cout << "Travel         : Rs. " << travel;
    cout << "\nShopping       : Rs. " << shopping;
    cout << "\nEntertainment  : Rs. " << entertainment;
    cout << "\nOther          : Rs. " << other;

    cout << "\n\n---------------- FINANCIAL HEALTH ----------------------";

    if (needsPercent <= 50)
        cout << "\nNeeds Budget   : PASS";
    else
        cout << "\nNeeds Budget   : FAIL";

    if (wantsPercent <= 30)
        cout << "\nWants Budget   : PASS";
    else
        cout << "\nWants Budget   : FAIL";

    if (savingsRate >= 20)
        cout << "\nSavings Target : PASS";
    else
        cout << "\nSavings Target : FAIL";

    cout << "\n=========================================================\n";
}

int main()
{
    cout << "\n";
    cout << "  _ _ _ _    _ _ _    _       _    _       _       _ _       _ _ _ _ _    _ _ _ _   \n";
    cout << " |_|_|_|_|  |_|_|_|  |_|_    |_|  |_|     |_|   _ |_|_| _   |_|_|_|_|_|  |_|_|_|_|  \n";
    cout << " |_|_ _       |_|    |_|_|_  |_|  |_|\\ _ /|_|  |_|_ _ _|_|      |_|      |_|_ _       \n";
    cout << " |_|_|_|      |_|    |_| |_|_|_|  |_| |_| |_|  |_|_|_|_|_|      |_|      |_|_|_|    \n";
    cout << " |_|         _|_|_   |_|   |_|_|  |_|     |_|  |_|     |_|      |_|      |_|_ _ _     \n";
    cout << " |_|        |_|_|_|  |_|     |_|  |_|     |_|  |_|     |_|      |_|      |_|_|_|_|  \n\n";

    Hello userProfile;
    userProfile.hi();

    ExpenseManager manager;
    FinanceCalculator calc;

    int choice;

    while (true)
    {
        cout << "\n";
        cout << "                                         _____ _____ _____ _____ " << endl;
        cout << "                                        |  |  |     |     |   __|" << endl;
        cout << "                                        |     |  |  | | | |   __|" << endl;
        cout << "                                        |__|__|_____|_|_|_|_____|" << endl;
        cout << "                                        ~-~-~-~-~-~-~-~-~-~-~-~-~-\n\n";

        cout << "  1. Expense Tracker\n";
        cout << "  2. EMI Calculator\n";
        cout << "  3. Savings Goal\n";
        cout << "  4. Monthly Report\n";
        cout << "  5. Profile\n";
        cout << "  6. Exit\n\n";

        cout << "=====================================\n";
        cout << "Enter Choice: ";
        choice = readInt();

        switch (choice)
        {
        case 1:
            expenseTrackerMenu(manager);
            break;

        case 2:
            calc.emiCalculator();
            break;

        case 3:
            calc.savingsGoal(manager.getSavings());
            break;

        case 4:
            manager.monthlyReport(
                userProfile.getTotalEMI(),
                calc.getEMI(),
                userProfile.getIncome()
            );
            break;

        case 5:
            userProfile.profile();
            break;

        case 6:
            cout << "\nThank you for using FINMATE!\n";
            return 0;

        default:
            cout << "\nInvalid Choice!\n";
        }
    }

    return 0;
}