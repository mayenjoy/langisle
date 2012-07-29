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

	// 增加一个词库
	int addBook(const QString &cbook, int cid);

	void delBook(int bid);


};
