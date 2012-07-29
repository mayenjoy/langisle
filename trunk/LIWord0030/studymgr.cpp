#include "stdafx.h"
#include "studymgr.h"
#include "BCMgr.h"
StudyMgr::StudyMgr(QObject *parent)
	: QObject(parent)
{
	pudb = g_pbcmgr->getUDataDB();
	pudb->init();
}

StudyMgr::~StudyMgr()
{
	pudb->uninit();
}

QList<QVariant> StudyMgr::getTest()
{
	// 		QList<QVariant> rs;
	// 		QList<QVariant> one;
	// 		//[true,'hello','Ñ§Ï°ÖÐ','50%','4%','1999'],
	// 		one.push_back(true);
	// 		one.push_back("hello");
	// 		one.push_back("2");
	// 		one.push_back("50%");
	// 		one.push_back("4%");
	// 		one.push_back("1999");
	// 		rs.push_back(one);
	// 		rs.push_back(one);
	// 		rs.push_back(one);
	// 		return rs;
	return pudb->getAllWordList();
}

void StudyMgr::logOut(QString slog)
{
	DBG(qDebug() << slog);
}
