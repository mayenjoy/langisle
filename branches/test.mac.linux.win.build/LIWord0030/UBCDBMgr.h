#pragma once
#include "BCDBMgrBase.h"
#include "UDataDB.h"
class UBCDBMgr :
	public BCDBMgrBase
{
public:
	UBCDBMgr(const QString &dbpath, const QString &connName);
	virtual ~UBCDBMgr(void);

	virtual void initData();

	virtual BCDBModel *createModel();

	// ����һ���ʿ�
	int addBook(const QString &cbook, int cid);

	void delBook(int bid);


};
