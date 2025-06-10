// settingswindow.cpp
#include "settingwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

SettingsWindow::SettingsWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Settings");
    setFixedSize(800, 600);

    QLabel *title = new QLabel("Settings");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: bold;");

    emailNotificationCheckbox = new QCheckBox("Enable Overheating Email Notifications");
    QSettings settings("MyCompany", "SEMS");
    emailNotificationCheckbox->setChecked(settings.value("emailNotifications", false).toBool());

    QPushButton *saveButton = new QPushButton("Apply");

    connect(saveButton, &QPushButton::clicked, [=]() {
        QSettings settings("MyCompany", "SEMS");
        settings.setValue("emailNotifications", emailNotificationCheckbox->isChecked());
    });

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(title);
    layout->addSpacing(30);
    layout->addWidget(emailNotificationCheckbox);
    layout->addSpacing(50);
    layout->addWidget(saveButton);

    setLayout(layout);
}

bool SettingsWindow::isEmailEnabled() const {
    return emailNotificationCheckbox->isChecked();
}
