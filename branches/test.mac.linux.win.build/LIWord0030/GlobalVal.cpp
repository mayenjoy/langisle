#include "stdafx.h"
#include "GlobalVal.h"
#include "config.h"
#include "PictureMgr.h"
#include "SpeakMgr.h"
#include "SoundFilePlayMgr.h"
#include "LIConfig.h"
#include "ThemeMgr.h"
#include "UserMgr.h"
#include "studymgrmodel.h"
SpeakMgr *GlobalVal::s_pSpeakMgr = NULL;
LIConfig *GlobalVal::s_pcfg = NULL;
SoundFilePlayMgr *GlobalVal::s_psfpm = NULL;
QSplashScreen *GlobalVal::s_pSplash = NULL;
PictureMgr *GlobalVal::s_pPm = NULL;
ThemeMgr *GlobalVal::s_ptm = NULL;
UserMgr *GlobalVal::s_um = NULL;
LIWord0030 *GlobalVal::s_pmw = NULL;
GlobalVal::GlobalVal(void)
{
}

GlobalVal::~GlobalVal(void)
{
}

bool GlobalVal::init()
{
// 	QList<QtSpeech::VoiceName> VoiceNames = QtSpeech::voices();
// 	foreach (QtSpeech::VoiceName vn, VoiceNames)
// 	{
// 		DBG(qDebug() << vn.id << vn.name);
// 	}
// 	s_pSpeech = new QtSpeech(QtSpeech::voices()[2], NULL);

	QPixmap pixmap(Global::makeRelativeFilePath("/html/images/Splash.png"));
	s_pSplash = new QSplashScreen(pixmap);
	s_pSplash->show();
	s_pSplash->showMessage(QString("build %1").arg(VER_4), Qt::AlignLeft | Qt::AlignBottom, QColor(200, 200, 200));//ʾϢ


	Qt::Alignment topRight = Qt::AlignLeft | Qt::AlignBottom;
	s_pcfg = new LIConfig(Global::makePathStr("/gconfig.conf"));
	s_psfpm = new SoundFilePlayMgr;
	s_pSpeakMgr = new SpeakMgr;
	s_pPm = new PictureMgr;
	s_ptm = new ThemeMgr;
	s_um = new UserMgr(Global::makePathStr("/udata/user.db"), "UserDB");

	StudyMgrModel::initC2hmap();
	StudyMgrModel::initN2size();

	return true;
}


bool GlobalVal::uninit()
{
	delete s_um;
	delete s_ptm;
	delete s_pPm;
	delete s_pSpeakMgr;
	delete s_psfpm;
	delete s_pcfg;
	delete s_pSplash;
	return true;
}