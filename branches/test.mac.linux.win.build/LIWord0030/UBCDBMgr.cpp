#include "stdafx.h"
#include "UBCDBMgr.h"
#include "UBCDBModel.h"
UBCDBMgr::UBCDBMgr(const QString &dbpath, const QString &connName) 
: BCDBMgrBase(dbpath, connName, "ucategory", "ubook")
{
	if (!isTableExist("ucategory"))	// �½������ݿ�,������
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
	int cid = addCategory("�ҵ����ʱ�", 0);
	addBook("Ĭ�����ʱ�", cid);
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
