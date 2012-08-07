#include "stdafx.h"
#include "UBCDBMgr.h"
#include "UBCDBModel.h"
UBCDBMgr::UBCDBMgr(const QString &dbpath, const QString &connName) 
: BCDBMgrBase(dbpath, connName, "ucategory", "ubook")
{
	if (!isTableExist("ucategory"))	// 新建的数据库,创建表
	{
		this->createTable();
		this->initData();
	}
}

UBCDBMgr::~UBCDBMgr(void)
{

}

void UBCDBMgr::initData()
{
	int cid = addCategory("我的生词本", 0);
	addBook("默认生词本", cid);
}

BCDBModel *UBCDBMgr::createModel()
{
	return new UBCDBModel();
}


int UBCDBMgr::addBook(const QString &cbook, int cid)
{
	if (isBookExist(cid, cbook))
		return 0;


	QSqlQuery query(*db);
	query.prepare("insert into ubook(cid, orderid, name, wcount) values(:cid, 0, :name, 0)");
	query.bindValue(":cid", cid);
	query.bindValue(":name", cbook);

	if (!query.exec())
	{
		DBG(qDebug() << "UBCDBMgr::addBook faild." << db->lastError());
	}

	return lastInsertID();;
}


void UBCDBMgr::delBook(int bid)
{
	QSqlQuery query(*db);
	query.prepare("delete from ubook where bid = :bid");
	query.bindValue(":bid", bid);

	if (!query.exec())
	{
		DBG(qDebug() << "UBCDBMgr::delBook faild." << db->lastError());
	}
}
