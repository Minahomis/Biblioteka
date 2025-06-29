#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QLineEdit>
#include "database.h"
#include "addbookdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTableChanged(const QString &tableName);
    void onAddClicked();
    void onDeleteClicked();
    void onSearchTextChanged(const QString &text);
    void onSaveClicked();

private:
    Ui::MainWindow *ui;
    Database *m_db;
    QComboBox *m_tableCombo;
    QTableView *m_tableView;
    QPushButton *m_addButton;
    QPushButton *m_deleteButton;
    QPushButton *m_saveButton;
    QLineEdit *m_searchEdit;
    QSqlTableModel *m_currentModel;
    QSqlRelationalTableModel *m_booksRelModel;
    
    void setupUI();
    void loadTable(const QString &tableName);
    void updateTableHeaders();
    void setupBooksRelationalModel();
    void applyBookFilter(const QString &text);
};
#endif // MAINWINDOW_H
