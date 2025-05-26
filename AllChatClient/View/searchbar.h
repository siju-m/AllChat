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
private:
    void initBar();
    void initMenu();

private:
    QLineEdit* m_inputBox;
    QPushButton* m_moreButton;
};

#endif // SEARCHBAR_H
