#include "stdafx.h"
#include "SpeakWTTS.h"

#include "GlobalVal.h"
#include "LIConfig.h"
#include "BCMgr.h"
SpeakWTTS::SpeakWTTS() : SpeakBase("SpeakWTTS")
{
	try {
		m_pSpeech = new QtSpeech(NULL);
		connect(m_pSpeech, SIGNAL(finished()), this, SIGNAL(finished()));
	}
	catch (QtSpeech::InitError ie)
	{
		//QMessageBox::warning(0, "警告", "合成语音引擎TTS初始化失败.",  QMessageBox::Ok);
		m_pSpeech = NULL;
	}


}

SpeakWTTS::~SpeakWTTS(void)
{
	if (m_pSpeech) delete m_pSpeech;
}

bool SpeakWTTS::speak(const QString &text)
{
	//DBG(QDateTime start   =   QDateTime::currentDateTime());
	if (m_pSpeech) m_pSpeech->tell(text);
	//DBG(qDebug() << "SpeakWTTS::speak??" << start.msecsTo(QDateTime::currentDateTime()));
	return true;
}

void SpeakWTTS::setVoice(const QString &voiceName)
{
	if (m_pSpeech) m_pSpeech->setVoice(voiceName);
}



void SpeakWTTS::stop()
{
	if (m_pSpeech) m_pSpeech->stop();
}

QStringList SpeakWTTS::getVoiceList()
{
	QStringList slRet;
	if (m_pSpeech) 
	{
		QtSpeech::VoiceNames vs = m_pSpeech->voices();
		foreach (QtSpeech::VoiceName vn, vs)
		{
			slRet << vn.name;
		}
	}
	

	return slRet; 
}

void SpeakWTTS::reset()
{
	LIConfig *cfg = g_pbcmgr->getConfig();
	QString voiceName = cfg->getSpeakWTTSVoiceName();
	if (voiceName.isEmpty())
	{
		QStringList vl = getVoiceList();
		if (!vl.isEmpty())
		{
			voiceName = vl[0];
			cfg->setSpeakWTTSVoiceName(voiceName);
		}
	}
	setVoice(voiceName);
	DBG(qDebug() << "SpeakWTTS::reset" << voiceName);
}


