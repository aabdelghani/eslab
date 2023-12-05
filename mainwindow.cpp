#include "mainwindow.h"
#include "CustomRowHeightDelegate.h"
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
#include <customsqlquerymodel.h>
#include <QHeaderView>
#include <QItemDelegate>
#include <QPainter>
#include <QKeyEvent>
#define BASE_COLOR_BLUE rgba(20,36,65,255)
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    searchLineEdit(new QLineEdit(this)),
    tableView(new QTableView(this)),
    blueBackgroundWidget(new QWidget(this))
{
    // Set window flags to remove the title bar
    setWindowFlags(Qt::FramelessWindowHint);

    ui->setupUi(this);
    // Set the title of the main window
    setWindowTitle("Embedded System Lab Management");
    /* Adjust the radius for rounded corners */
    setStyleSheet("QMainWindow { background-color: white; border-radius: 8px; border: 3px solid white; }");


    // Set the window icon using the given file path
    //setWindowIcon(QIcon("C:/Users/Ahmed/OneDrive/Documents/eslabProject/eslab/logo.ico"));
    setWindowIcon(QIcon("C:/Users/ahmedabdel-ghany/Documents/EmbeddedLabProject/logo.ico"));


    setupGridLayout();
    setupDatabaseConnection("C:/Users/ahmedabdel-ghany/Documents/EmbeddedLabProject/eslab.db");

    //setupDatabaseConnection("C:/Users/Ahmed/OneDrive/Documents/eslabProject/eslab/eslab.db");
    setupUIComponents();
    connectSignalsAndSlots();


}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W && event->modifiers() == Qt::ControlModifier) {
        // Check if Ctrl+W is pressed
        QApplication::quit(); // Quit the application
    } else {
        QMainWindow::keyPressEvent(event);
    }
}
// Custom delegate class to alternate row colors
class AlternateRowDelegate : public QItemDelegate {
public:
    AlternateRowDelegate(QObject* parent = nullptr) : QItemDelegate(parent) {}

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        if (index.row() % 2 == 0) {
            // Even rows: white background
            painter->fillRect(option.rect, Qt::white);
        } else {
            // Odd rows: super light grey background
            painter->fillRect(option.rect, QColor(245, 245, 245)); // Adjust the color as needed
        }

        QItemDelegate::paint(painter, option, index);
    }
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        QSize size = QItemDelegate::sizeHint(option, index);
        size.setHeight(60); // Set the desired row height here (e.g., 24 pixels)
        return size;
    }
};

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
    setModelQuery(std::move(query));
}

