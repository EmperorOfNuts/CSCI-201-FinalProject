#include "MainWindow.hpp"
#include <QMessageBox>
#include <QInputDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMenuBar>
#include <QMenu>
#include <QStackedWidget>
#include <QStatusBar>
#include <QGroupBox>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>

MainWindow::MainWindow(Library* lib, QWidget *parent)
    : QMainWindow(parent)
    , library(lib)
{
    setupUI();
    setupMenuBar();
    refreshBookTable();

    statusBar()->showMessage("Ready");
}

void MainWindow::setupUI() {
    auto* centralWidget = new QWidget(this);
    auto* mainLayout = new QVBoxLayout(centralWidget);

    // ===== Title =====
    auto* titleLabel = new QLabel("Library Management System", centralWidget);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin: 10px;");
    mainLayout->addWidget(titleLabel);

    // ===== Search Section =====
    auto* searchGroup = new QGroupBox("Search Books", centralWidget);
    auto* searchLayout = new QHBoxLayout(searchGroup);

    searchEdit = new QLineEdit(searchGroup);
    searchEdit->setPlaceholderText("Enter search term...");
    searchLayout->addWidget(searchEdit);

    auto* searchButton = new QPushButton("Search", searchGroup);
    searchLayout->addWidget(searchButton);

    searchTypeCombo = new QComboBox(searchGroup);
    searchTypeCombo->addItems({"Title", "Author", "Genre"});
    searchLayout->addWidget(searchTypeCombo);

    mainLayout->addWidget(searchGroup);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);

    // ===== Book Table =====
    bookTable = new QTableWidget(centralWidget);
    bookTable->setColumnCount(4);
    QStringList headers;
    headers << "Title" << "Author" << "Genre" << "Status";
    bookTable->setHorizontalHeaderLabels(headers);

    // Set alternating row colors to subtle grays
    // Screw CSS styling again BTW I hate this
    bookTable->setAlternatingRowColors(true);
    bookTable->setStyleSheet(
        "QTableWidget {"
        "    alternate-background-color: #303234;"
        "    background-color: #18191a;"
        "}"
    );

    bookTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    mainLayout->addWidget(bookTable);

    // ===== Patron Lookup Section =====
    auto* patronGroup = new QGroupBox("Patron Lookup", centralWidget);
    auto* patronLayout = new QHBoxLayout(patronGroup);

    auto* lookupLabel = new QLabel("Patron ID or Name:", patronGroup);
    patronLayout->addWidget(lookupLabel);

    patronLookupEdit = new QLineEdit(patronGroup);
    patronLookupEdit->setPlaceholderText("Enter ID number or full name...");
    patronLayout->addWidget(patronLookupEdit);

    auto* lookupButton = new QPushButton("Lookup Patron", patronGroup);
    patronLayout->addWidget(lookupButton);

    mainLayout->addWidget(patronGroup);

    connect(lookupButton, &QPushButton::clicked, this, &MainWindow::onLookupPatronClicked);
    connect(patronLookupEdit, &QLineEdit::returnPressed, this, &MainWindow::onLookupPatronClicked);

    // ===== Transactions Section =====
    // EVERY CHILD NEEDS A PARENT!!!
    auto* inputGroup = new QGroupBox("Transaction", centralWidget);
    auto* inputGroupLayout = new QVBoxLayout(inputGroup);
    auto* formLayout = new QFormLayout();

    patronIdEdit = new QLineEdit(inputGroup);
    patronIdEdit->setPlaceholderText("Enter patron ID");
    formLayout->addRow("Patron ID:", patronIdEdit);

    bookTitleEdit = new QLineEdit(inputGroup);
    bookTitleEdit->setPlaceholderText("Enter book title");
    formLayout->addRow("Book Title:", bookTitleEdit);

    inputGroupLayout->addLayout(formLayout);

    // Buttons
    auto* actionButtons = new QHBoxLayout();

    checkoutButton = new QPushButton("Checkout Book", inputGroup);
    connect(checkoutButton, &QPushButton::clicked, this, &MainWindow::onCheckoutClicked);
    actionButtons->addWidget(checkoutButton);

    returnButton = new QPushButton("Return Book", inputGroup);
    connect(returnButton, &QPushButton::clicked, this, &MainWindow::onReturnClicked);
    actionButtons->addWidget(returnButton);

    inputGroupLayout->addLayout(actionButtons);

    mainLayout->addWidget(inputGroup);

    // ===== Action Buttons =====
    auto* actionGroup = new QGroupBox("Management", centralWidget);
    auto* actionGroupLayout = new QHBoxLayout(actionGroup);

    // Transactions button - leftmost
    auto* viewTransactionsButton = new QPushButton("View Transactions", actionGroup);
    connect(viewTransactionsButton, &QPushButton::clicked, this, &MainWindow::onViewTransactionsClicked);
    actionGroupLayout->addWidget(viewTransactionsButton);

    auto* addPatronButton = new QPushButton("Add New Patron", actionGroup);
    connect(addPatronButton, &QPushButton::clicked, this, &MainWindow::onAddPatronClicked);
    actionGroupLayout->addWidget(addPatronButton);

    auto* addBookButton = new QPushButton("Add New Book", actionGroup);
    connect(addBookButton, &QPushButton::clicked, this, &MainWindow::onAddBookClicked);
    actionGroupLayout->addWidget(addBookButton);

    auto* refreshButton = new QPushButton("Refresh", actionGroup);
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::refreshBookTable);
    actionGroupLayout->addWidget(refreshButton);

    mainLayout->addWidget(actionGroup);

    // Set the central widget
    setCentralWidget(centralWidget);

    // Window properties
    resize(1000, 800);
    setWindowTitle("Library Management System");
}

