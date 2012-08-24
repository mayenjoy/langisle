#ifndef PAGEMAINBOOK_H
#define PAGEMAINBOOK_H

#include <QtGui>
#include "BCMgr.h"
#include "LITreeWidget.h"
#include "TabBookDetail.h"

class LITableView;
class PageMainBook : public QWidget
{
	Q_OBJECT

public:
	PageMainBook(QWidget *parent);
	~PageMainBook();

	void endlearn();
	void update();

	void learnAllBook();

	int getSelSize();
	void selAll();
	int getBookCount();
signals:
	void slearn();
	void  uptoolbarState();
	void upASInfo();

public slots:
	void currentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous);	// 分类改变
	//void currentRowChanged (const QModelIndex & current, const QModelIndex & previous);	// 课本改变
	void bookSortIndicatorChanged (int logicalIndex, Qt::SortOrder order );	// 排序指示器改变
	void selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );

	void learn();
	void del();
	void down();
	void editBook();
	
	void addCategory();
	void delCategory();
	void modifyCategory();
	void addBook();
	void search();
	void tableDoubleClicked( const QModelIndex & index );
	void tableActivated ( const QModelIndex & index );
	
	void bookTableContextMenu ( const QPoint & pos );
	void categoryTreeContextMenu ( const QPoint & pos );
private:
	QPushButton *createButton(const QString &text, const char *member);
	void addOrModifyCategory(const QString &title);	// 添加或修改分类

	

protected:
	// 顶部
	QLineEdit *searchEdit;	// 搜索框
	QPushButton *searchButton; // 搜索按钮
	QLabel *labelCurPath;	// 当前路径


	QTreeWidget *categoryTree;
	LITableView *bookTable;

	QPushButton *learnButton;
	QPushButton *delButton;
	
	QPushButton *addUserCategoryButton;
	QPushButton *delUserCategoryButton;
	QPushButton *modifyCategoryButton;
	QPushButton *addBookButton;
	QPushButton *editBookButton;
	// 详细信息
	//TabBookDetail * bookDetail;

	//QTabWidget *tabDetail;
	QTreeWidgetItem * myitem;
};



#endif // PAGEMAINBOOK_H
