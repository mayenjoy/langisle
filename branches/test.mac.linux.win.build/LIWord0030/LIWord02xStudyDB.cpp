#include "stdafx.h"
#include "LIWord02xStudyDB.h"

LIWord02xStudyDB::LIWord02xStudyDB(const QString &dbpath, const QString &connName): LISqlite(dbpath, connName)
{
}

LIWord02xStudyDB::~LIWord02xStudyDB(void)
{
}

QList<WordInfo> LIWord02xStudyDB::getWordInfoList(const QString &sdbPath)
{
	QList<WordInfo> result;
	db->exec(QString("ATTACH DATABASE '%1' as sdb").arg(sdbPath));
	QSqlQuery query(*db);
	query.prepare("select word, state, fp, times, lasttime from TSTUDYWORD left join sdb.TWORD as tword on tword.id = wid where STATE > 0 ");
	if (!query.exec())
	{
		DBG(qDebug() << "LIWord02xStudyDB::getWordInfoList faild" << query.lastError());
	}
	while (query.next())
	{
		WordInfo wi;
		wi["word"] = query.record().value("word");
		wi["state"] = query.record().value("state");
		wi["st"] = query.record().value("fp");
		wi["lst"] = query.record().value("LastTime");
		wi["sc"] = query.record().value("times");

		//DBG(qDebug() << wi);

		result << wi;
	}

	return result;
}