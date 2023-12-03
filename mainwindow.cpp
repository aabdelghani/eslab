#include "mainwindow.h"
#include "ui_mainwindow.h"
/*added from chatgpt*/
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <utility>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , searchLineEdit(new QLineEdit(this)) // Initialize the search box
    , tableView(new QTableView(this)) // Initialize the table view
{
    ui->setupUi(this);

    // Set up the search box
    searchLineEdit->setPlaceholderText("Enter search text"); // Optional placeholder text
    searchLineEdit->move(10, 10); // Position the search box, adjust as needed
    searchLineEdit->resize(200, 20); // Set the size of the search box, adjust as needed

    // Connect the textChanged signal of the search box to the onSearchTextChanged slot
    connect(searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);

    // Set up the database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("eslab.db"); // Path to your database file
    if (!db.open()) {
        qDebug() << "Error opening database:" << db.lastError().text();
    }
    // Set up the table view
    tableView->move(10, 40);
    tableView->resize(400, 300);
    loadAllData(); // Load all data at startup

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadAllData() {
    QSqlQuery query("SELECT * FROM component");
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(std::move(query));
    tableView->setModel(model);
}


void MainWindow::onSearchTextChanged(const QString &text)
{
    if (text.trimmed().isEmpty()) {
        loadAllData(); // Load all data if search text is empty
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM component WHERE component_name LIKE :name");
    query.bindValue(":name", "%" + text + "%");

    if(query.exec()) {
        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery(std::move(query));
        tableView->setModel(model);
    } else {
        qDebug() << "Search query error:" << query.lastError().text();
    }
}

