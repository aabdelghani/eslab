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
#include <QIcon>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    searchLineEdit(new QLineEdit(this)),
    tableView(new QTableView(this))
{
    // Set window flags to remove the title bar
    setWindowFlags(Qt::FramelessWindowHint);

    ui->setupUi(this);
    // Set the title of the main window
    setWindowTitle("Embedded System Lab Management");

    // Set the window icon
    setWindowIcon(QIcon(":/path/to/your/icon.png"));

    setupGridLayout();
    //"C:/Users/ahmedabdel-ghany/Documents/EmbeddedLabProject/eslab.db"
    setupDatabaseConnection("C:/Users/Ahmed/OneDrive/Documents/eslabProject/eslab/eslab.db");
    setupUIComponents();
    connectSignalsAndSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupDatabaseConnection(const QString &dbFilePath) {
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
    QSqlQuery query("SELECT locker_number, component_img,component_name,  component_qty, componenet_description, borrowed_status, borrower_name, borrower_id, borrowing_date, return_date, Status, Notes FROM component");
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(std::move(query));
    // Hide grid lines in the table view
    tableView->setShowGrid(false);

    tableView->setModel(model);
    tableView->resizeColumnsToContents();
    // If you want to use stylesheets for more advanced styling
    tableView->setStyleSheet("QTableView {gridline-color: transparent;}");
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
    gridLayout->setColumnStretch(1, 22); // 22% for second column
    gridLayout->setColumnStretch(2, 1);  // 1% for third column
    gridLayout->setColumnStretch(3, 75); // 70% for fourth column
    gridLayout->setColumnStretch(4, 1);  // 1% for fifth column

    // Set row stretch factors
    gridLayout->setRowStretch(0, 1);     // 1% for first row
    for (int row = 1; row < 9; ++row) {
        gridLayout->setRowStretch(row, 10); // Evenly distribute middle rows
    }
    gridLayout->setRowStretch(9, 1);     // 1% for last row

    // Initialize tableView and add it to the grid layout
    tableView = new QTableView(centralWidget);
    gridLayout->addWidget(tableView, 3, 3, 6, 1); // Span from row 4 to row 9 (6 rows) in the third column
    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);


    // Eliminate padding between cells
    gridLayout->setSpacing(0); // This will set both horizontal and vertical spacing to zero
    gridLayout->setContentsMargins(0, 0, 0, 0); // This will set the margins to zero on all sides

    // Create one widget with a gradient background spanning from row 2 to row 9
    QWidget *blueBackgroundWidget = new QWidget;
    blueBackgroundWidget->setStyleSheet(
        "background: qlineargradient("
        "x1:0, y1:0, x2:0, y2:1, "
        "stop:0 rgba(45,64,134,255), "
        "stop:1 rgba(20,36,65,255));"
        "border-radius: 10px;" // Adjust the radius as needed

        );
    // Add it to the layout with a row span
    gridLayout->addWidget(blueBackgroundWidget, 1, 1, 8, 1); // Row, Column, RowSpan, ColumnSpan


    // Initialize searchLineEdit and add it to the grid layout
    searchLineEdit = new QLineEdit(centralWidget);
    searchLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    searchLineEdit->setPlaceholderText("Enter search text");
    // Style the searchLineEdit with rounded edges
    searchLineEdit->setStyleSheet(
        "QLineEdit {"
        "    border: 1px solid #a0a0a0;"  // Adjust color as needed
        "    border-radius: 10px;"         // Adjust the radius as needed
        "    padding: 0 8px;"              // Optional: Adjust text padding
        "    selection-background-color: darkgray;"  // Optional: Adjust selection color
        "}"
        );
    gridLayout->addWidget(searchLineEdit, 1, 3, 2, 1);// Row, Column, RowSpan, ColumnSpan


    setCentralWidget(centralWidget);
}
