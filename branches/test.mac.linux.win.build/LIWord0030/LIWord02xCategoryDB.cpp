#include "stdafx.h"
#include "LIWord02xCategoryDB.h"

LIWord02xCategoryDB::LIWord02xCategoryDB(const QString &dbpath, const QString &connName): LISqlite(dbpath, connName)
{
}

LIWord02xCategoryDB::~LIWord02xCategoryDB(void)
{
}

QString LIWord02xCategoryDB::getBookName(int mid)
{
	QSqlQuery query(*db);
	query.prepare("select name from TCourse where mid = :mid");
	query.bindValue(":mid", mid);

	if (!query.exec())
	{
		DBG(qDebug() << "LIWord02xCategoryDB::getBookName" << query.lastError());
	}
	query.next();
	return query.record().value(0).toString();
}