void MainWindow::setupMenuBar()
{
    // File menu
    auto* fileMenu = menuBar()->addMenu("&File");

    const auto* saveAction = fileMenu->addAction("&Save Data");
    connect(saveAction, &QAction::triggered, this, [this]() {
        try {
            library->saveData();
            statusBar()->showMessage("Data saved successfully", 3000);
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Error", QString("Failed to save: ") + e.what());
        }
    });

    fileMenu->addSeparator();

    const auto* exitAction = fileMenu->addAction("E&xit");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // View menu
    auto* viewMenu = menuBar()->addMenu("&View");

    const auto* availableAction = viewMenu->addAction("Available Books Only");
    connect(availableAction, &QAction::triggered, this, [this]() {
        bookTable->setRowCount(0);
        for (const auto& books = library->getBooks(); const auto& book : books) {
            if (book->getStatus() == Book::BookStatus::Available) {
                const int row = bookTable->rowCount();
                bookTable->insertRow(row);
                bookTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(book->getTitle())));
                bookTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(book->getAuthor())));
                bookTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(Book::genreToString(book->getGenre()))));
                bookTable->setItem(row, 3, new QTableWidgetItem("Available"));
            }
        }
    });

    auto* checkedOutAction = viewMenu->addAction("Checked Out Books Only");
    connect(checkedOutAction, &QAction::triggered, this, [this]() {
        bookTable->setRowCount(0);
        for (const auto& books = library->getBooks(); const auto& book : books) {
            if (book->getStatus() == Book::BookStatus::CheckedOut) {
                const int row = bookTable->rowCount();
                bookTable->insertRow(row);
                bookTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(book->getTitle())));
                bookTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(book->getAuthor())));
                bookTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(Book::genreToString(book->getGenre()))));
                bookTable->setItem(row, 3, new QTableWidgetItem("Checked Out"));
            }
        }
    });

    viewMenu->addSeparator();

    const auto* allBooksAction = viewMenu->addAction("All Books");
    connect(allBooksAction, &QAction::triggered, this, &MainWindow::refreshBookTable);

    // Help menu
    auto* helpMenu = menuBar()->addMenu("&Help");

    const auto* aboutAction = helpMenu->addAction("&About");
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, "About Library System",
            "<h2>Library Management System</h2>"
            "<p>Final Project for CSCI 201</p>"
            "<p>A complete library management system with book tracking, "
            "patron management, and transaction history.</p>"
            "<p>I really wished that there would be no more Raptor or "
            "flowcharting, but I stand corrected. - Stella Song</p>");
    });
}

