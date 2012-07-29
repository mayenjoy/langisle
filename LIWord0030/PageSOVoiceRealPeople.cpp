#include "stdafx.h"
#include "PageSOVoiceRealPeople.h"
#include "GlobalVal.h"
#include "LIConfig.h"
#include "SpeakMgr.h"
#include "BCMgr.h"
PageSOVoiceRealPeople::PageSOVoiceRealPeople(QWidget *parent)
	: PageSOVoiceNotTTSBase(parent)
{
	mainLayout->addWidget(new QLabel("ÕæÈËÓïÒô¿âÎ»ÖÃ:"));
	QHBoxLayout *layoutPath = new QHBoxLayout;
	editPath = new QLineEdit;
	editPath->setReadOnly(true);
	layoutPath->addWidget(editPath);
	QPushButton *buttonBrowerFile = new QPushButton("ä¯ÀÀ...");
	layoutPath->addWidget(buttonBrowerFile);
	mainLayout->addLayout(layoutPath);
	mainLayout->addStretch();

	connect(buttonBrowerFile, SIGNAL(clicked()), this, SLOT(browerPath()));

	editPath->setText(g_pbcmgr->getConfig()->getSpeakRealPeoplePath());
	checkBoxOnFailUserTTS->setChecked(g_pbcmgr->getConfig()->getSpeakRealPeopleOnFailUseTTS());
}

PageSOVoiceRealPeople::~PageSOVoiceRealPeople()
{

}

void PageSOVoiceRealPeople::browerPath()
{
	QString path = QFileDialog::getExistingDirectory(this,tr("ÇëÑ¡ÔñÕæÈËÓïÒô¿âÂ·¾¶:"));

	if (!path.isEmpty()) 
	{
		editPath->setText(QDir::convertSeparators(path));
		GlobalVal::s_pSpeakMgr->setRealPeoplePath(path);
	}
}

void PageSOVoiceRealPeople::save()
{
	g_pbcmgr->getConfig()->setSpeakRealPeopleOnFailUseTTS(checkBoxOnFailUserTTS->isChecked());
	g_pbcmgr->getConfig()->setSpeakRealPeoplePath(editPath->text());
}
