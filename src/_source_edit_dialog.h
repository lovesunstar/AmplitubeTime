#ifndef SOURCE_EDIT_DIALOG_H
#define SOURCE_EDIT_DIALOG_H
//
#include <QtGui>
#include "audiosource.h"
//
class SourceEditDialog : public QDialog
{
Q_OBJECT

	AudioSource *m_au;
	
	QMap<int, int> m_freq_map;
	QMap<int, QPair<int, int> > m_format_map;
	
	QLineEdit * m_filename_edit;
	QComboBox * m_freq_combo;
	QComboBox * m_format_combo;
	QComboBox * m_format_signed_combo;
	QComboBox * m_swap_samples_order_combo;
	
public:
	SourceEditDialog(QWidget *parent);
	
private slots:
	void onOk();
	void selectLocation();
	
public slots:
	void editSource(AudioSource *au);
	void newSource();
	
signals:
	void sourceCreated(AudioSource *);
	void sourceModified(AudioSource *);
};
#endif
