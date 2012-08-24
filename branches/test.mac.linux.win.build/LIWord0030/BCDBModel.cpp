#include "stdafx.h"
#include "BCDBModel.h"
#include "config.h"

QMap<QString, QString> BCDBModel::c2hmap;
QMap<QString, int> BCDBModel::n2size;
BCDBModel::BCDBModel(QObject *parent)
	: LISqlQueryModel(parent)
{

}

BCDBModel::~BCDBModel()
{

}

int BCDBModel::getRowByBID(int bid)
{
	int size = getRowCount();
	// qDebug() << "BCDBModel::getRowByBID" << bid  << size;
	for(int i = 0; i< size; i++)
	{
		int tbid =  record(i).value("bid").toInt();
		// qDebug() << tbid << bid;
		if(tbid == bid)
		{
			return i;
		}
	}
	
	return -1;
}

void BCDBModel::initC2hmap()
{
	c2hmap["name"] = "词库名称";
	c2hmap["wcount"] = "单词数";

	c2hmap["exist"] = "是否安装";
	c2hmap["btype"] = "词库类型";

	c2hmap["createTime"] = "首次学习时间";
	c2hmap["scount"] = "学习数";
	c2hmap["kncount"] = "掌握数";
	c2hmap["progress"] = "学习进度";

	c2hmap["bwid"] = "ID";
	c2hmap["status"] = "状态";
	c2hmap["word"] = "单词";
	c2hmap["pron"] = "音标";
	c2hmap["mean"] = "释义";

	c2hmap["mwid"] = "ID";
	c2hmap["error"] = "错误率";
	c2hmap["knowndegree"] = "熟悉度";
	c2hmap["sc"] = "次数";
	c2hmap["st"] = "阶段";
	c2hmap["score"] = "得分";
	c2hmap["lst"] = "最后学习时间";
	c2hmap["phase"] = "阶段";
	
}

void BCDBModel::initN2size()
{
	n2size["name"] = 240;
	n2size["wcount"] = 80;
	n2size["scount"] = 80;
	n2size["kncount"] = 80;
	n2size["progress"] = 100;
	n2size["createTime"] = 125;
}

int BCDBModel::getSizeSection(const QString &sname)
{
	if (n2size.contains(sname))
	{
		return n2size[sname];
	}
	return 0;
}

void BCDBModel::updateHeaderName()
{
	for (int i = 0; i < record().count(); i++)
	{
		setHeaderData(i,  Qt::Horizontal, getHeaderNameByColname(record().fieldName(i)));
	}
}

QString BCDBModel::getHeaderNameByColname(QString colname)
{
	if (c2hmap.contains(colname))
		return c2hmap[colname];

	return colname;
}

QVariant BCDBModel::data ( const QModelIndex & item, int role)  const
{
	if (Qt::DisplayRole == role)
	{
		QString colName = record().fieldName(item.column());
		QVariant val = record(item.row()).value(colName);
		if (colName == "exist")
		{
			if (val.toBool())
			{
				return "已安装";
			}
			else 
			{
				return "未安装";
			}
		}
		else if (colName == "wcount" || colName == "scount" || colName == "kncount")
		{
			return QString("%1个").arg(val.toInt());
		}
		else if (colName == "progress")
		{
			return QString("%1%").arg(QString().setNum(val.toFloat() * 100, 'f', 2));
		}
		else if (colName == "createTime")
		{
			return val.toDateTime();
		}
		else if (colName == "btype")
		{
			if (val.toInt() == btOfficial)
			{
				return "官方词库";
			}
			else // btUser
			{
				return "用户词库";
			}

		}
	} else if (Qt::TextColorRole == role && record().fieldName(item.column()) == QString("exist"))
	{
		if (record(item.row()).value("exist").toBool())
			return QVariant(QColor("#219A44"));
		else
			return QVariant(QColor("#666666"));

	}
	else if (Qt::BackgroundColorRole == role)
	{
		//DBG(qDebug() << "BCDBModel::data Qt::BackgroundColorRole row" << item.row() << item.column() << hoverRow);
		if (item.row() == hoverRow)
		{
			//return 	QBrush(QColor(225, 240, 255));
			// return 	QBrush(QColor(0xf0, 0xf0, 0xf0));
			return 	QBrush(QColor(0xbf, 0xcd, 0xe4));
		}
		
		
	}

	return QSqlQueryModel::data(item, role);
}

QVariant BCDBModel::getRecordByRow(int row, const QString &colName)
{
	return record(row).value(colName);
}

