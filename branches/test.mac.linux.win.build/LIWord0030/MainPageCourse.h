#ifndef MAINPAGECOURSE_H
#define MAINPAGECOURSE_H

#include <QWidget>
#include "BCMgr.h"
class MainPageCourse : public QWidget
{
	Q_OBJECT

public:
	MainPageCourse(BCMgr *ccmgr, QWidget *parent);
	~MainPageCourse();


private slots:
	void learn();
	void addCategory();
	void delCategory();
	void modifyCategory();
	void addCourse();
private:
	QPushButton *createButton(const QString &text, const char *member);
protected:
	QTreeWidget *catecoryTree;
	QTableView *courseTable;

	QPushButton *learnButton;

	QPushButton *addUserCategoryButton;
	QPushButton *delUserCategoryButton;
	QPushButton *modifyCategoryButton;
	QPushButton *addCourseButton;


	BCMgr *m_ccmgr;
};

#endif // MAINPAGECOURSE_H
