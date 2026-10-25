# Testing and QA

This document records the manual tests completed for the current learning-project version. Tests were performed locally by running the program in a Windows PowerShell terminal with GCC.

## Build check

Compile with warnings enabled:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic Bank-Management-System.c -o bank.exe
```

## Results confirmed in manual testing

### Menu and input validation

- [x] Program starts and displays the menu.
- [x] Existing accounts can be viewed.
- [x] Invalid menu choices are handled.
- [x] Non-numeric menu input is handled without getting stuck.
- [x] Numeric-only account names are rejected.
- [x] Empty account names are handled safely.

### Transactions

- [x] Deposit a valid amount; the balance updates correctly.
- [x] Withdraw a valid amount; the balance updates correctly.
- [x] Withdrawal greater than the available balance is rejected.
- [x] Zero-value withdrawal is rejected.
- [x] Negative withdrawals and deposits are rejected.
- [x] Transfer between existing accounts updates the balances correctly.
- [x] Transfer to a nonexistent account is rejected.
- [x] Non-numeric transaction amounts are handled safely.

### Persistence and transaction history

- [x] Account records and balances remain correct after restarting the program.
- [x] Transaction history remains available after restarting.
- [x] Transaction history was inspected and confirmed to contain the expected records.

### Final regression check

- [x] The user confirmed that the core account and transaction features were tested and working together.

## Scope and limitations

These are manual test results, not automated tests. Missing-file recovery, corrupted-file handling, the 100-account capacity limit, and every possible edge case have **not** been claimed as tested here.

For safe testing, use fictional account details and back up `accounts.txt` and `transactions.txt` before experimenting with saved data. These files contain local plain-text records and should not contain real or sensitive financial information.

This is an educational console application, not production banking software.
