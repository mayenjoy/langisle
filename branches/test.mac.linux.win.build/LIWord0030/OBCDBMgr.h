#pragma once
#include "BCDBMgrBase.h"

// 官方课本分类数据库
class OBCDBMgr :
	public BCDBMgrBase
{
public:
	OBCDBMgr(const QString &dbpath, const QString &connName);
	virtual ~OBCDBMgr(void);
	// 更新所有课本是否存在状态
	void updateAllBookExistState();
	// 更新间个课本是否存在状态
	void updateBookExistState(int bid, bool state);
	// 课本是否存在
	bool isBookExist(int bid);

	// 课本对应的路径
	QString getBookPath(int bid);

	virtual BCDBModel *createModel();
};
