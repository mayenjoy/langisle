#include "stdafx.h"
#include "SpeakBase.h"

SpeakBase::SpeakBase(const QString &name, QObject *parent) : m_name(name)
{
	reset();
}

SpeakBase::~SpeakBase(void)
{
}
