#include "stdafx.h"
#include "speaker.h"
#include "GlobalVal.h"
#include "LIConfig.h"
#include "BCMgr.h"
SpeakLTTS::SpeakLTTS():SpeakBase("SpeakWTTS")
{
    m_speechProcess = new QProcess;
    m_nVol.clear();
    m_nSpeed.clear();
    m_strVoice.clear();
    m_speechCmd.clear();

    connect(m_speechProcess, SIGNAL(finished(int)), this, SLOT(emitPalyFinished()));
}

SpeakLTTS::~SpeakLTTS()
{
    delete m_speechProcess;
}

bool SpeakLTTS::speak(const QString &text)
{
    QTemporaryFile file;
    file.setAutoRemove(true);
    if (m_speechCmd.isEmpty())
        return false;

    if (m_speechProcess->state() != QProcess::NotRunning)
        m_speechProcess->kill();
    
    if (m_speechCmd.contains("espeak"))
    {
        m_speechCmd = m_speechCmd + m_nVol ;
        m_speechProcess->start(m_speechCmd +" \"" + text  + "\"");
        //        qDebug()<<m_speechCmd +" \"" + text  + "\"";
    }
    else
    {
        if(file.open())
        {
            QByteArray ba = text.toLatin1();
            const char *word = ba.data();
            file.write(word);
            file.close();
        }
        m_speechProcess->start(m_speechCmd + file.fileName());
        //#if defined(Q_WS_X11)
//                         sleep(1);
        //#endif

    }

    if (! m_speechProcess->waitForStarted())
        return false;

    return true;
}

void SpeakLTTS::stop()
{
        m_speechProcess->kill();
}

QStringList SpeakLTTS::getVoiceList()
{
    QStringList voiceslist;
#if defined(Q_WS_X11)

    if(QFile::exists("/usr/bin/espeak"))
    {
        voiceslist <<"eSpeak";
    }
    if(QFile::exists("/usr/bin/festival"))
    {
        voiceslist <<"Festival";
    }

    //    if(nType == 0)
    //    {
    //        voiceslist<<"m1"<<"m2"<<"m3"<<"m4"<<"m5"<<"m6"<<"m7"<<"croak"<<"whisper";
    //    }
    //    else
    //    {
    //        voiceslist<<"f1"<<"f2"<<"f3"<<"f4";
    //    }
#endif
#if  defined(Q_WS_MAC)

#endif

    return voiceslist;
}

bool SpeakLTTS::setVoice(const QString &vtext)
{

    m_speechCmd = vtext.toLower();//ת��ΪСд
    if (m_speechCmd == "espeak")
    {
        m_strVoice = " -ven+f1";// + CGlobalVal::pgcfg->value ("Learn/VoiceType","f1").toString();
        m_nSpeed =" -s160"; //+  // QString::number(CGlobalVal::pgcfg->value ("Learn/Speed","160").toInt());
        m_speechCmd = m_speechCmd + m_strVoice +m_nSpeed ;
        return true;
    }
    else
    {
        m_speechCmd = m_speechCmd + " --tts " ;
        return true;
    }

    //	DBG(qDebug() << m_strVoice);
    return false;
}

void SpeakLTTS::setVolume(int nVol)
{
    m_nVol =" -a"  + QString::number(nVol);
}

void SpeakLTTS::setSpeed(int nSpeed)
{
    m_nSpeed =" -s" +   QString::number(nSpeed);
}

void SpeakLTTS::reset()
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
	DBG(qDebug() << "SpeakLTTS::reset" << voiceName);
}

void SpeakLTTS::emitPalyFinished()
{
        int ms = 1000;//CGlobalVal::pgcfg->value ("Learn/IntervalTime",1000).toInt();
        //        qDebug()<<"tts finished.....";
        QTime dieTime = QTime::currentTime().addMSecs(ms);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        emit finished();
}


