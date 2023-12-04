#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTableView>
#include <QSqlQuery>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
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
    QLineEdit *searchLineEdit; // The search box for inputting queries
    QTableView *tableView; // Table view to display data
    QWidget *blueBackgroundWidget; //left hand panel
    void setupUIComponents();
    void connectSignalsAndSlots();
    void setupGridLayout();
    void setupDatabaseConnection(const QString&); // Initializes the database connection
    void loadAllData(); // Loads data into the table view
    // Add new method declarations
    void configureTableView();
    void setModelQuery(QSqlQuery&&);
};

#endif // MAINWINDOW_H
