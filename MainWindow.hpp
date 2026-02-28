#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include "Library.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Library* lib, QWidget *parent = nullptr);

private:
    void setupUI();
    void setupMenuBar();
    void displayPatronInfo(const Patron* patron);

    Library* library;
    QTableWidget* bookTable{};
    QLineEdit* patronIdEdit{};
    QLineEdit* bookTitleEdit{};
    QLineEdit* searchEdit{};
    QComboBox* searchTypeCombo{};
    QPushButton* checkoutButton{};
    QPushButton* returnButton{};
    QLineEdit* patronLookupEdit{};

private slots:
    void refreshBookTable();
    void onSearchClicked();
    void onLookupPatronClicked();
    void onCheckoutClicked();
    void onReturnClicked();
    void onViewTransactionsClicked();
    void onAddPatronClicked();
    void onAddBookClicked();
};

#endif