#include "QtnConnections.h"

#include <QObject>

void QtnConnections::disconnect()
{
	for (auto &connection : *this)
		QObject::disconnect(connection);

	clear();
}

QtnConnections::~QtnConnections()
{
	if (!empty())
		disconnect();
}
