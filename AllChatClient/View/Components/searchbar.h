#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class SearchBar : public QWidget
{
    Q_OBJECT
public:
    explicit SearchBar(QWidget *parent = nullptr);

signals:
    void createGroup();
    void addFriend();
    void search(const QString &name);
    void hasClear();

private:
    void initBar();
    void initMenu();
    void onSearch();

private:
    QLineEdit *m_inputBox;
    QPushButton *m_moreButton;
};

#endif // SEARCHBAR_H
