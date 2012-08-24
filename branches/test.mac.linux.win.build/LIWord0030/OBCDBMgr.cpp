#include "stdafx.h"
#include "OBCDBMgr.h"
#include "BCMgr.h"
#include "OBCDBModel.h"


OBCDBMgr::OBCDBMgr(const QString &dbpath, const QString &connName) : BCDBMgrBase(dbpath, connName)
{
	
}

OBCDBMgr::~OBCDBMgr(void)
{
}

void OBCDBMgr::updateAllBookExistState()
{

	DBG(QDateTime   start   =   QDateTime::currentDateTime());

	QSqlQuery query(*db);
	query.prepare("select bid from book");
	query.exec();
	db->transaction();
	while (query.next())
	{
		int bid = query.value(0).toInt();
		updateBookExistState(bid, isBookExist(bid));

	}


	
	if (!db->commit())
	{
		DBG(qDebug() << "OBCDBMgr::updateAllBookExistState" << db->lastError().text());
	}
	DBG(qDebug() << "OBCDBMgr::updateAllBookExistStateºÄÊ±" << start.msecsTo(QDateTime::currentDateTime()));

}

bool OBCDBMgr::isBookExist(int bid)
{
	return QFile::exists(getBookPath(bid));
}

QString OBCDBMgr::getBookPath(int bid)
{
	return Global::makePathStr("/odata/book/" + QString::number(bid) + ".db");
}

void OBCDBMgr::updateBookExistState(int bid, bool state)
{
	exec(QString("update book set exist = %1 where bid = %2").arg(state).arg(bid));
}

BCDBModel *OBCDBMgr::createModel()
{
	return new OBCDBModel();
}