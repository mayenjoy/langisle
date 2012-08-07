#include "stdafx.h"
#include "DlgSA.h"
#include "PageSADefault.h"
DlgSA::DlgSA(QWidget *parent)
	: QDialog(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	QHBoxLayout *topLayout = new QHBoxLayout;
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(bottomLayout);
	setLayout(mainLayout);

	QListWidget *lw = new QListWidget;
	QStackedWidget *sw = new QStackedWidget;
	sw->setFrameShape(QFrame::StyledPanel);
	//sw->setFrameStyle(QFrame::Plain);
	sw->setContentsMargins(-1, -1, -1, 1);
	// sw->setStyleSheet("border: 2px solid green;	border-radius: 4px;padding: 2px;");
	//sw->setStyleSheet("border-width:1px; border: 1px solid white;");



	topLayout->addWidget(lw);
	topLayout->addWidget(sw);
	bottomLayout->addStretch();	
	QPushButton *okButton = new QPushButton("确定");
	QPushButton *cancelButton = new QPushButton("取消");

	bottomLayout->addWidget(okButton);
	bottomLayout->addWidget(cancelButton);

	//lw->setMaximumWidth(120);
	lw->setFixedWidth(80);
	resize(720, 520);	// 800*600刚刚好

	lw->addItem("基本信息");
	PageSADefault *sad = new PageSADefault(this);
	sw->addWidget(sad);

	connect(lw, SIGNAL(currentRowChanged(int)), sw, SLOT(setCurrentIndex(int)));

	connect(okButton, SIGNAL(clicked()), this, SLOT(okClicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	setWindowTitle("统计分析");
}

DlgSA::~DlgSA()
{

}


void DlgSA::resizeEvent ( QResizeEvent * event )
{
	//DBG(qDebug() << "DlgSA::resizeEvent" << event->size());

}

void DlgSA::okClicked()
{
	// 保存
	accept();
}
