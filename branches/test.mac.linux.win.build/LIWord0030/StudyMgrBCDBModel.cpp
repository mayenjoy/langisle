#include "stdafx.h"
#include "StudyMgrBCDBModel.h"
#include <math.h>
#include "BCMgr.h"


StudyMgrBCDBModel::StudyMgrBCDBModel(QObject *parent)
: BCDBModel(parent)
{
}

StudyMgrBCDBModel::~StudyMgrBCDBModel()
{
}

void StudyMgrBCDBModel::sort(int column, Qt::SortOrder order)
{

	sortCmd = sqlCmd + QString(" order by %1 %2")
		.arg(record().fieldName(column))
		.arg(Qt::AscendingOrder == order ? "" : "desc");
		DBG(qDebug() << "PageSM::PageSM SQL:"<<sortCmd);
	setQuery(sortCmd, *pdb);
	resetCustomInfo();
	//qDebug() << "LISqlQueryModel::sort" << record().fieldName(column) << sortCmd;
}

QVariant StudyMgrBCDBModel::data ( const QModelIndex & item, int role) const
{
	if (Qt::DisplayRole == role)
	{
		QString colName = record().fieldName(item.column());
		QVariant val = record(item.row()).value(colName);
		int maxfp = g_pbcmgr->getUDataDB()->getMaxFP();

		if (colName == "mwid")
		{
			return item.row()+1;
		}
		if (colName == "lst")
		{
			return record(item.row()).value("lst").toString().split('T').join(" ");
		}
		if (colName == "mean")
		{
		    return record(item.row()).value("mean").toString().split('\n')[0];
			//return record(item.row()).value("mean").toString().split('\n').join(" ");
		}

		if (colName == "phase")
		{		
			if (record(item.row()).value("phase").toString()!=NULL)
			{
				return QString("%1->%2").arg(record(item.row()).value("phase").toInt()).arg(record(item.row()).value("phase").toInt()+1);
			}
		}

		if (colName == "pron")
		{
			if(record(item.row()).value("pron").toString()!=NULL)
			{
				return QString("/%1/").arg(record(item.row()).value("pron").toString());
			}		
		}


	 if (colName == "error")
		{
			if (record(item.row()).value("error").toString()!=NULL)
			{
				return QString("%1%").arg(record(item.row()).value("error").toInt());
			}
		}

		if (colName == "knowndegree")
		{
			if (record(item.row()).value("knowndegree").toString()!=NULL)
			{
				return QString("%1%").arg(record(item.row()).value("knowndegree").toInt());
			}
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
