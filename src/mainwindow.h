#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//
#include <QtGui>
//#include "atcwidget.h"
//#include "audioplayer.h"
//#include "source_edit_dialog.h"
//#include "volume_edit_widget.h"
#include "apu_chain.h"

#include "track_browser.h"
//
class MainWindow : public QMainWindow
{
Q_OBJECT

//	QDockWidget * wave_view_dock;
//	QDateTimeEdit * m_date_edit;
//	QPushButton * m_autoscroll_button;
	
//	VolumeEditWidget * m_volume_widget;
	
//	SourceEditDialog *m_source_edit_dialog;
	QBoxLayout *m_sources_box;
//	ATCWidget *m_main_atc;
//	QList<ATCWidget*> m_atc_list;
	TrackBrowser track_browser;
	
//	AudioPlayer m_player;
	ApuChain * m_apu_chain;
	float m_player_jump_len;
	
//	QMenu *m_source_context_menu;
//	ATCWidget *m_context_menu_atc;
	
//	QAction * m_act_play;
//	QAction * m_act_repeat;
//	QAction * m_act_volume;
	QAction * m_act_about;
//	QAction * m_act_save;
	QAction * m_act_quit;

//	QAction * m_act_jump_left;
//	QAction * m_act_jump_right;
	
//	QAction * m_act_add_source;
//	QAction * m_act_edit_source;
//	QAction * m_act_delete_source;
	
	QSettings m_settings;
	
//	QIcon m_play_icon;
//	QIcon m_stop_icon;

public:
	MainWindow( QWidget * parent = 0, Qt::WFlags f = 0 );
	~MainWindow();
	
protected:
	void timerEvent(QTimerEvent *event);
	
private slots:
//	void addAudioSource(AudioSource *au);
//	void removeAudioSource(AudioSource *au);
//	void setSourcesScale(float scale);
//	void setSourcesOffset(ATCWidget::offset_t offset);
//	void setSourcesOffset(QDateTime datetime);
//	void setSourceSelected();
//	void setSourcesAutoscroll(bool autoscroll);
//	void mainATCOffsetChange(ATCWidget::offset_t offset);
//	void mainATCScaleChange(float scale);
//	void updateMainATCView();
	
//	void dayBackward();
//	void dayForward();
	
//	void play();
//	void saveFragment();
	
//	void playerJumpLeft();
//	void playerJumpRight();
	
//	void sourceContextMenuPopup(QPoint);
//	void sourceContextMenuEdit();
//	void sourceContextMenuDelete();
	
	void showAbout();
	
	void loadSettings();
	void saveSettings();
};
#endif




