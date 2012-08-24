#ifndef MYSORTFILTERPROXYMODEL_H
#define MYSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class MySortFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	MySortFilterProxyModel(QObject *parent = 0);
	~MySortFilterProxyModel();

protected:
	bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
private:
	
};

#endif // MYSORTFILTERPROXYMODEL_H
