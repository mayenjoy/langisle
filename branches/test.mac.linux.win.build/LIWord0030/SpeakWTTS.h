#pragma once

#include "SpeakBase.h"
#include <QtSpeech>
class SpeakWTTS :
	public SpeakBase
{
	Q_OBJECT
public:
	SpeakWTTS();
	virtual ~SpeakWTTS(void);
	virtual bool speak(const QString &text);
	void setVoice(const QString &voiceName);
	virtual void stop();
	QStringList getVoiceList();
	virtual void reset();
protected:
	QtSpeech *m_pSpeech;
};


