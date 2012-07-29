#include "stdafx.h"
#include "BCDBMgrBase.h"
#include "BCMgr.h"
#include "BCDBModel.h"
BCDBMgrBase::BCDBMgrBase(const QString &dbpath, const QString &connName, const QString& categoryTName, const QString &bookTName) 
: LISqlite(dbpath, connName), m_categoryTName(categoryTName), m_bookTName(bookTName)
{
	model = 0;


}

BCDBMgrBase::~BCDBMgrBase(void)
{
	if (model)
	{
		delete model;
		model = 0;
	}
}


void BCDBMgrBase::fillCagegoryToTree(QTreeWidgetItem *parentItem, int pid)
{
	fillCagegoryToTreeByTableName(m_categoryTName, parentItem, pid);
}

void BCDBMgrBase::fillCagegoryToTreeByTableName(const QString &tableName, QTreeWidgetItem *parentItem, int pid)
{
	QSqlQuery query(*db);
	query.prepare(QString("select * from %1 where pid = ? order by orderid").arg(tableName));
	query.addBindValue(pid);
	query.exec();

	QSqlRecord rec = query.record();
	int iname = rec.indexOf("name");
	int icid = rec.indexOf("cid");

	while (query.next())
	{		
		QString name = query.value(iname).toString();
		int cid =  query.value(icid).toInt();
		QTreeWidgetItem *item = new QTreeWidgetItem(parentItem, QStringList(name), parentItem->type());
		item->setData(0, UserRoleCID, cid);
		//item->setIcon(0, QIcon(":/LIWord0030/t.png"));

		//item->setData(0, Qt::SizeHintRole, QSize(item->data(0, Qt::SizeHintRole).toSize().width(), 48));
		parentItem->addChild(item);
		fillCagegoryToTree(item, cid);
	}
}

void BCDBMgrBase::fillBookToTable(QTableView *table, int cid, const QString &searchText, bool bSelectRows )
{
	if (bSelectRows)
	{
		// table->setSelectionMode(QAbstractItemView::ContiguousSelection);
		table->setSelectionMode(QAbstractItemView::ExtendedSelection);
		 
	}
	else
	{
		table->setSelectionMode(QAbstractItemView::SingleSelection);
	}
	fillBookToTableByTableName(m_bookTName, table, cid, searchText);
}

BCDBModel *BCDBMgrBase::createModel()
{
	return new BCDBModel();
}

void BCDBMgrBase::fillBookToTableByTableName(const QString &tableName, QTableView *table, int cid, const QString &searchText)
{
// 	BCDBModel *oldmodel = (BCDBModel *)table->model();
// 	if (oldmodel)
// 	{
// 		//qDebug() << "BCDBMgrBase::fillBookToTable model ?";
// 		oldmodel->clear();
// 		delete oldmodel;
// 	}

	if (model)
	{
		delete model;
		model = 0;
	}

	model = createModel();
	QString sql;
	QString text = searchText;
	text.replace("'", "''");
	if (cid == 0) // 根节点
	{
		if (!text.isEmpty())
		{
			sql = QString("select * from %2 where name like '%%1%'").arg(text).arg(m_bookTName);
		}
		else
			sql = QString("select * from %2").arg(m_bookTName);
	}
	else
	{
		if (!text.isEmpty())
		{
			sql = QString("select * from %4 where (cid = %1 or cid in (select cid from %5 where pid = %2)) and name like '%%3%'").arg(cid).arg(cid).arg(text).arg(m_bookTName).arg(m_categoryTName);
		}
		else
			sql = QString("select * from %3 where cid = %1 or cid in (select cid from %4 where pid = %2)").arg(cid).arg(cid).arg(m_bookTName).arg(m_categoryTName);
	}
	//qDebug() << sql;
	model->setSql(sql, *db);
	model->updateHeaderName();
	table->setModel(model);

	
}

void BCDBMgrBase::createTable()
{
	db->transaction();

	exec(QString("CREATE TABLE [%1] \
		(\
		[cid] INTEGER PRIMARY KEY AUTOINCREMENT, \
		[pid] INTEGER DEFAULT 0,\
		[orderid] INTEGER NOT NULL ON CONFLICT FAIL,\
		[name] TEXT NOT NULL ON CONFLICT FAIL\
		);").arg(m_categoryTName));

	exec(QString("CREATE TABLE [%1] \
		(\
		[bid] INTEGER PRIMARY KEY AUTOINCREMENT, \
		[cid] INTEGER NOT NULL ON CONFLICT FAIL DEFAULT 0,\
		[orderid] INTEGER NOT NULL ON CONFLICT FAIL,\
		[name] TEXT NOT NULL ON CONFLICT FAIL,\
		[wcount] INTEGER NOT NULL ON CONFLICT FAIL DEFAULT 0 \
		);").arg(m_bookTName));


	//exec("CREATE TABLE [otherinfo] \
	//	(\
	//	[iname] TEXT NOT NULL ON CONFLICT FAIL,\
	//	[ivalue] TEXT\
	//	);");


	db->commit();
}


int BCDBMgrBase::addCategory(const QString &cname, int pid, int orderid)
{
	if (isCategoryExist(pid, cname))
		return 0;	

	if (-1 == orderid)
		orderid = getMaxOrderId() + 10;

	QString sql = QString("insert into %4 (pid, orderid, name) values(%1, %2, '%3')").arg(pid).arg(orderid).arg(cname).arg(m_categoryTName);
	exec(sql);
	return lastInsertID();

}

int BCDBMgrBase::getMaxOrderId()
{
	return rexec(QString("select max(orderid) from %1").arg(m_categoryTName)).toInt();
}

