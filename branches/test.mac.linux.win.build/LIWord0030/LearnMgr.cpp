#include "stdafx.h"
#include "LearnMgr.h"
#include "BCMgr.h"
#include "GlobalVal.h"
#include "PictureMgr.h"
#include "LIConfig.h"
#include "ThemeMgr.h"
#include "LIWord0030.h"

LearnMgr::LearnMgr(QObject *parent) : QObject(parent)
{
	cbid = 0;
	bdb = NULL;
	pudb = g_pbcmgr->getUDataDB();
	pudb->init();
	pLM = new LMFree(pudb);
	pSpeakMgr = GlobalVal::s_pSpeakMgr;
	pSpeakMgr->reset();
	GlobalVal::s_pPm->reset();
	timerID = startTimer(1000);

	
	todayTSCount = pudb->getTodaySWCount();
	todayNSCount = pudb->getTodaySNWCount(); //当日新学单词数
	todayRVCount = pudb->getTodayRSCount();
	todayKNCount = pudb->getTodayKNCount();
	todaySTime = pudb->getTodaySTime();
	DBG(qDebug() << "LearnMgr::LearnMgr 1" << todaySTime);
	//setGWRandom(IsGWRandom().toBool());

	//fullScreen(g_pbcmgr->getConfig()->getValue("LearnMgr/fullScreen", false).toBool());
}

LearnMgr::~LearnMgr(void)
{
	DBG(QDateTime   start   =   QDateTime::currentDateTime());
	pudb->uninit();

	insertTLog();

	if (bdb) delete bdb;

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	killTimer(timerID);
	
	pSpeakMgr->stop();
	DBG(qDebug() << "LearnMgr::~LearnMgr 耗时1" << start.msecsTo(QDateTime::currentDateTime()));
	

	QApplication::restoreOverrideCursor();

	delete pLM;
	GlobalVal::s_pmw->showNormal();
	DBG(qDebug() << "LearnMgr::~LearnMgr 耗时" << start.msecsTo(QDateTime::currentDateTime()));
}

void LearnMgr::init()
{
	QMap<QString, QVariant> params;
	params["Name"] = pudb->getCurrentBookInfo();
	emit updateCurBookInfo(params);

}

int LearnMgr::getNextWord()
{
	int mid = pLM->getNextWord();
	if (mid <= 0)
	{
		switch (mid)
		{
		case -7:
			// 恭喜您,当前词库您已经全部掌握了!
			QMessageBox::information((QWidget *)parent(), "提示", "恭喜您,当前词库您已经全部掌握了!", QMessageBox::Ok);

			break;
		case -3:
			QMessageBox::information((QWidget *)parent(), "提示", "当前没有需要学习的单词了,您可以学习其它词库或休息一会!", QMessageBox::Ok);
			break;
		default:
			QString info = QString("取词致命错误,错误代码%1").arg(mid);
			QMessageBox::critical((QWidget *)parent(), "错误", info);
			// DBG(qDebug() << "没有旧单词需要复习, 也没有新单词可学");
		}
	}
	
	return mid;
}

QMap<QString, QVariant> LearnMgr::getCWordInfo()
{	
	return pLM->wordInfo();
}

QMap<QString, QVariant> LearnMgr::getCurrentLearnBookInfo()
{
	QMap<QString, QVariant> result;
	result["Name"] = pudb->getCurrentBookInfo();
	return result;
}




void LearnMgr::testSignal()
{
	QMap<QString, QVariant> result;
	result["Name"] = "testSignal";
	emit updateCurBookInfo(result);
}

void LearnMgr::logOut(QString slog)
{
	DBG(qDebug() << slog);
}

void LearnMgr::speak(const QString &text)
{
	pSpeakMgr->speak(text);
}

void LearnMgr::updateMInfo(QMap<QString, QVariant> tval)
{
	pLM->updateMInfo(tval);
// 	DBG(qDebug() << "judgetime" << tval["judgetime"]);
// 	DBG(qDebug() << "affirmtime" << tval["affirmtime"]);
// 	DBG(qDebug() << "studytime" << tval["studytime"]);
// 	DBG(qDebug() << "judgeodletime" << tval["judgeodletime"]);`````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````
// 	DBG(qDebug() << "affirmodletime" << tval["affirmodletime"]);
// 	DBG(qDebug() << "studyodletime" << tval["studyodletime"]);
// 	DBG(qDebug() << "selJudge" << tval["selJudge"]);
// 	DBG(qDebug() << "selAffirm" << tval["selAffirm"]);

// 	WordInfo &wi = pLM->wordInfo();
// 
// 
// 	wi["st"] = 
// 
// 	winfo["score"] = winfo["score"].toInt() + 1;
// 
// 	pudb->updateWordMInfo(wi);
	// DBG(qDebug() << tval);
}


