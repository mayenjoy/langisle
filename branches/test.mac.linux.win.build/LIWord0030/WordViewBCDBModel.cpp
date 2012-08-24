#include "stdafx.h"
#include "WordViewBCDBModel.h"

WordViewBCDBModel::WordViewBCDBModel(QObject *parent)
	: BCDBModel(parent)
{

}

WordViewBCDBModel::~WordViewBCDBModel()
{

}


QVariant WordViewBCDBModel::data ( const QModelIndex & item, int role) const
{
	if (Qt::DisplayRole == role)
	{
		QString colName = record().fieldName(item.column());
		QVariant val = record(item.row()).value(colName);
		if (colName == "mean")
		{
			return record(item.row()).value("mean").toString().split('\n')[0];
		}

	}
	if (Qt::ToolTipRole == role)
	{

		static int row = -1;
		if (row != item.row())
		{
			//DBG(qDebug() << "WordViewBCDBModel::data Qt::ToolTipRole row");
			row = item.row();
			return QString("%1").arg(record(row).value("mean").toString());			

		}
	}
	return BCDBModel::data(item, role);
}