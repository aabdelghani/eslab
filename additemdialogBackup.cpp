// additemdialog.cpp
#include "additemdialog.h"
#include <QSqlError>
#include "ui_additemdialog.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QPushButton>

AddItemDialog::AddItemDialog(QWidget *parent)
    : QDialog(parent)
{


    connect(ui.confirmButton, &QPushButton::clicked, this, &AddItemDialog::onConfirm);
    connect(ui.cancelButton, &QPushButton::clicked, this, &AddItemDialog::onCancel);
}

void AddItemDialog::onConfirm()
{
    QString itemName = ui.itemNameLineEdit->text();
    int itemQty = ui.itemQtySpinBox->value();
    QString itemDesc = ui.itemDescTextEdit->toPlainText();
    if (!itemName.isEmpty()) {
        QSqlQuery query;
        query.prepare("INSERT INTO component (component_name, component_qty, componenet_description) VALUES (:name, :qty, :desc)");
        query.bindValue(":name", itemName);
        query.bindValue(":qty", itemQty);
        query.bindValue(":desc", itemDesc);

        if (query.exec()) {
            // Item added successfully, emit the signal
            emit itemAdded();

            QMessageBox::information(this, "Success", "Item added successfully.");

            accept(); // Closes the dialog with QDialog::Accepted result
        } else {
            QMessageBox::critical(this, "Error", "Failed to add item: " + query.lastError().text());
        }
    } else {
        QMessageBox::warning(this, "Input Error", "Item name cannot be empty.");
    }
}

void AddItemDialog::onCancel()
{
    reject(); // Closes the dialog with QDialog::Rejected result
}

AddItemDialog::~AddItemDialog()
{
}