void LearnMgr::timerEvent(QTimerEvent *te)
{
	QObject::timerEvent(te);
	QMap<QString, QVariant> params;
	QString strSTime;
	int stime =  pLM->getSTime() / 1000  + todaySTime;

	int m = stime / 60;
/*	if (m != 0)*/
		strSTime += QString("%1分钟").arg(m);
	int s = stime % 60;
	strSTime += QString().sprintf("%02d秒", s);
	
	
	params["stime"] = strSTime;
	emit updateSTime(params);
	
}

QMap<QString, QVariant> LearnMgr::getCSTATSInfo()
{
	QMap<QString, QVariant> result;
	result["tscount"] = pLM->getTSCount() + todayTSCount;
	result["nscount"] = pLM->getNSCount() + todayNSCount;
	result["rvcount"] = pLM->getRVCount() + todayRVCount;
	result["kncount"] = pLM->getKNCount() + todayKNCount;
	result["sspeed"] = pLM->getSSpeed();
	return result;
}

QMap<QString, QVariant> LearnMgr::getDictInfo()
{
	QMap<QString, QVariant> result;
	LISTMAPINFO lmi = g_pbcmgr->getConfig()->getDictInfo();
	int count = 0;
	for (int i = 0; i < lmi.size(); i++)
	{
		if (!lmi.at(i)["used"].toBool())
			continue;
		result[QString("%1,name").arg(count)] = lmi.at(i)["name"];
		result[QString("%1,addr").arg(count)] = lmi.at(i)["addr"];
		count++;
	}
	result["count"] = count;
	return result;
}

QString LearnMgr::getConfigInfo(const QString &key, const QString &dvalue)
{
	return g_pbcmgr->getConfig()->getValue(key, dvalue).toString();
}

void LearnMgr::setConfigInfo(const QString &key, const QString &value)
{
	g_pbcmgr->getConfig()->setValue(key, value);
}


bool LearnMgr::getConfigInfoBool(const QString &key, const bool &dvalue)
{
	return g_pbcmgr->getConfig()->getValue(key, dvalue).toBool();
}

void LearnMgr::setConfigInfoBool(const QString &key, const bool &value)
{
	g_pbcmgr->getConfig()->setValue(key, value);
}

QVariant LearnMgr::getThemeList()
{
// 	QMap<QString, QVariant> result;
// 	result["ThemeList"] =  GlobalVal::s_ptm->getThemeList();
// 	return result;
	return GlobalVal::s_ptm->getThemeList();;
}

QVariant LearnMgr::getFileData(QVariant filePath)
{
	QVariant result;
	QString path = Global::makePathStr(filePath.toString());
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return "";
	QTextStream in(&file);
	in.setCodec(QTextCodec::codecForName("utf-8"));
	result = in.readAll();
	file.close();
	return result;
}




QVariant LearnMgr::IsGWRandom()
{
	return g_pbcmgr->getConfig()->getGWRandom();
}
// void LearnMgr::setGWOrder(EnumGWOrder GWOrder)
// {
// 	pLM->setGWRandom(bRandom.toBool());
// 	g_pbcmgr->getConfig()->setGWRandom(bRandom.toBool());
// }


void LearnMgr::linkClicked(const QUrl & url)
{
	
// 	QString suf = QFileInfo(url.path()).suffix();
// 	if (suf == "mp3" || suf == "wav")
// 	{
// 		DBG(qDebug() << "LearnMgr::linkClicked" << url);
// 		pSpeakMgr->m_pWebSpeak->speakUrl(url);
// 	}
}



QString LearnMgr::getReplaytimes()
{
	return g_pbcmgr->getConfig()->getValue("LearnMgr/Replaytimes", "1").toString();
}
void LearnMgr::setReplaytimes(const QString &sTimes)
{
	pSpeakMgr->setLoopTimes(sTimes.toInt());
	g_pbcmgr->getConfig()->setValue("LearnMgr/Replaytimes", sTimes);
}

