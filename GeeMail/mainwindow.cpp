#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gee_main.h"
#include "composewindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_ButtonGetMail_clicked()
{
    Gee_Main Mail;
    std::vector <std::string> MailList;
    MailList = Mail.GetMail(username);

    ui->ListMailItem->clear();
    ui->TextMailHeader->clear();
    ui->textMailBody->clear();

    for (auto MailItem : MailList)
    {
        std::vector <std::string> Item;
        Item = Mail.split(MailItem, "\a");
        QListWidgetItem * ListItem = new QListWidgetItem;
        ListItem->setText(QString::fromStdString("From: " + Item[3] + "\n" + Item[4]));
        // set Item Data
        QVariant VectorItem;
        VectorItem.setValue(QString::fromStdString(MailItem));
        ListItem->setData(Qt::UserRole, VectorItem);

        ui->ListMailItem->addItem(ListItem);

        //ui->TextMailHeader->setText(QString::fromStdString("From: " + Item[3]));
    }
    QObject::connect(ui->ListMailItem, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(on_MailItem_clicked(QListWidgetItem*)));
}

void MainWindow::on_MailItem_clicked(QListWidgetItem * ListItem)
{
    Gee_Main Mail;
    QVariant data = ListItem->data(Qt::UserRole);
    std::string strItem = data.value<QString>().toStdString();
    std::vector <std::string> Item = Mail.split(strItem, "\a");
    ui->TextMailHeader->setText(QString::fromStdString("From: " + Item[3] + "\nTo: " + Item[2] + "\n" + Item[4]));
    ui->textMailBody->setText(QString::fromStdString("\n\n" + Item[5]));
}

void MainWindow::on_ButtonSendMail_clicked()
{
    ComposeWindow * WindowCompose;
    WindowCompose = new ComposeWindow;

    WindowCompose->set_username(username);
    WindowCompose->show();
}
