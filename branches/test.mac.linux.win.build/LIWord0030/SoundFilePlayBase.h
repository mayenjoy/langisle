#ifndef SOUNDFILEPLAYBASE_H
#define SOUNDFILEPLAYBASE_H

#include <QObject>

class SoundFilePlayBase : public QObject
{
	Q_OBJECT

public:
	SoundFilePlayBase(QObject *parent = 0);
	virtual ~SoundFilePlayBase();

	virtual bool play(const QString &filePath) = 0;
	virtual void stop() = 0;
signals:
	void finished();
private:
	
};

#endif // SOUNDFILEPLAYBASE_H