void MainWindow::onSearchTextChanged(const QString &text) {
    if (text.trimmed().isEmpty()) {
        loadAllData();
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM component WHERE component_name LIKE :name");
    query.bindValue(":name", "%" + text + "%");
    setModelQuery(std::move(query));
}

void MainWindow::setModelQuery(QSqlQuery &&query) {
    if (query.exec()) {
        CustomSqlQueryModel *model = new CustomSqlQueryModel;
        model->setQuery(std::move(query));
        tableView->setModel(model);
        configureTableView();
    } else {
        qDebug() << "Query execution error:" << query.lastError().text();
    }
}

void MainWindow::configureTableView() {
    // Set custom header names
    auto *model = dynamic_cast<CustomSqlQueryModel *>(tableView->model());
    if (model) { // Make sure the model is valid before setting header data
        // Set custom header names
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Locker"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Image"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Component"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Quantity"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Description"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Borrowed Status"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Borrower"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("Borrower ID"));
        model->setHeaderData(8, Qt::Horizontal, QObject::tr("Borrowing Date"));
        model->setHeaderData(9, Qt::Horizontal, QObject::tr("Return Date"));
        model->setHeaderData(10, Qt::Horizontal, QObject::tr("Status"));
        model->setHeaderData(11, Qt::Horizontal, QObject::tr("Notes"));

        // Resize each column to fit its content
        for (int i = 0; i < model->columnCount(); ++i) {
            tableView->resizeColumnToContents(i);
        }
        // Create and set the custom delegate with your desired row height (e.g., 34 pixels)

    }
    //int desiredRowHeight = 34;
   // tableView->setItemDelegate(new CustomRowHeightDelegate(desiredRowHeight));
    // Hide the vertical header (the row numbers)
    tableView->verticalHeader()->hide();
    // Set the row height to 24 pixels
    int desiredRowHeight = 24;
    tableView->verticalHeader()->setDefaultSectionSize(desiredRowHeight);
    // Make scrollbars invisible but functional by setting their style
    // Hide scrollbar arrows but keep the scrollbar itself visible
    // Customize the scrollbar to hide arrows and remove shadows
    tableView->setStyleSheet(R"(
        QScrollBar:vertical, QScrollBar:horizontal {
            border: none;
            background: white;
            width: 10px; /* Adjust the width as needed */
            height: 10px; /* Adjust the height as needed */
        }
        QScrollBar::handle:vertical, QScrollBar::handle:horizontal {
            background: BASE_COLOR_BLUE;
            border: none; /* Remove borders */
            border-width: 1px;
            border-radius: 5px; /* Optional: Set to 0 to have square corners */
        }
        QScrollBar::add-line, QScrollBar::sub-line {
            border: none;
            background: none;
        }
        QScrollBar::up-arrow, QScrollBar::down-arrow,
        QScrollBar::left-arrow, QScrollBar::right-arrow {
            width: 0px;
            height: 0px;
            background: none;
        }
        QScrollBar::add-page, QScrollBar::sub-page {
            background: none;
        }
        QTableView {
            gridline-color: transparent;
            border-radius: 20px; /* Adjust the radius for rounded corners */

            /* Include other styling for QTableView here if necessary */
        }

        QHeaderView::section {
            background-color: rgba(20, 36, 65, 255);
            color: white;
            border: none;
            font: bold 14px; /* Add 'bold' to make the font bold */
            text-align: left; /* Center the text horizontally */
            padding: 0; /* Set padding to 0 */
        }


    )");

    //alternating the row colors, base is white, the alternate-base is grey
    tableView->setItemDelegate(new AlternateRowDelegate);
    // Disable item selection in the QTableView

    tableView->setSelectionMode(QAbstractItemView::NoSelection);

}

void MainWindow::setupGridLayout() {
    QWidget *centralWidget = new QWidget(this);
    QGridLayout *gridLayout = new QGridLayout(centralWidget);

    // Set column stretch factors based on the desired percentage widths
    gridLayout->setColumnStretch(0, 1);  // 1% for first column (Left Margin)
    gridLayout->setColumnStretch(1, 15); // 15% for second column (left Hand Panel)
    gridLayout->setColumnStretch(2, 1);  // 1% for third column
    gridLayout->setColumnStretch(3, 60); // 60% for fourth column
    gridLayout->setColumnStretch(4, 10);  // 10% for fifth column
    gridLayout->setColumnStretch(5, 1);  // 1% for sixth column


    // Set row stretch factors
    gridLayout->setRowStretch(0, 1);     // 1% for first row
    for (int row = 1; row < 11; ++row) {
        gridLayout->setRowStretch(row, 10); // Evenly distribute middle rows
    }
    gridLayout->setRowStretch(11, 1);     // 1% for last row

    // Initialize tableView and add it to the grid layout
    //tableView = new QTableView(centralWidget);
    gridLayout->addWidget(tableView, 3, 3, 8, 2); // Span from row 3 to row 11 (6 rows) in the third column
    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);


    // Eliminate padding between cells
    gridLayout->setSpacing(0); // This will set both horizontal and vertical spacing to zero
    gridLayout->setContentsMargins(0, 0, 0, 0); // This will set the margins to zero on all sides

    // Create one widget with a gradient background spanning from row 2 to row 9
    //blueBackgroundWidget = new QWidget (centralWidget);
    blueBackgroundWidget->setStyleSheet(
        "background: qlineargradient("
         "x1:0, y1:0, x2:0, y2:1, "
        "stop:0 rgba(45,64,134,255), "
        "stop:1 BASE_COLOR_BLUE);"
        "border-radius: 10px;" // Adjust the radius as needed

        );
    // Add it to the layout with a row span
    gridLayout->addWidget(blueBackgroundWidget, 1, 1, 10, 1); // Row, Column, RowSpan, ColumnSpan

    // Create a QPalette and set the placeholder text color
    QPalette palette = searchLineEdit->palette();
    palette.setColor(QPalette::PlaceholderText, QColor(128, 128, 128, 150)); // Set placeholder text color to transparent grey


    searchLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // Apply the modified palette to the QLineEdit
    searchLineEdit->setPalette(palette);
    searchLineEdit->setPlaceholderText("Enter search text");
    // Style the searchLineEdit with rounded edges
    searchLineEdit->setStyleSheet(
        "QLineEdit {"
        "    border: 1px solid #a0a0a0;"
        "    border-radius: 10px;"
        "    padding: 5px 8px;"
        "    selection-background-color: darkgray;"
        "    font-size: 24px;"
        "    color: BASE_COLOR_BLUE;"
        "    margin-bottom: 5px;" // Add a bottom margin of 5 pixels
        "}"
        );
    gridLayout->addWidget(searchLineEdit, 2, 3, 1, 1);// Row, Column, RowSpan, ColumnSpan


    setCentralWidget(centralWidget);
}
