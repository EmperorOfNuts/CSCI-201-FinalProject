#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include "Library.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Library* lib, QWidget *parent = nullptr);

private slots:
    void refreshBookTable() const;
    void onCheckoutClicked();
    void onReturnClicked();

    void onViewTransactionsClicked();
    void onAddPatronClicked();
    void onAddBookClicked();
    void onSearchClicked();

private:
    void setupUI();
    void setupMenuBar();

    Library* library;
    QTableWidget* bookTable{};
    QLineEdit* patronIdEdit{};
    QLineEdit* bookTitleEdit{};
    QLineEdit* searchEdit{};
    QComboBox* searchTypeCombo{};
    QPushButton* checkoutButton{};
    QPushButton* returnButton{};
};

#endif