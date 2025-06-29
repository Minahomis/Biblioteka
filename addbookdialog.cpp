#include "addbookdialog.h"
#include <QMessageBox>

AddBookDialog::AddBookDialog(Database *db, QWidget *parent)
    : QDialog(parent)
    , m_db(db)
{
    setWindowTitle("Добавить книгу");
    setModal(true);
    resize(400, 300);
    
    setupUI();
    loadComboBoxes();
}

void AddBookDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    QFormLayout *formLayout = new QFormLayout();
    
    // Название книги
    m_titleEdit = new QLineEdit(this);
    formLayout->addRow("Название:", m_titleEdit);
    
    // Автор
    m_authorCombo = new QComboBox(this);
    formLayout->addRow("Автор:", m_authorCombo);
    
    // Жанр
    m_genreCombo = new QComboBox(this);
    formLayout->addRow("Жанр:", m_genreCombo);
    
    // Издательство
    m_publisherCombo = new QComboBox(this);
    formLayout->addRow("Издательство:", m_publisherCombo);
    
    // Год издания
    m_yearSpinBox = new QSpinBox(this);
    m_yearSpinBox->setRange(1800, QDate::currentDate().year());
    m_yearSpinBox->setValue(QDate::currentDate().year());
    m_yearSpinBox->setSuffix(" г.");
    m_yearSpinBox->setToolTip("Год издания книги");
    formLayout->addRow("Год издания:", m_yearSpinBox);
    
    // Количество копий
    m_copiesSpinBox = new QSpinBox(this);
    m_copiesSpinBox->setRange(1, 100);
    m_copiesSpinBox->setValue(1);
    m_copiesSpinBox->setSuffix(" шт.");
    m_copiesSpinBox->setToolTip("Количество копий книги");
    formLayout->addRow("Количество копий:", m_copiesSpinBox);
    
    mainLayout->addLayout(formLayout);
    
    // Кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Добавить", this);
    QPushButton *cancelButton = new QPushButton("Отмена", this);
    
    connect(okButton, &QPushButton::clicked, this, &AddBookDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &AddBookDialog::reject);
    
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    
    mainLayout->addLayout(buttonLayout);
    
    setLayout(mainLayout);
}

void AddBookDialog::loadComboBoxes()
{
    // Загрузка авторов
    QMap<int, QString> authors = m_db->getAuthorsMap();
    for (auto it = authors.begin(); it != authors.end(); ++it) {
        m_authorCombo->addItem(it.value(), it.key());
    }
    
    // Загрузка жанров
    QMap<int, QString> genres = m_db->getGenresMap();
    for (auto it = genres.begin(); it != genres.end(); ++it) {
        m_genreCombo->addItem(it.value(), it.key());
    }
    
    // Загрузка издательств
    QMap<int, QString> publishers = m_db->getPublishersMap();
    for (auto it = publishers.begin(); it != publishers.end(); ++it) {
        m_publisherCombo->addItem(it.value(), it.key());
    }
}

void AddBookDialog::accept()
{
    if (m_titleEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите название книги");
        return;
    }
    
    int year = m_yearSpinBox->value();
    int currentYear = QDate::currentDate().year();
    
    if (year > currentYear) {
        QMessageBox::warning(this, "Ошибка", "Год издания не может быть больше текущего года");
        return;
    }
    
    if (year < 1800) {
        QMessageBox::warning(this, "Ошибка", "Год издания не может быть меньше 1800");
        return;
    }
    
    QString title = m_titleEdit->text().trimmed();
    int authorId = m_authorCombo->currentData().toInt();
    int genreId = m_genreCombo->currentData().toInt();
    int publisherId = m_publisherCombo->currentData().toInt();
    int copies = m_copiesSpinBox->value();
    
    if (m_db->addBook(title, authorId, genreId, publisherId, year, copies)) {
        QMessageBox::information(this, "Успех", "Книга успешно добавлена");
        QDialog::accept();
    }
} 