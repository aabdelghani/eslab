#include "mainwindow.h"
#include "ui_mainwindow.h"
/*added from chatgpt*/
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <utility>
#include <QScreen> // Include the QScreen header
#include <QScrollArea>
// Define a QString variable to store the absolute path to the database

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , searchLineEdit(new QLineEdit(this)) // Initialize the search box
    , tableView(new QTableView(this)) // Initialize the table view
{
    ui->setupUi(this);

    // Set up the search box
    searchLineEdit->setPlaceholderText("Enter search text"); // Optional placeholder text

    // Get the screen's geometry
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();


    // Calculate the width and height as 75% of the screen's width and height
    int width = screenGeometry.width() * 0.75;
    int height = screenGeometry.height() * 0.75;


    // Set the width and position of the searchLineEdit
    searchLineEdit->resize(width / 2, 20); // Set the width to half of the calculated width
    searchLineEdit->move(width / 2 + 10, 10); // Position it on the right side
    // Create a QScrollArea to wrap around the tableView
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true); // Make the widget inside the scroll area resizable


    // Connect the textChanged signal of the search box to the onSearchTextChanged slot
    connect(searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);

    QString dbFilePath = "C:/Users/Ahmed/OneDrive/Documents/eslabProject/eslab/eslab.db"; // Replace with your absolute path
    // Set up the database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    // Specify the absolute path to the database file
    db.setDatabaseName(dbFilePath);
    // Add a test query to check the database connection
    QSqlQuery testQuery("SELECT 1", db);
    if (!db.open()) {
        qDebug() << "Error opening database:" << db.lastError().text();
    }
    // Set the fixed size of the main window
    setFixedSize(width, height); // Set the width and height to 75% of the screen's dimensions

    // Set the width and height of the tableView
    tableView->resize(width, height - 50); // Set the width to the calculated width and subtract 50 for some padding
    tableView->move(width / 2 + 10, 40); // Position it on the right side
    // Set the tableView as the widget inside the scroll area
    scrollArea->setWidget(tableView);

    // Position the scroll area on the right side
    scrollArea->resize(width, height - 50); // Set the width to the calculated width and subtract 50 for some padding
    scrollArea->move(width / 2 + 10, 40);

    // Make the tableView scrollable
    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    loadAllData(); // Load all data at startup

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::loadAllData() {
    // Specify the columns you want to select, excluding the "id" column
    QSqlQuery query("SELECT locker_number, component_img, component_qty, component_name, componenet_description, borrowed_status, borrower_name, borrower_id, borrowing_date, return_date, Status, Notes FROM component");

    // Create a QSqlQueryModel object to store the query results
    QSqlQueryModel *model = new QSqlQueryModel;

    // Populate the QSqlQueryModel with the results of the executed query
    model->setQuery(std::move(query));

    // Set the QTableView widget to use the QSqlQueryModel as its data source
    tableView->setModel(model);

    // Adjust column widths to fit content
    tableView->resizeColumnsToContents();
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

