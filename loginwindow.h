#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>

class QLineEdit;
class QPushButton;
class QLabel;

class LoginWindow : public QDialog {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    QString getUsername() const;

signals:
    void loginSuccessful();

private slots:
    void handleLogin();

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QLabel *statusLabel;
};

#endif // LOGINWINDOW_H
