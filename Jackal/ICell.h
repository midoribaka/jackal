#pragma once

#include "CenteredRoundedRect.h"

class ICell : public CenteredRoundedRect
{
	Q_OBJECT

public:
	virtual int mask() const = 0;
};