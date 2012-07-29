#pragma once
#include "../comm/LISqlite.h"
#include "BCDBModel.h"
#include <QTreeWidgetItem>
#include <QTableView>
#include "config.h"

class BCDBMgrBase :
	public LISqlite
{
public:
	BCDBMgrBase(const QString &dbpath, const QString &connName, const QString& categoryTName = "category", const QString &bookTName = "book");
	virtual ~BCDBMgrBase(void);

	// �ѷ���������
	virtual void fillCagegoryToTree(QTreeWidgetItem *parentItem, int pid = 0);
	void fillCagegoryToTreeByTableName(const QString &tableName, QTreeWidgetItem *parentItem, int pid);
	// �ѿα������б�
	virtual void fillBookToTable(QTableView *table, int cid, const QString &searchText = "", bool bSelectRows = false/*��ѡ */);
	void fillBookToTableByTableName(const QString &tableName, QTableView *table, int cid, const QString &searchText = "");
	
	virtual BCDBModel *createModel();
	// ������
	virtual void createTable();

	// ����һ������, ���ط���id
	int addCategory(const QString &cname, int pid, int orderid = -1);

	// ��ȡ��ǰ������orderid
	int getMaxOrderId();

	// �жϷ����Ƿ�Ϊ2������(�˷��಻��������ӷ���)
	bool isSecondCategory(int cid);

	bool isBookExist(int cid, const QString &bname);

	// �����������µ�ĳ�������Ƿ����, ����0�Ѿ����� ����Ϊ�¼�������cid
	bool isCategoryExist(int pid, const QString &cname);

	// ɾ��һ������
	void removeCategory(int cid);

	// �޸ķ�����
	void modifyCagegory(const QString &ncName, int cid);

	// ��ȡ������
	QString getCategoryNameByCid(int cid);

	//////////////////////////////////////////////////////////////////////////
	// ͨ��ģ�ͻ�ȡ��Ϣ
	// ��ȡ�γ�id
	int getBidByRow(int row);
	// 
	int getBidByName(const QString &bname, int cid);
	// ��ȡ����id
	int getCidbyCName(const QString &cname);
	// ��ȡ�γ���
	QString getBookNameByRow(int row);
	// ��ȡ˵����Ϣ
	QString getBookCommentByRow(int row);
	// ��ȡ������
	int getBookWCountByRow(int row);
	
	// �Ƿ���ɾ������
	bool isCanDeleteCategory(int cid); 
	// �Ƿ����˷���
	bool isHaveSubCategory(int cid);
	// �Ƿ���ڷ���γ�
	bool isHaveCategoryBook(int cid);
	//////////////////////////////////////////////////////////////////////////

	// ��ȡ�к�
	int getRowByBID(int bid);

	void updateWordCount(int bid, int wcount);


	QList<BOOKINFO> getAllBookInfoByCid(int cid);

	BCDBModel *model;

	QString m_categoryTName;
	QString m_bookTName;
};
