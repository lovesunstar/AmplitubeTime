#ifndef APU_CHAIN_H
#define APU_CHAIN_H
//
#include <QtGui>
#include "apu.h"
#include "apu_spectrum_view.h"
#include "afc_widget.h"
//
class ApuChain : public QWidget
{
Q_OBJECT

	int m_freq;

	SpectrumView * m_spectrum_in;
	SpectrumView * m_spectrum_out;
	
	QStandardItemModel * m_apu_list;
	QTreeView * m_apu_list_view;
	
	QList<Apu*> m_apu;
	QMap<QWidget*, QString> m_apu_type;
	QWidget * m_apu_widget_preview;
	Apu * m_apu_preview;
	QBoxLayout * m_apu_queue_layout;
	QBoxLayout * m_apu_queue_vlayout;

	QPushButton * m_remove_apu_button;
	QPushButton * m_moveup_apu_button;
	QPushButton * m_movedown_apu_button;
	QPushButton * m_coeffs_button;

	AFCWidget * m_final_afc;

	void selectApuWidget(QWidget * apu, Apu * apu);
	Apu * addApu(QString type, void * data);
	Apu * addApu(Apu * apu, QString apu_type);

	QMenu * createAddApuMenu();
	
private slots:
	void addApuAction();
	void moveUpApu();
	void moveDownApu();
	void removeApu();

	void viewFilterCoeffs();
	void updateFinalAFC();

	void apuListSelected(QItemSelection sel);

	void saveSettings();
	void loadSettings();
	
public:
	ApuChain();
	
	void processData(short * data, long num_samples, long long start_time);//start_time in msec
	void setFreq(long freq);
	void reset();
	void clear();

	void saveSettings(QString fname);
	void loadSettings(QString fname);

};
#endif
