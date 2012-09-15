#ifndef __SOURCE_H
#define __SOURCE_H
//
#include <QtGui>
//
class Source : public QObject
{
Q_OBJECT

protected:

public:
	Source(QObject * parent=0);
	virtual ~Source();
	
};
#endif
