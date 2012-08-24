#pragma once
#include "../comm/LISqlite.h"

#include "config.h"
#include "MBCDBMgr.h"
#include <QDateTime>

enum EnumGWOrder 
{
	EGWabc = 0, // 顺序
	EGWcba = 1, // 倒序
	EGWbac = 2, // 乱序
};

enum EnumREVWOrder // 复习单词顺序
{
	RVWabc = 0, // 顺序
	RVWbac = 2, // 乱序
	
};

class UDataDB :
	public LISqlite
{
public:
	UDataDB(const QString &dbpath, const QString &connName);
	virtual ~UDataDB(void);

	void init();
	void uninit();

	void loadDict(int dbname);//添加关联数据库

	void importStudyInfo(QList<BOOKINFO> &bil);
	void importStudyInfo(BOOKINFO &bi);

	//////////////////////////////////////////////////////////////////////////
	// 专入v02X
	void updateMDataFrom02XSData(QList<WordInfo> &wil);
	int getMidByDictWord(const QString &word);
	int getMidByUWord(const QString &word);
	void updateWordMInfo2(WordInfo &winfo);
	//////////////////////////////////////////////////////////////////////////
	WordInfo getWordInfoByMid(int mid);
	QString getCurrentBookInfo(); // 获取当前课本信息,用于显示
	int getWantReviewMID(); // 获取需要复习的单词id,没有返回0
	int getWantReviewMID(EnumREVWOrder RVWOrder); // 获取需要复习的单词id,没有返回0
	int getNotLearnMID(EnumGWOrder GWRandom = EGWabc); // 获取未学习的单词id,没有返回0
	int getStudyDataWordCount();	// 获取当前学习的单词总数
	void updateWordMInfo(WordInfo &winfo);	// 更新单词记忆信息
	void setConfigInfo(const QString &name, const QString &value);
	QString getConfigInfo(const QString &name);
	int getNextFP(int CFP); // CFP当前的遗忘点,下一个遗忘点
	double getNowFP(int CFP); // 获取st在遗忘点中的位置,即学习进度
	int getMaxFP(); // 获取最大的遗忘点

	void updateWordLearnLog(QMap<QString, QVariant> &tval);

	int getGreaterThanStCount(int st);	// 获取时间间隔大于给定值的单词个数
	int getLEThanStCount(int st); // 获取时间间隔小于等于给定值的单词个数
	EnumMyCategory getBookCid(const QString &bookTableName, int maxst); // 根据最大遗忘间隔判断某个词库是学习中,还是已掌握
	void updateMBCid(int bid, bookType bt, int cid); // 更新我的词库cid


	int getWcount(const QString &bookTableName);//单词总数
	//void updateWcount(int bid, bookType bt, int scount);

	int getSCount(const QString &bookTableName); // 已学单词数
	void updateSCount(int bid, bookType bt, int scount);

	int getKNCount(const QString &bookTableName, int maxst); // 已掌握单词数
	void updateKNCount(int bid, bookType bt, int kncount);

	void updateProgress(int bid, bookType bt, float progress);
	float getProgress(const QString &bookTableName,int maxst);
	void updateStudyProgress(); // 更新学习进度信息
	void updateBookStudyProgress(int bid, bookType bt); // 更新学习进度信息

	void updateStudyDBInfo(int bid, bookType bt);	// 用户课程加入单词后,学习过的词库需要更新学习信息,同步修改后的词库

	bool isStudying(int bid, bookType bt);	// 该课程是在学习中吗
	void deleteBookData(const QString &tableName);
	void deleteStudyDBInfo(int bid, bookType bt, bool bClearBookStudyInfo);

	int getBookReviewCount(int bid, bookType bt); // 给定课程待复习单词数,-1失败
	int getReviewCount();	// 当前需要复习的单词数

	void initFP(); // 初始化遗忘点

	QDateTime getMaxLst(); // 最后学习时间

	void getUserBookBookListInfo(QList<QVariant> &info); // 用户词库列表

	int getWidByDictWord(const QString &word);
	WordInfo getWordInfoByWid(int wid);

	// 清理挂起的单词 需要实现一个清理功能,在记忆库中删除所有课程中都没有的单词
	void cleanSuspendWord();

	//清空一个单词的学习信息
	void clearBookStudyInfo(int mid);
	//将一个单词学习状态改成掌握
	void updateBookStudyInfoknown(int mid);

	// 插入一次学习日志记录
	void insertTLog(QMap<QString, QVariant> &tlog); 

	int getTodaySWCount();	// 当天学习单词数
	int getTodaySNWCount();	// 当天新学单词数
	int getTodayRSCount();	// 当天复习单词数
	int getTodayKNCount();	// 当天掌握单词数
	int getTodaySTime();	// 当天学习时间

protected:
	

	void createTable();
	void createBookTable(const QString &tableName); // 建立课本表
	void CreateGroupTable(const QString &tableName); // 建立分组表
	void importBookData(const QString &tableName, const QString &bookDBPath); // 导出课本数据
	void importToUWord(); // 导出官方字典里不存在的词条到用户词库表UWord
	int importToMData(wordType wtype, int wid); // 导入单词到用户记忆库

	void insertToMyBook(BOOKINFO &bookInfo, QString &tableName);	// 插入课本到到我的课本分类中

	void createMemoryMDataTable();
	void insertCurrentStudyData(const QString &bookTableName);	// 插入当前学习数据	
	void insertCurrentGroupData(const QString &groupTableName);

	void syncMData();	// 同步学习记到文件数据库中
	QString getSens(int wid); // 获取例句

	QList<BOOKINFO> getAllStudyBookInfo();

	QString getTableName(int bid, bookType bt);

	// 设置给定课程全部单词为未学习状态
	void clearBookStudyInfo(int bid, bookType bt);

	bool getAllBookTableName(QStringList &sl);
	
	void createTLogTable(); // 建立每次学习日志表
	void synDSLog(); // 从内存表同步dslog

public:
	QList<QVariant> getAllWordList();	// 获取用于学习管理里显示的单词列表
protected:
	QList<BOOKINFO> m_biList;
	QList<int> m_fpList;
	int maxfp;
	QDateTime m_lstMax;
};
