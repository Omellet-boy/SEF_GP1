#include "loginwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Login");
    setFixedSize(750, 500);

    // Set background color
    setStyleSheet("background-color: #964c40;");

    // Optional: Add logo
    QLabel *logoLabel = new QLabel();
    logoLabel->setPixmap(QPixmap(":/images/logo.png").scaled(64, 64, Qt::KeepAspectRatio));
    logoLabel->setAlignment(Qt::AlignCenter);

    QLabel *title = new QLabel("Welcome to Energy Simulator");
    title->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
    title->setAlignment(Qt::AlignCenter);

    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("Username");

    passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login");
    loginButton->setStyleSheet("padding: 8px; font-weight: bold; background-color: #2c89f4; color: white; border-radius: 5px;");

    statusLabel = new QLabel();
    statusLabel->setStyleSheet("color: red;");
    statusLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(logoLabel);       // <-- Add logo
    layout->addWidget(title);
    layout->addSpacing(10);
    layout->addWidget(usernameEdit);
    layout->addWidget(passwordEdit);
    layout->addWidget(loginButton);
    layout->addSpacing(5);
    layout->addWidget(statusLabel);
    layout->setContentsMargins(40, 20, 40, 20);

    setLayout(layout);

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);
}

QString LoginWindow::getUsername() const {
    return usernameEdit->text();
}

void LoginWindow::handleLogin() {
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        statusLabel->setText("Please enter both username and password.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM Users WHERE username = ? AND password = ?");
    query.addBindValue(username);
    query.addBindValue(password);

    if (query.exec() && query.next()) {
        accept(); // closes the login dialog successfully
        emit loginSuccessful();
    } else {
        statusLabel->setText("Invalid username or password.");
    }
}

