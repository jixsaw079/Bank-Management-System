#include<stdio.h>
#include<string.h>

#define MAX_ACCOUNTS 100

struct Account
{
    int accountNumber;
    char name[50];
    float balance;
    float loan;
}
;

struct Account
accounts[MAX_ACCOUNTS];
int accountCount=0;

void createAccount();
void viewAccounts();
void depositMoney();
void withdrawMoney();
void checkBalance();
void transferMoney();
void applyLoan();
void repayLoan();

int main(){
    int choice;
    while(1){
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
        printf("Enter choice: ");
        scanf("%d",&choice);

        if(choice == 1)
          createAccount();
        else if(choice == 2)
          viewAccounts();
        else if(choice == 3)
          depositMoney();
        else if(choice == 4)
          withdrawMoney();
        else if(choice == 5)
          checkBalance();
        else if(choice == 6)
          transferMoney();
        else if(choice == 7)
          applyLoan();
        else if(choice == 8)
          repayLoan();
        else if(choice == 0)
          break;
        else
        printf("Invalid Choice!\n");
    }
    return 0;
}

void createAccount()
{
    accounts[accountCount].accountNumber=1001+accountCount;

    printf("Enter Name: ");
    scanf("%49s",&accounts[accountCount].name);

    printf("Enter Initial Deposit: ");
    scanf("%f",&accounts[accountCount].balance);

    accounts[accountCount].loan = 0; //Initialize loan amount

    printf("Account created! \nAccount Number: %d\n",accounts[accountCount].accountNumber);

    accountCount++;
}

void viewAccounts(){
    int i;
    if(accountCount==0){
        printf("No Account Found\n");
        return;
    }

    for(i=0;i<accountCount;i++){
        printf("\nAccount Number: %d\n",accounts[i].accountNumber);
        printf("Name: %s\n",accounts[i].name);
        printf("Balance: %.2f\n",accounts[i].balance);
        printf("Loan: %.2f\n",accounts[i].loan);
    }
}

 void depositMoney(){

    int accountNumber;
    float amount;
    int i;

    printf("Enter Account Number: ");
    scanf("%d",&accountNumber);

    for(i=0;i<accountCount;i++){
        if(accounts[i].accountNumber==accountNumber){
            printf("Enter Deposit Amount: ");
            scanf("%f",&amount);

            accounts[i].balance=accounts[i].balance + amount;

            printf("Deposit Successful!\n");
            printf("New Balance: %.2f\n",accounts[i].balance);
            return;
        }
    }
    printf("Account not found!\n");
 }

 void withdrawMoney(){
    int accountNumber;
    float amount;
    int i;

    printf("Enter Account Number: ");
    scanf("%d",&accountNumber);

    for(i=0;i<accountCount;i++){
        if(accounts[i].accountNumber== accountNumber){
            printf("Enter Withdrawal Amount: ");
            scanf("%f",&amount);

            if(amount>accounts[i].balance){
                printf("Insufficient Balance!\n");
            }
            else{
                accounts[i].balance= accounts[i].balance-amount;

                printf("Withdrawal Successful!\n");
                printf("Remaining Balance:%.2f\n",accounts[i].balance);
            }
            return;
        }
    }
    printf("Account not found!\n");
 }

 void checkBalance(){
    int accountNumber;
    int i;

    printf("Enter account number: ");
    scanf("%d",&accountNumber);

    for(i=0;i<accountCount;i++){
        if(accounts[i].accountNumber==accountNumber){
            printf("Current Balance: %.2f\n",accounts[i].balance);
            return;
        }
    }
    printf("Account not found!\n");
 }

 void transferMoney(){
    int sender,receiver;
    float amount;
    int i,j;

    printf("Enter sender account number: ");
    scanf("%d",&sender);

    printf("Enter receiver account number: ");
    scanf("%d",&receiver);

    printf("Enter transfer amount: ");
    scanf("%f",&amount);

    for(i=0;i<accountCount;i++){
        if(accounts[i].accountNumber==sender){
            if(accounts[i].balance<amount){
                printf("Insufficient Balance!\n");
                return;
            }
            for(i=0;j<accountCount;j++){
                if(accounts[j].accountNumber==receiver){
                    accounts[i].balance=accounts[i].balance-amount;
                    accounts[j].balance = accounts[j].balance + amount;

                    printf("Transfer successful!\n");
                    return;
                }
            }
            printf("Receiver account not found!\n");
            return;
        }
    }
    printf("Sender account not found!\n");
 }

 void applyLoan(){
    int accountNumber;
    float amount;
    int i;

    printf("Enter account number: ");
    scanf("%d",&accountNumber);

    for(i=0;i<accountNumber;i++){
        if(accounts[i].accountNumber==accountNumber){
            printf("Enter loan amount: ");
            scanf("%f",&amount);

            accounts[i].loan += amount;
            accounts[i].balance += amount;

            printf("Loan Approved!\n");
            printf("Loan Amount: %.2f\n",accounts[i].loan);
            printf("New Balance: %.2f\n",accounts[i].balance);

            return;
        }
    }
    printf("Account not found!\n");
 }

 void repayLoan(){
    int accountNumber;
    float amount;
    int i;
    printf("Enter account number: ");
    scanf("%d",&accountNumber);

    for(i=0;i<accountCount;i++){
        if(accounts[i].accountNumber== accountNumber){
            if(accounts[i].loan==0){
                printf("No loan to repay\n");
                return;
            }
            printf("Current loan: %.2f\n",accounts[i].loan);
            printf("Enter repayment amount: ");
            scanf("%f",&amount);

            if(amount>accounts[i].balance){
                printf("Insufficient balance!\n");
                return;
            }
            if(amount>accounts[i].loan)
            amount = accounts[i].loan;

            accounts[i].balance-=amount;
            accounts[i].loan -=amount;

            printf("Loan repayment successful!\n");
            printf("Remaining loan: %.2f\n",accounts[i].loan);
            printf("Remaining Balance: %.2f\n",accounts[i].balance);
            return;
        }
    }
    printf("Account not found!\n");
 }
