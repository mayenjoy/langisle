#include "stdafx.h"
#include "MyBCDBModel.h"
#include "BCMgr.h"
MyBCDBModel::MyBCDBModel(QObject *parent)
	: BCDBModel(parent)
{

}

MyBCDBModel::~MyBCDBModel()
{

}


QVariant MyBCDBModel::data( const QModelIndex & item, int role) const
{
	if (Qt::ToolTipRole == role)
	{
		static int row = -1;
		if (row != item.row())
		{
			DBG(qDebug() << "MyBCDBModel::data Qt::ToolTipRole row");
			row = item.row();
			//return QString("%1").arg(row);
		}
		return QString("待复习单词数:%1个")
			.arg(lcinfo[row]["WSCoun"].toInt());
	}
	else if (Qt::TextColorRole == role )
	{
		if (g_pbcmgr->getRemindMgr()->isEnableHightLight())
		{
			if (record(item.row()).value("wcount") == record(item.row()).value("kncount")) // 完全掌握
				return QVariant(QColor("#219A44"));
			float percent = lcinfo[item.row()]["WSCoun"].toInt() / (g_pbcmgr->getRemindMgr()->getThreshold() * 1.0);
			return QVariant(QColor(percent > 1.0 ? 255 : 255 * percent, 0, 0));
		}
	}
	return BCDBModel::data(item, role);
}

void MyBCDBModel::resetCustomInfo()
{
	// DBG(qDebug() << "MyBCDBModel::resetCustomInfo");
	while(canFetchMore())	// sqlite 不支持返回全部记录数,在全部读出后rowCount才会正确
		fetchMore(); 

	lcinfo.clear();
	count = rowCount();
	for(int i = 0; i < count; i++)
	{
		int bid =  record(i).value("bid").toInt();
		bookType bt = (bookType)record(i).value("btype").toInt();
		int nWSCount = g_pbcmgr->getUDataDB()->getBookReviewCount(bid, bt);
		QMap<QString, QVariant> cinfo;
		cinfo["WSCoun"] = nWSCount;
		lcinfo << cinfo;

	}
}
