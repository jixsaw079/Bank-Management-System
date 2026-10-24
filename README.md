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
- Store account records in an array while the program is running

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

## How account numbers work

The first account receives account number `1001`, the second `1002`, and so on. The program supports up to 100 accounts during one run.

## Important limitations

- **Data is temporary:** accounts are stored in memory, so all account data is lost when the program exits.
- This is an educational simulation, not real banking software. It does not connect to a bank or use a real database.
- Loan applications are recorded immediately for demonstration purposes; there is no real credit assessment or approval process.
- Money values use `float` for introductory learning. Real financial systems require more careful monetary representation and security controls.

## Concepts demonstrated

- `struct` and arrays of structures
- Functions and function declarations
- `if` / `else` statements and loops
- Input/output with `scanf` and `printf`
- Basic input validation and account lookup

## Academic note

This project is intended for learning and demonstration. Make sure you understand how each function works so you can explain the code during your assignment presentation or viva.
