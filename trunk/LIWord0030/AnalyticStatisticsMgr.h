#pragma once

class QSqlDatabase;
class UDataDB;
class AnalyticStatisticsMgr
{
public:
	AnalyticStatisticsMgr(void);
	virtual ~AnalyticStatisticsMgr(void);

	int getUseDays(); // ѧϰ��ʼ���������
	int getStudyCount(); // ѧϰ���ĵ�����
	int getKnowWellCount(); // ���յĵ�����
	double getProgress(); // ѧϰ����

	QSqlDatabase *db;
	UDataDB *udb;
};
