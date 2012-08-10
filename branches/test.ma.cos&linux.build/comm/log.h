#ifndef LOG_H
#define LOG_H


//#define __DBG           // ע�ͺ󲻱���DBG�еĴ���
//#define __LOG_CONSOLE   // ע�ͺ������־������̨
//#define __LOG_FILE      // ע�ͺ������־���ļ�

#ifdef __DBG
#define DBG(x) x
#else
#define DBG(x)
#endif

#include <QDebug>

extern void MyMsgHandler(QtMsgType, const char *);

#endif // LOG_H
