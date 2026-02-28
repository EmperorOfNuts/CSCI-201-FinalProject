# Library Management System

## Core Features

### Book Management
- Track books by title, author, genre, and type (Printed/E-Book)
- Status tracking (Available/Checked Out)
- Search by title, author, or genre
- Filter views for available books and checked-out books, 

### Patron Management
- Register library patrons with auto-generated IDs
- Track currently checked out books per patron
- Look up patrons by ID or name
- Prevent duplicate checkouts

### Transaction System
- Checkout and return books with a single click
- Complete transaction history with timestamps
- Overdue tracking (visual indicator in transaction view)

### Data Persistence
- All data automatically saved to text files
- Load on startup, save on exit or manually
- Files used:
  - `Books.txt`
  - `Patrons.txt`
  - `Transactions.txt`

## Technical Implementation

### Architecture
- **C++20** with Qt6 framework for GUI
- Object-oriented design with inheritance (`Book` → `EBook`/`PrintedBook`)
- Memory management
- Exception handling

### Key Classes
- `Library`: Central coordinator for all operations
- `Book` (abstract)
- `EBook`/`PrintedBook`: Specialized book types
- `Patron`: Library member management
- `Transaction`: Records of checkouts/returns
- `Date`: Timestamp handling

### Design Patterns
- Observer pattern (Qt signals/slots)
- Factory methods (file parsing)
- Resource management

## User Interface

The dark-themed GUI includes:
- Main book table with sortable columns
- Search bar with type selection
- Patron lookup by ID or name
- Transaction history viewer with detailed view
- Patron information dialog with borrowed books list

## Requirements

- C++20 
- Qt6 framework
- CMake 3.16+
