#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>

#define MAX_ACCOUNTS 100

struct Account
{
    int accountNumber;
    char name[50];
    float balance;
    float loan;
};

struct Account accounts[MAX_ACCOUNTS];
int accountCount = 0;
int nextAccountNumber = 1001;
#define DATA_FILE "accounts.txt"
#define TRANSACTION_FILE "transactions.txt"

void clearInputBuffer(void);
int readInt(const char prompt[], int *value);
int readFloat(const char prompt[], float *value);
int canAddAmount(float current, float amount);
int readName(char name[]);
void createAccount(void);
void viewAccounts(void);
void depositMoney(void);
void withdrawMoney(void);
void checkBalance(void);
void transferMoney(void);
void applyLoan(void);
void repayLoan(void);
int loadAccounts(void);
int saveAccounts(void);
int recordTransaction(int accountNumber, const char type[], float amount, float balance);
void viewTransactionHistory(void);

int main(void)
{
    int choice;

    if (loadAccounts())
        printf("Loaded %d account(s) from %s.\n", accountCount, DATA_FILE);
    else
        printf("No saved account data loaded. Starting with an empty account list.\n");

    while (1)
    {
        printf("\n==== BANK MANAGEMENT SYSTEM ====\n");
        printf("1. Create Account\n");
        printf("2. View All Accounts\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Check Balance\n");
        printf("6. Transfer Money\n");
        printf("7. Apply For Loan\n");
        printf("8. Repay Loan\n");
        printf("9. View Transaction History\n");
        printf("0. Exit\n");

        if (!readInt("Enter choice: ", &choice))
        {
            printf("Please enter a valid menu number.\n");
            continue;
        }

        if (choice == 1)
            createAccount();
        else if (choice == 2)
            viewAccounts();
        else if (choice == 3)
            depositMoney();
        else if (choice == 4)
            withdrawMoney();
        else if (choice == 5)
            checkBalance();
        else if (choice == 6)
            transferMoney();
        else if (choice == 7)
            applyLoan();
        else if (choice == 8)
            repayLoan();
        else if (choice == 9)
            viewTransactionHistory();
        else if (choice == 0)
        {
            if (!saveAccounts())
                printf("Warning: account data could not be saved.\n");
            break;
        }
        else
            printf("Invalid choice! Please select 0 to 8.\n");
    }

    printf("Thank you for using the Bank Management System.\n");
    return 0;
}

/* Load account records from a text file when the program starts. */
int loadAccounts(void)
{
    FILE *file = fopen(DATA_FILE, "r");
    int loaded = 0;

    if (file == NULL)
        return 0;

    accountCount = 0;
    nextAccountNumber = 1001;

    while (accountCount < MAX_ACCOUNTS &&
           fscanf(file, "%d|%49[^|]|%f|%f",
                  &accounts[accountCount].accountNumber,
                  accounts[accountCount].name,
                  &accounts[accountCount].balance,
                  &accounts[accountCount].loan) == 4)
    {
        int ch;

        if (accounts[accountCount].accountNumber <= 0 ||
            !isfinite(accounts[accountCount].balance) ||
            !isfinite(accounts[accountCount].loan) ||
            accounts[accountCount].balance < 0 ||
            accounts[accountCount].loan < 0)
        {
            printf("Warning: invalid account record found; loading stopped.\n");
            break;
        }

        if (accounts[accountCount].accountNumber >= nextAccountNumber)
        {
            if (accounts[accountCount].accountNumber >= INT_MAX - 1)
                nextAccountNumber = INT_MAX;
            else
                nextAccountNumber = accounts[accountCount].accountNumber + 1;
        }

        accountCount++;
        loaded = 1;
        while ((ch = fgetc(file)) != '\n' && ch != EOF)
        {
            /* Skip any extra characters on this record. */
        }
    }

    fclose(file);
    return loaded;
}

/* Save all current accounts to a text file. */
int saveAccounts(void)
{
    FILE *file = fopen(DATA_FILE, "w");
    int i;

    if (file == NULL)
    {
        printf("Error: could not open %s for writing.\n", DATA_FILE);
        return 0;
    }

    for (i = 0; i < accountCount; i++)
    {
        if (fprintf(file, "%d|%s|%.2f|%.2f\n",
                    accounts[i].accountNumber, accounts[i].name,
                    accounts[i].balance, accounts[i].loan) < 0)
        {
            fclose(file);
            printf("Error: failed while writing account data.\n");
            return 0;
        }
    }

    if (fclose(file) != 0)
    {
        printf("Error: could not finish saving account data.\n");
        return 0;
    }

    return 1;
}