void MainWindow::displayPatronInfo(const Patron* patron) {
    QDialog dialog(this);
    dialog.setWindowTitle("Patron Information");
    dialog.resize(500, 500);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(10);

    // Header
    const QString headerText = QString("Patron: %1 (ID: %2)")
        .arg(QString::fromStdString(patron->getName()))
        .arg(patron->getId());

    QLabel* headerLabel = new QLabel(headerText, &dialog);
    headerLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    headerLabel->setAlignment(Qt::AlignLeft);
    layout->addWidget(headerLabel);

    const auto& borrowedBooks = patron->getBorrowedBooks();

    QLabel* booksHeader = new QLabel(
        QString("Currently Checked Out Books (%1):").arg(borrowedBooks.size()),
        &dialog
    );
    booksHeader->setStyleSheet("font-size: 14px; font-weight: bold;");
    booksHeader->setAlignment(Qt::AlignLeft);
    layout->addWidget(booksHeader);

    if (borrowedBooks.empty()) {
        QLabel* noBooksLabel = new QLabel("No books currently checked out.", &dialog);
        noBooksLabel->setStyleSheet("color: #888888; margin-left: 10px;");
        layout->addWidget(noBooksLabel);
    } else {
        QTableWidget* booksTable = new QTableWidget(borrowedBooks.size(), 2, &dialog);
        booksTable->setHorizontalHeaderLabels({"Title", "Author"});
        booksTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        booksTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        booksTable->setAlternatingRowColors(true);
        booksTable->verticalHeader()->setVisible(false);
        booksTable->setShowGrid(false);

        booksTable->setStyleSheet(
            "QTableWidget {"
            "    alternate-background-color: #303234;"
            "    background-color: #18191a;"
            "}"
            "QTableWidget::item {"
            "    padding: 1px;"
            "}"
        );

        for (size_t i = 0; i < borrowedBooks.size(); ++i) {
            booksTable->setItem(i, 0, new QTableWidgetItem(
                QString::fromStdString(borrowedBooks[i]->getTitle())));
            booksTable->setItem(i, 1, new QTableWidgetItem(
                QString::fromStdString(borrowedBooks[i]->getAuthor())));
        }

        booksTable->resizeColumnsToContents();
        booksTable->setFixedHeight(450);
        layout->addWidget(booksTable);
    }

    layout->addStretch();

    // Close button
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->addStretch();

    QPushButton* closeButton = new QPushButton("Close", &dialog);
    closeButton->setFixedWidth(80);
    closeButton->setFixedHeight(25);
    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    buttonLayout->addWidget(closeButton);

    layout->addLayout(buttonLayout);

    // Autofill the transaction patron ID for convenience
    patronIdEdit->setText(QString::number(patron->getId()));

    dialog.exec();
}

void MainWindow::refreshBookTable() const {
    bookTable->setRowCount(0);

    const auto& books = library->getBooks();
    for (const auto& book : books) {
        const int row = bookTable->rowCount();
        bookTable->insertRow(row);

        bookTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(book->getTitle())));
        bookTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(book->getAuthor())));
        bookTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(Book::genreToString(book->getGenre()))));

        QString status = (book->getStatus() == Book::BookStatus::Available) ? "Available" : "Checked Out";
        bookTable->setItem(row, 3, new QTableWidgetItem(status));
    }

    statusBar()->showMessage(QString("Displaying %1 books").arg(books.size()), 3000);
}

