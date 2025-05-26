#include "searchbar.h"

#include <QHBoxLayout>
#include <QMenu>

SearchBar::SearchBar(QWidget *parent)
    : QWidget{parent}
{
    initBar();

    initMenu();
}

void SearchBar::initBar()
{
    m_inputBox = new QLineEdit(this);
    m_inputBox->setFixedHeight(30);
    m_inputBox->setStyleSheet(R"(
    QLineEdit {
        border: none;
        border-radius: 10px;
        padding: 6px 10px;
        margin:1px;
        font-size: 14px;
        background-color: #e3e3e3;
    }
    QLineEdit:focus {
        border: 1px solid #0078d7;
        background-color: #ffffff;
    })");

    m_moreButton = new QPushButton(this);
    m_moreButton->setFixedSize(30, 30);
    m_moreButton->setIcon(QIcon(":/Icon/plus.png"));
    m_moreButton->setStyleSheet(R"(
    QPushButton{
        border: none ;
        border-radius:5px;
        background-color:#e3e3e3;
    }

    /**鼠标停留在按钮上的样式**/
    QPushButton::hover{
        background-color: #dedede;
    }

    /**鼠标按压下去的样式**/
    QPushButton::pressed,QPushButton::checked{
        background-color: #bcbcbc;
    })");

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(m_inputBox);
    layout->addWidget(m_moreButton);

    layout->setContentsMargins(12,6,10,8);
    this->setLayout(layout);
}

void SearchBar::initMenu()
{
    QMenu *menu = new QMenu();
    QAction *createGroupAction = menu->addAction("创建群聊");
    QAction *addFriendAction = menu->addAction("添加好友");

    connect(menu, &QMenu::triggered, this, [=](QAction *action){
        if (action == createGroupAction) {
            emit createGroup();
        } else if (action == addFriendAction) {
            emit addFriend();
        }
    });


    menu->setWindowFlags(menu->windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    menu->setAttribute(Qt::WA_TranslucentBackground);

    menu->setStyleSheet(R"(
    QMenu {
        background-color: rgba(255, 255, 255, 230); /* 半透明白 */
        border: none;
        border-radius: 8px; /* 圆角 */
        padding: 4px;
        font: 10pt "Segoe UI";
    }

    QMenu::item {
        background-color: transparent;
        padding: 8px 20px;
        border-radius: 4px; /* 菜单项也圆角 */
        color: #333333;
        font-weight: bold;
    }

    QMenu::item:selected {
        background-color: #e3e3e3; /* 灰色高亮 */
    }

    QMenu::separator {
        height: 1px;
        background: #e0e0e0;
        margin: 6px 12px;
    }
    )");


    QObject::connect(m_moreButton, &QPushButton::clicked, this, [=]() {
        // 在按钮左下角弹出菜单
        QPoint pos = m_moreButton->mapToGlobal(QPoint(0, m_moreButton->height()));
        menu->exec(pos);
    });
}
