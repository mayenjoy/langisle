#pragma once
#include "SpeakYodao.h"
#include "SpeakGoogle.h"
#include "SpeakRealPeople.h"
#include <qglobal.h>
#if defined(Q_WS_WIN)
   #include "SpeakWTTS.h"
#endif

#if defined(Q_WS_X11)
   #include "speaker.h"
#endif

#if defined(Q_WS_MAC)
    #include "speaker.h"
#endif

class SpeakMgr : public QObject
{
	Q_OBJECT
public:
	SpeakMgr();
	~SpeakMgr(void);

	void speak(const QString &text);
	void stop();
	void setLoopTimes(int nTimes) {m_nTimes = nTimes;};
	void setSleep(int nSleep) {m_nSleep = nSleep;}
	void testSpeak(const QString &curSelEngine, const QString &text);
	QStringList getTTSVoiceList();
	void setTTSVoice(const QString &voiceName);

	void setRealPeoplePath(const QString &path);

	void reset();

	void speakTTS(const QString &text);
	void stopTTS();

#if defined(Q_WS_WIN)
        SpeakWTTS *m_pSpeakWTTS;
#endif
#if defined(Q_WS_X11)
        SpeakLTTS *m_pSpeakWTTS;
#endif
#if defined(Q_WS_MAC)
    SpeakLTTS *m_pSpeakWTTS;
#endif
	SpeakYodao *m_pSpeakYoudao;
	SpeakGoogle *m_pSpeakGoogle;
	SpeakRealPeople *m_pSpeakRealPeople;

public slots:
	void finished();
protected:


	SpeakBase *m_pSpeak;
	int m_nTimes;
	QString m_text;
	int m_nTimeCount;
	int m_nSleep;
};

