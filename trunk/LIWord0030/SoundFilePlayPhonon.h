#ifndef SOUNDFILEPLAYPHONON_H
#define SOUNDFILEPLAYPHONON_H
#include <phonon>
#include "SoundFilePlayBase.h"



class SoundFilePlayPhonon : public SoundFilePlayBase
{
	Q_OBJECT

public:
	SoundFilePlayPhonon(QObject *parent = 0);
	~SoundFilePlayPhonon();

	virtual bool play(const QString &filePath);
	virtual void stop();
public slots:
	void stateChanged ( Phonon::State newstate, Phonon::State oldstate );
private:
	Phonon::MediaObject *mediaObject;
	Phonon::AudioOutput *audioOutput;
	Phonon::MediaSource *source;
	volatile Phonon::State curState;
};

#endif // SOUNDFILEPLAYPHONON_H
