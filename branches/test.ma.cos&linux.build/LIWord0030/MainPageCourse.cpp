#include "StdAfx.h"
#include "MainPageCourse.h"

MainPageCourse::MainPageCourse(BCMgr *ccmgr, QWidget *parent)
	: QWidget(parent), m_ccmgr(ccmgr)
{
	// �γ̷���
	catecoryTree = new QTreeWidget;
	catecoryTree->setHeaderHidden(true);

	// �γ��б�
	courseTable = new QTableView;

	// ��ť
	learnButton = new QPushButton("ѧϰ");
	addUserCategoryButton = createButton("��ӷ���", SLOT(addCategory()));
	addUserCategoryButton->hide();
	delUserCategoryButton = createButton("��ӷ���", SLOT(delCategory()));
	delUserCategoryButton->hide();
	modifyCategoryButton = createButton("�޸ķ���", SLOT(modifyCategory()));
	modifyCategoryButton->hide();
	addCourseButton = createButton("����γ�", SLOT(addCourse()));
	//addCourseButton->hide();

	QSplitter *split = new QSplitter;
	split->addWidget(catecoryTree);
	split->addWidget(courseTable);


	//// ����,�б�Layout
	//QHBoxLayout *catecoryAndCourseLayout = new QHBoxLayout;
	//catecoryAndCourseLayout->addWidget(catecoryTree);
	//catecoryAndCourseLayout->addWidget(courseTable);

	// ���水ťLayout
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
