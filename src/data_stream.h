#ifndef __DATA_STREAM_H
#define __DATA_STREAM_H
//
#include <QtGui>
//
class DataStream : public QObject
{
Q_OBJECT

protected:

public:
	DataStream(QObject * parent=0);
	virtual ~DataStream();
	
};
#endif
