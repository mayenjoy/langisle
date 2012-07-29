#include "stdafx.h"
#include "UBCDBModel.h"

UBCDBModel::UBCDBModel(QObject *parent)
	: BCDBModel(parent)
{

}

UBCDBModel::~UBCDBModel()
{

}

QVariant UBCDBModel::data( const QModelIndex & item, int role) const
{
	if (Qt::ToolTipRole == role)
	{

		static int row = -1;
		if (row != item.row())
		{
			DBG(qDebug() << "UBCDBModel::data Qt::ToolTipRole row");
			row = item.row();
			return QString("%1").arg(record(row).value("comment").toString());			

		}
	}
	return BCDBModel::data(item, role);
}