void MainWindow::onSearchClicked()
{
    const QString term = searchEdit->text();
    const QString type = searchTypeCombo->currentText();

    if (term.isEmpty()) {
        refreshBookTable();
        return;
    }

    bookTable->setRowCount(0);

    std::vector<Book*> results;
    if (type == "Title") results = library->searchBooksByTitle(term.toStdString());
    else if (type == "Author") results = library->searchBooksByAuthor(term.toStdString());
    else if (type == "Genre") {
        try {
            Book::Genre genre = Book::stringToGenre(term.toStdString());
            results = library->searchBooksByGenre(genre);
        } catch (...) {
            QMessageBox::warning(this, "Invalid Genre.",
            "Please enter a valid genre (Fiction, NonFiction, Mystery, Science, Biography)");
            refreshBookTable();
            return;
        }
    }

    for (const auto& book : results) {
        const int row = bookTable->rowCount();
        bookTable->insertRow(row);
        bookTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(book->getTitle())));
        bookTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(book->getAuthor())));
        bookTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(Book::genreToString(book->getGenre()))));

        QString status = (book->getStatus() == Book::BookStatus::Available) ? "Available" : "Checked Out";
        bookTable->setItem(row, 3, new QTableWidgetItem(status));
    }

    statusBar()->showMessage(QString("Found %1 books.").arg(results.size()));
}

void MainWindow::onLookupPatronClicked() {
    const QString input = patronLookupEdit->text().trimmed();

    if (input.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a Patron ID or Name.");
        return;
    }

    const Patron* foundPatron = nullptr;

    // Try to parse as integer (ID lookup)
    bool isId;
    const int id = input.toInt(&isId);

    if (isId) {
        // Lookup by ID
        foundPatron = library->findPatron(id);
        if (!foundPatron) {
            QMessageBox::information(this, "Not Found",
                QString("No patron found with ID: %1").arg(id));
            return;
        }
    } else {
        // Lookup by name
        const auto& patrons = library->getPatrons();
        const QString searchName = input.toLower();

        for (auto& patron : patrons) {
            if (QString patronName = QString::fromStdString(patron.getName()).toLower(); patronName == searchName) {
                foundPatron = const_cast<Patron*>(&patron);
                break;
            }
        }

        if (!foundPatron) {
            QMessageBox::information(this, "Not Found",
                QString("No patron found with name: %1").arg(input));
            return;
        }
    }

    displayPatronInfo(foundPatron);
}

void MainWindow::onCheckoutClicked()
{
    const QString patronIdText = patronIdEdit->text();
    const QString bookTitle = bookTitleEdit->text();

    if (patronIdText.isEmpty() || bookTitle.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both Patron ID and Book Title.");
        return;
    }

    bool ok;
    const int patronId = patronIdText.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Patron ID must be a number.");
        return;
    }

    // Proper saving/loading to ensure refresh is still allowed
    try {
        library->checkoutBook(patronId, bookTitle.toStdString());
        refreshBookTable();
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Checkout Failed", QString("Error: ") + e.what());
    }
}

void MainWindow::onReturnClicked()
{
    const QString patronIdText = patronIdEdit->text();
    const QString bookTitle = bookTitleEdit->text();

    if (patronIdText.isEmpty() || bookTitle.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both Patron ID and Book Title.");
        return;
    }

    bool ok;
    const int patronId = patronIdText.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Patron ID must be a number.");
        return;
    }

    try {
        library->returnBook(patronId, bookTitle.toStdString());
        refreshBookTable();
        patronIdEdit->clear();
        bookTitleEdit->clear();
        statusBar()->showMessage("Book returned successfully!", 3000);
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Return Failed", QString("Error: ") + e.what());
    }
}

void MainWindow::onViewTransactionsClicked()
{
    const auto& transactions = library->getTransactions();

    if (transactions.empty()) {
        QMessageBox::information(this, "Transactions", "No transactions recorded.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Transaction History");
    dialog.resize(600, 500);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    QLabel* titleLabel = new QLabel(QString("Total Transactions: %1").arg(transactions.size()), &dialog);
    titleLabel->setStyleSheet("font-size: 14px; font-weight: bold; margin: 5px;");
    layout->addWidget(titleLabel);

    QTableWidget* table = new QTableWidget(&dialog);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"Date", "Patron ID", "Type", "Book Title", "Return By"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setAlternatingRowColors(true);
    table->setRowCount(transactions.size());
    table->setStyleSheet(
        "QTableWidget {"
        "    alternate-background-color: #303234;"
        "    background-color: #18191a;"
        "}"
    );

    int row = 0;
    for (const auto& t : transactions) {
        table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(t.getDate().toString())));
        table->setItem(row, 1, new QTableWidgetItem(QString::number(t.getPatronID())));
        table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(t.typeToString())));
        table->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(t.getBookTitle())));

        QString returnInfo;
        if (auto retDate = t.getReturnDate(); retDate.has_value()) returnInfo = QString::fromStdString(retDate->toString());
        table->setItem(row, 4, new QTableWidgetItem(returnInfo));

        row++;
    }

    table->resizeColumnsToContents();
    layout->addWidget(table);

    auto* closeButton = new QPushButton("Close", &dialog);
    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout->addWidget(closeButton, 0, Qt::AlignRight);

    dialog.exec();
}

