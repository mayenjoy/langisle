#include "stdafx.h"
#include "BookDB.h"

BookDB::BookDB(const QString &dbpath, const QString &connName) : LISqlite(dbpath, connName)
{
	if (!isTableExist("word"))	// 新建的数据库,创建表
	{
		this->createTable();
	}
	loadDictDB();
}  
 
BookDB::~BookDB(void)
{
	unloadDictDB();
}

void BookDB::createTable()
{
	db->transaction();

	exec("CREATE TABLE [word] (\
		[bwid] INTEGER PRIMARY KEY AUTOINCREMENT, \
		[wid] INTEGER, \
		[word] TEXT, \
		[mean] TEXT, \
		[gid] INTEGER);");

	exec("CREATE TABLE [grp] \
		(\
		[gid] INTEGER,\
		[gname] TEXT\
		);\
		");

	exec("CREATE TABLE [otherinfo] \
		(\
		[name] TEXT NOT NULL ON CONFLICT FAIL,\
		[value] TEXT\
		);\
		 ");
	exec("CREATE UNIQUE INDEX [unique_wid] ON [word] ([wid]);");
	exec("CREATE UNIQUE INDEX [unique_word] ON [word] ([word]);");

	if (!db->commit())
	{
		DBG(qDebug() << "BookDB::createTable" << db->lastError());
	}
}

void BookDB::unloadDictDB()
{
	QSqlQuery query(*db);
	QString sql = QString("DETACH DATABASE dict;");
	if (!query.exec(sql))
	{
		DBG(qDebug() << "BookDB::unloadDictDB faild." << sql << query.lastError());
	}
}

void BookDB::loadDictDB()
{
	QString dictdbpath = Global::makePathStr("/odata/dict.db");
	QSqlQuery query(*db);
	QString sql = QString("ATTACH DATABASE '%1' as dict;").arg(dictdbpath);
	if (!query.exec(sql))
	{
		DBG(qDebug() << "BookDB::loadDictDB faild." << sql << query.lastError());
	}
}

void BookDB::addEditWord(const QString &word, const QString &mean)
{
	int wid = getWidByDict(word);
	QSqlQuery query(*db);
	query.prepare("INSERT OR REPLACE INTO [word](wid, word, mean) values(:wid, :word, :mean)");
	query.bindValue(":word", word);
	query.bindValue(":mean", mean);
	query.bindValue(":wid", wid ? wid : QVariant(QVariant::Int));
	if (!query.exec())
	{
		DBG(qDebug() << "BookDB::addEditWord" << query.lastError());
	}
	return;
}

bool BookDB::isWordExist(const QString &word)
{
	QSqlQuery query(*db);
	query.prepare("select bwid from [word] where lower(word) = lower(:word)");
	query.bindValue(":word", word);
	if (!query.exec())
	{
		DBG(qDebug() << "BookDB::isWordExist" << query.lastError());
	}

	return query.next();
}



int BookDB::wordCount()
{
	QSqlQuery query(*db);
	query.prepare("select count(bwid) from [word]");
	if (!query.exec())
	{
		DBG(qDebug() << "BookDB::wordCount" << query.lastError());
	}
	query.next();
	return query.record().value(0).toInt();
}

void BookDB::delWord(int bwid)
{
	QSqlQuery query(*db);
	query.prepare("delete from [word] where bwid = :bwid");
	query.bindValue(":bwid", bwid);
	if (!query.exec())
	{
		DBG(qDebug() << "BookDB::delWord" << query.lastError());
	}
}

void BookDB::delAllWord()
{
	QSqlQuery query(*db);
	query.prepare("delete from [word]");

	if (!query.exec())
	{
		DBG(qDebug() << "BookDB::delAllWord" << query.lastError());
	}
}

int BookDB::getWidByDict(const QString &word)
{
	QSqlQuery query(*db);
	query.prepare("select wid from dict.[word] where word = :word");
	query.bindValue(":word", word);
	if (!query.exec())
	{
		DBG(qDebug() << "BookDB::getWidByDict" << query.lastError());
	}
	if (query.next())
	{
		return query.record().value(0).toInt();
	}
	return 0;
}

void BookDB::addEditWord(QList< QMap<QString, QString> > &wList)
{
	db->transaction();

	for (int i = 0; i < wList.size(); i++)
	{
		QMap<QString, QString> wInfo = wList.at(i);

		addEditWord(wInfo["word"], wInfo["trans"]);
	}


	db->commit();
}
