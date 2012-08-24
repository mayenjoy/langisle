#include "stdafx.h"
#include "PageSOPicture.h"
#include "GlobalVal.h"
#include "LIConfig.h"
#include "PictureMgr.h"
#include "BCMgr.h"
#include "PictureMgr.h"
#include <QPixmap>
PageSOPicture::PageSOPicture(QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);


	mainLayout->addWidget(new QLabel("图片位置:"));
	QHBoxLayout *layoutPath = new QHBoxLayout;
	editPath = new QLineEdit;
	editPath->setReadOnly(true);
	layoutPath->addWidget(editPath);
	QPushButton *buttonBrowerFile = new QPushButton("浏览...");
	lineTestWord = new QLineEdit("Happy", this);
	QPushButton *buttonTest = new QPushButton("测试");
	QHBoxLayout *layoutTest = new QHBoxLayout();

	picLabel = new QLabel();

	layoutTest->addWidget(lineTestWord);
	layoutTest->addWidget(buttonTest);

	layoutPath->addWidget(buttonBrowerFile);
	mainLayout->addLayout(layoutPath);
	mainLayout->addLayout(layoutTest);
	mainLayout->addWidget(picLabel);
	mainLayout->addStretch();

	connect(buttonBrowerFile, SIGNAL(clicked()), this, SLOT(browerPath()));
	connect(buttonTest, SIGNAL(clicked()), this, SLOT(showPic()));

	editPath->setText(g_pbcmgr->getConfig()->getPicturePath());
	
}

PageSOPicture::~PageSOPicture()
{

}

void PageSOPicture::browerPath()
{
	QString path = QFileDialog::getExistingDirectory(this,tr("请选择图片库路径:"));

	if (!path.isEmpty()) 
	{
		editPath->setText(QDir::convertSeparators(path));
		GlobalVal::s_pPm->setPath(path);
	}
}

void PageSOPicture::save()
{
	g_pbcmgr->getConfig()->setPicturePath(editPath->text());
}


void PageSOPicture::showPic()
{
	QString hash = QCryptographicHash::hash(lineTestWord->text().toLower().toUtf8(), QCryptographicHash::Md5).toHex().mid(8, 16).toLower();
	QString path = QDir::convertSeparators(editPath->text() + "/" + lineTestWord->text().at(0).toLower() + "/" + hash);
	if (QFile::exists(path))
	{
                picLabel->setPixmap(QPixmap(path));

	}
	else
	{
		QMessageBox::warning(this, "提示", QString("目标图片文件<b>%1</b>不存在,请检查图片路径设置.").arg(path));
	}
	

}

