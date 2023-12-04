#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QScreen>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    searchLineEdit(new QLineEdit(this)),
    tableView(new QTableView(this))
{
    ui->setupUi(this);
    initializeDatabase();
    setupUIComponents();
    connectSignalsAndSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeDatabase() {
    QString dbFilePath = "C:/Users/Ahmed/OneDrive/Documents/eslabProject/eslab/eslab.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilePath);

    if (!db.open()) {
        qDebug() << "Error opening database:" << db.lastError().text();
        // Consider more robust error handling here
    } else {
        loadAllData(); // Load data only after ensuring the database is open
    }
}

void MainWindow::setupUIComponents() {
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int width = screenGeometry.width() * 0.75;
    int height = screenGeometry.height() * 0.75;

    // Set up searchLineEdit
    searchLineEdit->setPlaceholderText("Enter search text");
    searchLineEdit->resize(width / 2, 20);
    searchLineEdit->move(width / 2 + 10, 10);

    // Set up tableView and its scroll area
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(tableView);
    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableView->resize(width, height - 50);
    tableView->move(width / 2 + 10, 40);
    scrollArea->resize(width, height - 50);
    scrollArea->move(width / 2 + 10, 40);

    // Configure the main window
    setFixedSize(width, height);
}

void MainWindow::connectSignalsAndSlots() {
    connect(searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    // Connect other signals and slots here
}

void MainWindow::loadAllData() {
    QSqlQuery query("SELECT locker_number, component_img, component_qty, component_name, componenet_description, borrowed_status, borrower_name, borrower_id, borrowing_date, return_date, Status, Notes FROM component");
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(std::move(query));
    tableView->setModel(model);
    tableView->resizeColumnsToContents();
}

void MainWindow::onSearchTextChanged(const QString &text) {
    if (text.trimmed().isEmpty()) {
        loadAllData();
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM component WHERE component_name LIKE :name");
    query.bindValue(":name", "%" + text + "%");
    if (query.exec()) {
        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery(std::move(query));
        tableView->setModel(model);
    } else {
        qDebug() << "Search query error:" << query.lastError().text();
    }
}

// Add the implementation for onAddItemButtonClicked if you have this functionality
