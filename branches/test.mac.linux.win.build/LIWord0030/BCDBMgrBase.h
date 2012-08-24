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

	// 把分类填入树
	virtual void fillCagegoryToTree(QTreeWidgetItem *parentItem, int pid = 0);
	void fillCagegoryToTreeByTableName(const QString &tableName, QTreeWidgetItem *parentItem, int pid);
	// 把课本填入列表
	virtual void fillBookToTable(QTableView *table, int cid, const QString &searchText = "", bool bSelectRows = false/*多选 */);
	void fillBookToTableByTableName(const QString &tableName, QTableView *table, int cid, const QString &searchText = "");
	
	virtual BCDBModel *createModel();
	// 建立表
	virtual void createTable();

	// 增加一个分类, 返回分类id
	int addCategory(const QString &cname, int pid, int orderid = -1);

	// 获取当前的最大的orderid
	int getMaxOrderId();

	// 判断分类是否为2级分类(此分类不能再添加子分类)
	bool isSecondCategory(int cid);

	bool isBookExist(int cid, const QString &bname);

	// 给定父分类下的某个分类是否存在, 返回0已经存在 其它为新加入分类的cid
	bool isCategoryExist(int pid, const QString &cname);

	// 删除一个分类
	void removeCategory(int cid);

	// 修改分类名
	void modifyCagegory(const QString &ncName, int cid);

	// 获取分类名
	QString getCategoryNameByCid(int cid);

	//////////////////////////////////////////////////////////////////////////
	// 通过模型获取信息
	// 获取课程id
	int getBidByRow(int row);
	// 
	int getBidByName(const QString &bname, int cid);
	// 获取分类id
	int getCidbyCName(const QString &cname);
	// 获取课程名
	QString getBookNameByRow(int row);
	// 获取说明信息
	QString getBookCommentByRow(int row);
	// 获取单词数
	int getBookWCountByRow(int row);
	
	// 是否能删除分类
	bool isCanDeleteCategory(int cid); 
	// 是否有了分类
	bool isHaveSubCategory(int cid);
	// 是否存在分类课程
	bool isHaveCategoryBook(int cid);
	//////////////////////////////////////////////////////////////////////////

	// 获取行号
	int getRowByBID(int bid);

	void updateWordCount(int bid, int wcount);


	QList<BOOKINFO> getAllBookInfoByCid(int cid);

	BCDBModel *model;

	QString m_categoryTName;
	QString m_bookTName;
};
