#pragma once

#include "Auxiliary/PropertyAux.h"

struct QTN_IMPORT_EXPORT IQtnPropertyStateProvider
{
	virtual ~IQtnPropertyStateProvider() {}

	virtual QtnPropertyState getPropertyState(const QMetaProperty &metaProperty) const = 0;
};
