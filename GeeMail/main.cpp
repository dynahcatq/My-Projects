#include "loginwindow.h"
#include "mainwindow.h"
#include "composewindow.h"
#include <QApplication>
#include <QPushButton>
#include <QStateMachine>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStateMachine GeeMailMachine;
    QState * StateLogin = new QState();
    QState * StateMain = new QState();

    LoginWindow WindowLogin;
    MainWindow WindowMain;

    QObject::connect(StateLogin, SIGNAL(entered()), &WindowLogin, SLOT(show()));
    QObject::connect(StateLogin, SIGNAL(exited()), &WindowLogin, SLOT(hide()));
    QObject::connect(StateMain, SIGNAL(entered()), &WindowMain, SLOT(show()));
    QObject::connect(StateMain, SIGNAL(exited()), &WindowMain, SLOT(hide()));

    QObject::connect(&WindowLogin, SIGNAL(login_succeed(std::string)), &WindowMain, SLOT(set_username(std::string)));

    StateLogin->addTransition(&WindowLogin, SIGNAL(login_succeed()), StateMain);

    GeeMailMachine.addState(StateLogin);
    GeeMailMachine.addState(StateMain);
    GeeMailMachine.setInitialState(StateLogin);
    GeeMailMachine.start();

    return a.exec();
}
