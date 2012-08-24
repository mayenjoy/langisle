#include "stdafx.h"
#include "OBCDBModel.h"

OBCDBModel::OBCDBModel(QObject *parent)
	: BCDBModel(parent)
{

}

OBCDBModel::~OBCDBModel()
{

}

QVariant OBCDBModel::data( const QModelIndex & item, int role) const
{
	if (Qt::ToolTipRole == role)
	{

		static int row = -1;
		if (row != item.row())
		{
			DBG(qDebug() << "OBCDBModel::data Qt::ToolTipRole row");
			row = item.row();
			return QString("%1").arg(record(row).value("comment").toString());			

		}
	}
	return BCDBModel::data(item, role);
}