/* Append one successful balance-changing operation to the transaction log. */
int recordTransaction(int accountNumber, const char type[], float amount, float balance)
{
    FILE *file;
    time_t now;
    struct tm *localTime;
    char timestamp[20];

    now = time(NULL);
    localTime = localtime(&now);

    if (localTime == NULL ||
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localTime) == 0)
    {
        printf("Error: could not create a transaction timestamp.\n");
        return 0;
    }

    file = fopen(TRANSACTION_FILE, "a");
    if (file == NULL)
    {
        printf("Error: could not open %s for writing.\n", TRANSACTION_FILE);
        return 0;
    }

    if (fprintf(file, "%d|%s|%.2f|%.2f|%s\n",
                accountNumber, type, amount, balance, timestamp) < 0)
    {
        fclose(file);
        printf("Error: failed while writing transaction history.\n");
        return 0;
    }

    if (fclose(file) != 0)
    {
        printf("Error: could not finish saving transaction history.\n");
        return 0;
    }

    return 1;
}

/* Show all saved transactions for an account number. */
void viewTransactionHistory(void)
{
    FILE *file;
    int accountNumber;
    int savedAccountNumber;
    int foundAccount = 0;
    int foundTransaction = 0;
    float amount;
    float balance;
    char type[32];
    char timestamp[64];
    int i;

    if (!readInt("Enter account number: ", &accountNumber))
    {
        printf("Invalid account number.\n");
        return;
    }

    for (i = 0; i < accountCount; i++)
    {
        if (accounts[i].accountNumber == accountNumber)
        {
            foundAccount = 1;
            break;
        }
    }

    if (!foundAccount)
    {
        printf("Account not found.\n");
        return;
    }

    file = fopen(TRANSACTION_FILE, "r");
    if (file == NULL)
    {
        printf("No transaction history found for this account yet.\n");
        return;
    }

    printf("\n========== TRANSACTION HISTORY ==========\n");
    printf("Account: %d\n", accountNumber);
    printf("%-20s %-18s %-12s %s\n",
           "Date / Time", "Transaction", "Amount", "Balance After");
    printf("--------------------------------------------------------------------------\n");

    while (fscanf(file, "%d|%31[^|]|%f|%f|%63[^\n]\n",
                  &savedAccountNumber, type, &amount, &balance, timestamp) == 5)
    {
        if (savedAccountNumber == accountNumber)
        {
            printf("%-20s %-18s %10.2f %16.2f\n",
                   timestamp, type, amount, balance);
            foundTransaction = 1;
        }
    }

    if (ferror(file))
        printf("Warning: an error occurred while reading transaction history.\n");

    fclose(file);

    if (!foundTransaction)
        printf("No transactions recorded for this account yet.\n");

    printf("==========================================\n");
}

/* Remove any extra characters left on the current input line. */
void clearInputBuffer(void)
{
    int character;

    while ((character = getchar()) != '\n' && character != EOF)
    {
        /* Keep reading until the end of the line. */
    }
}

/* Read an integer safely, rejecting malformed and out-of-range input. */
int readInt(const char prompt[], int *value)
{
    char input[100];
    char *end;
    long number;
    int character;

    printf("%s", prompt);

    if (fgets(input, sizeof(input), stdin) == NULL)
        return 0;

    if (strchr(input, '\n') == NULL)
    {
        character = getchar();
        if (character != '\n' && character != EOF)
        {
            while (character != '\n' && character != EOF)
                character = getchar();
            return 0;
        }
    }

    errno = 0;
    number = strtol(input, &end, 10);

    if (end == input)
        return 0;

    while (isspace((unsigned char)*end))
        end++;

    if (*end != '\0' || errno == ERANGE || number < INT_MIN || number > INT_MAX)
        return 0;

    *value = (int)number;
    return 1;
}

