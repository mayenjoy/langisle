#pragma once

class GlobalVal
{
public:
	GlobalVal(void);
	virtual ~GlobalVal(void);

public:
	QString appName;	// ������
	QString exeName;	// exe��,��:LIWord.exe
	QString updatedDir;	// ����Ŀ¼
	QString updateUrl;	// ��������
	static GlobalVal gVal;
};