bool BCDBMgrBase::isSecondCategory(int cid)
{
	return rexec(QString("select count(*) from %2 where pid = 0 and cid = %1").arg(cid).arg(m_categoryTName)).toBool();
}

bool BCDBMgrBase::isCategoryExist(int pid, const QString &cname)
{
	return rexec(QString("select count(*) from %3 where pid = %1 and name = '%2'").arg(pid).arg(cname).arg(m_categoryTName)).toBool();
}

bool BCDBMgrBase::isBookExist(int cid, const QString &bname)
{

	QSqlQuery query(*db);
	query.prepare(QString("select * from %1 where name = :name").arg(m_bookTName)); // 全部用户词库中是否有重复的
	query.bindValue(":name", bname);
	/* 本层和下层是否存在
	query.prepare(QString("select * from %1 where name = :name and (cid = :cid1 or cid in (select cid from %2 where pid = :cid2) or cid in (select pid from %2 where cid = :cid3)) ").arg(m_bookTName).arg(m_categoryTName));
 	query.bindValue(":cid1", cid);
	query.bindValue(":cid2", cid);
	query.bindValue(":cid3", cid);
	query.bindValue(":name", bname);
	*/

	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::isBookExist faild." << query.lastError() << query.lastQuery());
	}


	return query.next();

}


void BCDBMgrBase::removeCategory(int cid)
{
	QString sql = QString("delete from %3 where cid = %1 or pid = %2").arg(cid).arg(cid).arg(m_categoryTName);
	//qDebug() << sql;
	exec(sql);
}

void BCDBMgrBase::modifyCagegory(const QString &ncName, int cid)
{
	exec(QString("update %3 set name = '%1' where cid = %2").arg(ncName).arg(cid).arg(m_categoryTName));
}

QString BCDBMgrBase::getCategoryNameByCid(int cid)
{
	return rexec(QString("select name from %2 where cid = %1").arg(cid).arg(m_categoryTName)).toString();
}

int BCDBMgrBase::getBidByRow(int row)
{
	return model->getValueByItemname("bid", row).toInt();
}

QString BCDBMgrBase::getBookNameByRow(int row)
{
	return model->getValueByItemname("name", row).toString();
}

QString BCDBMgrBase::getBookCommentByRow(int row)
{
	return model->getValueByItemname("comment", row).toString();
}

int BCDBMgrBase::getBookWCountByRow(int row)
{
	return model->getValueByItemname("wcount", row).toInt();
}

int BCDBMgrBase::getRowByBID(int bid)
{
	return model->getRowByBID(bid);
}

void BCDBMgrBase::updateWordCount(int bid, int wcount)
{
	QSqlQuery query(*db);
	query.prepare(QString("update %1 set wcount = :wcount where bid = :bid").arg(m_bookTName));
	query.bindValue(":bid", bid);
	query.bindValue(":wcount", wcount);

	if (!query.exec())
	{
		DBG(qDebug() << "BCDBMgrBase::updateWordCount faild." << db->lastError());
	}
}

int BCDBMgrBase::getBidByName(const QString &bname, int cid)
{
	QSqlQuery query(*db);
	query.prepare(QString("select bid from %1 where name = :name and cid = :cid").arg(m_bookTName));
	query.bindValue(":name", bname);
	query.bindValue(":cid", cid);

	if (!query.exec())
	{
		DBG(qDebug() << "BCDBMgrBase::getBidByName faild." << db->lastError());
	}
	query.next();
	return query.record().value(0).toInt();
}

int BCDBMgrBase::getCidbyCName(const QString &cname)
{
	QSqlQuery query(*db);
	query.prepare(QString("select cid from %1 where name = :name").arg(m_categoryTName));
	query.bindValue(":name", cname);


	if (!query.exec())
	{
		DBG(qDebug() << "BCDBMgrBase::getCidbyCName faild." << db->lastError());
	}
	query.next();
	return query.record().value(0).toInt();
}

QList<BOOKINFO> BCDBMgrBase::getAllBookInfoByCid(int cid)
{
	QList<BOOKINFO> result;

	QSqlQuery query(*db);
	query.prepare(QString("select * from %1 where cid = :cid").arg(m_bookTName));
	query.bindValue(":cid", cid);
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::getAllBookInfoByCid faild.");
	}

	while (query.next())
	{
		BOOKINFO bi;
		bi["btype"] = btUser;
		bi["bid"] = query.record().value("bid");
		bi["name"] = query.record().value("name");
		bi["wcount"] = query.record().value("wcount");
		//DBG(qDebug() << "UDataDB::getAllBookInfoByCid" << query.record().value("name").toString());
		result.append(bi);
	}


	return result;
}

bool BCDBMgrBase::isHaveSubCategory(int cid)
{
	QSqlQuery query(*db);
	query.prepare(QString("select cid from %1 where pid = %2").arg(m_categoryTName).arg(cid));
	if (!query.exec())
	{
		DBG(qDebug() << "BCDBMgrBase::isHaveSubCategory" << query.lastError());
		return false;
	}

	return query.next();
}

bool BCDBMgrBase::isCanDeleteCategory(int cid)
{
	return !isHaveSubCategory(cid) && !isHaveCategoryBook(cid);
}

bool BCDBMgrBase::isHaveCategoryBook(int cid)
{
	QSqlQuery query(*db);
	query.prepare(QString("select bid from %1 where cid = %2").arg(m_bookTName).arg(cid));
	if (!query.exec())
	{
		DBG(qDebug() << "BCDBMgrBase::isHaveCategoryBook" << query.lastError());
		return false;
	}

	return query.next();
}