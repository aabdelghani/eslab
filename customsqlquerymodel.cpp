// CustomSqlQueryModel.cpp
#include "CustomSqlQueryModel.h"
#include <QVariant>
#include <QSqlQueryModel>
QVariant CustomSqlQueryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::TextAlignmentRole && orientation == Qt::Horizontal) {
        return Qt::AlignLeft;
    }
    return QSqlQueryModel::headerData(section, orientation, role);
}
