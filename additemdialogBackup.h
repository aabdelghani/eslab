// additemdialog.h
#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
class AddItemDialog;
}

class AddItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddItemDialog(QWidget *parent = nullptr);
    ~AddItemDialog();

private slots:
    void onConfirm(); // Rename the slot
    void onCancel(); // Rename the slot


private:
    Ui::AddItemDialog *ui;
    QPushButton *confirmButton;
    QPushButton *cancelButton;
};

#endif // ADDITEMDIALOG_H
