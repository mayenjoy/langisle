#ifndef SPEAKREALPEOPLE_H
#define SPEAKREALPEOPLE_H

#include "SpeakBase.h"

class SpeakRealPeople : public SpeakBase
{
	Q_OBJECT

public:
	SpeakRealPeople(QObject *parent = 0);
	~SpeakRealPeople();
	virtual bool speak(const QString &text);
	virtual void stop();
	void setPath(const QString &path) {m_path = path;};
	virtual void reset();
private:
	QString m_path;
	bool m_bUseTTSOnFail;
};

#endif // SPEAKREALPEOPLE_H
