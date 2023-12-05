// CustomSqlQueryModel.cpp
#include "CustomSqlQueryModel.h"
#include <QFont>
#include <QVariant>
#include <QSqlQueryModel>
#include <QBrush>
QVariant CustomSqlQueryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::TextAlignmentRole && orientation == Qt::Horizontal) {
        return Qt::AlignLeft;
    }
    if (role == Qt::FontRole && orientation == Qt::Horizontal) {
        QFont boldFont; // Create a QFont object for bold font
        boldFont.setBold(true); // Set the font to bold
        boldFont.setPointSize(10); // Set the font size (adjust as needed)
        return boldFont; // Return the bold and larger font for the header labels
    }
    if (role == Qt::ForegroundRole && orientation == Qt::Horizontal) {
        // Set the text color (adjust as needed)
        return QBrush(Qt::black);
    }
    return QSqlQueryModel::headerData(section, orientation, role);
}
