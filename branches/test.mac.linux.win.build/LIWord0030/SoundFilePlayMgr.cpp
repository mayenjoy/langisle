#include "stdafx.h"
#include "SoundFilePlayMgr.h"

SoundFilePlayMgr::SoundFilePlayMgr(void)
{
	m_pSFPP = new SoundFilePlayPhonon;
	m_pCurSFP = m_pSFPP;
	connect(m_pSFPP, SIGNAL(finished()), this, SIGNAL(finished()));
}

SoundFilePlayMgr::~SoundFilePlayMgr(void)
{
	delete m_pSFPP;
}

bool SoundFilePlayMgr::play(const QString &filePath)
{
	DBG(qDebug() << "SoundFilePlayMgr::play" << filePath);
	return m_pCurSFP->play(filePath);
}

void SoundFilePlayMgr::stop()
{
	m_pCurSFP->stop();
}