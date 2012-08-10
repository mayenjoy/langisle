#include "stdafx.h"
#include "SpeakWTTS.h"

#include "GlobalVal.h"
#include "LIConfig.h"
#include "BCMgr.h"
SpeakWTTS::SpeakWTTS() : SpeakBase("SpeakWTTS")
{
    m_pSpeech = 0;
    try {
        if (m_pSpeech != 0)
            delete m_pSpeech;
        m_pSpeech = new QtSpeech();
        //connect(m_pSpeech, SIGNAL(finished()), this, SIGNAL(finished()));
    }
    catch (QtSpeech::InitError ie)
    {
        QMessageBox::warning(0, "警告", "合成语音引擎初始化失败.",  QMessageBox::Ok);
        m_pSpeech = 0;
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
	//DBG(qDebug() << "SpeakWTTS::speakʱ" << start.msecsTo(QDateTime::currentDateTime()));
	return true;
}

void SpeakWTTS::setVoice(const QString &voiceName)
{
    //if (m_pSpeech) m_pSpeech->setVoice(voiceName);
    reset();
}



void SpeakWTTS::stop()
{
#if defined(Q_WS_WIN)
        if (m_pSpeech) m_pSpeech->stop();
#endif

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
    qDebug() << voiceName;
    QtSpeech::VoiceName v;
    if (!voiceName.isEmpty())
    {
        foreach(v, QtSpeech::voices())
        {
                qDebug() << "id:" << v.id << "name:" << v.name;
                if (v.name == voiceName)
                {
                    break;
                }
        }

    }
    try {
        if (m_pSpeech != 0)
            delete m_pSpeech;
        m_pSpeech = new QtSpeech(v);
        //connect(m_pSpeech, SIGNAL(finished()), this, SIGNAL(finished()));
    }
    catch (QtSpeech::InitError ie)
    {
        QMessageBox::warning(0, "警告", "合成语音引擎初始化失败.",  QMessageBox::Ok);
        m_pSpeech = 0;
    }


    //setVoice(voiceName);
	DBG(qDebug() << "SpeakWTTS::reset" << voiceName);
}


