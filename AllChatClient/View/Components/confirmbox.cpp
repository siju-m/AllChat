#include "confirmbox.h"

#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

ConfirmBox::ConfirmBox(QWidget *parent)
    : QDialog{parent}
{
    this->setFixedSize(300, 200);
    window()->setWindowFlags((window()->windowFlags()) | Qt::WindowMinimizeButtonHint | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);     //窗口透明

    QWidget *textWidget = new QWidget(this);
    textWidget->setStyleSheet("QWidget{"
                              "border-top-right-radius:10px;"
                              "border-top-left-radius:10px;"
                              "background-color: white;}");
    QLabel *title = new QLabel("删除", this);
    title->setFont(QFont("微软雅黑", 28, QFont::DemiBold));
    QLabel *subTitle = new QLabel("确认删除", this);
    subTitle->setFont(QFont("微软雅黑", 13));
    QVBoxLayout *layout1 = new QVBoxLayout(textWidget);
    layout1->setContentsMargins(15, 15, 5, 15);
    layout1->addWidget(title);
    layout1->addWidget(subTitle);

    QWidget *btnWidget = new QWidget(this);
    QPushButton *confirm = new QPushButton("删除", this);
    QPushButton *cancel = new QPushButton("取消", this);
    QString style("QPushButton{"
                        "font: 10pt '微软雅黑';"
                        "color: white;"
                        "background-color: rgb(53, 93, 144);"
                        "color: rgb(255, 255, 255);"
                        "border-radius: 8px;"
                        "border-style: none;"
                        "border-width: 1px;"
                        "padding: 5px 10px 5px 10px;"
                    "}"

                    /**鼠标停留在按钮上的样式**/
                    "QPushButton::hover{	"
                        "color: #FFFFFF;"
                        "background-color: rgb(93, 125, 166);"
                        "border-color: #2f3640;"
                    "}"

                    /**鼠标按压下去的样式**/
                    "QPushButton::pressed,QPushButton::checked{"
                        "color: #FFFFFF;"
                        "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #273c75, stop:1 #487eb0);"
                    "}"

                    /**按钮失能情况下样式**/
                    "QPushButton::disabled{"
                        "color: #FFFFFF;"
                        "background-color: #dcdde1;"
                        "border-color: #dcdde1;"
                    "}");
    confirm->setStyleSheet(style);
    cancel->setStyleSheet(style);
    confirm->setFixedSize(80, 40);
    cancel->setFixedSize(80, 40);
    QHBoxLayout *layout2 = new QHBoxLayout(btnWidget);
    layout2->addWidget(confirm);
    layout2->addSpacing(10);
    layout2->addWidget(cancel);



    QFrame *frame = new QFrame(this);
    QVBoxLayout *layout3 = new QVBoxLayout(this);
    layout3->setContentsMargins(20, 20, 20, 20);
    //设置投影效果
    QGraphicsDropShadowEffect *windowShadow;        //阴影效果
    windowShadow = new QGraphicsDropShadowEffect(this);
    windowShadow->setBlurRadius(30);
    windowShadow->setColor(QColor(0, 0, 0));
    windowShadow->setOffset(0, 0);
    frame->setGraphicsEffect(windowShadow);
    frame->setStyleSheet("QFrame{ border-radius:10px; background-color: rgb(241, 241, 241);}");
    layout3->addWidget(frame);

    QVBoxLayout *layout4 = new QVBoxLayout(frame);
    layout4->setContentsMargins(0,0,0,0);
    layout4->addWidget(textWidget);
    layout4->addWidget(btnWidget);
    btnWidget->setStyleSheet("QWidget{"
                             "border-bottom-right-radius:10px;"
                             "border-bottom-left-radius:10px;}");

    // 设置窗体关闭时自动释放内存
    this->setAttribute(Qt::WA_DeleteOnClose);

    connect(confirm, &QPushButton::clicked, this, &ConfirmBox::accept);
    connect(cancel, &QPushButton::clicked, this, &ConfirmBox::reject);
}

bool ConfirmBox::question(QWidget *parent)
{
    ConfirmBox *box = new ConfirmBox(parent);
    return box->exec();
}
