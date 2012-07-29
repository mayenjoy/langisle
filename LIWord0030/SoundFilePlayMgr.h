#pragma once
#include "SoundFilePlayPhonon.h"
class SoundFilePlayMgr : public QObject
{
	Q_OBJECT
public:
	SoundFilePlayMgr(void);
	~SoundFilePlayMgr(void);

	bool play(const QString &filePath);
	void stop();
signals:
	void finished();
protected:
	SoundFilePlayPhonon *m_pSFPP;
	SoundFilePlayBase *m_pCurSFP;
};
