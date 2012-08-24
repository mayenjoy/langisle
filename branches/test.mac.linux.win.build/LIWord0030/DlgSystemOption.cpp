#include "stdafx.h"
#include "DlgSystemOption.h"

#include "qframe.h"

#include "PageSOVoice.h"
#include "PageSOGeneral.h"
#include "PageSOPicture.h"
#include "PageSODict.h"
#include "PageSOAdvanced.h"
#include "PageSORemind.h"

#include "PageSOUser.h"
#include "pagesonetword.h"

DlgSystemOption::DlgSystemOption(QWidget *parent)
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
	QPushButton *okButton = new QPushButton("ȷ��");
	QPushButton *cancelButton = new QPushButton("ȡ��");

	bottomLayout->addWidget(okButton);
	bottomLayout->addWidget(cancelButton);

	lw->setMaximumWidth(120);
	resize(580, 435);

	lw->addItem("����");
	lw->addItem("����");
	lw->addItem("ͼƬ");
	lw->addItem("�ʵ�");
	
	lw->addItem("����");

	lw->addItem("����");
	//wuling
	lw->addItem("�û�");
	lw->addItem("�߼�");

	psog = new PageSOGeneral(this);
	psov = new PageSOVoice(this);
	psop = new PageSOPicture(this);
	psod = new PageSODict(this);
	psoa = new PageSOAdvanced(this);
	psor = new PageSORemind(this);
	//wuling
	psou = new PageSOUser(this);

	psnw = new PageSONetword(this);

	sw->addWidget(psog);
	sw->addWidget(psov);
	sw->addWidget(psop);
	sw->addWidget(psod);
	
	sw->addWidget(psor);

	sw->addWidget(psnw);
	//wuling
	sw->addWidget(psou);
	sw->addWidget(psoa);

	connect(lw, SIGNAL(currentRowChanged(int)), sw, SLOT(setCurrentIndex(int)));

	connect(okButton, SIGNAL(clicked()), this, SLOT(okClicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

DlgSystemOption::~DlgSystemOption()
{

}

void DlgSystemOption::okClicked()
{
	// ����
	psog->save();
	psov->save();
	psop->save();
	psod->save();
	psor->save();
	psoa->save();
	psnw->save();

	accept();
}
