#include "stdafx.h"
#include "studymgrmodel.h"
#include "BCMgr.h"
#include "ExportWordBase.h"
static QMap<QString, QString> c2hmap;
static QMap<QString, int> n2size;

StudyMgrModel::StudyMgrModel(QObject *parent, QTableView *view)
	: LISqlQueryModel(parent)
{
	this->view = view;
	bAllSel = false;
	bnotStudySel = false;
	bstudingSel = false;
	bmasterSel = false;

}

StudyMgrModel::~StudyMgrModel()
{

}

void StudyMgrModel::initC2hmap()
{
	c2hmap["word"] = "单词";
	c2hmap["status"] = "状态";
	c2hmap["st"] = "进度";
	c2hmap["sc"] = "难度";
	c2hmap["lst"] = "最后学习时间";
	c2hmap["nst"] = "下次学习时间";

}

void StudyMgrModel::initN2size()
{
	n2size["word"] = 180;
	n2size["status"] = 60;	
	n2size["st"] = 60;
	n2size["sc"] = 35;
	n2size["lst"] = 130;
	n2size["nst"] = 130;
}
int StudyMgrModel::getSizeSection(const QString &sname)
{
	if (n2size.contains(sname))
	{
		return n2size[sname];
	}
	return 0;
}
void StudyMgrModel::updateHeaderName()
{
	for (int i = 0; i < record().count(); i++)
	{
		setHeaderData(i,  Qt::Horizontal, getHeaderNameByColname(record().fieldName(i)));
	}
}

QString StudyMgrModel::getHeaderNameByColname(QString colname)
{
	if (c2hmap.contains(colname))
		return c2hmap[colname];

	return colname;
}


Qt::ItemFlags StudyMgrModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;
	if (index.column()==0)//对于第一列设置标志位
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
	else if (index.column()==2)
	{
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
	}
	//CheckBox
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant StudyMgrModel::data ( const QModelIndex & item, int role)  const
{
	if (item.column()==0 && role == Qt::CheckStateRole)
	{
		return this->getCheck(item);
	}
	if (Qt::DisplayRole == role)
	{
		QString colName = record().fieldName(item.column());
		QVariant val = record(item.row()).value(colName);
		if (colName == "status")
		{
			int value = val.toInt();
			switch (value)
			{
			case 0:
				return "未学";
			case 1:
				return "学习中";
			case 2:
				return "已掌握";
			}
		}
		else if (colName == "st")
		{
			return QString("%1%").arg(QString().setNum(g_pbcmgr->getUDataDB()->getNowFP(val.toInt()) * 100, 'f', 2));
		}
	}
	return QSqlQueryModel::data(item, role);
}

bool StudyMgrModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
		return false;

	if ((index.column()==0)&&(role==Qt::CheckStateRole))
		//CheckStateRole表示执行复选框状态的数据更新
	{
		setCheck(index, value);
	}
	return true;

}

void StudyMgrModel::setCheck(const QModelIndex &index, const QVariant &value1)
{
	QSqlQuery query(*pdb);
	query.prepare("update studyData.mdata set selstatus = :selstatus where mid = :mid");
	query.bindValue(":selstatus", value1.toInt());
	query.bindValue(":mid", record(index.row()).value("mid").toInt());
	if (!query.exec())
	{
		DBG(qDebug() << "StudyMgrModel::setCheck faild" << query.lastError() << query.executedQuery());
	}

	emit updateCheck();

}

void StudyMgrModel::setCheckReverse(const QModelIndex &index)
{
	//setData(index, getCheck(index).toBool() ? 0 : 2, Qt::CheckStateRole);
	setCheck(index, getCheck(index).toBool() ? 0 : 2);
	emit dataChanged(index, index);
}

QVariant StudyMgrModel::getCheck(const QModelIndex &index) const
{
	QSqlQuery query(*pdb);
	query.prepare("select selstatus from studyData.mdata where mid = :mid");
	query.bindValue(":mid", record(index.row()).value("mid").toInt());
	if (!query.exec())
	{
		DBG(qDebug() << "StudyMgrModel::getCheck faild" << query.lastError() << query.executedQuery());
	}
	if (!query.next())
		return 0;

	return query.record().value(0);
}

void StudyMgrModel::setNotStudyStatus(const QModelIndex &index)
{

	int value = record(index.row()).value("status").toInt();
	if (0 != value)
	{
		//if (QMessageBox::Yes == QMessageBox::question(0, "确定", "学习信息将丢失,确定修改吗?", QMessageBox::Yes | QMessageBox::No))
		{
			int mid = record(index.row()).value("mid").toInt();
			QSqlQuery query(*pdb);
			query.prepare("update studyData.mdata set sc = null, st = null, lst = null, score = null where mid = :mid");
			query.bindValue(":mid", mid);
			if (!query.exec())
			{
				DBG(qDebug() << "StudyMgrModel::setNotStudyStatus faild" << query.lastError() << query.executedQuery());
			}

			update();
		}

	}

}

