#include "stdafx.h"
#include "SoundFilePlayPhonon.h"
#if defined(Q_WS_WIN)
#include <windows.h>
#endif

SoundFilePlayPhonon::SoundFilePlayPhonon(QObject *parent)
	: SoundFilePlayBase(parent)
{
	mediaObject = new Phonon::MediaObject();
	audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory);
	Phonon::createPath(mediaObject, audioOutput);
	connect(mediaObject, SIGNAL(finished()), this, SIGNAL(finished()));
	connect(mediaObject, SIGNAL(stateChanged(Phonon::State, Phonon::State)), this, SLOT(stateChanged(Phonon::State, Phonon::State)));
    connect(this, SIGNAL(toPlay()), mediaObject, SLOT(play()));
    source = NULL;
	curState = Phonon::PausedState;
}

SoundFilePlayPhonon::~SoundFilePlayPhonon()
{
	stop();
	delete audioOutput;
	delete mediaObject;
}
bool SoundFilePlayPhonon::play(const QString &filePath)
{
    DBG(qDebug() << "SoundFilePlayPhonon::play" << filePath );
	stop();
	
	source = new Phonon::MediaSource(filePath);
	
	mediaObject->clear();
	mediaObject->setCurrentSource(*source);
    //mediaObject->play();
    emit toPlay();
	return true;
}
//#include <windows.h>
void SoundFilePlayPhonon::stop()
{	
	//DBG(qDebug() << "SoundFilePlayPhonon::stop");
	if (source != NULL)
	{
		while (mediaObject->state() == Phonon::LoadingState || mediaObject->state() == Phonon::PlayingState || mediaObject->state() == Phonon::BufferingState)
		{
			//DBG(qDebug() << "SoundFilePlayPhonon::stop...." << mediaObject->state() << GetCurrentThreadId());
			mediaObject->stop();
			Global::sleep(10);
		}
			delete source;
			source = NULL;

	}

}

void SoundFilePlayPhonon::stateChanged( Phonon::State newstate, Phonon::State oldstate )
{
	//DBG(qDebug() << "SoundFilePlayPhonon::stateChanged" << newstate << oldstate);
	curState = newstate;
}
