#pragma once
#include "ExportWordBase.h"

class ExportWordQA :
	public ExportWordBase
{
public:
	ExportWordQA(void);
	~ExportWordQA(void);
	void WriteFile( QString & fileName );
};
