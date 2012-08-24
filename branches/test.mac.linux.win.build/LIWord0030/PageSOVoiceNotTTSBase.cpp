#include "stdafx.h"
#include "PageSOVoiceNotTTSBase.h"

PageSOVoiceNotTTSBase::PageSOVoiceNotTTSBase(QWidget *parent)
	: PageSOVoiceBase(parent)
{
	mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	checkBoxOnFailUserTTS = new QCheckBox("当发音失败时使用\"合成语音\"");

	mainLayout->addWidget(checkBoxOnFailUserTTS);

}

PageSOVoiceNotTTSBase::~PageSOVoiceNotTTSBase()
{

}
