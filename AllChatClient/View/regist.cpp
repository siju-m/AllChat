#include "regist.h"
#include "ui_regist.h"

Regist::Regist(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Regist)
{
    ui->setupUi(this);
}

Regist::~Regist()
{
    delete ui;
}

void Regist::on_btnRegist_clicked(){
    if(ui->userName->text().isEmpty() || ui->password->text().isEmpty() || ui->confirmPassword->text().isEmpty()){
        QMessageBox::warning(this, "警告", "用户名或密码为空!");
        return;
    }
    if(ui->password->text()!=ui->confirmPassword->text()){
        QMessageBox::warning(this, "警告", "前后密码不一致!");
        return;
    }
    emit regist(ui->userName->text(),ui->password->text());
}

void Regist::closeWindow(CommonEnum::message_type result){
    switch(result){
    case CommonEnum::message_type::REGISTER_SUCCESS:{

        QMessageBox::information(this, "成功", "注册成功!");
        this->accept();
    }break;
    case CommonEnum::message_type::REGISTER_FAILED:{
        ui->userName->clear();
        ui->password->clear();
        ui->confirmPassword->clear();
        QMessageBox::warning(this, "警告", "用户名已存在!");
    }
    default:break;
    }

}

void Regist::closeEvent(QCloseEvent *event)
{
    ui->userName->clear();
    ui->password->clear();
    ui->confirmPassword->clear();
    QDialog::closeEvent(event);
}