void MainWindow::onAddPatronClicked()
{
    bool ok;
    const QString name = QInputDialog::getText(this, "Add New Patron",
                                          "Enter patron name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    try {
        library->addPatron(Patron(name.toStdString()));
        statusBar()->showMessage("Patron added successfully!", 3000);
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Failed to Add Patron", QString("Error: ") + e.what());
    }
}

// Orphans here consumed 20 GB of my RAM
void MainWindow::onAddBookClicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Book");

    QFormLayout form(&dialog);

    QLineEdit titleEdit(&dialog);
    form.addRow("Title:", &titleEdit);

    QLineEdit authorEdit(&dialog);
    form.addRow("Author:", &authorEdit);

    QComboBox genreCombo(&dialog);
    genreCombo.addItems({"Fiction", "Non-Fiction", "Mystery", "Science", "Biography"});
    form.addRow("Genre:", &genreCombo);

    QComboBox typeCombo(&dialog);
    typeCombo.addItems({"Printed Book", "EBook"});
    form.addRow("Type:", &typeCombo);

    QStackedWidget extraStack(&dialog);

    auto* printedWidget = new QWidget(&extraStack);
    auto* printedLayout = new QHBoxLayout(printedWidget);
    QLineEdit pageCountEdit(printedWidget);
    pageCountEdit.setPlaceholderText("Enter page count");
    printedLayout->addWidget(new QLabel("Pages:", printedWidget));
    printedLayout->addWidget(&pageCountEdit);
    extraStack.addWidget(printedWidget);

    auto* ebookWidget = new QWidget(&extraStack);
    auto* ebookLayout = new QHBoxLayout(ebookWidget);
    QLineEdit fileSizeEdit(ebookWidget);
    fileSizeEdit.setPlaceholderText("Enter file size in MB");
    ebookLayout->addWidget(new QLabel("File Size (MB):", ebookWidget));
    ebookLayout->addWidget(&fileSizeEdit);
    extraStack.addWidget(ebookWidget);

    form.addRow("Details:", &extraStack);

    QObject::connect(&typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [&](const int index) { extraStack.setCurrentIndex(index); });

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttons);

    QObject::connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        const QString title = titleEdit.text();
        const QString author = authorEdit.text();
        const QString genreStr = genreCombo.currentText();

        if (title.isEmpty() || author.isEmpty()) {
            QMessageBox::warning(this, "Error", "Title and author are required.");
            return;
        }

        try {
            const Book::Genre genre = Book::stringToGenre(genreStr.toStdString());

            if (typeCombo.currentText() == "Printed Book") {
                bool ok;
                const int pages = pageCountEdit.text().toInt(&ok);
                if (!ok || pages <= 0) {
                    QMessageBox::warning(this, "Error", "Invalid page count.");
                    return;
                }
                library->addBook(new PrintedBook(title.toStdString(), author.toStdString(), genre, pages));
            } else {
                bool ok;
                const double fileSize = fileSizeEdit.text().toDouble(&ok);
                if (!ok || fileSize <= 0) {
                    QMessageBox::warning(this, "Error", "Invalid file size.");
                    return;
                }
                library->addBook(new EBook(title.toStdString(), author.toStdString(), genre, fileSize));
            }

            refreshBookTable();
            statusBar()->showMessage("Book added successfully", 3000);

        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Error", QString("Failed to add book: ") + e.what());
        }
    }
}

