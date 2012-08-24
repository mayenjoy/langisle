#ifndef LICONFIG_H
#define LICONFIG_H



typedef QList< QMap<QString, QVariant> > LISTMAPINFO;

class LIConfig
{


public:
	LIConfig(const QString &path);
	~LIConfig();

	QString getDefaultSpeakType()
	{
		return cfg->value("Speak/DefaultSpeakType", "SpeakGoogle").toString();
	}
	void setDefaultSpeakType(const QString &speakType)
	{
		cfg->setValue("Speak/DefaultSpeakType", speakType);
	}
	QString getSpeakRealPeoplePath()
	{
		return cfg->value("Speak/SpeakRealPeople/path", Global::makePathStr("/odata/voice")).toString();
	}
	void setSpeakRealPeoplePath(const QString &path)
	{
		cfg->setValue("Speak/SpeakRealPeople/path", path);
	}
	QString getSpeakWTTSVoiceName()
	{
		return cfg->value("Speak/SpeakWTTS/VoiceName", "").toString();
	}
	void setSpeakWTTSVoiceName(const QString &voiceName)
	{
		cfg->setValue("Speak/SpeakWTTS/VoiceName", voiceName);
	}

	bool getSpeakRealPeopleOnFailUseTTS()
	{
		return cfg->value("Speak/SpeakRealPeople/OnFailUseTTS", true).toBool();
	}
	void setSpeakRealPeopleOnFailUseTTS(bool bUse)
	{
		cfg->setValue("Speak/SpeakRealPeople/OnFailUseTTS", bUse);
	}

	bool getLoopPlay()
	{
		return cfg->value("Speak/LoopPlay", false).toBool();
	}
	void setLoopPlay(bool bUse)
	{
		cfg->setValue("Speak/LoopPlay", bUse);
	}
	bool getGWRandom()
	{
		return cfg->value("Other/GWRandom", true).toBool();
	}
	void setGWRandom(bool bRandom)
	{
		cfg->setValue("Other/GWRandom", bRandom);
	}
	QString getPicturePath()
	{
		return cfg->value("Picture/path", Global::makePathStr("/odata/pic")).toString();
	}
	void setPicturePath(const QString &path)
	{
		cfg->setValue("Picture/path", path);
	}

	void setValue(const QString &key, const QVariant &value)
	{
		cfg->setValue(key, value);
	}

	QVariant getValue(const QString &key, const QVariant &dvalue)
	{
		return cfg->value(key, dvalue);
	}
	LISTMAPINFO getDictInfo()
	{

		return getLMI("Dictlist");
	}
	void setDictInfo(LISTMAPINFO &lmi)
	{
		setLMI("Dictlist", lmi);
	}

	void initDefault()
	{
		LISTMAPINFO lmi;
		QMap<QString, QVariant> dictInfo;
// 		dictInfo["used"] = true;
// 		dictInfo["name"] = "有道手机词典";
// 		dictInfo["addr"] = "http://dict.youdao.com/m/search?keyfrom=dict.mindex&vendor=%24vendor&q=[LIWord]";
// 		lmi << dictInfo;

		dictInfo["used"] = true;
		dictInfo["name"] = "海词";
		dictInfo["addr"] = "http://dict.cn/mini.php?q=[LIWord]";
		lmi << dictInfo;

// 		dictInfo["used"] = true;
// 		dictInfo["name"] = "谷歌";
// 		dictInfo["addr"] = "http://www.google.com.hk/dictionary?hl=zh-CN&q=[LIWord]&langpair=en%7Czh-CN";
// 		lmi << dictInfo;

		dictInfo["used"] = true;
		dictInfo["name"] = "有道";
		dictInfo["addr"] = "http://dict.youdao.com/search?q=[LIWord]";
		lmi << dictInfo;

		dictInfo["used"] = true;
		dictInfo["name"] = "必应";
		dictInfo["addr"] = "http://dict.bing.com.cn/#[LIWord]";
		lmi << dictInfo;
	


		setLMI("Dictlist", lmi);


		cfg->setValue("UI/nowUI", "li1");

	}
	QString getThemeID()
	{
		return cfg->value("UI/nowUI", "").toString();
	}

	QSettings *getCfg() {return cfg;};

	QString getLastLoginUser()
	{
		return cfg->value("Login/LastLoginUser", "default").toString();
	}
	void setLastLoginUser(const QString &userName)
	{
		cfg->setValue("Login/LastLoginUser", userName);
	}

	bool getAutoLogin()
	{
		return cfg->value("Login/AutoLogin", true).toBool();
	}
	void setAutoLogin(bool bAuto)
	{
		cfg->setValue("Login/AutoLogin", bAuto);
	}

private:
	LISTMAPINFO getLMI(const QString & prefix);
	void setLMI(const QString & prefix, LISTMAPINFO &lmi);
private:
	QSettings *cfg;
};

#endif // LICONFIG_H
