#pragma once

#include "QtnProperty/Core/Auxiliary/PropertyAux.h"

struct IQtnPropertyStateProvider
{
	virtual ~IQtnPropertyStateProvider() {}

	virtual QtnPropertyState getPropertyState(const QMetaProperty &metaProperty) const = 0;
};
