#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    setWindowTitle("AllChat");
    btnRegist = ui->btnRegist;
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
