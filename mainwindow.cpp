#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QScreen>
#include <QScrollArea>
#include <QGridLayout>
#include <QPushButton>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    searchLineEdit(new QLineEdit(this)),
    tableView(new QTableView(this))
{
    ui->setupUi(this);
    setupGridLayout();
    loadAllData();
    initializeDatabase();
    setupUIComponents();
    connectSignalsAndSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeDatabase() {
    //QString dbFilePath = "C:/Users/ahmedabdel-ghany/Documents/EmbeddedLabProject/eslab.db"
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

void MainWindow::setupGridLayout() {
    QWidget *centralWidget = new QWidget(this);
    QGridLayout *gridLayout = new QGridLayout(centralWidget);

    // Set column stretch factors based on the desired percentage widths
    gridLayout->setColumnStretch(0, 1);  // 1% for first column
    gridLayout->setColumnStretch(1, 22); // 20% for second column
    gridLayout->setColumnStretch(2, 1);  // 1% for third column
    gridLayout->setColumnStretch(3, 75); // 70% for fourth column
    gridLayout->setColumnStretch(4, 1);  // 1% for fifth column

    // Set row stretch factors
    gridLayout->setRowStretch(0, 1);     // 2% for first row
    for (int row = 1; row < 9; ++row) {
        gridLayout->setRowStretch(row, 10); // Evenly distribute middle rows
    }
    gridLayout->setRowStretch(9, 1);     // 1% for last row

    // Initialize tableView and add it to the grid layout
    tableView = new QTableView(centralWidget);
    gridLayout->addWidget(tableView, 2, 3, 7, 1); // Span from row 3 to row 9 (7 rows) in the third column
    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);


    // Initialize searchLineEdit and add it to the grid layout
    searchLineEdit = new QLineEdit(centralWidget);
    searchLineEdit->setPlaceholderText("Enter search text");
    gridLayout->addWidget(searchLineEdit, 1, 3, 1, 1); // Placed in row 2, column 3, spanning 1 row and 1 column
    setCentralWidget(centralWidget);
}
