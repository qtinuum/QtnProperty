#pragma once

#include <QMetaObject>

#include <vector>

class QtnConnections : public std::vector<QMetaObject::Connection>
{
public:
	void disconnect();

	~QtnConnections();
};
