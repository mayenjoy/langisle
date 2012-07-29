#pragma once



class SpeakMgr;
class LIConfig;
class SoundFilePlayMgr;
class PictureMgr;
class ThemeMgr;
class UserMgr;
class LIWord0030;
class GlobalVal
{
public:
	GlobalVal(void);
	virtual ~GlobalVal(void);
	
	static bool init();
	static bool uninit();

	static SpeakMgr *s_pSpeakMgr;
	static LIConfig *s_pcfg;
	static SoundFilePlayMgr *s_psfpm;
	static QSplashScreen *s_pSplash;
	static PictureMgr *s_pPm;
	static ThemeMgr *s_ptm;
	static UserMgr *s_um;
	static LIWord0030 *s_pmw;
};

