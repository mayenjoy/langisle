#pragma once
#include "BCMgr.h"
#include "LMFree.h"
#include "SpeakMgr.h"
#include "ThemeMgr.h"
#include "BookDB.h"

#include "AnalyticStatisticsMgr.h"
class LearnMgr : public QObject
{
	Q_OBJECT

public:
	LearnMgr(QObject *parent = 0);
	virtual ~LearnMgr(void);

	void testSignal();
	void setHtmlWordBaseInfo();
public slots:
	void init();
	QMap<QString, QVariant> getCurrentLearnBookInfo();// 获取当前学习的课本信息,用于显示
	int getNextWord();
	QMap<QString, QVariant> getCWordInfo();// 获取当前学习的单词信息
	void logOut(QString slog);
	void updateMInfo(QMap<QString, QVariant> tval);
	void speak(const QString &text);
	QMap<QString, QVariant> getCSTATSInfo(); // 获取当前统计信息
	QMap<QString, QVariant> getDictInfo();	// 获取词典信息
	QString getConfigInfo(const QString &key, const QString &dvalue);
	void setConfigInfo(const QString &key, const QString &value);

	bool getConfigInfoBool(const QString &key, const bool &dvalue);
	void setConfigInfoBool(const QString &key, const bool &value);

	QVariant getThemeList();
	QVariant getFileData(QVariant filePath); // 读取文件内容
	QString getReplaytimes();
	void setReplaytimes(const QString &sTimes);
	QString getReplaySleep();
	void setReplaySleep(const QString &sSleep);
	void linkClicked(const QUrl & url);
	/*void setGWRandom(QVariant bRandom);*/
	QVariant IsGWRandom();
	void fullScreen(QVariant bTrue);
	QList<QVariant> getNewWordBookList(); // 获取生词本信息
	QVariant getWordInfo(const QString &word);
	bool addWordToBook(int bid, const QString &word, const QString &mean);

	void setOption(const QString &optionName, const QString &val);

	void insertTLog();	// 插入本次学习信息到到数据库
signals:
	void updateCurBookInfo(QMap<QString, QVariant> tval);
	void updateCurWordBaseInfo(QMap<QString, QVariant> tval);	
	void goback();
	void updateSTime(QMap<QString, QVariant> tval);	// 更新学习时间
protected:
	virtual void timerEvent(QTimerEvent *);
protected:
	UDataDB *pudb;
	LMBase *pLM;
	SpeakMgr *pSpeakMgr;
	int timerID;
	BookDB *bdb;
	int cbid;
	AnalyticStatisticsMgr asMgr;
	int todayTSCount;
	int todayNSCount;
	int todayRVCount;
	int todayKNCount;
	int todaySTime;
};
