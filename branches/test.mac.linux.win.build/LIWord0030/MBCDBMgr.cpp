#include "stdafx.h"
#include "MBCDBMgr.h"
#include "MyBCDBModel.h"
MBCDBMgr::MBCDBMgr(const QString &dbpath, const QString &connName) 
: BCDBMgrBase(dbpath, connName, "mycategory", "mybook")
{
	if (!isTableExist("mycategory"))	// 新建的数据库,创建表
	{
		this->createTable();
		this->initData();
	}
}

MBCDBMgr::~MBCDBMgr(void)
{
}

int MBCDBMgr::getBTypeByRow(int row)
{
	return model->getValueByItemname("btype", row).toInt();
}

void MBCDBMgr::createTable()
{
	db->transaction();

	exec(QString("CREATE TABLE [%1] \
				 (\
				 [cid] INTEGER PRIMARY KEY AUTOINCREMENT, \
				 [pid] INTEGER DEFAULT 0,\
				 [orderid] INTEGER NOT NULL ON CONFLICT FAIL,\
				 [name] TEXT NOT NULL ON CONFLICT FAIL\
				 );").arg(m_categoryTName));

	exec(QString("CREATE TABLE [%1] (\
		[bid] INTEGER, \
		[cid] INTEGER, \
		[orderid] INTEGER DEFAULT 0, \
		[name] TEXT, \
		[wcount] INTEGER DEFAULT 0, \
		[scount] INTEGER DEFAULT 0, \
		[kncount] INTEGER DEFAULT 0, \
		[progress] FLOAT DEFAULT 0, \
		[btype] INTEGER, \
		[createTime] DATETIME, \
		CONSTRAINT [sqlite_autoindex_mybook_1] PRIMARY KEY ([bid], [btype]));").arg(m_bookTName));


	if (!db->commit())
	{
		DBG(qDebug() << "MBCDBMgr::createTable faild" << db->lastError());
	}
}

void MBCDBMgr::initData()
{
	addCategory("学习中..", 0);
	addCategory("已掌握..", 0);
}

BCDBModel *MBCDBMgr::createModel()
{
	return new MyBCDBModel();
}

void MBCDBMgr::updateWordCount(int bid, bookType bt, int wcount)
{
	QSqlQuery query(*db);
	query.prepare(QString("update %1 set wcount = :wcount where bid = :bid and btype = :btype").arg(m_bookTName));
	query.bindValue(":bid", bid);
	query.bindValue(":btype", bt);
	query.bindValue(":wcount", wcount);

	if (!query.exec())
	{
		DBG(qDebug() << "MBCDBMgr::updateWordCount faild." << db->lastError());
	}
}

void MBCDBMgr::delBook(int bid, bookType bt)
{
	QSqlQuery query(*db);
	query.prepare("delete from mybook where bid = :bid and btype = :btype");
	query.bindValue(":bid", bid);
	query.bindValue(":btype", bt);

	if (!query.exec())
	{
		DBG(qDebug() << "MBCDBMgr::delBook faild." << db->lastError());
	}
}

int MBCDBMgr::getSBookCount()
{
	QSqlQuery query(*db);
	query.prepare("select count(bid) from mybook");

	if (!query.exec())
	{
		DBG(qDebug() << "MBCDBMgr::getSBookCount faild." << db->lastError());
	}

	query.next();
	return query.record().value(0).toInt();
}
