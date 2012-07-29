#include "stdafx.h"
#include "LIWord02xBookDB.h"

LIWord02xBookDB::LIWord02xBookDB(const QString &dbpath, const QString &connName): LISqlite(dbpath, connName)
{
	
}

LIWord02xBookDB::~LIWord02xBookDB(void)
{
}

void LIWord02xBookDB::getAllWord(QList< QMap<QString, QString> > &wList)
{
	QSqlQuery query(*db);
	query.prepare("select word, trans from tword");
	if (!query.exec())
	{
		DBG(qDebug() << "LIWord02xBookDB::getAllWord" << query.lastError());
	}

	while (query.next())
	{
		QMap<QString, QString> winfo;
		winfo["word"] = query.record().value(0).toString();
		winfo["trans"] = query.record().value(1).toString();
		wList.push_back(winfo);
	}
		
}