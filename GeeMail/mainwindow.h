#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_ButtonGetMail_clicked();
    void on_MailItem_clicked(QListWidgetItem *ListItem);

    void on_ButtonSendMail_clicked();
    void set_username(std::string name) { username = name; }

private:
    Ui::MainWindow *ui;
    std::string username;
};

#endif // MAINWINDOW_H
