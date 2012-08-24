#include "stdafx.h"
#include "LMBase.h"

LMBase::LMBase(UDataDB *puddb) : pUDDB(puddb)
{
	nscount = 0;
	rvcount = 0;
	kncount = 0;
	tscount = 0;
	startTime  = QDateTime::currentDateTime();
	m_GWOrder = EGWabc;
}

LMBase::~LMBase(void)
{
}
