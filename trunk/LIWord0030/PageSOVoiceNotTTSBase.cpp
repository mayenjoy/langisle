#include "stdafx.h"
#include "PageSOVoiceNotTTSBase.h"

PageSOVoiceNotTTSBase::PageSOVoiceNotTTSBase(QWidget *parent)
	: PageSOVoiceBase(parent)
{
	mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	checkBoxOnFailUserTTS = new QCheckBox("������ʧ��ʱʹ��\"�ϳ�����\"");

	mainLayout->addWidget(checkBoxOnFailUserTTS);

}

PageSOVoiceNotTTSBase::~PageSOVoiceNotTTSBase()
{

}
