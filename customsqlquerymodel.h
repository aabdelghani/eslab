// CustomSqlQueryModel.h
#ifndef CUSTOMSQLQUERYMODEL_H
#define CUSTOMSQLQUERYMODEL_H

#include <QSqlQueryModel>

class CustomSqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    using QSqlQueryModel::QSqlQueryModel; // Inherit constructors

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};

#endif // CUSTOMSQLQUERYMODEL_H
