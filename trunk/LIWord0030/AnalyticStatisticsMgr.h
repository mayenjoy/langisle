#pragma once

class QSqlDatabase;
class UDataDB;
class AnalyticStatisticsMgr
{
public:
	AnalyticStatisticsMgr(void);
	virtual ~AnalyticStatisticsMgr(void);

	int getUseDays(); // 学习开始至今的天数
	int getStudyCount(); // 学习过的单词数
	int getKnowWellCount(); // 掌握的单词数
	double getProgress(); // 学习进度

	QSqlDatabase *db;
	UDataDB *udb;
};
