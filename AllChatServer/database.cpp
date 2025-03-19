#include "database.h"

DataBase::DataBase(QObject *parent):QObject(parent){}

void DataBase::initDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");

    if (!db.open()) {
        qWarning() << "数据库打开失败：" << db.lastError().text();
        return;
    }

    // QSqlQuery query;
    // QString createTable = "CREATE TABLE IF NOT EXISTS users ("
    //                       "id TEXT PRIMARY KEY, "
    //                       "username TEXT UNIQUE, "
    //                       "password TEXT)";
    // if (!query.exec(createTable)) {
    //     qWarning() << "创建用户表失败：" << query.lastError().text();
    // }
}

bool DataBase::registerUser(const QString &username, const QString &password) {
    if (username.isEmpty() || password.isEmpty()) return false;

    // 生成唯一 ID
    QString userId = QUuid::createUuid().toString(QUuid::WithoutBraces);

    // 密码哈希加密存储（SHA-256）
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    QString hashedPassword = hash.toHex();

    QSqlQuery query;
    query.prepare("INSERT INTO users (id, username, password) VALUES (:id, :username, :password)");
    query.bindValue(":id", userId);
    query.bindValue(":username", username);
    query.bindValue(":password", hashedPassword);

    if (!query.exec()) {
        qWarning() << "用户注册失败：" << query.lastError().text();
        return false;
    }
    return true;
}

QString DataBase::loginUser(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT password,id FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec() || !query.next()) {
        qWarning() << "用户不存在：" << username;
        return "";
    }

    QString storedPassword = query.value(0).toString();
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    QString hashedPassword = hash.toHex();

    return storedPassword == hashedPassword?query.value(1).toString():NULL;
}

bool DataBase::addFriends(const QString &userId, const QString &friendId)
{
    QSqlQuery query;
    query.prepare("INSERT INTO friends (userId, friendId) VALUES (:userId, :friendId)");
    query.bindValue(":userId", userId);
    query.bindValue(":friendId", friendId);

    if (!query.exec()) {
        qWarning() << "好友添加失败：" << query.lastError().text();
        return false;
    }
    return true;
}

QSet<QString> DataBase::selectFriends(const QString &userId)
{
    QSet<QString> friendsId;
    QSqlQuery query;
    query.prepare("SELECT friendId From friends WHERE userId = :userId");
    query.bindValue(":userId", userId);
    if (query.exec()) {
        while (query.next()) {  // 遍历查询结果
            QString friendId = query.value(0).toString();
            friendsId.insert(friendId);
        }
    } else {
        qDebug() << "好友查询失败:" << query.lastError().text();
    }
    return friendsId;
}

QMap<QString, QString> DataBase::selectFriendsId_name(const QString &userId)
{
    QMap<QString, QString> userId_name;
    QSqlQuery query;
    query.prepare("SELECT users.id, users.username "
                  "FROM users "
                  "JOIN friends ON users.id = friends.friendId "
                  "WHERE friends.userId = :userId");
    query.bindValue(":userId", userId);
    if (query.exec()) {
        while (query.next()) {  // 遍历查询结果
            userId_name[query.value(0).toString()]=query.value(1).toString();
        }
    } else {
        qDebug() << "好友查询失败:" << query.lastError().text();
    }
    return userId_name;
}

QMap<QString, QString> DataBase::selectUser_byName(const QString &name)
{
    QMap<QString, QString> userId_name;
    QSqlQuery query;
    query.prepare("SELECT id, username "
                  "FROM users "
                  "WHERE username like :name");
    query.bindValue(":name", "%"+name+"%");
    if (query.exec()) {
        while (query.next()) {  // 遍历查询结果
            userId_name[query.value(0).toString()]=query.value(1).toString();
        }
    } else {
        qDebug() << "查询用户失败:" << query.lastError().text();
    }
    return userId_name;
}

bool DataBase::updateAvatar(const QString &userId, const QByteArray &imageData)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET avatar = ? WHERE id = ?");
    query.addBindValue(imageData);
    query.addBindValue(userId);

    if (!query.exec()) {
        qDebug() << "Failed to insert image:" << query.lastError().text();
        return false;
    }
    return true;
}

QByteArray DataBase::getAvatar(const QString &userId)
{
    QSqlQuery query;
    query.prepare("SELECT avatar FROM users WHERE id = ?");
    query.addBindValue(userId);

    if (query.exec() && query.next()) {
        QByteArray imageData = query.value(0).toByteArray();
        return imageData;
    }
    return QByteArray();
}

QMap<QString, QByteArray> DataBase::getFriendsAvatar(const QString &userId)
{
    QMap<QString, QByteArray> userId_avatar;
    QSqlQuery query;
    query.prepare("SELECT users.id, users.avatar "
                  "FROM users "
                  "JOIN friends ON users.id = friends.friendId "
                  "WHERE friends.userId = :userId");
    query.bindValue(":userId", userId);
    if (query.exec()) {
        while (query.next()) {  // 遍历查询结果
            userId_avatar[query.value(0).toString()]=query.value(1).toByteArray();
        }
    } else {
        qDebug() << "好友头像查询失败:" << query.lastError().text();
    }
    return userId_avatar;
}

