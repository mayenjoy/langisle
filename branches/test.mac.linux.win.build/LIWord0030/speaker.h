#ifndef SPEAKER_H
#define SPEAKER_H

#include <QProcess>
#include "SpeakBase.h"

class SpeakLTTS: public SpeakBase
{
    Q_OBJECT

public:
    SpeakLTTS();
    virtual ~SpeakLTTS(void);
    virtual bool speak(const QString &text);
    bool setVoice(const QString &vtext);
    QStringList getVoiceList(); 
    virtual void stop();   
    void setVolume(int nVol);
    void setSpeed(int nSpeed);
    virtual void reset();

signals:
    void finished();

private slots:
    void emitPalyFinished();

private:
    QString m_nVol;
    QString m_nSpeed;
    QString m_strVoice;
    QString m_speechCmd;
    QProcess *m_speechProcess;
};

#endif //SPEAKER_H
