#pragma once
#include "LMBase.h"

class LMFree :
	public LMBase
{
public:
	LMFree(UDataDB *puddb);
	virtual ~LMFree(void);

	virtual int getNextWord();
	virtual void updateMInfo(QMap<QString, QVariant> &tval);
};
