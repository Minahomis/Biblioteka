#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

class Database : public QObject
{
    Q_OBJECT

public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    bool connectToDatabase();
    QSqlTableModel* getTableModel(const QString &tableName);
    bool addBook(const QString &title, int authorId, int genreId, int publisherId, int year, int copies);
    bool deleteRecord(const QString &tableName, int recordId);
    bool saveRecord(const QString &tableName, int recordId);
    QStringList getAuthors();
    QStringList getGenres();
    QStringList getPublishers();
    QMap<int, QString> getAuthorsMap();
    QMap<int, QString> getGenresMap();
    QMap<int, QString> getPublishersMap();

private:
    QSqlDatabase m_db;
    bool createTablesIfNotExist();
};

#endif // DATABASE_H 