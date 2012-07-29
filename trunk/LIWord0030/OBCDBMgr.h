#pragma once
#include "BCDBMgrBase.h"

// �ٷ��α��������ݿ�
class OBCDBMgr :
	public BCDBMgrBase
{
public:
	OBCDBMgr(const QString &dbpath, const QString &connName);
	virtual ~OBCDBMgr(void);
	// �������пα��Ƿ����״̬
	void updateAllBookExistState();
	// ���¼���α��Ƿ����״̬
	void updateBookExistState(int bid, bool state);
	// �α��Ƿ����
	bool isBookExist(int bid);

	// �α���Ӧ��·��
	QString getBookPath(int bid);

	virtual BCDBModel *createModel();
};