/* Read a complete line and reject malformed or out-of-range numbers. */
int readFloat(const char prompt[], float *value)
{
    char input[100];
    char *end;
    float number;
    int character;

    printf("%s", prompt);

    if (fgets(input, sizeof(input), stdin) == NULL)
        return 0;

    /* Reject input lines that are longer than the buffer. */
    if (strchr(input, '\n') == NULL)
    {
        character = getchar();

        if (character != '\n' && character != EOF)
        {
            while (character != '\n' && character != EOF)
                character = getchar();

            return 0;
        }
    }

    errno = 0;
    number = strtof(input, &end);

    /* Reject input that contains no number. */
    if (end == input)
        return 0;

    /* Allow trailing whitespace, but reject any other extra characters. */
    while (isspace((unsigned char)*end))
        end++;

    if (*end != '\0')
        return 0;

    /* Reject overflow, underflow, infinity, and NaN. */
    if (errno == ERANGE || !isfinite(number))
        return 0;

    *value = number;
    return 1;
}

/* Read and validate a name; reject numeric-only and unsupported input. */
int readName(char name[])
{
    char input[100];
    size_t length;
    size_t start = 0;
    size_t end;
    size_t i;
    int hasLetter = 0;
    int character;

    printf("Enter name: ");

    if (fgets(input, sizeof(input), stdin) == NULL)
        return 0;

    if (strchr(input, '\n') == NULL)
    {
        character = getchar();
        if (character != '\n' && character != EOF)
        {
            while (character != '\n' && character != EOF)
                character = getchar();

            printf("Name is too long. Please enter a shorter name.\n");
            return 0;
        }
    }

    length = strlen(input);

    while (start < length && isspace((unsigned char)input[start]))
        start++;

    end = length;
    while (end > start && isspace((unsigned char)input[end - 1]))
        end--;

    if (start == end || end - start >= sizeof(((struct Account *)0)->name))
    {
        printf("Invalid name. Enter a name containing letters.\n");
        return 0;
    }

    for (i = start; i < end; i++)
    {
        unsigned char ch = (unsigned char)input[i];

        if (isalpha(ch) || ch >= 128)
        {
            hasLetter = 1;
        }
        else if (isspace(ch) || ch == '-' || ch == '\'' || ch == '.')
        {
            /* Allow spaces and common punctuation used in names. */
        }
        else
        {
            printf("Invalid name. Use letters, spaces, hyphens, apostrophes, or periods.\n");
            return 0;
        }
    }

    if (!hasLetter)
    {
        printf("Invalid name. A name cannot contain only numbers or punctuation.\n");
        return 0;
    }

    memcpy(name, input + start, end - start);
    name[end - start] = '\0';
    return 1;
}

/* Check whether adding a positive amount stays within the finite float range. */
int canAddAmount(float current, float amount)
{
    return isfinite(current) && isfinite(amount) && current >= 0.0f &&
           amount > 0.0f && amount <= FLT_MAX - current;
}

void createAccount(void)
{
    float initialDeposit;

    if (accountCount >= MAX_ACCOUNTS)
    {
        printf("Account limit reached. No more accounts can be created.\n");
        return;
    }

    if (nextAccountNumber <= 0 || nextAccountNumber == INT_MAX)
    {
        printf("Cannot create another account because the account number range has been reached.\n");
        return;
    }

    if (!readName(accounts[accountCount].name))
        return;

    if (!readFloat("Enter initial deposit (0 or more): ", &initialDeposit))
    {
        printf("Invalid amount. Please enter a number.\n");
        return;
    }

    if (initialDeposit < 0)
    {
        printf("Initial deposit cannot be negative.\n");
        return;
    }

    accounts[accountCount].accountNumber = nextAccountNumber++;
    accounts[accountCount].balance = initialDeposit;
    accounts[accountCount].loan = 0;

    printf("Account created successfully!\n");
    printf("Account number: %d\n", accounts[accountCount].accountNumber);

    accountCount++;
    if (initialDeposit > 0 &&
        !recordTransaction(accounts[accountCount - 1].accountNumber, "INITIAL_DEPOSIT",
                           initialDeposit, accounts[accountCount - 1].balance))
        printf("Warning: the initial deposit was not added to transaction history.\n");
    if (!saveAccounts())
        printf("Warning: this change is not safely saved to disk.\n");
}

