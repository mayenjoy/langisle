#pragma once
#include <QObject>

class SpeakBase : public QObject
{
	Q_OBJECT
public:
	SpeakBase(const QString &name, QObject *parent = 0);
	virtual ~SpeakBase(void);

	virtual bool speak(const QString &text) = 0;
	virtual void stop() = 0;
	virtual void reset() {};
	QString name(){return m_name;};
signals:
	void finished();
protected:
	QString m_name;
};
