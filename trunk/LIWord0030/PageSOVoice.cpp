#include "stdafx.h"
#include "PageSOVoice.h"
#include "GlobalVal.h"
#include "PageSOVoiceBase.h"
#include "PageSOVoiceTTS.h"
#include "PageSOVoiceYoudao.h"
#include "PageSOVoiceGoogle.h"
#include "PageSOVoiceRealPeople.h"
#include "SpeakMgr.h"
#include "LIConfig.h"
#include "BCMgr.h"
Q_DECLARE_METATYPE(void *);

PageSOVoice::PageSOVoice(QWidget *parent)
	: QWidget(parent)
{
	GlobalVal::s_pSpeakMgr->stop();

	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);


	voiceSelComboBox = new QComboBox(this);
	QVariant var;
	var.setValue((void *)GlobalVal::s_pSpeakMgr->m_pSpeakWTTS);
	voiceSelComboBox->addItem("�ϳ�����", var);
	var.setValue((void *)GlobalVal::s_pSpeakMgr->m_pSpeakYoudao);
	voiceSelComboBox->addItem("�е���������", var);
	var.setValue((void *)GlobalVal::s_pSpeakMgr->m_pSpeakGoogle);
	voiceSelComboBox->addItem("�ȸ���������", var);
	var.setValue((void *)GlobalVal::s_pSpeakMgr->m_pSpeakRealPeople);
	voiceSelComboBox->addItem("��������", var);

	lineTestWord = new QLineEdit("Happy", this);
	QPushButton *buttonTest = new QPushButton("����");
	QHBoxLayout *layoutTest = new QHBoxLayout();
	//layoutTest->addStretch();
	layoutTest->addWidget(lineTestWord);
	layoutTest->addWidget(buttonTest);

	stacketWidget = new QStackedWidget(this);
	stacketWidget->setFrameShape(QFrame::StyledPanel);

	pageSOVoiceTTS = new PageSOVoiceTTS(this);
	pageSOVoiceYoudao = new PageSOVoiceYoudao(this);
	pageSOVoiceGoogle = new PageSOVoiceGoogle(this);
	pageSOVoiceRealPeople = new PageSOVoiceRealPeople(this);
	stacketWidget->addWidget(pageSOVoiceTTS);
	stacketWidget->addWidget(pageSOVoiceYoudao);
	stacketWidget->addWidget(pageSOVoiceGoogle);
	stacketWidget->addWidget(pageSOVoiceRealPeople);
	
// 	checkBoxLoopPlay = new QCheckBox("ѭ����������");
// 	mainLayout->addWidget(checkBoxLoopPlay);

	mainLayout->addWidget(new QLabel("��������:"));
	mainLayout->addWidget(voiceSelComboBox);
	mainLayout->addLayout(layoutTest);
	mainLayout->addWidget(stacketWidget);

	connect(voiceSelComboBox, SIGNAL(currentIndexChanged( int)), stacketWidget, SLOT(setCurrentIndex(int)));
	connect(buttonTest, SIGNAL(clicked()), this, SLOT(testSpeak()));

//	checkBoxLoopPlay->setChecked(g_pbcmgr->getConfig()->getLoopPlay());
	QString speakType = g_pbcmgr->getConfig()->getDefaultSpeakType();

	//DBG(qDebug() << "PageSOVoice::PageSOVoice speakType" << speakType);

	for (int i = 0; i < voiceSelComboBox->count(); i++)
	{
		if (((SpeakBase *)voiceSelComboBox->itemData(i).value<void *>())->name() == speakType)
		{
			voiceSelComboBox->setCurrentIndex(i);
			break;
		}
		//DBG(qDebug() << "PageSOVoice::PageSOVoice SpeakBase name" << i << ((SpeakBase *)voiceSelComboBox->itemData(i).value<void *>())->name());
	}
	
}

PageSOVoice::~PageSOVoice()
{

}


void PageSOVoice::testSpeak()
{
	GlobalVal::s_pSpeakMgr->testSpeak(voiceSelComboBox->currentText(), lineTestWord->text());
}

void PageSOVoice::save()
{
	SpeakBase *pSpeak = (SpeakBase *)voiceSelComboBox->itemData(voiceSelComboBox->currentIndex()).value<void *>();
	g_pbcmgr->getConfig()->setDefaultSpeakType(pSpeak->name());

//	g_pbcmgr->getConfig()->setLoopPlay(checkBoxLoopPlay->isChecked());
//  	PageSOVoiceBase * psovb = (PageSOVoiceBase *)stacketWidget->widget(voiceSelComboBox->currentIndex());
// 	psovb->save();
	pageSOVoiceTTS->save();
	pageSOVoiceYoudao->save();
	pageSOVoiceGoogle->save();
	pageSOVoiceRealPeople->save();

	GlobalVal::s_pSpeakMgr->reset();

}