void viewAccounts(void)
{
    int i;

    if (accountCount == 0)
    {
        printf("No accounts found.\n");
        return;
    }

    for (i = 0; i < accountCount; i++)
    {
        printf("\nAccount number: %d\n", accounts[i].accountNumber);
        printf("Name: %s\n", accounts[i].name);
        printf("Balance: %.2f\n", accounts[i].balance);
        printf("Outstanding loan: %.2f\n", accounts[i].loan);
    }
}

void depositMoney(void)
{
    int accountNumber;
    int i;
    float amount;

    if (!readInt("Enter account number: ", &accountNumber))
    {
        printf("Invalid account number.\n");
        return;
    }

    for (i = 0; i < accountCount; i++)
    {
        if (accounts[i].accountNumber == accountNumber)
        {
            if (!readFloat("Enter deposit amount: ", &amount))
            {
                printf("Invalid amount.\n");
                return;
            }

            if (amount <= 0)
            {
                printf("Deposit amount must be greater than zero.\n");
                return;
            }

            if (!canAddAmount(accounts[i].balance, amount))
            {
                printf("Deposit would exceed the supported balance range.\n");
                return;
            }

            accounts[i].balance += amount;
            printf("Deposit successful!\n");
            printf("New balance: %.2f\n", accounts[i].balance);
            if (!recordTransaction(accounts[i].accountNumber, "DEPOSIT", amount, accounts[i].balance))
                printf("Warning: deposit history could not be saved.\n");
            if (!saveAccounts())
                printf("Warning: this change is not safely saved to disk.\n");
            return;
        }
    }

    printf("Account not found.\n");
}

void withdrawMoney(void)
{
    int accountNumber;
    int i;
    float amount;

    if (!readInt("Enter account number: ", &accountNumber))
    {
        printf("Invalid account number.\n");
        return;
    }

    for (i = 0; i < accountCount; i++)
    {
        if (accounts[i].accountNumber == accountNumber)
        {
            if (!readFloat("Enter withdrawal amount: ", &amount))
            {
                printf("Invalid amount.\n");
                return;
            }

            if (amount <= 0)
            {
                printf("Withdrawal amount must be greater than zero.\n");
                return;
            }

            if (amount > accounts[i].balance)
            {
                printf("Insufficient balance.\n");
                return;
            }

            accounts[i].balance -= amount;
            printf("Withdrawal successful!\n");
            printf("Remaining balance: %.2f\n", accounts[i].balance);
            if (!recordTransaction(accounts[i].accountNumber, "WITHDRAWAL", amount, accounts[i].balance))
                printf("Warning: withdrawal history could not be saved.\n");
            if (!saveAccounts())
                printf("Warning: this change is not safely saved to disk.\n");
            return;
        }
    }

    printf("Account not found.\n");
}

void checkBalance(void)
{
    int accountNumber;
    int i;

    if (!readInt("Enter account number: ", &accountNumber))
    {
        printf("Invalid account number.\n");
        return;
    }

    for (i = 0; i < accountCount; i++)
    {
        if (accounts[i].accountNumber == accountNumber)
        {
            printf("Current balance: %.2f\n", accounts[i].balance);
            printf("Outstanding loan: %.2f\n", accounts[i].loan);
            return;
        }
    }

    printf("Account not found.\n");
}

