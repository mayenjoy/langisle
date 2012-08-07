#include "stdafx.h"
#include "SpeakMgr.h"
#include "GlobalVal.h"
#include "LIConfig.h"
#include "BCMgr.h"
SpeakMgr::SpeakMgr(void)
{
        //GlobalVal::s_pSplash->showMessage("正在初始化TTS模块...", Qt::AlignLeft | Qt::AlignBottom, Qt::black);
	//m_pSpeakWTTS->setVoice("VW Paul");
#if defined(Q_WS_WIN)
        m_pSpeakWTTS = new SpeakWTTS();
#endif
#if defined(Q_WS_X11)
        m_pSpeakWTTS = new SpeakLTTS();
#endif
	m_pSpeakYoudao = new SpeakYodao;

	m_pSpeakGoogle = new SpeakGoogle;

	m_pSpeakRealPeople = new SpeakRealPeople;

	//m_pSpeakRealPeople->setPath("F:\\eMule\\] Webster[1] Part1\\Webster");

	connect(m_pSpeakWTTS, SIGNAL(finished()), this, SLOT(finished()));

	m_pSpeak = NULL;

}

SpeakMgr::~SpeakMgr(void)
{
	delete m_pSpeakRealPeople;
	delete m_pSpeakGoogle;
	delete m_pSpeakYoudao;
	delete m_pSpeakWTTS;
}

void SpeakMgr::speak(const QString &text)
{
	// DBG(qDebug() << "SpeakMgr::speak" << text);
	//setLoopSpeak(true);
	m_text = text;
	m_nTimeCount = 1;
	m_pSpeak->speak(text);
}

void SpeakMgr::stop()
{
	// DBG(qDebug() << "SpeakMgr::stop");
	m_nTimeCount = 0;
	m_pSpeak->stop();
}

void SpeakMgr::finished()
{
	if (m_nTimeCount == 0)
		return; // stop()
	if (m_nTimeCount < m_nTimes)
	{
		Global::sleep(m_nSleep);
		m_pSpeak->speak(m_text);
		m_nTimeCount++;
	}
}

QStringList SpeakMgr::getTTSVoiceList()
{
	return m_pSpeakWTTS->getVoiceList();
}

void SpeakMgr::testSpeak(const QString &curSelEngine, const QString &text)
{
	SpeakBase *pSpeak = NULL;
	DBG(qDebug() << " SpeakMgr::testSpeak" << curSelEngine);
    if ("合成语音" == curSelEngine)
	{
		pSpeak = m_pSpeakWTTS;
	}
        else if ("有道在线语音" == curSelEngine)
	{
		pSpeak = m_pSpeakYoudao;
	}
        else if ("谷歌在线语音" == curSelEngine)
	{
		pSpeak = m_pSpeakGoogle;
	}
        else if ("真人语音" == curSelEngine)
	{
		pSpeak = m_pSpeakRealPeople;
	}
	if (pSpeak != NULL)
		pSpeak->speak(text);
}

void SpeakMgr::setTTSVoice(const QString &voiceName)
{

	m_pSpeakWTTS->setVoice(voiceName);
}

void SpeakMgr::setRealPeoplePath(const QString &path)
{
	m_pSpeakRealPeople->setPath(path);
}

void SpeakMgr::reset()
{
	if (m_pSpeak != NULL && m_pSpeak != m_pSpeakWTTS)
	{
		disconnect(m_pSpeak, SIGNAL(finished()), this, SLOT(finished()));
	}

	QString vn = g_pbcmgr->getConfig()->getDefaultSpeakType();
	if (vn == "SpeakWTTS")
	{
		m_pSpeak = m_pSpeakWTTS;
	}
	else if (vn == "SpeakYodao")
	{
		m_pSpeak = m_pSpeakYoudao;
	}
	else if (vn == "SpeakGoogle")
	{
		m_pSpeak = m_pSpeakGoogle;
	}
	else if (vn == "SpeakRealPeople")
	{
		m_pSpeak = m_pSpeakRealPeople;
		m_pSpeakWTTS->reset();
	}
	
	m_pSpeak->reset();

	setLoopTimes(g_pbcmgr->getConfig()->getValue("LearnMgr/Replaytimes", "1").toInt());
	m_nSleep = g_pbcmgr->getConfig()->getValue("LearnMgr/ReplaySleep", "200").toInt();

	if (m_pSpeak != m_pSpeakWTTS)
		connect(m_pSpeak, SIGNAL(finished()), this, SLOT(finished()));

	
//	DBG(qDebug() << "使SpeakMgr::reset" << m_pSpeak->name());
}

void SpeakMgr::speakTTS(const QString &text)
{
	m_pSpeakWTTS->speak(text);
}

void SpeakMgr::stopTTS()
{
	m_pSpeakWTTS->stop();
}