QString LearnMgr::getReplaySleep()
{
	return g_pbcmgr->getConfig()->getValue("LearnMgr/ReplaySleep", "200").toString();
}
void LearnMgr::setReplaySleep(const QString &sSleep)
{
	pSpeakMgr->setSleep(sSleep.toInt());
	g_pbcmgr->getConfig()->setValue("LearnMgr/ReplaySleep", sSleep);
}



void LearnMgr::fullScreen(QVariant bTrue)
{
	//DBG(qDebug() << "LearnMgr::fullScreen" << bTrue.toBool());
	g_pbcmgr->getConfig()->setValue("LearnMgr/fullScreen", bTrue.toBool());
	if (bTrue.toBool())
	{
		GlobalVal::s_pmw->showFullScreen();
	}
	else
	{
		GlobalVal::s_pmw->showNormal();
	}
}

QList<QVariant> LearnMgr::getNewWordBookList()
{
	QList<QVariant> result;
	pudb->getUserBookBookListInfo(result);
	return result;	
}

QVariant LearnMgr::getWordInfo(const QString &word)
{
	int wid = pudb->getWidByDictWord(word);
	if (wid > 0)
	{
		return pudb->getWordInfoByWid(wid);
	}
	return WordInfo();
}

bool LearnMgr::addWordToBook(int bid, const QString &word, const QString &mean)
{
	QString path = g_pbcmgr->getBookPath(btUser, bid);
	if (cbid != bid || bdb == NULL)
	{
		if (bdb) delete bdb;
		bdb = new BookDB(path, path);
		
	}
	if (bdb->isWordExist(word))
	{
		return false;
	}

	bdb->addEditWord(word, mean);
	g_pbcmgr->getUbcdb()->updateWordCount(bid, bdb->wordCount());
	g_pbcmgr->getMybcdb()->updateWordCount(bid, btUser, bdb->wordCount());
	return true;
}

void LearnMgr::setOption(const QString &optionName, const QString &val)
{
	if ("gWordOrder" == optionName) 
	{
		DBG(qDebug() << "LearnMgr::setOption" << val);
		if (val == "a_b_c")
		{
			pLM->setGWGWOrder(EGWabc);
		}
		else if (val == "c_b_a")
		{
			pLM->setGWGWOrder(EGWcba);
		}
		else if (val == "b_a_c")
		{
			pLM->setGWGWOrder(EGWbac);
		}
	}
	if ("rvWordOrder" == optionName)
	{
		DBG(qDebug() << "LearnMgr::setOption 2" << val);
		if (val == "a_b_c")
		{
			pLM->setRVWOrder(RVWabc);
		}
		else if (val == "b_a_c")
		{
			pLM->setRVWOrder(RVWbac);
		}
	}
}

void LearnMgr::insertTLog()
{
	DBG(qDebug() << "begintime" << pLM->getStartTime());
	DBG(qDebug() << "endtime" << QDateTime::currentDateTime());
	DBG(qDebug() << "stime" << pLM->getStartTime().secsTo(QDateTime::currentDateTime()));
	DBG(qDebug() << "swcount" << pLM->getTSCount());
	DBG(qDebug() << "snwcount" << pLM->getNSCount());
	DBG(qDebug() << "rscount" << pLM->getRVCount());
	DBG(qDebug() << "kncount" << pLM->getKNCount());
	DBG(qDebug() << "sspeed" << pLM->getSSpeed());
	DBG(qDebug() << "stwcount" << asMgr.getStudyCount());
	DBG(qDebug() << "stnwcount" << asMgr.getKnowWellCount());
	QMap<QString, QVariant> tlog;
	QDateTime startTime = pLM->getStartTime();
	QDateTime curTime = QDateTime::currentDateTime();
	tlog["begintime"] = startTime;
	tlog["endtime"] = curTime;
	tlog["stime"] = startTime.secsTo(curTime);
	tlog["swcount"] = pLM->getTSCount();
	tlog["snwcount"] = pLM->getNSCount();
	tlog["rscount"] = pLM->getRVCount();
	tlog["kncount"] = pLM->getKNCount();
	tlog["sspeed"] = pLM->getSSpeed();
	tlog["stwcount"] = asMgr.getStudyCount();
	tlog["stnwcount"] = asMgr.getKnowWellCount();

	pudb->insertTLog(tlog);
}