void transferMoney(void)
{
    int sender;
    int receiver;
    int senderIndex = 0;
    int receiverIndex = 0;
    int senderFound = 0;
    int receiverFound = 0;
    int i;
    float amount;

    if (!readInt("Enter sender account number: ", &sender))
    {
        printf("Invalid sender account number.\n");
        return;
    }

    if (!readInt("Enter receiver account number: ", &receiver))
    {
        printf("Invalid receiver account number.\n");
        return;
    }

    for (i = 0; i < accountCount; i++)
    {
        if (accounts[i].accountNumber == sender)
        {
            senderIndex = i;
            senderFound = 1;
            break;
        }
    }

    if (!senderFound)
    {
        printf("Sender account not found.\n");
        return;
    }

    for (i = 0; i < accountCount; i++)
    {
        if (accounts[i].accountNumber == receiver)
        {
            receiverIndex = i;
            receiverFound = 1;
            break;
        }
    }

    if (!receiverFound)
    {
        printf("Receiver account not found.\n");
        return;
    }

    if (senderIndex == receiverIndex)
    {
        printf("Sender and receiver must be different accounts.\n");
        return;
    }

    if (!readFloat("Enter transfer amount: ", &amount))
    {
        printf("Invalid amount.\n");
        return;
    }

    if (amount <= 0)
    {
        printf("Transfer amount must be greater than zero.\n");
        return;
    }

    if (amount > accounts[senderIndex].balance)
    {
        printf("Insufficient balance.\n");
        return;
    }

    if (!canAddAmount(accounts[receiverIndex].balance, amount))
    {
        printf("Transfer would exceed the receiver's supported balance range.\n");
        return;
    }

    accounts[senderIndex].balance -= amount;
    accounts[receiverIndex].balance += amount;

    printf("Transfer successful!\n");
    printf("Sender's remaining balance: %.2f\n", accounts[senderIndex].balance);
    printf("Receiver's new balance: %.2f\n", accounts[receiverIndex].balance);
    if (!recordTransaction(accounts[senderIndex].accountNumber, "TRANSFER_OUT", amount, accounts[senderIndex].balance))
        printf("Warning: sender transfer history could not be saved.\n");
    if (!recordTransaction(accounts[receiverIndex].accountNumber, "TRANSFER_IN", amount, accounts[receiverIndex].balance))
        printf("Warning: receiver transfer history could not be saved.\n");
    if (!saveAccounts())
        printf("Warning: this change is not safely saved to disk.\n");
}

void applyLoan(void)
{
    int accountNumber;
    int i;
    float amount;

    if (!readInt("Enter account number: ", &accountNumber))
    {
        printf("Invalid account number.\n");
        return;
    }

    for (i = 0; i < accountCount; i++)
    {
        if (accounts[i].accountNumber == accountNumber)
        {
            if (!readFloat("Enter loan amount: ", &amount))
            {
                printf("Invalid amount.\n");
                return;
            }

            if (amount <= 0)
            {
                printf("Loan amount must be greater than zero.\n");
                return;
            }

            if (!canAddAmount(accounts[i].loan, amount) ||
                !canAddAmount(accounts[i].balance, amount))
            {
                printf("Loan would exceed the supported numeric range.\n");
                return;
            }

            accounts[i].loan += amount;
            accounts[i].balance += amount;

            printf("Loan recorded successfully for this educational simulation.\n");
            printf("Outstanding loan: %.2f\n", accounts[i].loan);
            printf("Available balance: %.2f\n", accounts[i].balance);
            if (!recordTransaction(accounts[i].accountNumber, "LOAN", amount, accounts[i].balance))
                printf("Warning: loan history could not be saved.\n");
            if (!saveAccounts())
                printf("Warning: this change is not safely saved to disk.\n");
            return;
        }
    }

    printf("Account not found.\n");
}

void repayLoan(void)
{
    int accountNumber;
    int i;
    float amount;

    if (!readInt("Enter account number: ", &accountNumber))
    {
        printf("Invalid account number.\n");
        return;
    }

    for (i = 0; i < accountCount; i++)
    {
        if (accounts[i].accountNumber == accountNumber)
        {
            if (accounts[i].loan <= 0)
            {
                printf("This account has no outstanding loan.\n");
                return;
            }

            printf("Outstanding loan: %.2f\n", accounts[i].loan);

            if (!readFloat("Enter repayment amount: ", &amount))
            {
                printf("Invalid amount.\n");
                return;
            }

            if (amount <= 0)
            {
                printf("Repayment amount must be greater than zero.\n");
                return;
            }

            if (amount > accounts[i].loan)
            {
                printf("Repayment cannot exceed the outstanding loan.\n");
                return;
            }

            if (amount > accounts[i].balance)
            {
                printf("Insufficient balance to make this repayment.\n");
                return;
            }

            accounts[i].balance -= amount;
            accounts[i].loan -= amount;

            printf("Loan repayment successful!\n");
            printf("Remaining loan: %.2f\n", accounts[i].loan);
            printf("Remaining balance: %.2f\n", accounts[i].balance);
            if (!recordTransaction(accounts[i].accountNumber, "LOAN_REPAYMENT", amount, accounts[i].balance))
                printf("Warning: loan repayment history could not be saved.\n");
            if (!saveAccounts())
                printf("Warning: this change is not safely saved to disk.\n");
            return;
        }
    }

    printf("Account not found.\n");
}
