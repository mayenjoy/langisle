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
	void currentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous);	// ����ı�
	//void currentRowChanged (const QModelIndex & current, const QModelIndex & previous);	// �α��ı�
	void bookSortIndicatorChanged (int logicalIndex, Qt::SortOrder order );	// ����ָʾ���ı�
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
	void addOrModifyCategory(const QString &title);	// ��ӻ��޸ķ���

	

protected:
	// ����
	QLineEdit *searchEdit;	// ������
	QPushButton *searchButton; // ������ť
	QLabel *labelCurPath;	// ��ǰ·��


	QTreeWidget *categoryTree;
	LITableView *bookTable;

	QPushButton *learnButton;
	QPushButton *delButton;
	
	QPushButton *addUserCategoryButton;
	QPushButton *delUserCategoryButton;
	QPushButton *modifyCategoryButton;
	QPushButton *addBookButton;
	QPushButton *editBookButton;
	// ��ϸ��Ϣ
	//TabBookDetail * bookDetail;

	//QTabWidget *tabDetail;
	QTreeWidgetItem * myitem;
};



#endif // PAGEMAINBOOK_H
