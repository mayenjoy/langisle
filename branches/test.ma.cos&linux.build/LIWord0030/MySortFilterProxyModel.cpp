#include "StdAfx.h"
#include "MySortFilterProxyModel.h"

MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent)
	: QSortFilterProxyModel(parent)
{

}

MySortFilterProxyModel::~MySortFilterProxyModel()
{

}

bool MySortFilterProxyModel::lessThan(const QModelIndex &left,
									  const QModelIndex &right) const
{
	QVariant leftData = sourceModel()->data(left);
	QVariant rightData = sourceModel()->data(right);

	if (leftData.type() == QVariant::DateTime) 
	{
		return leftData.toDateTime() < rightData.toDateTime();
	} 
	else 
	{

		QString leftString = leftData.toString();

		QString rightString = rightData.toString();

		return QString::localeAwareCompare(leftString, rightString) < 0;
	}
}