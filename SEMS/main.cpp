#include "mainwindow.h"
#include "loginwindow.h"
#include "database.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Database db;
    if (!db.init()) {
        return -1;
    }

    LoginWindow login;
    if (login.exec() != QDialog::Accepted) {
        return 0; // exit if login fails or is canceled
    }

    MainWindow w;
    w.show();

    return a.exec();
}