void StudyMgrModel::setKnowStatus(const QModelIndex &index)
{
	int value = record(index.row()).value("status").toInt();
	if (2 != value)
	{
		//if (QMessageBox::Yes == QMessageBox::question(0, "确定","学习信息将丢失,确定修改吗?", QMessageBox::Yes | QMessageBox::No))
		{
			int mid = record(index.row()).value("mid").toInt();
			QSqlQuery query(*pdb);
			query.prepare("update studyData.mdata set sc = 1, st = :st, lst = :lst, score = null where mid=:mid");
			query.bindValue(":st", g_pbcmgr->getUDataDB()->getMaxFP());
			query.bindValue(":lst", QDateTime::currentDateTime());
			query.bindValue(":mid", mid);
			if (!query.exec())
			{
				DBG(qDebug() << "StudyMgrModel::setKnowStatus faild." << query.lastError());
			}

			update();
		}

	}


}

void StudyMgrModel::allButtonClick()
{
	bAllSel = !bAllSel;
	QSqlQuery query(*pdb);
	query.prepare("update studyData.mdata set selstatus = :selstatus");
	query.bindValue(":selstatus", bAllSel ? 2 : 0);
	if (!query.exec())
	{
		DBG(qDebug() << "StudyMgrModel::allButtonClick faild" << query.lastError() << query.executedQuery());
	}
	update();
}

void StudyMgrModel::notStudyClick()
{
	bnotStudySel = !bnotStudySel;
	QSqlQuery query(*pdb);
	query.prepare("update studyData.mdata set selstatus = :selstatus where lst is null");
	query.bindValue(":selstatus", bnotStudySel ? 2 : 0);
	if (!query.exec())
	{
		DBG(qDebug() << "StudyMgrModel::notStudyClick faild" << query.lastError() << query.executedQuery());
	}
	update();
}
void StudyMgrModel::studingClick()
{
	bstudingSel = !bstudingSel;
	QSqlQuery query(*pdb);
	query.prepare("update studyData.mdata set selstatus = :selstatus where (julianday(\'now\', \'localtime\') - julianday(lst)) * 86400.0 > st and st < :maxfp");
	query.bindValue(":selstatus", bstudingSel ? 2 : 0);
	query.bindValue(":maxfp", g_pbcmgr->getUDataDB()->getMaxFP());
	if (!query.exec())
	{
		DBG(qDebug() << "StudyMgrModel::studingClick faild" << query.lastError() << query.executedQuery());
	}
	update();
}
void StudyMgrModel::masterClick()
{
	bmasterSel = !bmasterSel;
	QSqlQuery query(*pdb);
	query.prepare("update studyData.mdata set selstatus = :selstatus where st >= :maxfp");
	query.bindValue(":selstatus", bmasterSel ? 2 : 0);
	query.bindValue(":maxfp", g_pbcmgr->getUDataDB()->getMaxFP());
	if (!query.exec())
	{
		DBG(qDebug() << "StudyMgrModel::masterClick faild" << query.lastError() << query.executedQuery());
	}
	update();
}

int StudyMgrModel::getSelCount()
{	
	QSqlQuery query(*pdb);
	query.prepare("select * from studyData.mdata where selstatus = 2");
	if (!query.exec())
	{
		DBG(qDebug() << "StudyMgrModel::getSelCount faild" << query.lastError() << query.executedQuery());
	}

	query.last();
	return query.at() >= 0 ? query.at() + 1 : 0;

}

void StudyMgrModel::MakeExportWord(ExportWordBase *ewb)
{
	QSqlQuery query(*pdb);
	if (!query.exec("select mdata.mid, word, mdata.mean from (select * from studyData.mdata where selstatus = 2 and wtype = 2) as mdata inner join dict.word on mdata.wid = dict.word.wid \
					union \
					select mdata.mid, word, mdata.mean from (select * from studyData.mdata where selstatus = 2 and wtype = 1) as mdata INNER join uword on mdata.wid = uword.wid order by mdata.mid"))
	{
		DBG(qDebug() << "UDataDB::MakeExportWord faild ." << query.lastError());
		return;
	}


	while (query.next())
	{
		//DBG(qDebug() << "UDataDB::getWordInfoByMid 3" << query.record().value("word") << query.record().value("mean"));
		ewb->AddWord(query.record().value("word").toString(), query.record().value("mean").toString());
	}

}
