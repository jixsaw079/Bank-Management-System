#include <stdio.h>
#include <math.h>

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

void clearInputBuffer(void);
int readInt(const char prompt[], int *value);
int readFloat(const char prompt[], float *value);
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
            nextAccountNumber = accounts[accountCount].accountNumber + 1;

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

/* Remove any extra characters left on the current input line. */
void clearInputBuffer(void)
{
    int character;

    while ((character = getchar()) != '\n' && character != EOF)
    {
        /* Keep reading until the end of the line. */
    }
}

/* Read an integer safely. */
int readInt(const char prompt[], int *value)
{
    printf("%s", prompt);

    if (scanf("%d", value) != 1)
    {
        clearInputBuffer();
        return 0;
    }

    clearInputBuffer();
    return 1;
}

/* Read a number and reject invalid values such as text or infinity. */
int readFloat(const char prompt[], float *value)
{
    printf("%s", prompt);

    if (scanf("%f", value) != 1 || !isfinite(*value))
    {
        clearInputBuffer();
        return 0;
    }

    clearInputBuffer();
    return 1;
}

/* Read a name that may contain spaces, with room for the null character. */
int readName(char name[])
{
    printf("Enter name: ");

    if (scanf(" %49[^\n]", name) != 1)
    {
        clearInputBuffer();
        printf("Invalid name. Please try again.\n");
        return 0;
    }

    clearInputBuffer();
    return 1;
}

void createAccount(void)
{
    float initialDeposit;

    if (accountCount >= MAX_ACCOUNTS)
    {
        printf("Account limit reached. No more accounts can be created.\n");
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

            accounts[i].balance += amount;
            printf("Deposit successful!\n");
            printf("New balance: %.2f\n", accounts[i].balance);
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

    accounts[senderIndex].balance -= amount;
    accounts[receiverIndex].balance += amount;

    printf("Transfer successful!\n");
    printf("Sender's remaining balance: %.2f\n", accounts[senderIndex].balance);
    printf("Receiver's new balance: %.2f\n", accounts[receiverIndex].balance);
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

            accounts[i].loan += amount;
            accounts[i].balance += amount;

            printf("Loan recorded successfully for this educational simulation.\n");
            printf("Outstanding loan: %.2f\n", accounts[i].loan);
            printf("Available balance: %.2f\n", accounts[i].balance);
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
                amount = accounts[i].loan;
                printf("Repayment adjusted to the outstanding loan amount.\n");
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
            if (!saveAccounts())
                printf("Warning: this change is not safely saved to disk.\n");
            return;
        }
    }

    printf("Account not found.\n");
}
