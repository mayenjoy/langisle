#include "stdafx.h"
#include "RemindMgr.h"
#include "LIConfig.h"
#include "BCMgr.h"
#include "DlgRemind.h"
#include "GlobalVal.h"
#include "LIWord0030.h"

RemindMgr::RemindMgr(QObject *parent) : QObject(parent)
{
	m_bEnableRemind = g_pbcmgr->getConfig()->getValue("RemindMgr/isEnableRemind", true).toBool();
	m_bEnableHightLight = g_pbcmgr->getConfig()->getValue("RemindMgr/bEnableHightLight", true).toBool();
	m_nThreshold = g_pbcmgr->getConfig()->getValue("RemindMgr/Threshold", 30).toInt();

	timerID = startTimer(1000);
	timerEventCount = 1;
	tempDisable = false;
	dlgRemind = new DlgRemind(0, this);
	m_nCheckSpacing = 480;//5·ÖÖÓ 
	//m_nCheckSpacing = 10;//5·ÖÖÓ 
}

RemindMgr::~RemindMgr(void)
{
	killTimer(timerID);
	delete dlgRemind;
}


void RemindMgr::setEnableRemind(bool bEnable)
{
	g_pbcmgr->getConfig()->setValue("RemindMgr/isEnableRemind", bEnable);
	tempDisable = !bEnable;
	m_bEnableRemind = bEnable;
}

void RemindMgr::setEnableHightLight(bool bEnable)
{
	g_pbcmgr->getConfig()->setValue("RemindMgr/bEnableHightLight", bEnable);
	m_bEnableHightLight = bEnable;
}


void RemindMgr::setThreshold(int nVal)
{
	g_pbcmgr->getConfig()->setValue("RemindMgr/Threshold", nVal);
	m_nThreshold = nVal;
}


void RemindMgr::timerEvent(QTimerEvent *te)
{
	QObject::timerEvent(te);
	
	//DBG(qDebug() << "RemindMgr::timerEvent" << "¼ì²â" << timerEventCount, m_bEnableRemind, tempDisable);

	if (GlobalVal::s_pmw->isShowDlgRemind() && m_bEnableRemind && !tempDisable && timerEventCount % m_nCheckSpacing == 0)
	{
		// ¼ì²â
		int count = g_pbcmgr->getUDataDB()->getReviewCount();
		if (count > m_nThreshold)
		{
			dlgRemind->updateWeb(count);
			QTimer::singleShot(0, dlgRemind, SLOT(topShow()));
			dlgRemind->exec();
			DBG(qDebug() << "DlgRemind::timerEvent 1");
		}
		else
		{
			DBG(qDebug() << "RemindMgr::timerEvent ¸´Ï°Êý²»×ã" << count << m_nThreshold);
		}
		
	}

	timerEventCount++;
}
