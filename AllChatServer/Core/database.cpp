#include "database.h"

#include "configmanager.h"

DataBase* DataBase::m_instance;
std::mutex DataBase::m_mutex;

DataBase::DataBase(QObject *parent):QObject(parent){
    initDatabase();
}

void DataBase::initDatabase() {
    // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    // db.setDatabaseName("users.db");

    // 添加 MySQL 数据库驱动
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    // 设置数据库连接参数
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("AllChatServer"); // 数据库名
    db.setUserName("root");          // 用户名
    db.setPassword("123456");          // 密码

    if (!db.open()) {
        qWarning() << "数据库打开失败：" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    // 创建表 users
    QString createTable = "CREATE TABLE IF NOT EXISTS users ("
                          "id VARCHAR(36) PRIMARY KEY, "
                          "username VARCHAR(50) UNIQUE, "
                          "password VARCHAR(100),"
                          "avatar LONGBLOB)";
    if (!query.exec(createTable)) {
        qWarning() << "创建用户表失败：" << query.lastError().text();
    }

    // 创建表 friends
    createTable = "CREATE TABLE IF NOT EXISTS friends ("
                  "userId VARCHAR(36),"
                  "friendId VARCHAR(36))";
    if (!query.exec(createTable)) {
        qWarning() << "创建联系人表失败：" << query.lastError().text();
    }

    // 创建表 groups
    createTable = "CREATE TABLE IF NOT EXISTS chat_groups ("
                  "group_id VARCHAR(36) PRIMARY KEY,"
                  "group_name VARCHAR(50) NOT NULL,"
                  "creator_id VARCHAR(36) NOT NULL,"
                  "created_time DATETIME DEFAULT CURRENT_TIMESTAMP,"
                  "FOREIGN KEY (creator_id) REFERENCES users(id));";
    if (!query.exec(createTable)) {
        qWarning() << "创建群聊表失败：" << query.lastError().text();
    }

    // 创建表 group_members
    createTable = "CREATE TABLE IF NOT EXISTS group_members ("
                  "group_id VARCHAR(36) NOT NULL,"
                  "user_id VARCHAR(36) NOT NULL,"
                  "PRIMARY KEY (group_id, user_id),"
                  "FOREIGN KEY (group_id) REFERENCES chat_groups(group_id),"
                  "FOREIGN KEY (user_id) REFERENCES users(id));";
    if (!query.exec(createTable)) {
        qWarning() << "创建群聊表失败：" << query.lastError().text();
    }
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

QString DataBase::selectNameById(const QString &userId)
{
    QSqlQuery query;
    query.prepare("SELECT username From users WHERE id = :userId");
    query.bindValue(":userId", userId);
    if (!query.exec() || !query.next()) {
        qWarning() << "好友查询失败" ;
        return "";
    }

    QString name = query.value(0).toString();
    return name;
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

QMap<QString, QByteArray> DataBase::selectAvatar_byName(const QString &name)
{
    QMap<QString, QByteArray> userId_name;
    QSqlQuery query;
    query.prepare("SELECT id, avatar "
                  "FROM users "
                  "WHERE username like :name");
    query.bindValue(":name", "%"+name+"%");
    if (query.exec()) {
        while (query.next()) {  // 遍历查询结果
            userId_name[query.value(0).toString()]=query.value(1).toByteArray();
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

bool DataBase::deleteFriend(const QString &userId, const QString &friendId)
{
    QSqlQuery query;
    query.prepare("Delete FROM friends "
                  "WHERE userId = :userId AND friendId = :friendId");
    query.bindValue(":userId", userId);
    query.bindValue(":friendId", friendId);
    if(!query.exec()){
        qDebug() << "好友删除失败:" << query.lastError().text();
        return false;
    }
    query.bindValue(":userId", friendId);
    query.bindValue(":friendId", userId);
    if(!query.exec()){
        qDebug() << "好友删除失败:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DataBase::createGroup(const QString &groupId, const QString &groupName, const QString &creatorId)
{
    QSqlQuery query;
    query.prepare("INSERT INTO chat_groups (group_id, group_name, creator_id, created_time) "
                  "VALUES (:group_id, :group_name, :creator_id, CURRENT_TIMESTAMP)");
    query.bindValue(":group_id", groupId);
    query.bindValue(":group_name", groupName);
    query.bindValue(":creator_id", creatorId);

    if(!query.exec()){
        qDebug() << "新建群聊失败:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DataBase::insertGroupMember(const QVector<QString> &ids, const QString &groupId)
{
    QSqlQuery query;
    query.prepare("INSERT INTO group_members (group_id, user_id) "
                  "VALUES (:group_id, :user_id)");

    for (const QString &userId : ids) {
        query.bindValue(":group_id", groupId);
        query.bindValue(":user_id", userId);

        if (!query.exec()) {
            qDebug() << "插入失败:" << query.lastError().text();
            QSqlDatabase::database().rollback();
        }
    }
    QSqlDatabase::database().commit();
    return true;
}

QByteArray DataBase::selectGroupsByUserId(const QString &id)
{
    struct Group
    {
        QString id;
        QString name;
        int count;
    };
    QVector<Group> groups;
    QSqlQuery query;
    query.prepare("SELECT g.group_id, g.group_name, COUNT(gm2.user_id) AS member_count "
                    "FROM group_members gm1 "
                    "JOIN chat_groups g ON gm1.group_id = g.group_id "
                    "JOIN group_members gm2 ON g.group_id = gm2.group_id "
                    "WHERE gm1.user_id = :userId "
                    "GROUP BY g.group_id, g.group_name;");
    query.bindValue(":userId", id);
    if (query.exec()) {
        while (query.next()) {  // 遍历查询结果
            groups << Group{query.value(0).toString(), query.value(1).toString(), query.value(2).toInt()};
        }
    } else {
        qDebug() << "用户群聊列表查询失败:" << query.lastError().text();
    }

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(ConfigManager::dataStreamVersion()); // 或你当前 Qt 版本
    out << groups.size();
    for(const auto &group : groups){
        out << group.id;
        out << group.name;
        out << group.count;
    }
    return data;
}

QVector<QString> DataBase::selectUsersByGroupId(const QString &groupId)
{
    QVector<QString> users;
    QSqlQuery query;
    query.prepare("SELECT u.* "
                  "FROM group_members gm "
                  "JOIN users u ON gm.user_id = u.id "
                  "WHERE gm.group_id = :groupId;");
    query.bindValue(":groupId", groupId);
    if (query.exec()) {
        while (query.next()) {  // 遍历查询结果
            users << query.value(0).toString();
        }
    } else {
        qDebug() << "群成员列表查询失败:" << query.lastError().text();
    }
    return users;
}

QByteArray DataBase::selectGroupStrangers(const QString &userId)
{
    struct User{
        QString id;
        QString name;
        QByteArray avatar;
    };
    QVector<User> users;

    QSqlQuery query;
    query.prepare("SELECT DISTINCT u.id, u.username, u.avatar "
                  "FROM group_members gm1 "
                   "JOIN group_members gm2 ON gm1.group_id = gm2.group_id "
                    "JOIN users u ON gm2.user_id = u.id "
                    "WHERE gm1.user_id = :myId "
                    "AND gm2.user_id != :myId "
                    "AND NOT EXISTS ( "
                            "SELECT 1 "
                            "FROM friends f "
                            "WHERE "
                            "(f.userId = :myId AND f.friendId = gm2.user_id) OR "
                            "(f.friendId = :myId AND f.userId = gm2.user_id));");
    query.bindValue(":myId", userId);
    if (query.exec()) {
        while (query.next()) {  // 遍历查询结果
            User user{query.value(0).toString(), query.value(1).toString(), query.value(2).toByteArray()};
            // qDebug() << query.value(0).toString();
            users << user;
        }
    } else {
        qDebug() << "群成员列表查询失败:" << query.lastError().text();
    }

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(ConfigManager::dataStreamVersion()); // 或你当前 Qt 版本
    out << users.size();
    for(const auto &user : users){
        out << user.id;
        out << user.name;
        out << user.avatar;
    }
    return data;
}

int DataBase::getGroupMemberCount(const QString &groupId)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM group_members WHERE group_id = :groupId");
    query.bindValue(":groupId", groupId);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    } else {
        qDebug() << "统计群成员数量失败:" << query.lastError().text();
        return -1;
    }
}

