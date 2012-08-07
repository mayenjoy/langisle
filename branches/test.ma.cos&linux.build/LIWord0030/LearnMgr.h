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
	QMap<QString, QVariant> getCurrentLearnBookInfo();// ��ȡ��ǰѧϰ�Ŀα���Ϣ,������ʾ
	int getNextWord();
	QMap<QString, QVariant> getCWordInfo();// ��ȡ��ǰѧϰ�ĵ�����Ϣ
	void logOut(QString slog);
	void updateMInfo(QMap<QString, QVariant> tval);
	void speak(const QString &text);
	QMap<QString, QVariant> getCSTATSInfo(); // ��ȡ��ǰͳ����Ϣ
	QMap<QString, QVariant> getDictInfo();	// ��ȡ�ʵ���Ϣ
	QString getConfigInfo(const QString &key, const QString &dvalue);
	void setConfigInfo(const QString &key, const QString &value);

	bool getConfigInfoBool(const QString &key, const bool &dvalue);
	void setConfigInfoBool(const QString &key, const bool &value);

	QVariant getThemeList();
	QVariant getFileData(QVariant filePath); // ��ȡ�ļ�����
	QString getReplaytimes();
	void setReplaytimes(const QString &sTimes);
	QString getReplaySleep();
	void setReplaySleep(const QString &sSleep);
	void linkClicked(const QUrl & url);
	/*void setGWRandom(QVariant bRandom);*/
	QVariant IsGWRandom();
	void fullScreen(QVariant bTrue);
	QList<QVariant> getNewWordBookList(); // ��ȡ���ʱ���Ϣ
	QVariant getWordInfo(const QString &word);
	bool addWordToBook(int bid, const QString &word, const QString &mean);

	void setOption(const QString &optionName, const QString &val);

	void insertTLog();	// ���뱾��ѧϰ��Ϣ�������ݿ�
signals:
	void updateCurBookInfo(QMap<QString, QVariant> tval);
	void updateCurWordBaseInfo(QMap<QString, QVariant> tval);	
	void goback();
	void updateSTime(QMap<QString, QVariant> tval);	// ����ѧϰʱ��
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
