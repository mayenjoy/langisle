#include "stdafx.h"
#include "PageSOVoiceTTS.h"
#include "GlobalVal.h"
#include "SpeakMgr.h"
#include "LIConfig.h"
#include "BCMgr.h"
PageSOVoiceTTS::PageSOVoiceTTS(QWidget *parent)
	: PageSOVoiceBase(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	comboVoiceList = new QComboBox(this);
	comboVoiceList->addItems(GlobalVal::s_pSpeakMgr->getTTSVoiceList());

	mainLayout->addWidget(new QLabel("ÓïÒô:"));
	mainLayout->addWidget(comboVoiceList);
	mainLayout->addStretch();

	connect(comboVoiceList, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(currentIndexChanged(const QString &)));

	QString voiceName = g_pbcmgr->getConfig()->getSpeakWTTSVoiceName();

	for (int i = 0; i < comboVoiceList->count(); i++)
	{
		if (comboVoiceList->itemText(i) == voiceName)
		{
			comboVoiceList->setCurrentIndex(i);
			break;
		}
	}
	GlobalVal::s_pSpeakMgr->setTTSVoice(voiceName);
}

PageSOVoiceTTS::~PageSOVoiceTTS()
{

}

QString PageSOVoiceTTS::currentVoiceName()
{
	return comboVoiceList->currentText();
}

void PageSOVoiceTTS::currentIndexChanged(const QString & text)
{
	g_pbcmgr->getConfig()->setSpeakWTTSVoiceName(comboVoiceList->currentText());
	GlobalVal::s_pSpeakMgr->setTTSVoice(text);
}

void PageSOVoiceTTS::save()
{
	g_pbcmgr->getConfig()->setSpeakWTTSVoiceName(comboVoiceList->currentText());
}	
