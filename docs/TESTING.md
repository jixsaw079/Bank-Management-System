# Manual Testing Checklist

Use this checklist when verifying changes to the Bank Management System. These are **test cases to run**, not a claim that every case has already passed.

## Before testing

1. Compile with warnings enabled:

   ```powershell
   gcc -std=c11 -Wall -Wextra -pedantic Bank-Management-System.c -o bank.exe
   ```

2. Run the executable from the project folder.
3. Use only fictional test names and amounts.
4. If you need a clean starting state, close the program and back up or remove the local `accounts.txt` file. This deletes the program's saved test records, so do not do it if you want to keep them.

## Core functional checks

- [ ] Create an account with a name and zero initial deposit.
- [ ] Create an account with a positive initial deposit.
- [ ] View all accounts and confirm the displayed details.
- [ ] Check the balance for an existing account.
- [ ] Deposit a positive amount and confirm the new balance.
- [ ] Withdraw an amount smaller than the balance.
- [ ] Attempt to withdraw more than the available balance; it should be rejected.
- [ ] Transfer money between two different accounts and check both balances.
- [ ] Attempt a transfer to an unknown account; it should be rejected.
- [ ] Attempt to transfer money from an account to itself; it should be rejected.
- [ ] Record a loan and verify the loan and balance values.
- [ ] Repay part of a loan and verify the remaining loan and balance.
- [ ] Attempt to repay more than the outstanding loan and verify that repayment is capped at the outstanding amount.
- [ ] Exit the program cleanly.

## Input validation checks

- [ ] Enter text instead of a menu number.
- [ ] Enter a menu number outside the available range.
- [ ] Enter text instead of an account number.
- [ ] Try a negative initial deposit.
- [ ] Try zero or a negative deposit, withdrawal, transfer, loan, or repayment amount.
- [ ] Try an account number that does not exist.
- [ ] Try a name containing spaces.
- [ ] Try a name containing the pipe character (`|`); this character is reserved as the file-format separator and should be avoided.

## Persistence checks

- [ ] Create an account, exit, relaunch, and verify that the account loads.
- [ ] Make a deposit, exit, relaunch, and verify the updated balance.
- [ ] Make a withdrawal or transfer, relaunch, and verify the saved balances.
- [ ] Record or repay a loan, relaunch, and verify the saved loan values.
- [ ] Confirm that `accounts.txt` is created in the working directory and is ignored by Git.

## Capacity check

- [ ] Verify that the program refuses to create an account after reaching its 100-account limit. Use a disposable test data file for this test.

## Record results

For a portfolio, record the date, compiler version, tests run, and any failures fixed. Do not mark a case as passed until you have actually run it.
