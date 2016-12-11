#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

signals:
    void login_succeed();
    void login_succeed(std::string name);

private slots:
    void on_ButtonLogin_clicked();

    void on_ButtonRegister_clicked();

private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
