#ifndef COMPOSEWINDOW_H
#define COMPOSEWINDOW_H

#include <QMainWindow>

namespace Ui {
class ComposeWindow;
}

class ComposeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ComposeWindow(QWidget *parent = 0);
    ~ComposeWindow();

    void set_username(std::string name) { username = name; }

private slots:
    void on_ButtonCancel_clicked();

    void on_ButtonSend_clicked();

private:
    Ui::ComposeWindow *ui;
    std::string username;
};

#endif // COMPOSEWINDOW_H
