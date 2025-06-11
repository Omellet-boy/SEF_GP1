#include "settingwindow.h"
#include "database.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QSettings>
#include <QSqlQuery>
#include <QSqlError>
#include <QFrame>

SettingsWindow::SettingsWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Settings");
    setStyleSheet(R"(
        QWidget {
            background-color: #2B2C2E;
            color: white;
            font-family: 'Kode Mono';
        }
        QLabel {
            font-weight: bold;
        }
        QLineEdit {
            padding: 8px;
            background-color: #3A3B3D;
            border: none;
            border-radius: 8px;
            color: white;
            font-weight: bold;
        }
        QPushButton {
            padding: 10px;
            background-color: #FED722;
            color: black;
            border: none;
            border-radius: 6px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #e5c21e;
        }
        QCheckBox {
            font-weight: bold;
        }
    )");

    // Title inside the form card
    QLabel *title = new QLabel("Settings");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 22px; margin-bottom: 15px;");

    emailNotificationCheckbox = new QCheckBox("Enable Overheating Email Notifications");
    QSettings settings("MyCompany", "SEMS");
    emailNotificationCheckbox->setChecked(settings.value("emailNotifications", false).toBool());

    QLabel *emailLabel = new QLabel("Notification Email:");
    emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText("example@email.com");

    QString currentUsername = settings.value("currentUsername", "admin").toString();
    QString currentEmail = Database().getEmail(currentUsername);
    emailEdit->setText(currentEmail);

    QLabel *userLabel = new QLabel("Change Username:");
    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("New Username");

    QLabel *passLabel = new QLabel("Change Password:");
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("New Password");

    confirmPasswordEdit = new QLineEdit();
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit->setPlaceholderText("Confirm New Password");

    QPushButton *changeButton = new QPushButton("Change Credentials");
    QPushButton *saveButton = new QPushButton("Apply Settings");

    connect(saveButton, &QPushButton::clicked, this, &SettingsWindow::applySettings);
    connect(changeButton, &QPushButton::clicked, this, &SettingsWindow::changeCredentials);

    // === Form Layout ===
    QVBoxLayout *formLayout = new QVBoxLayout;
    formLayout->addWidget(title);
    formLayout->addWidget(emailNotificationCheckbox);
    formLayout->addSpacing(10);
    formLayout->addWidget(emailLabel);
    formLayout->addWidget(emailEdit);
    formLayout->addSpacing(15);
    formLayout->addWidget(userLabel);
    formLayout->addWidget(usernameEdit);
    formLayout->addWidget(passLabel);
    formLayout->addWidget(passwordEdit);
    formLayout->addWidget(confirmPasswordEdit);
    formLayout->addSpacing(20);
    formLayout->addWidget(changeButton);
    formLayout->addWidget(saveButton);

    QFrame *formCard = new QFrame;
    formCard->setLayout(formLayout);
    formCard->setStyleSheet("QFrame { background-color: #1E1F21; border-radius: 12px; padding: 20px; }");
    formCard->setMaximumWidth(450);

    // === Main Wrapper Layout ===
    QVBoxLayout *wrapper = new QVBoxLayout;
    wrapper->addStretch();
    wrapper->addWidget(formCard, 0, Qt::AlignCenter);
    wrapper->addStretch();
    wrapper->setContentsMargins(40, 30, 40, 30);

    setLayout(wrapper);
}



bool SettingsWindow::isEmailEnabled() const {
    return emailNotificationCheckbox->isChecked();
}

void SettingsWindow::applySettings() {
    QSettings settings("MyCompany", "SEMS");
    settings.setValue("emailNotifications", emailNotificationCheckbox->isChecked());

    QString currentUsername = settings.value("currentUsername", "admin").toString();
    QString newEmail = emailEdit->text().trimmed();

    if (!newEmail.isEmpty()) {
        if (Database().updateEmail(currentUsername, newEmail)) {
            QMessageBox::information(this, "Success", "Email updated successfully.");
        } else {
            QMessageBox::warning(this, "Error", "Failed to update email.");
        }
    }

    QMessageBox::information(this, "Settings", "Settings applied successfully.");
}

void SettingsWindow::changeCredentials() {
    QString newUsername = usernameEdit->text().trimmed();
    QString newPassword = passwordEdit->text();
    QString confirm = confirmPasswordEdit->text();

    if (newUsername.isEmpty() || newPassword.isEmpty()) {
        QMessageBox::warning(this, "Error", "Username and password cannot be empty.");
        return;
    }
    if (newPassword != confirm) {
        QMessageBox::warning(this, "Error", "Passwords do not match.");
        return;
    }

    QSettings settings("MyCompany", "SEMS");
    QString currentUsername = settings.value("currentUsername", "admin").toString();

    QSqlQuery query;
    query.prepare("UPDATE Users SET username = ?, password = ? WHERE username = ?");
    query.addBindValue(newUsername);
    query.addBindValue(newPassword);
    query.addBindValue(currentUsername);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to update credentials: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "Success", "Credentials updated.");
        settings.setValue("currentUsername", newUsername);
    }
}
