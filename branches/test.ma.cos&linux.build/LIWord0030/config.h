#pragma once

//////////////////////////////////////////////////////////////////////////
// VER
#define VER_1 "0"
#define VER_2 "3"
#define VER_3 "4"
#define VER_4 "554"
#define VER VER_1 "." VER_2 "." VER_3 "." VER_4
// һ��ver3��Ӧһ�����������ļ� 
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
// ����
// ��ʱĿ¼
#define TEMP_DIR "/temp"
// ������
#define APPNAME "���Ե����ܼǵ���"
// exe��
#if defined(Q_WS_WIN)
#define EXENAME "liword.exe"
#endif
#if defined(Q_WS_X11)
#define EXENAME "LIWord"
#endif


// �ٷ�����Ŀ¼
#define O_PATH "/odata"
// �ٷ��ʿ�Ŀ¼
#define O_BOOK_PATH O_PATH "/book"

// �γ̿����ص�ַ
#define OBOOK_DOWN_URL "http://down.langisle.com/003/obook/"


enum wordType
{
	eUWord = 1,
	eOWord = 2
};

enum bookType		// �γ�����
{
	btUser = 1,			// �û�
	btOfficial = 2		// �ٷ�
};

enum categoryType	// ��������
{
	ctMyself = 1,	// �ҵ�
	ctUser = 2,     // �û� 
	ctOfficial = 3,	// �ٷ�
};

typedef QMap<QString, QVariant> BOOKINFO;
typedef QMap<QString, QVariant> WordInfo;
