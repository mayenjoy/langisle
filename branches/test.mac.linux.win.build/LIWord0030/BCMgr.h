#ifndef CCMGR_H
#define CCMGR_H

#include <QObject>
#include "OBCDBMgr.h"
#include "UBCDBMgr.h"
#include "MBCDBMgr.h"
#include "UDataDB.h"
#include "RemindMgr.h"
#define UserRoleCID (Qt::UserRole + 1)	// 分类ID(数据库中的)

class LIConfig;
// 课本与分类管理
class BCMgr : public QObject
{
	Q_OBJECT
public:

public:
	BCMgr(const QString &userName, QObject *parent);
	~BCMgr();

	QTreeWidgetItem * addOTreeItem(QTreeWidget *tree);	// 加入官方分类到树中
	QTreeWidgetItem * addUTreeItem(QTreeWidget *tree);	// 加入用户分类到树中
	QTreeWidgetItem * addMTreeItem(QTreeWidget *tree);	// 加入我的课本分类到树中
	QTreeWidgetItem *insertRootItem(QTreeWidget *tree, const QString itemName, categoryType type);	// 插入一个根分类

	//void setTreeTable(QTreeWidget *tree, QTableView *table)
	//{
	//	categoryTree = tree;
	//	bookTable = table;
	//}
	// 是否显示学习按钮
	bool isShowStudyButton();

	// 是否显示搜索栏
	bool isShowSearchBar();
	bool isCanAddUCategory();	// 显示增加分类按钮
	bool isCanModifyUCategory();	// 显示修改分类按钮
	bool isCanAddUBook();	// 显示增加课本按钮	
	bool isCanDelCategory();	// 显示删除分类按钮
	bool isShowEdit();
	bool isCUser();	// 当前分类为用户类型
	bool isShowDelButton();
	bool isCategoryExist(const QString &cname);	// 当前项目父分类下的某个分类是否存在
	bool isShowDownButton();
	bool isShowStudyMgrButton();//是否显示学习管理菜单项
	// 获取节点分类类型
	categoryType getCType(QTreeWidgetItem * item);
	// 获取节点分类ID
	int getCID(QTreeWidgetItem * item);
	// 获取节点分类类型
	categoryType getCType()	
	{
		return curtype;
	};
	// 获取节点分类ID
	int getCID()		
	{
		return curcid;
	};

	// 获取当前节点分类名
	QString getCurrentCategoryName();

	// 获取当前分类路径
	QStringList getCurCategoryPath();

	// 获取当前课程名
	QString getCurrentBookName();

	//////////////////////////////////////////////////////////////////////////
	// 通过模型获取信息
	// 获取课程id
	int getBidByRow(int row);
	// 获取课程名
	QString getBookNameByRow(int row);
	// 获取说明信息
	QString getBookCommentByRow(int row);

	// 获取单词数
	int getBookWCountByRow(int row);
	// 获取选择的课本类型和id type+bid
	QList<BOOKINFO> getSelectBook();
	//////////////////////////////////////////////////////////////////////////

	// 更新课本是否存在状态
	void updateAllBookExistState();
	// 获取当前课本行号
	int getCurrentBookRow();

	// 更新当前学习课本列表
	void updateBookList(QModelIndexList &ml);

	// 获取用户数据库对象
	UDataDB *getUDataDB()
	{
		return udatadb;
	};

	// 获取课程路径
	QString getBookPath(bookType type, int bid);
	// 
	// 获取登录用户名

	// 指定的书本是否存在
	bool isBookExist(bookType bt, int bid);

	// 更新指定书本的是否存在状态
	void updateBookExistState(bookType bt, int bid);

	UBCDBMgr *getUbcdb() {return ubcdb;};
	MBCDBMgr *getMybcdb() {return mbcdb;};
	int addURootCategory(const QString &cname);
	int addUBook(const QString &bname, int cid);

	LIConfig *getConfig() {return config;};
	RemindMgr *getRemindMgr() {return m_remindMgr;};

	//获得当前用户
	QString getCurUserName(){return m_userName;};




public slots:
	// 更新当前分类
	void updateCurrentCategory(QTreeWidgetItem * current);
	void updateCurrentBook(const QModelIndex & current);
	void fillBookToTable(QTableView *table);
	int addCategory(const QString &cname);
	void delCategory();
	void modifyCategory(const QString &cname);
	bool addBook(const QString &bname);
	void search(const QString &text, QTableView *table);
protected:
	// 获取树结点
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
