#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QHeaderView>
#include <QSqlRelationalDelegate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_db(new Database(this))
    , m_currentModel(nullptr)
    , m_booksRelModel(nullptr)
{
    ui->setupUi(this);
    
    if (!m_db->connectToDatabase()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к базе данных");
        return;
    }
    
    setupUI();
    if (m_tableCombo->count() > 0) {
        loadTable(m_tableCombo->currentText());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    setWindowTitle("Библиотека - Управление базой данных");
    resize(1000, 700);
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QLabel *titleLabel = new QLabel("Библиотечная система", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin: 10px;");
    mainLayout->addWidget(titleLabel);
    QHBoxLayout *comboLayout = new QHBoxLayout();
    QLabel *comboLabel = new QLabel("Выберите таблицу:", this);
    m_tableCombo = new QComboBox(this);
    m_tableCombo->addItems({"Книги", "Авторы", "Жанры", "Издательства", "Читатели", "Выдачи"});
    comboLayout->addWidget(comboLabel);
    comboLayout->addWidget(m_tableCombo);
    comboLayout->addStretch();
    mainLayout->addLayout(comboLayout);
    // Строка поиска
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Поиск...");
    m_searchEdit->setToolTip("Введите текст для поиска");
    mainLayout->addWidget(m_searchEdit);
    // Таблица
    m_tableView = new QTableView(this);
    m_tableView->setAlternatingRowColors(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    m_tableView->setToolTip("Двойной клик — редактировать запись");
    mainLayout->addWidget(m_tableView);
    // Кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_addButton = new QPushButton("Добавить", this);
    m_addButton->setToolTip("Добавить новую запись");
    m_deleteButton = new QPushButton("Удалить", this);
    m_deleteButton->setToolTip("Удалить выбранную запись");
    m_saveButton = new QPushButton("Сохранить", this);
    m_saveButton->setToolTip("Сохранить изменения в таблице");
    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_deleteButton);
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    // Сигналы
    connect(m_tableCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged), this, &MainWindow::onTableChanged);
    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(m_deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);
    connect(m_saveButton, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
    connect(m_searchEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
}

void MainWindow::setupBooksRelationalModel()
{
    if (m_booksRelModel) {
        delete m_booksRelModel;
        m_booksRelModel = nullptr;
    }
    m_booksRelModel = new QSqlRelationalTableModel(this, QSqlDatabase::database());
    m_booksRelModel->setTable("books");
    m_booksRelModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_booksRelModel->setRelation(2, QSqlRelation("genres", "genre_id", "name"));
    m_booksRelModel->setRelation(3, QSqlRelation("authors", "author_id", "full_name"));
    m_booksRelModel->setRelation(4, QSqlRelation("publishers", "publisher_id", "name"));
    m_booksRelModel->select();
    m_tableView->setModel(m_booksRelModel);
    m_tableView->setItemDelegate(new QSqlRelationalDelegate(m_tableView));
}

void MainWindow::onTableChanged(const QString &tableName)
{
    loadTable(tableName);
}

void MainWindow::loadTable(const QString &tableName)
{
    // Очищаем старые модели
    if (m_currentModel) {
        delete m_currentModel;
        m_currentModel = nullptr;
    }
    if (m_booksRelModel) {
        delete m_booksRelModel;
        m_booksRelModel = nullptr;
    }
    
    if (tableName == "Книги") {
        setupBooksRelationalModel();
        updateTableHeaders();
        m_tableView->resizeColumnsToContents();
        m_tableView->horizontalHeader()->setStretchLastSection(true);
        applyBookFilter(m_searchEdit->text());
    } else {
        QString dbTableName;
        if (tableName == "Авторы") dbTableName = "authors";
        else if (tableName == "Жанры") dbTableName = "genres";
        else if (tableName == "Издательства") dbTableName = "publishers";
        else if (tableName == "Читатели") dbTableName = "readers";
        else if (tableName == "Выдачи") dbTableName = "issues";
        else return;
        
        m_currentModel = m_db->getTableModel(dbTableName);
        if (m_currentModel) {
            m_tableView->setModel(m_currentModel);
            updateTableHeaders();
            m_tableView->resizeColumnsToContents();
            m_tableView->horizontalHeader()->setStretchLastSection(true);
        }
    }
}

void MainWindow::updateTableHeaders()
{
    if (m_tableCombo->currentText() == "Книги" && m_booksRelModel) {
        m_booksRelModel->setHeaderData(0, Qt::Horizontal, "ID книги");
        m_booksRelModel->setHeaderData(1, Qt::Horizontal, "Название");
        m_booksRelModel->setHeaderData(2, Qt::Horizontal, "Жанр");
        m_booksRelModel->setHeaderData(3, Qt::Horizontal, "Автор");
        m_booksRelModel->setHeaderData(4, Qt::Horizontal, "Издательство");
        m_booksRelModel->setHeaderData(5, Qt::Horizontal, "Год издания");
        m_booksRelModel->setHeaderData(6, Qt::Horizontal, "Количество копий");
    } else if (m_currentModel) {
        QString tableName = m_tableCombo->currentText();
        if (tableName == "Авторы") {
            m_currentModel->setHeaderData(0, Qt::Horizontal, "ID автора");
            m_currentModel->setHeaderData(1, Qt::Horizontal, "Полное имя");
        } else if (tableName == "Жанры") {
            m_currentModel->setHeaderData(0, Qt::Horizontal, "ID жанра");
            m_currentModel->setHeaderData(1, Qt::Horizontal, "Название");
        } else if (tableName == "Издательства") {
            m_currentModel->setHeaderData(0, Qt::Horizontal, "ID издательства");
            m_currentModel->setHeaderData(1, Qt::Horizontal, "Название");
            m_currentModel->setHeaderData(2, Qt::Horizontal, "Адрес");
        } else if (tableName == "Читатели") {
            m_currentModel->setHeaderData(0, Qt::Horizontal, "ID читателя");
            m_currentModel->setHeaderData(1, Qt::Horizontal, "Полное имя");
            m_currentModel->setHeaderData(2, Qt::Horizontal, "Дата рождения");
            m_currentModel->setHeaderData(3, Qt::Horizontal, "Пол");
        } else if (tableName == "Выдачи") {
            m_currentModel->setHeaderData(0, Qt::Horizontal, "ID выдачи");
            m_currentModel->setHeaderData(1, Qt::Horizontal, "Книга ID");
            m_currentModel->setHeaderData(2, Qt::Horizontal, "Читатель ID");
            m_currentModel->setHeaderData(3, Qt::Horizontal, "Дата выдачи");
            m_currentModel->setHeaderData(4, Qt::Horizontal, "Дата возврата");
        }
    }
}

void MainWindow::onAddClicked()
{
    QString tableName = m_tableCombo->currentText();
    if (tableName == "Книги") {
        AddBookDialog dialog(m_db, this);
        if (dialog.exec() == QDialog::Accepted) {
            loadTable(tableName);
        }
    } else {
        QMessageBox::information(this, "Информация", 
                                "Добавление записей реализовано только для таблицы 'Книги'.\n"
                                "Для других таблиц используйте прямой доступ к базе данных.");
    }
}

void MainWindow::onDeleteClicked()
{
    QModelIndex currentIndex = m_tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Предупреждение", "Выберите запись для удаления");
        return;
    }
    
    int row = currentIndex.row();
    int recordId = 0;
    
    if (m_tableCombo->currentText() == "Книги" && m_booksRelModel) {
        recordId = m_booksRelModel->data(m_booksRelModel->index(row, 0)).toInt();
    } else if (m_currentModel) {
        recordId = m_currentModel->data(m_currentModel->index(row, 0)).toInt();
    } else {
        return;
    }
    
    QString tableName = m_tableCombo->currentText();
    QString dbTableName;
    if (tableName == "Книги") dbTableName = "books";
    else if (tableName == "Авторы") dbTableName = "authors";
    else if (tableName == "Жанры") dbTableName = "genres";
    else if (tableName == "Издательства") dbTableName = "publishers";
    else if (tableName == "Читатели") dbTableName = "readers";
    else if (tableName == "Выдачи") dbTableName = "issues";
    else return;
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение", 
                                                              "Вы уверены, что хотите удалить эту запись?",
                                                              QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (m_db->deleteRecord(dbTableName, recordId)) {
            loadTable(tableName);
            QMessageBox::information(this, "Успех", "Запись успешно удалена");
        }
    }
}

void MainWindow::onSaveClicked()
{
    bool success = false;
    
    if (m_tableCombo->currentText() == "Книги" && m_booksRelModel) {
        success = m_booksRelModel->submitAll();
    } else if (m_currentModel) {
        success = m_currentModel->submitAll();
    }
    
    if (success) {
        QMessageBox::information(this, "Успех", "Изменения успешно сохранены");
        loadTable(m_tableCombo->currentText()); // Обновляем таблицу
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить изменения");
    }
}

void MainWindow::onSearchTextChanged(const QString &text)
{
    if (m_tableCombo->currentText() == "Книги" && m_booksRelModel) {
        applyBookFilter(text);
    } else if (m_currentModel) {
        QString filter;
        if (!text.isEmpty()) {
            // Простой поиск по первой текстовой колонке
            filter = QString("name ILIKE '%%1%' OR full_name ILIKE '%%1%' OR title ILIKE '%%1%'").arg(text);
        }
        m_currentModel->setFilter(filter);
        m_currentModel->select();
    }
}

void MainWindow::applyBookFilter(const QString &text)
{
    if (!m_booksRelModel) return;
    
    QString filter;
    if (!text.isEmpty()) {
        filter = QString("title ILIKE '%%1%'").arg(text);
    }
    
    m_booksRelModel->setFilter(filter);
    m_booksRelModel->select();
}
