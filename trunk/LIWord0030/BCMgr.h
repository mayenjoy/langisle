#ifndef CCMGR_H
#define CCMGR_H

#include <QObject>
#include "OBCDBMgr.h"
#include "UBCDBMgr.h"
#include "MBCDBMgr.h"
#include "UDataDB.h"
#include "RemindMgr.h"
#define UserRoleCID (Qt::UserRole + 1)	// ����ID(���ݿ��е�)

class LIConfig;
// �α���������
class BCMgr : public QObject
{
	Q_OBJECT
public:

public:
	BCMgr(const QString &userName, QObject *parent);
	~BCMgr();

	QTreeWidgetItem * addOTreeItem(QTreeWidget *tree);	// ����ٷ����ൽ����
	QTreeWidgetItem * addUTreeItem(QTreeWidget *tree);	// �����û����ൽ����
	QTreeWidgetItem * addMTreeItem(QTreeWidget *tree);	// �����ҵĿα����ൽ����
	QTreeWidgetItem *insertRootItem(QTreeWidget *tree, const QString itemName, categoryType type);	// ����һ��������

	//void setTreeTable(QTreeWidget *tree, QTableView *table)
	//{
	//	categoryTree = tree;
	//	bookTable = table;
	//}
	// �Ƿ���ʾѧϰ��ť
	bool isShowStudyButton();

	// �Ƿ���ʾ������
	bool isShowSearchBar();
	bool isCanAddUCategory();	// ��ʾ���ӷ��ఴť
	bool isCanModifyUCategory();	// ��ʾ�޸ķ��ఴť
	bool isCanAddUBook();	// ��ʾ���ӿα���ť	
	bool isCanDelCategory();	// ��ʾɾ�����ఴť
	bool isShowEdit();
	bool isCUser();	// ��ǰ����Ϊ�û�����
	bool isShowDelButton();
	bool isCategoryExist(const QString &cname);	// ��ǰ��Ŀ�������µ�ĳ�������Ƿ����
	bool isShowDownButton();
	bool isShowStudyMgrButton();//�Ƿ���ʾѧϰ����˵���
	// ��ȡ�ڵ��������
	categoryType getCType(QTreeWidgetItem * item);
	// ��ȡ�ڵ����ID
	int getCID(QTreeWidgetItem * item);
	// ��ȡ�ڵ��������
	categoryType getCType()	
	{
		return curtype;
	};
	// ��ȡ�ڵ����ID
	int getCID()		
	{
		return curcid;
	};

	// ��ȡ��ǰ�ڵ������
	QString getCurrentCategoryName();

	// ��ȡ��ǰ����·��
	QStringList getCurCategoryPath();

	// ��ȡ��ǰ�γ���
	QString getCurrentBookName();

	//////////////////////////////////////////////////////////////////////////
	// ͨ��ģ�ͻ�ȡ��Ϣ
	// ��ȡ�γ�id
	int getBidByRow(int row);
	// ��ȡ�γ���
	QString getBookNameByRow(int row);
	// ��ȡ˵����Ϣ
	QString getBookCommentByRow(int row);

	// ��ȡ������
	int getBookWCountByRow(int row);
	// ��ȡѡ��Ŀα����ͺ�id type+bid
	QList<BOOKINFO> getSelectBook();
	//////////////////////////////////////////////////////////////////////////

	// ���¿α��Ƿ����״̬
	void updateAllBookExistState();
	// ��ȡ��ǰ�α��к�
	int getCurrentBookRow();

	// ���µ�ǰѧϰ�α��б�
	void updateBookList(QModelIndexList &ml);

	// ��ȡ�û����ݿ����
	UDataDB *getUDataDB()
	{
		return udatadb;
	};

	// ��ȡ�γ�·��
	QString getBookPath(bookType type, int bid);
	// 
	// ��ȡ��¼�û���

	// ָ�����鱾�Ƿ����
	bool isBookExist(bookType bt, int bid);

	// ����ָ���鱾���Ƿ����״̬
	void updateBookExistState(bookType bt, int bid);

	UBCDBMgr *getUbcdb() {return ubcdb;};
	MBCDBMgr *getMybcdb() {return mbcdb;};
	int addURootCategory(const QString &cname);
	int addUBook(const QString &bname, int cid);

	LIConfig *getConfig() {return config;};
	RemindMgr *getRemindMgr() {return m_remindMgr;};

	//��õ�ǰ�û�
	QString getCurUserName(){return m_userName;};




public slots:
	// ���µ�ǰ����
	void updateCurrentCategory(QTreeWidgetItem * current);
	void updateCurrentBook(const QModelIndex & current);
	void fillBookToTable(QTableView *table);
	int addCategory(const QString &cname);
	void delCategory();
	void modifyCategory(const QString &cname);
	bool addBook(const QString &bname);
	void search(const QString &text, QTableView *table);
protected:
	// ��ȡ�����
	QTreeWidgetItem *getTreeWidgetItemByID(QTreeWidget *tree, int cid, categoryType type);
	
protected:
	OBCDBMgr *obcdb;
	UBCDBMgr *ubcdb;
	MBCDBMgr *mbcdb;
	UDataDB *udatadb;

	int curcid;
	int curbid;
	categoryType curtype;
	QTreeWidgetItem * curitem;
	BCDBMgrBase *curbcdb;
	QTreeWidgetItem *uRootItem;
	RemindMgr *m_remindMgr;
	//QTreeWidget *categoryTree;
	//QTableView *bookTable;

	QModelIndexList cml;
private:
	QString m_userName;
	QStringList fieldsHide;
	LIConfig *config;
};

extern BCMgr *g_pbcmgr;
#endif // CCMGR_H
