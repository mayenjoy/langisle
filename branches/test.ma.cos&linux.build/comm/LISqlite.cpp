#include "stdafx.h"
#include "LISqlite.h"

LISqlite::LISqlite(const QString &dbpath, const QString &connName) : m_connName(connName)
{
	db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connName));
	db->setDatabaseName(dbpath);
	if (!db->open()) 
	{
		DBG(qDebug() << "LISqlite::LISqlite打开数据库失败" << dbpath << connName);
	}
}

LISqlite::~LISqlite(void)
{

	db->close();
	delete db;
	QSqlDatabase::removeDatabase(m_connName);
}

QVariant LISqlite::rexec(const QString &sql)
{
	QSqlQuery query = db->exec(sql);
	if (query.next())
	{
		return query.record().value(0);
	}
	else
	{
		if (query.lastError().number() != -1)	// 没有对应的数据
		{
			DBG(qDebug() << "LISqlite::rexec没有对应的数据" << sql << query.lastError() << query.lastError().number() << query.lastError().type());
		}

	}
	return "";
}

uint  LISqlite::lastInsertID()
{
	return rexec("SELECT last_insert_rowid();").toUInt();
}

bool LISqlite::exec(const QString &sql)
{
	db->exec(sql);

	QSqlError::ErrorType type = db->lastError().type();

	if (type != QSqlError::NoError)
	{
		QString message = db->lastError().text();

		DBG(qDebug() << message);
		return false;
	}


	return true;

// 	if(message.isEmpty() || message.size() <= 2)
// 		return true;
// 	else
// 	{
// 		qDebug() << message;
// 		return false;
// 	}
}

bool LISqlite::isTableExist(const QString &tableName)
{
	return db->tables(QSql::Tables).contains(tableName);
	//return rexec(QString("SELECT COUNT(*) FROM sqlite_master where type='table' and name='%1'").arg(tableName)).toBool();
}
