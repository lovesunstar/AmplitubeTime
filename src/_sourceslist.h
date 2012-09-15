#ifndef SOURCESLIST_H
#define SOURCESLIST_H
//
#include <QtGui>
#include "atcwidget.h"
//
class SourcesList : public QWidget
{
Q_OBJECT

	QBoxLayout *layout;
	QList<ATCWidget*> atc_list;

public:
	SourcesList();
	~SourcesList();
	
	void addSource(QString name, int freq, int sample_size);
	
public slots:
	void setScale(float scale_);
};
#endif
