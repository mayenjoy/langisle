#include "stdafx.h"
#include "AnalyticStatisticsMgr.h"
#include "BCMgr.h"
AnalyticStatisticsMgr::AnalyticStatisticsMgr(void)
{
	udb = g_pbcmgr->getUDataDB();
	db = udb->getdb();
}

AnalyticStatisticsMgr::~AnalyticStatisticsMgr(void)
{
}

int AnalyticStatisticsMgr::getUseDays()
{
	//select julianday() - julianday(min(createTime)) from mybook 
	QSqlQuery query(*db);
	if (!query.exec("select julianday() - julianday(min(createTime)) from mybook "))
	{
		DBG(qDebug() << "AnalyticStatisticsMgr::getUseDays faild ." << query.lastError());
		return 0;
	}

	query.next();
	return query.record().value(0).toInt() + 1;
}

int AnalyticStatisticsMgr::getStudyCount()
{
	QSqlQuery query(*db);
	if (!query.exec("select count(mid) from mdata where st is not null"))
	{
		DBG(qDebug() << "AnalyticStatisticsMgr::getStudyCount faild ." << query.lastError());
		return 0;
	}

	query.next();
	return query.record().value(0).toInt();
}

int AnalyticStatisticsMgr::getKnowWellCount()
{
	QSqlQuery query(*db);

	QString sql = QString("SELECT count(mid) FROM mdata where st >= %1").arg(udb->getMaxFP());
	query.prepare(sql);
	if (!query.exec())
	{
		DBG(qDebug() << "AnalyticStatisticsMgr::getKnowWellCount faild." << query.lastError());
	}

	query.next();

	return query.record().value(0).toInt();
}

double AnalyticStatisticsMgr::getProgress()
{
	int maxst = udb->getMaxFP();
	QSqlQuery query(*db);
	QString sql = QString("SELECT sum(case when st > %1 then %2 else st end) * 100.0 / (%3 * count(mid)) FROM mdata")
						  .arg(maxst)
						  .arg(maxst)
						  .arg(maxst);

	DBG(qDebug() << sql);

	query.prepare(sql);
	if (!query.exec())
	{
		DBG(qDebug() << "AnalyticStatisticsMgr::getProgress faild." << query.lastError());
	}

	query.next();

	return  query.record().value(0).toDouble();
}
