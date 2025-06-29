#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFormLayout>
#include <QDate>
#include "database.h"

class AddBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBookDialog(Database *db, QWidget *parent = nullptr);

private slots:
    void accept() override;

private:
    Database *m_db;
    QLineEdit *m_titleEdit;
    QComboBox *m_authorCombo;
    QComboBox *m_genreCombo;
    QComboBox *m_publisherCombo;
    QSpinBox *m_yearSpinBox;
    QSpinBox *m_copiesSpinBox;
    
    void setupUI();
    void loadComboBoxes();
};

#endif // ADDBOOKDIALOG_H 