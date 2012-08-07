#pragma once

//////////////////////////////////////////////////////////////////////////
// VER
#define VER_1 "0"
#define VER_2 "3"
#define VER_3 "4"
#define VER_4 "554"
#define VER VER_1 "." VER_2 "." VER_3 "." VER_4
// 一个ver3对应一个升级配置文件 
#if defined(Q_WS_WIN)
#define NEWVER_CHECK_URL "http://down.langisle.com/003/verinfo" VER_1 "." VER_2 "." VER_3 ".xml"
#define VERINFOFILE "/verinfo.xml"
#endif
#if defined(Q_WS_X11)
#define NEWVER_CHECK_URL "http://down.langisle.com/003/verinfo_linux" VER_1 "." VER_2 "." VER_3 ".xml"
#define VERINFOFILE "/verinfo_linux.xml"
#endif
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////e///////////////////////////
// UPDate
#if defined(Q_WS_WIN)
#define UPDATE_EXE "LIUpdate.exe"
#endif
#if defined(Q_WS_X11)
#define UPDATE_EXE "LIUpdate"
#endif


//////////////////////////////////////////////////////////////////////////
// 其它
// 临时目录
#define TEMP_DIR "/temp"
// 程序名
#define APPNAME "语言岛智能记单词"
// exe名
#if defined(Q_WS_WIN)
#define EXENAME "liword.exe"
#endif
#if defined(Q_WS_X11)
#define EXENAME "LIWord"
#endif


// 官方数据目录
#define O_PATH "/odata"
// 官方词库目录
#define O_BOOK_PATH O_PATH "/book"

// 课程库下载地址
#define OBOOK_DOWN_URL "http://down.langisle.com/003/obook/"


enum wordType
{
	eUWord = 1,
	eOWord = 2
};

enum bookType		// 课程类型
{
	btUser = 1,			// 用户
	btOfficial = 2		// 官方
};

enum categoryType	// 分类类型
{
	ctMyself = 1,	// 我的
	ctUser = 2,     // 用户 
	ctOfficial = 3,	// 官方
};

typedef QMap<QString, QVariant> BOOKINFO;
typedef QMap<QString, QVariant> WordInfo;
