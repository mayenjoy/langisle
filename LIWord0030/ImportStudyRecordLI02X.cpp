#include "stdafx.h"
#include "ImportStudyRecordLI02X.h"
#include "BookDB.h"
#include "LIWord02xBookDB.h"
#include "LIWord02xStudyDB.h"
#include "BookConvertLI02x.h"
#include "LIWord02xCategoryDB.h"
#include "BCMgr.h"

ImportStudyRecordLI02X::ImportStudyRecordLI02X(void)
{
	typeName = "LIWord02XÑ§Ï°¼ÇÂ¼";
}

ImportStudyRecordLI02X::~ImportStudyRecordLI02X(void)
{
}

QStringList ImportStudyRecordLI02X::getAllStudyDBPath(const QString &path)
{
	QStringList files;
	QDir directory(path);

	if (!directory.exists()) 
	{
		return files;
	}

	files = directory.entryList(QStringList("*.ldb"), QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
	return files;
}

void ImportStudyRecordLI02X::importLI02XBookDBAndStudyData(const QString &sdbFName, const QString &sdbPath)
{
	DBG(qDebug() << "ImportStudyRecordLI02X::importLI02XBookDB 1" << sdbFName);
	QChar c = sdbFName.at(0);
	
	QDir dir(m_path);


	QString bookDBPath;

	if (c == 'u')
	{
		bookDBPath = dir.absoluteFilePath("../course");
	}
	else if (c == 'o')
	{
		bookDBPath = dir.absoluteFilePath("../../odata/course");
	}

	int mid = QFileInfo(sdbFName.mid(1)).completeBaseName().toInt();

	QString categorydbPath = QDir::convertSeparators(QDir::cleanPath(bookDBPath + "/categorycourse.db"));

	bookDBPath = QDir::convertSeparators(QDir::cleanPath(bookDBPath + "/" + QString::number(mid) + ".db"));
	
	DBG(qDebug() << "ImportStudyRecordLI02X::importLI02XBookDB" << bookDBPath << categorydbPath);

	LIWord02xCategoryDB cdb(categorydbPath, categorydbPath);
	//DBG(qDebug() << cdb.getBookName(mid));



	QString bname = cdb.getBookName(mid);
	int bid = g_pbcmgr->addUBook(bname, m_cid);
	if (!bid)
	{
		bid = g_pbcmgr->getUbcdb()->getBidByName(bname, m_cid);
	}

	QString path = g_pbcmgr->getBookPath(btUser, bid);
	

	BookDB bdb(path, path);
	BookConvertLI02x bcli;
	bcli.Convert(&bdb,bookDBPath);

	int wcount =  bdb.wordCount();



	BOOKINFO bi;
	bi["name"] = bname;
	bi["btype"] = btUser;
	bi["bid"] = bid;
	bi["wcount"] = wcount;
	
	g_pbcmgr->getUDataDB()->importStudyInfo(bi);


	LIWord02xStudyDB lsdb(sdbPath, sdbPath);
	QList<WordInfo> wil = lsdb.getWordInfoList(bookDBPath);
	
	g_pbcmgr->getUDataDB()->updateMDataFrom02XSData(wil);


	g_pbcmgr->getMybcdb()->updateWordCount(bid, btUser, wcount);
	g_pbcmgr->getUbcdb()->updateWordCount(bid, wcount);
	g_pbcmgr->getUDataDB()->updateStudyDBInfo(bid, btUser);


}

bool ImportStudyRecordLI02X::StartImport(UDataDB *udb, const QString &path)
{
	m_udb = udb;
	m_path = path;
	QDir dir(m_path);

	m_uName = dir.dirName();

	QString cname = "02x_" + m_uName;
	int cid = g_pbcmgr->addURootCategory(cname);
	if (!cid)
	{
		cid = g_pbcmgr->getUbcdb()->getCidbyCName(cname);
	}

	m_cid = cid;

	DBG(qDebug() << "ImportStudyRecordLI02X::StartImport" << path);
	QStringList files = getAllStudyDBPath(path);
	foreach (QString sfPath, files)
	{
		// ×ª»»´Ê¿â
		importLI02XBookDBAndStudyData(sfPath, QDir::convertSeparators(path + "/" + sfPath));

	}




	return true;
}

