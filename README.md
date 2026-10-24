# Bank Management System in C

A beginner-friendly, menu-driven Bank Management System written in C for a first-semester university project. The program demonstrates structures, arrays, functions, loops, conditional statements, and basic input validation in a single C source file.

## Features

- Create an account with an automatically assigned account number
- View all created accounts
- Deposit money
- Withdraw money while checking for sufficient balance
- Check an account's balance and outstanding loan
- Transfer money between two different accounts
- Record a loan for an account
- Repay an outstanding loan
- Reject invalid amounts and handle account numbers that cannot be found
- Save account records in `accounts.txt` and reload them at startup
- Automatically save changes after account creation, deposits, withdrawals, transfers, and loan operations

## Requirements

- A C compiler, such as GCC
- A terminal or command prompt

## Compile and run

Save or download `Bank-Management-System.c`, then run these commands in the folder containing the file:

```bash
gcc -std=c11 -Wall -Wextra -pedantic Bank-Management-System.c -o bank_management
./bank_management
```

On Windows, run the compiled program with:

```text
bank_management.exe
```

If your compiler reports an issue with `isfinite`, use a C11-compatible compiler such as a current version of GCC.

## File storage

The program uses standard C file handling to store account records in a text file named `accounts.txt` in the program's current working directory. It loads saved records when it starts and saves account changes after successful operations and when exiting. Keep `accounts.txt` in the same working directory when you run the program again.

Each line stores the account number, name, balance, and outstanding loan separated by `|`. Avoid using the `|` character in account names because it is used as the file's field separator.

The first account number is `1001`; new account numbers continue after the highest account number loaded from the file. The program supports up to 100 accounts.

## Important limitations

- **Keep the data file:** account records persist in `accounts.txt`. If that file is deleted or moved, the program will not find those saved records.
- This is an educational simulation, not real banking software. It does not connect to a bank or use a real database.
- Loan applications are recorded immediately for demonstration purposes; there is no real credit assessment or approval process.
- Money values use `float` for introductory learning. Real financial systems require more careful monetary representation and security controls.

## Concepts demonstrated

- `struct` and arrays of structures
- Functions and function declarations
- `if` / `else` statements and loops
- Console input/output with `scanf` and `printf`
- File input/output with `fopen`, `fscanf`, `fprintf`, and `fclose`
- Basic input validation and account lookup

## Academic note

This project is intended for learning and demonstration. Make sure you understand how each function works so you can explain the code during your assignment presentation or viva.
