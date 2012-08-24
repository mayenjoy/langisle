#pragma once
#include "BCDBMgrBase.h"
#include "config.h"
enum EnumMyCategory
{
	ELearning = 1,	// ѧϰ��
	EKnowwell = 2   // ������
};

class MBCDBMgr :
	public BCDBMgrBase
{
public:
	MBCDBMgr(const QString &dbpath, const QString &connName);
	virtual ~MBCDBMgr(void);

	virtual void createTable();
	virtual void initData();

	int getBTypeByRow(int row);

	virtual BCDBModel *createModel();

	void updateWordCount(int bid, bookType bt, int wcount);

	void delBook(int bid, bookType bt);

	int getSBookCount();
};
