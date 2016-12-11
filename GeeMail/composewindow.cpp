#include "composewindow.h"
#include "ui_composewindow.h"
#include "gee_main.h"

ComposeWindow::ComposeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ComposeWindow)
{
    ui->setupUi(this);
}

ComposeWindow::~ComposeWindow()
{
    delete ui;
}

void ComposeWindow::on_ButtonCancel_clicked()
{
    this->close();
}

void ComposeWindow::on_ButtonSend_clicked()
{
    QString Recipient = ui->TextRecipient->text();
    QString Title = ui->TextTitle->text();
    QString BodyMessage = ui->TextBody->toPlainText();

    Gee_Main Mail;
    Mail.SendMail(username, Recipient.toStdString(), Title.toStdString(), BodyMessage.toStdString());

    this->close();
}
