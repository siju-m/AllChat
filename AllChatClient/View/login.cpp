#include "login.h"
#include "ui_login.h"
#include <QMouseEvent>
#include <windows.h>

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    setWindowTitle("AllChat");
    btnRegist = ui->btnRegist;
    connect(ui->btnClose,&QPushButton::clicked,this,&QDialog::close);
    this->setWindowFlags(Qt::CustomizeWindowHint);

}

Login::~Login()
{
    delete ui;
}

void Login::on_btnLogin_clicked(){
    if(ui->lineEditUserName->text().isEmpty() || ui->lineEditPassword->text().isEmpty()){
        QMessageBox::warning(this, "警告", "用户名或密码为空!");
        return;
    }

    emit login(ui->lineEditUserName->text(),ui->lineEditPassword->text());

}

void Login::closeWindow(CommonEnum::message_type result){
    switch(result){
    case CommonEnum::message_type::LOGIN_SUCCESS:{
        this->accept();
    }break;
    case CommonEnum::message_type::LOGIN_FAILED:{
        ui->lineEditUserName->clear();
        ui->lineEditPassword->clear();
        QMessageBox::warning(this, "警告", "密码错误或用户不存在!");
    }break;
    default:break;
    }

}

// 鼠标按下事件，记录鼠标和窗口当前位置的偏移量
void Login::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_offset = event->globalPosition() - QPointF(this->pos());
        event->accept();
    }
}

// 鼠标移动事件，根据偏移量更新窗口位置
void Login::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        QPointF newPosF = event->globalPosition() - m_offset;
        this->move(newPosF.toPoint());
        event->accept();
    }
}

