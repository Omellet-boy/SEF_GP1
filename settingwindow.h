#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QSettings>

class SettingsWindow : public QWidget {
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    bool isEmailEnabled() const;

signals:
    void backToDashboard();

private:
    QCheckBox *emailNotificationCheckbox;
};

#endif // SETTINGSWINDOW_H
