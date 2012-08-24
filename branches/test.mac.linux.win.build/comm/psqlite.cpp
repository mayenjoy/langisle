#include "stdafx.h"
#include "psqlite.h"

// #pragma comment(lib, "qsqlite4.lib")

psqlite::psqlite(void)
{
	db = new QSqlDatabase;
}

psqlite::~psqlite(void)
{
	close();
}

bool psqlite::open(QString dbpath, QString connName)
{
	if (db == NULL)
		db = new QSqlDatabase;

	if (connName.isEmpty())
		connName = dbpath;


	m_connName = connName;
	m_dbpath = dbpath;
	*db = QSqlDatabase::addDatabase("QSQLITE", connName);
	db->setDatabaseName(dbpath);
	if (!db->open()) 
	{
		DBG(qDebug() << "打开数据库" << m_dbpath << "失败" << db->lastError());
		return false;
	}

	return true;
}

bool psqlite::close()
{
	if (NULL == db)
	{
		DBG(qDebug() << "重复卸载..");
		return false;
	}
	db->close();
	delete db;
	db = NULL;
	QSqlDatabase::removeDatabase(m_connName);
	return true;
}

bool psqlite::exec(QString sql)
{
	db->exec(sql);
	return true;	
}

QVariant psqlite::rexec(QString sql)
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
			DBG(qDebug() << "psqlite::rexec 1"<< sql << query.lastError() << query.lastError().number() << query.lastError().type());
		}
		
	}
	return "";
}

QVariant psqlite::rexec(QString itemName, QString sql)
{
	QSqlQuery query = db->exec(sql);
	if (query.next())
		return query.record().value(itemName);
	else
	{
		if (query.lastError().number() != -1)	// 没有对应的数据
		{
			DBG(qDebug() << "psqlite::rexec 1"<< sql << query.lastError() << query.lastError().number() << query.lastError().type());
		}
		return "";
	}
}

int  psqlite::lastInsertID()
{
    QString sql = "SELECT last_insert_rowid();";
    return rexec(sql).toInt();

}

void psqlite::printData(QString sql)
{
	qDebug() << "psqlite::printData" << sql;
	QSqlQuery query = db->exec(sql);
	while (query.next())
	{
		for (int i = 0; i < query.record().count(); i++)
		{
			qDebug() << query.record().fieldName(i) << query.record().value(i).toString();
		}
		qDebug() << endl;
	}
}
