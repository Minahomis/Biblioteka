#include "database.h"
#include <QApplication>

Database::Database(QObject *parent)
    : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setDatabaseName("biblioteka");
    m_db.setUserName("postgres");
    m_db.setHostName("localhost");
    m_db.setPort(5432);
}

Database::~Database()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool Database::connectToDatabase()
{
    if (!m_db.open()) {
        QMessageBox::critical(nullptr, "Ошибка подключения", 
                             "Не удалось подключиться к базе данных: " + m_db.lastError().text());
        return false;
    }
    
    qDebug() << "Подключение к базе данных успешно установлено";
    return createTablesIfNotExist();
}

QSqlTableModel* Database::getTableModel(const QString &tableName)
{
    QSqlTableModel *model = new QSqlTableModel(this, m_db);
    model->setTable(tableName);
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();
    return model;
}

bool Database::addBook(const QString &title, int authorId, int genreId, int publisherId, int year, int copies)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO books (title, author_id, genre_id, publisher_id, publish_year, total_copies) "
                  "VALUES (:title, :author_id, :genre_id, :publisher_id, :publish_year, :total_copies)");
    query.bindValue(":title", title);
    query.bindValue(":author_id", authorId);
    query.bindValue(":genre_id", genreId);
    query.bindValue(":publisher_id", publisherId);
    query.bindValue(":publish_year", year);
    query.bindValue(":total_copies", copies);
    
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Ошибка", "Не удалось добавить книгу: " + query.lastError().text());
        return false;
    }
    return true;
}

bool Database::deleteRecord(const QString &tableName, int recordId)
{
    QSqlQuery query(m_db);
    QString idColumn = "id";
    if (tableName == "books") {
        idColumn = "book_id";
    }
    query.prepare(QString("DELETE FROM %1 WHERE %2 = :id").arg(tableName, idColumn));
    query.bindValue(":id", recordId);
    
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Ошибка", "Не удалось удалить запись: " + query.lastError().text());
        return false;
    }
    return true;
}

QStringList Database::getAuthors()
{
    QStringList authors;
    QSqlQuery query("SELECT full_name FROM authors ORDER BY full_name", m_db);
    while (query.next()) {
        authors << query.value(0).toString();
    }
    return authors;
}

QStringList Database::getGenres()
{
    QStringList genres;
    QSqlQuery query("SELECT name FROM genres ORDER BY name", m_db);
    while (query.next()) {
        genres << query.value(0).toString();
    }
    return genres;
}

QStringList Database::getPublishers()
{
    QStringList publishers;
    QSqlQuery query("SELECT name FROM publishers ORDER BY name", m_db);
    while (query.next()) {
        publishers << query.value(0).toString();
    }
    return publishers;
}

QMap<int, QString> Database::getAuthorsMap()
{
    QMap<int, QString> authors;
    QSqlQuery query("SELECT author_id, full_name FROM authors ORDER BY full_name", m_db);
    while (query.next()) {
        authors[query.value(0).toInt()] = query.value(1).toString();
    }
    return authors;
}

QMap<int, QString> Database::getGenresMap()
{
    QMap<int, QString> genres;
    QSqlQuery query("SELECT genre_id, name FROM genres ORDER BY name", m_db);
    while (query.next()) {
        genres[query.value(0).toInt()] = query.value(1).toString();
    }
    return genres;
}

QMap<int, QString> Database::getPublishersMap()
{
    QMap<int, QString> publishers;
    QSqlQuery query("SELECT publisher_id, name FROM publishers ORDER BY name", m_db);
    while (query.next()) {
        publishers[query.value(0).toInt()] = query.value(1).toString();
    }
    return publishers;
}

bool Database::createTablesIfNotExist()
{
    QSqlQuery query(m_db);
    
    // Создание таблицы авторов
    if (!query.exec("CREATE TABLE IF NOT EXISTS authors ("
                   "author_id SERIAL PRIMARY KEY, "
                   "name VARCHAR(255) NOT NULL)")) {
        qDebug() << "Ошибка создания таблицы authors:" << query.lastError().text();
        return false;
    }
    
    // Создание таблицы жанров
    if (!query.exec("CREATE TABLE IF NOT EXISTS genres ("
                   "genre_id SERIAL PRIMARY KEY, "
                   "name VARCHAR(255) NOT NULL)")) {
        qDebug() << "Ошибка создания таблицы genres:" << query.lastError().text();
        return false;
    }
    
    // Создание таблицы издательств
    if (!query.exec("CREATE TABLE IF NOT EXISTS publishers ("
                   "publisher_id SERIAL PRIMARY KEY, "
                   "name VARCHAR(255) NOT NULL)")) {
        qDebug() << "Ошибка создания таблицы publishers:" << query.lastError().text();
        return false;
    }
    
    // Создание таблицы читателей
    if (!query.exec("CREATE TABLE IF NOT EXISTS readers ("
                   "reader_id SERIAL PRIMARY KEY, "
                   "name VARCHAR(255) NOT NULL, "
                   "email VARCHAR(255))")) {
        qDebug() << "Ошибка создания таблицы readers:" << query.lastError().text();
        return false;
    }
    
    // Создание таблицы книг
    if (!query.exec("CREATE TABLE IF NOT EXISTS books ("
                   "book_id SERIAL PRIMARY KEY, "
                   "title VARCHAR(255) NOT NULL, "
                   "author_id INTEGER REFERENCES authors(author_id), "
                   "genre_id INTEGER REFERENCES genres(genre_id), "
                   "publisher_id INTEGER REFERENCES publishers(publisher_id), "
                   "publish_year INTEGER, "
                   "total_copies INTEGER DEFAULT 1)")) {
        qDebug() << "Ошибка создания таблицы books:" << query.lastError().text();
        return false;
    }
    
    // Добавление тестовых данных, если таблицы пустые
    query.exec("SELECT COUNT(*) FROM authors");
    query.next();
    if (query.value(0).toInt() == 0) {
        query.exec("INSERT INTO authors (name) VALUES ('Лев Толстой'), ('Федор Достоевский'), ('Александр Пушкин')");
    }
    
    query.exec("SELECT COUNT(*) FROM genres");
    query.next();
    if (query.value(0).toInt() == 0) {
        query.exec("INSERT INTO genres (name) VALUES ('Роман'), ('Поэзия'), ('Драма'), ('Фантастика')");
    }
    
    query.exec("SELECT COUNT(*) FROM publishers");
    query.next();
    if (query.value(0).toInt() == 0) {
        query.exec("INSERT INTO publishers (name) VALUES ('АСТ'), ('Эксмо'), ('Росмэн'), ('Дрофа')");
    }
    
    return true;
}

bool Database::saveRecord(const QString &tableName, int recordId)
{
    // Для QSqlTableModel изменения сохраняются автоматически при редактировании
    // Эта функция может быть использована для дополнительной валидации или логирования
    QSqlQuery query(m_db);
    query.prepare(QString("SELECT COUNT(*) FROM %1 WHERE %2 = :id")
                  .arg(tableName, tableName == "books" ? "book_id" : "id"));
    query.bindValue(":id", recordId);
    
    if (!query.exec() || !query.next()) {
        QMessageBox::warning(nullptr, "Ошибка", "Не удалось проверить существование записи");
        return false;
    }
    
    if (query.value(0).toInt() == 0) {
        QMessageBox::warning(nullptr, "Ошибка", "Запись не найдена");
        return false;
    }
    
    return true;
} 