#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>

class SettingsWindow : public QWidget {
    Q_OBJECT
public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    bool isEmailEnabled() const;

private slots:
    void applySettings();
    void changeCredentials();

private:
    QCheckBox *emailNotificationCheckbox;
    QLineEdit *emailEdit;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *confirmPasswordEdit;
};

#endif // SETTINGSWINDOW_H
