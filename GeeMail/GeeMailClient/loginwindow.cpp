#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "gee_login.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_ButtonLogin_clicked()
{
    std::string username = ui->LineUsername->text().toStdString();
    std::string password = ui->LinePassword->text().toStdString();

    Gee_Login account;
    LoginMsg account_login = account.login(username, password);
    QMessageBox LoginErrMsg;
    LoginErrMsg.setWindowTitle("Login Error");

    switch (account_login)
    {
        case LoginMsg::USERNAME_REQUIRED:
            LoginErrMsg.setText("Username field required");
            LoginErrMsg.exec();
            break;
        case LoginMsg::PASSWORD_REQUIRED:
            LoginErrMsg.setText("Password field required");
            LoginErrMsg.exec();
            break;
        case LoginMsg::LOGIN_FAILED:
            LoginErrMsg.setText("Username and Password does not match!");
            LoginErrMsg.exec();
            break;
        case LoginMsg::LOGIN_SUCCEED:
            emit login_succeed();
            emit login_succeed(username);
    }
}

void LoginWindow::on_ButtonRegister_clicked()
{
    std::string username_reg = ui->LineUsernameReg->text().toStdString();
    std::string password_reg = ui->LinePasswordReg->text().toStdString();

    Gee_Login account;
    RegisterMsg account_register = account.reg(username_reg, password_reg);
    QMessageBox RegisterErrMsg;
    RegisterErrMsg.setWindowTitle("Register Error");

    switch (account_register)
    {
        case RegisterMsg::USERNAME_REQUIRED:
            RegisterErrMsg.setText("Username field required");
            RegisterErrMsg.exec();
            break;
        case RegisterMsg::PASSWORD_REQUIRED:
            RegisterErrMsg.setText("Password field required");
            RegisterErrMsg.exec();
            break;
        case RegisterMsg::REGISTER_NAME_EXIST:
            RegisterErrMsg.setText("Username already exist");
            RegisterErrMsg.exec();
            break;
        case RegisterMsg::REGISTER_SUCCEED:
            RegisterErrMsg.setWindowTitle("Register Ok");
            RegisterErrMsg.setText("Username register succeed");
            RegisterErrMsg.exec();
            ui->LineUsernameReg->clear();
            ui->LinePasswordReg->clear();
            break;
    }
}
