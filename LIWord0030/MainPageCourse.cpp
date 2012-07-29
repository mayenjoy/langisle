#include "StdAfx.h"
#include "MainPageCourse.h"

MainPageCourse::MainPageCourse(BCMgr *ccmgr, QWidget *parent)
	: QWidget(parent), m_ccmgr(ccmgr)
{
	// 课程分类
	catecoryTree = new QTreeWidget;
	catecoryTree->setHeaderHidden(true);

	// 课程列表
	courseTable = new QTableView;

	// 按钮
	learnButton = new QPushButton("学习");
	addUserCategoryButton = createButton("添加分类", SLOT(addCategory()));
	addUserCategoryButton->hide();
	delUserCategoryButton = createButton("添加分类", SLOT(delCategory()));
	delUserCategoryButton->hide();
	modifyCategoryButton = createButton("修改分类", SLOT(modifyCategory()));
	modifyCategoryButton->hide();
	addCourseButton = createButton("导入课程", SLOT(addCourse()));
	//addCourseButton->hide();

	QSplitter *split = new QSplitter;
	split->addWidget(catecoryTree);
	split->addWidget(courseTable);


	//// 分类,列表Layout
	//QHBoxLayout *catecoryAndCourseLayout = new QHBoxLayout;
	//catecoryAndCourseLayout->addWidget(catecoryTree);
	//catecoryAndCourseLayout->addWidget(courseTable);

	// 下面按钮Layout
	QHBoxLayout *buttonLayout = new QHBoxLayout;	
	buttonLayout->addWidget(addUserCategoryButton);
	buttonLayout->addWidget(delUserCategoryButton);
	buttonLayout->addWidget(modifyCategoryButton);
	buttonLayout->addWidget(addCourseButton);
	buttonLayout->addStretch();	
	buttonLayout->addWidget(learnButton);


	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(split);
	mainLayout->addLayout(buttonLayout);
	setLayout(mainLayout);


	m_ccmgr->addOTreeItem(catecoryTree);

}

MainPageCourse::~MainPageCourse()
{

}

QPushButton *MainPageCourse::createButton(const QString &text, const char *member)
{
	QPushButton *button = new QPushButton(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

void MainPageCourse::learn()
{

}

void MainPageCourse::addCategory()
{

}

void MainPageCourse::delCategory()
{

}

void MainPageCourse::modifyCategory()
{

}

void MainPageCourse::addCourse()
{

}
