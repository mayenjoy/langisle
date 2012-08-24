#pragma once

class GlobalVal
{
public:
	GlobalVal(void);
	virtual ~GlobalVal(void);

public:
	QString appName;	// 程序名
	QString exeName;	// exe名,如:LIWord.exe
	QString updatedDir;	// 更新目录
	QString updateUrl;	// 更新链接
	static GlobalVal gVal;
};
