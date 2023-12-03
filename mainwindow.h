#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTableView>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSearchTextChanged(const QString &text); // Slot for search text changes

private:
    Ui::MainWindow *ui;
    QLineEdit *searchLineEdit; // The search box
    QTableView *tableView; // Add this if you want to show results in a table
    void loadAllData(); // Add this line to declare the function

};
#endif // MAINWINDOW_H
