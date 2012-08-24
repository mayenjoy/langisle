#include "stdafx.h"
#include "SpeakRealPeople.h"
#include "GlobalVal.h"
#include "SoundFilePlayMgr.h"
#include "SpeakMgr.h"
#include "LIConfig.h"
#include "BCMgr.h"
SpeakRealPeople::SpeakRealPeople(QObject *parent)
	: SpeakBase("SpeakRealPeople")
{
	m_bUseTTSOnFail = false;
	connect(GlobalVal::s_psfpm, SIGNAL(finished()), this, SIGNAL(finished()));

}

SpeakRealPeople::~SpeakRealPeople()
{

}

bool SpeakRealPeople::speak(const QString &text)
{
	stop();
	if (m_bUseTTSOnFail)
	{
		GlobalVal::s_pSpeakMgr->stopTTS();
	}
	if (text.isEmpty())
		return false;
	//DBG(qDebug() << "SpeakRealPeople::speak" << text);
	QString path = QDir::convertSeparators(m_path + "/" + text.at(0) + "/" + text);
	QString mp3path = path + ".mp3";
	QString wavpath = path + ".wav";
	
	if (QFile::exists(mp3path))
	{
		GlobalVal::s_psfpm->play(mp3path);
		return true;
	}
	else if (QFile::exists(wavpath))
	{
		GlobalVal::s_psfpm->play(wavpath);
		return true;
	}
	DBG(qDebug() << "SpeakRealPeople::speak 未能找到相应的真人语音" << text << path);
	if (m_bUseTTSOnFail)
	{
		DBG(qDebug() << "使用TTS语音" << text);
		
		GlobalVal::s_pSpeakMgr->speakTTS(text);
	}
	return false;
}

void SpeakRealPeople::stop()
{
	GlobalVal::s_psfpm->stop();
}

void SpeakRealPeople::reset()
{
	setPath(g_pbcmgr->getConfig()->getSpeakRealPeoplePath());
	m_bUseTTSOnFail = g_pbcmgr->getConfig()->getSpeakRealPeopleOnFailUseTTS();

	DBG(qDebug() << "SpeakRealPeople::reset" << m_path << m_bUseTTSOnFail);
}
