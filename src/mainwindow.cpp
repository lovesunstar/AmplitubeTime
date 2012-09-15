#include "mainwindow.h"
//#include "sourceslist.h"
//#include "audiosource.h"
//#include "spell_checker.h"
#include "grammatical_text_edit.h"
//
MainWindow::MainWindow( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f),
	m_settings("config.ini", QSettings::IniFormat)
//	m_play_icon(":/images/play.png"),
//	m_stop_icon(":/images/stop.png")
{
	qDebug() << "Current timestamp" << QDateTime::currentDateTime().toTime_t() << QString::number(QDateTime::currentDateTime().toTime_t(), 16);
	
//	QIcon *icon;
//	m_act_play = new QAction(m_play_icon, tr("&Play selected range"), this);
//	icon = new QIcon(":/images/repeat.png");
//	icon->addFile(":/images/repeat_pressed.png", QSize(), QIcon::Active, QIcon::On);
//	m_act_repeat = new QAction(*icon, tr("&Toggles repeating"), this);
//	m_act_volume = new QAction(QIcon(":/images/volume.png"), tr("&Sound volume"), this);
	m_act_about = new QAction(QIcon(":/images/info.png"), tr("About"), this);
//	m_act_save = new QAction(QIcon(":/images/save.png"), tr("&Save fragment"), this);
	m_act_quit = new QAction(QIcon(":/images/quit.png"), tr("&Quit"), this);

//	m_act_jump_left = new QAction(QIcon(":/images/2left.png"), tr("Jump left"), this);
//	m_act_jump_right = new QAction(QIcon(":/images/2right.png"), tr("Jump right"), this);

//	m_act_play->setShortcut(QKeySequence(Qt::Key_Space));
//	m_act_jump_left->setShortcut(QKeySequence(Qt::Key_Left));
//	m_act_jump_right->setShortcut(QKeySequence(Qt::Key_Right));

//	m_act_jump_left->setShortcutContext(Qt::ApplicationShortcut);
	
	
//	connect(m_act_play, SIGNAL(triggered()), this, SLOT( play() ) );
//	connect(m_act_repeat, SIGNAL(toggled(bool)), &m_player, SLOT( setLooped(bool) ) );
//	connect(m_act_save, SIGNAL(triggered()), this, SLOT(saveFragment()) );
	
	connect(m_act_about, SIGNAL(triggered()), this, SLOT(showAbout()));
	connect(m_act_quit, SIGNAL(triggered()), this, SLOT(close()));
//	connect(m_act_jump_left, SIGNAL(triggered()), this, SLOT(playerJumpLeft()) );
//	connect(m_act_jump_right, SIGNAL(triggered()), this, SLOT(playerJumpRight()) );
	
//	m_volume_widget = new VolumeEditWidget(this);
//	connect(m_volume_widget, SIGNAL(volumeChanged(float)), &m_player, SLOT(setVolume(float)) );
//	connect(m_act_volume, SIGNAL(triggered()), m_volume_widget, SLOT(popup()) );
	
	
	
//	QWidget *sources_list = new QWidget;
//	m_sources_box = new QVBoxLayout;
//	sources_list->setLayout(m_sources_box);
//	m_sources_box->setContentsMargins(2,2,2,2);
//	m_sources_box->setSpacing(0);
	
//	connect(m_autoscroll_button, SIGNAL(clicked(bool)), this, SLOT(setSourcesAutoscroll(bool)) );
	
	
	
//	ATCWidget *atc;
//	atc = new ATCWidget(new AudioSource("", 1 , 192000, true), this);
//	m_atc_list.push_back(atc);
//	m_sources_box->addWidget(atc);
//	atc->setScrollbarSize(0);
//	atc->setTimingbarSize(25);
//	atc->setMinimumHeight(25);
//	atc->setMaximumHeight(25);
//	atc->setAutoscroll(true);
//	atc->setScale(0.7);
//	atc->setSelectionMode(ATCWidget::NoSelectionMode);
//	connect(atc, SIGNAL(offsetChanged(ATCWidget::offset_t)), this, SLOT(setSourcesOffset(ATCWidget::offset_t)) );
//	connect(atc, SIGNAL(scaleChanged(float)), this, SLOT(setSourcesScale(float)) );
	
//	m_main_atc = new ATCWidget(NULL, this);
//	m_main_atc->setScale(20.0);
//	m_main_atc->setScrollbarSize(0);
//	m_main_atc->setMinimumHeight(130);
//	m_main_atc->setScalingPivot(ATCWidget::SelectionPivot);
//	connect(m_main_atc, SIGNAL(offsetChanged(ATCWidget::offset_t)), this, SLOT(mainATCOffsetChange(ATCWidget::offset_t)) );
//	connect(m_main_atc, SIGNAL(scaleChanged(float)), this, SLOT(mainATCScaleChange(float)) );
//	connect(m_main_atc, SIGNAL(selectionChanged(ATCWidget::offset_t, ATCWidget::offset_t)), this, SLOT( updateMainATCView() ) );
	
//	m_source_edit_dialog = new SourceEditDialog(this);
//	connect(m_source_edit_dialog, SIGNAL(sourceCreated(AudioSource*)), this, SLOT(addAudioSource(AudioSource*)));
	
//	QPushButton *new_source_button = new QPushButton(QIcon(":images/add.png"), tr("Add source"));
//	connect(new_source_button, SIGNAL(clicked()), m_source_edit_dialog, SLOT(newSource()) );
//	QBoxLayout *add_source_box = new QHBoxLayout;
//	add_source_box->addStretch(1);
//	add_source_box->addWidget(new_source_button);
//	m_sources_box->addSpacing(6);
//	m_sources_box->addLayout(add_source_box);
	
	
//	m_act_repeat->setCheckable(true);
//	QToolBar *playback = new QToolBar;
//	playback->addAction(m_act_play);
//	playback->addAction(m_act_repeat);
//	playback->addAction(m_act_volume);
//	playback->addSeparator();
//	playback->addAction(m_act_jump_left);
//	playback->addAction(m_act_jump_right);
//	playback->addSeparator();
//	playback->addAction(m_act_save);
	
	
//	QBoxLayout * main_view_box = new QVBoxLayout;
//	main_view_box->setSpacing(2);
//	main_view_box->setContentsMargins(0,0,0,0);
	//main_view_box->addStretch(1);
	//main_view_box->addLayout(m_spectre_widgets_layout);
//	main_view_box->addWidget(playback);
	
//	QWidget * main_view_widget = new QWidget;
//	main_view_widget->setLayout(main_view_box);
	
//	QSplitter * m_main_view_splitter = new QSplitter(Qt::Vertical);
//	m_main_view_splitter->setChildrenCollapsible(false);
//	m_main_view_splitter->addWidget(m_main_atc);
//	m_main_view_splitter->addWidget(main_view_widget);
//	m_main_view_splitter->setStretchFactor(0, 1);
//	m_main_view_splitter->setStretchFactor(1, 0);
	
	
//	wave_view_dock = new QDockWidget(tr("Wave view"), this);
//	wave_view_dock->setObjectName("main_window_wave_view_dock");
//	wave_view_dock->setWidget(m_main_view_splitter);
//	addDockWidget(Qt::TopDockWidgetArea, wave_view_dock);
	
	QSplitter * sources_splitter = new QSplitter(Qt::Vertical);
	sources_splitter->setChildrenCollapsible(false);
	sources_splitter->addWidget(&track_browser);
	sources_splitter->addWidget(new QWidget);
	QDockWidget *sources_list_dock = new QDockWidget(tr("Multimedia sources"), this);
	sources_list_dock->setObjectName("main_window_sources_list_dock");
	sources_list_dock->setWidget(sources_splitter);
	addDockWidget(Qt::LeftDockWidgetArea, sources_list_dock);
	
	setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
	QTabWidget * tab_widget = new QTabWidget;
	setCentralWidget(tab_widget);
	
	m_apu_chain = new ApuChain;
	tab_widget->addTab(m_apu_chain, QIcon(":images/kmix.png"), tr("Audio Processing"));
//	m_player.setApuChain(m_apu_chain);

	int tab_num = tab_widget->addTab(new QWidget, QIcon(":images/videotape.png"), tr("Video Processing"));
	tab_widget->setTabEnabled(tab_num, false);

	QTextEdit * text_widget = new GrammaticalTextEdit; 
	tab_widget->addTab(text_widget, QIcon(":images/notepad.png"), tr("Text"));
	
	
//	m_act_add_source = new QAction(QIcon(":/images/add.png"), tr("Add"), this);
//	m_act_edit_source = new QAction(QIcon(":/images/edit.png"), tr("Edit"), this);
//	m_act_delete_source = new QAction(QIcon(":/images/delete.png"), tr("Delete"), this);
//	connect(m_act_add_source, SIGNAL(triggered()), m_source_edit_dialog, SLOT(newSource()));
//	connect(m_act_edit_source, SIGNAL(triggered()), this, SLOT(sourceContextMenuEdit()));
//	connect(m_act_delete_source, SIGNAL(triggered()), this, SLOT(sourceContextMenuDelete()));
//	m_source_context_menu = new QMenu;
//	m_source_context_menu->addAction(m_act_add_source);
//	m_source_context_menu->addAction(m_act_edit_source);
//	m_source_context_menu->addAction(m_act_delete_source);
	
	//MENUS
	QMenu * file_menu = menuBar()->addMenu(tr("&File"));
//	file_menu->addAction(m_act_add_source);
//	file_menu->addAction(m_act_save);
	file_menu->addSeparator();
	file_menu->addAction(m_act_quit);
	
	QMenu * about_menu = menuBar()->addMenu(tr("&About"));
	about_menu->addAction(m_act_about);
	
	
	loadSettings();
	startTimer(50);
}
//
MainWindow::~MainWindow()
{
	saveSettings();
}
//

//void MainWindow::dayBackward()
//{
//	m_date_edit->setDateTime( m_date_edit->dateTime().addDays(-1) );
//}

//void MainWindow::dayForward()
//{
//	m_date_edit->setDateTime( m_date_edit->dateTime().addDays(+1) );
//}

//void MainWindow::playerJumpLeft()
//{
//	m_player.jump(-5);
//}

//void MainWindow::playerJumpRight()
//{
//	m_player.jump(+5);
//}

//void MainWindow::addAudioSource(AudioSource *au)
//{
//	ATCWidget *atc;
//	atc = new ATCWidget(au);
//	atc->setScrollbarSize(0);
//	atc->setTimingbarSize(0);
//	atc->setMinimumHeight(30);
//	atc->setAutoscroll(true);
//	atc->setSelectionMode(ATCWidget::PositionSelectionMode);
//	
//	atc->setMinimumScale(0.002);
//	atc->setMaximumScale(0.3);
//	
//	atc->enableSelectionTracking(true);
//	atc->setScalingPivot(ATCWidget::SelectionPivot);
//	
//	m_atc_list.push_back(atc);
//	//m_sources_box->addWidget(atc);
//	m_sources_box->insertWidget(m_sources_box->count()-2, atc);
//	
//	connect(atc, SIGNAL(offsetChanged(ATCWidget::offset_t)), this, SLOT(setSourcesOffset(ATCWidget::offset_t)) );
//	connect(atc, SIGNAL(scaleChanged(float)), this, SLOT(setSourcesScale(float)) );
//	connect(atc, SIGNAL(selectionChanged(ATCWidget::offset_t, ATCWidget::offset_t)), this, SLOT( setSourceSelected() ) );
//	connect(atc, SIGNAL(contextMenuRequested(QPoint)), this, SLOT(sourceContextMenuPopup(QPoint)) );
//	
//	atc->setScale(m_atc_list.front()->getScale());
//	
//	saveSettings();
//}

//void MainWindow::removeAudioSource(AudioSource *au)
//{
//	foreach(ATCWidget *atc, m_atc_list){
//		if(atc == m_atc_list.front()) continue;
//		
//		if(au == atc->getAudioSource()){
//			if(m_player.playing() && m_player.getAudioSource()==au){
//				m_player.stop();
//			}
//			if(au == m_main_atc->getAudioSource()){
//				m_main_atc->setAudioSource(0);
//				//m_spectre_widget.setAudioSource(0);
//			}
//			m_sources_box->removeWidget(atc);
//			m_atc_list.removeOne(atc);
//			delete atc;
//			delete au;
//		}
//	}
//	
//	saveSettings();
//}

//void MainWindow::setSourcesScale(float scale)
//{
//	foreach(ATCWidget *wid, m_atc_list){
//		wid->setScale(scale);
//	}
//}
/*
void MainWindow::setSourcesOffset(ATCWidget::offset_t offset)
{
	static bool in=false;
	if(in)return;
	in=true;
	
	foreach(ATCWidget *atc, m_atc_list){
		atc->setOffset(offset);
	}
	
	if(!m_atc_list.empty()){
		QDateTime new_datetime=QDateTime::fromTime_t(0);
		new_datetime=new_datetime.addSecs((int)(offset+m_atc_list.first()->viewLength()));
		
		if(m_date_edit->dateTime()!=new_datetime){
			m_date_edit->setDateTime(new_datetime);
		}
		m_autoscroll_button->setChecked(m_atc_list.first()->isAutoscrolling());
	}
	
	in=false;
}

void MainWindow::setSourcesOffset(QDateTime datetime)
{
	static bool in=false;
	if(in)return;
	in=true;
	
	if(!m_atc_list.empty()){
		ATCWidget::offset_t offset;
		offset = (ATCWidget::offset_t)(datetime.toTime_t())
			+datetime.time().msec()/1000;
		
		setSourcesOffset(offset-m_atc_list.first()->viewLength());
		//foreach(ATCWidget *atc, m_atc_list){
//			atc->setOffset(offset);
//		}
		
	}
	
	in=false;
}

void MainWindow::updateMainATCView()
{
	QDateTime current_date=QDateTime::fromTime_t(0);
	current_date=current_date.addMSecs((qint64)m_main_atc->getOffset()*1000);
	
	QDateTime view_range=QDateTime::fromTime_t(0).toUTC();
	view_range=view_range.addMSecs((qint64)m_main_atc->viewLength()*1000);

	QDateTime selected_range=QDateTime::fromTime_t(0).toUTC();
	selected_range=selected_range.addMSecs((qint64)(m_main_atc->selection_end-m_main_atc->selection_begin)*1000);

	QString title;
	title += current_date.toString("dddd dd MMMM yyyy");
	title += " ["+tr("View range")+": " + view_range.toString("hh:mm:ss.zzz") + " " + QString::number((long long)(m_main_atc->viewLength()*m_main_atc->getAudioSource()->getFreq())) + " samples]";
	title += " ["+tr("Selected range")+": " + selected_range.toString("hh:mm:ss.zzz") + " " + QString::number((long long)((m_main_atc->selection_end-m_main_atc->selection_begin)*m_main_atc->getAudioSource()->getFreq())) + " samples]";
	
	wave_view_dock->setWindowTitle( title );
}

void MainWindow::mainATCOffsetChange(ATCWidget::offset_t offset)
{
	updateMainATCView();
}

void MainWindow::mainATCScaleChange(float scale)
{
	updateMainATCView();
}

void MainWindow::setSourceSelected()
{
	if(!sender()){
		qDebug() << "warning: MainWindow::setSourceSelected() called without sender";
		return;
	}
	
	
	m_main_atc->disconnect( SIGNAL(viewChanged(ATCWidget::offset_t, ATCWidget::offset_t)) );
	
	foreach(ATCWidget *atc, m_atc_list){
		if(atc==sender()){
			connect(m_main_atc, SIGNAL(viewChanged(ATCWidget::offset_t, ATCWidget::offset_t)), atc, SLOT(setSelectionRange(ATCWidget::offset_t, ATCWidget::offset_t)));
			atc->setBackground(QColor(200,230,135));
			atc->selection_end = atc->selection_begin + m_main_atc->viewLength();
			m_main_atc->setAudioSource( atc->getAudioSource() );
			//m_main_atc->setSelectionRange(0, 0);
			m_main_atc->setOffset(atc->selection_begin);
			//qDebug() << "m_main_atc->setOffset(" << atc->selection_begin << ")";
			//m_spectre_widget.setAudioSource(m_main_atc->getAudioSource());
		}
		else{
			atc->setBackground(QColor(225,225,225));
			atc->selection_begin = 0;
			atc->selection_end = 0;
		}
	}
}

void MainWindow::sourceContextMenuDelete()
{
	removeAudioSource(m_context_menu_atc->getAudioSource());
}
void MainWindow::sourceContextMenuEdit()
{
	m_source_edit_dialog->editSource(m_context_menu_atc->getAudioSource());
}
void MainWindow::sourceContextMenuPopup(QPoint pos)
{
	m_context_menu_atc = (ATCWidget*)sender();
	m_source_context_menu->popup(pos);
}

void MainWindow::setSourcesAutoscroll(bool autoscroll)
{
	if(!m_atc_list.empty() && autoscroll){
		setSourcesOffset((long long)(QDateTime::currentDateTime().toTime_t())*m_atc_list.first()->getAudioSource()->getFreq()-m_atc_list.first()->viewLength() );
	}
	
	foreach(ATCWidget *atc, m_atc_list){
		atc->setAutoscroll(autoscroll);
	}
}

void MainWindow::play()
{
	if(!m_main_atc->getAudioSource()){
		QMessageBox::warning(this, tr("Note"),
			tr("Select audio source, please!"));
		return;
	}
	if(m_player.playing()){
		m_player.stop();
		return;
	}
	//m_player.play( m_main_atc->getAudioSource(), m_main_atc->selection_begin+m_main_atc->crop_begin, m_main_atc->selection_end+m_main_atc->crop_begin);
	ATCWidget::offset_t play_begin = m_main_atc->selection_begin;
	ATCWidget::offset_t play_end = m_main_atc->selection_end;
	if(play_begin == play_end){
		play_end = m_main_atc->getOffset() + m_main_atc->viewLength();
	}
	if(play_begin == 0){
		play_begin = play_begin + m_main_atc->getOffset();
		play_end = play_begin + m_main_atc->viewLength();
	}
	
	m_player.play( m_main_atc->getAudioSource(), 
		(long long)(play_begin * m_main_atc->getAudioSource()->getFreq()),
		(long long)(play_end * m_main_atc->getAudioSource()->getFreq())
		);
	
	//m_spectre_widget.setAutoUpdating(true);
	
	m_act_play->setIcon(m_stop_icon);
}
*/

void MainWindow::showAbout()
{
	QMessageBox::about(this, tr("About message"), (const char*)QResource(":about.html").data());
}


#define MIN(x, y) (x>y?y:x)

/*
void MainWindow::saveFragment()
{
	if (!m_main_atc->getAudioSource() || m_main_atc->selection_begin==m_main_atc->selection_end) {
		QMessageBox::warning(this, tr("Note"),
			tr("Fragment not selected"));
		return;
	}
	
	
	QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "ttt", "WAVE audio (*.wav)");
	
	if(filename=="") return;
	
	QFile file(filename);
	
	if (!file.open(QFile::WriteOnly)) {
		QMessageBox::warning(this, tr("Note"),
			tr("Cannot write file %1:\n%2.")
				.arg(filename)
				.arg(file.errorString()));
		return;
	}
	
	QApplication::setOverrideCursor(Qt::WaitCursor);
	
	int i32;
	short i16;
	
	
	file.write("RIFF");
	file.write("____");
	file.write("WAVEfmt ");
	i32=16;
	file.write((char*)&i32, 4);
	
	//WAVE type format
	i16=1;
	file.write((char*)&i16, 2);
	
	//number of channels
	i16=1;
	file.write((char*)&i16, 2);
	
	//sample rate
	int freq = m_main_atc->getAudioSource()->getFreq();
	i32=freq;
	file.write((char*)&i32, 4);
	
	//bytes/sec
	int bps = m_main_atc->getAudioSource()->getBytesInSample();
	i32 = bps * freq;
	file.write((char*)&i32, 4);
	
	//block align
	i16=bps;
	file.write((char*)&i16, 2);
	
	//bits/sample
	i16 = bps*8;
	file.write((char*)&i16, 2);
	
	//data
	file.write("data");
	
	//data size
	int data_size_pos = file.pos();
	file.write("____");
	
	//audio data writing here
	long long begin = (long long)(m_main_atc->selection_begin*freq);
	long long end = (long long)(m_main_atc->selection_end*freq);
	
	char * temp_data = new char[(end-begin)*bps];
	m_main_atc->getAudioSource()->getData(begin, end, temp_data);
	file.write(temp_data, (end-begin) * bps);
	delete[] temp_data;
	
	//char *noise_data = new char[50000];
	//file.write(noise_data, 50000);
	//delete []noise_data;
	
	i32 = file.pos() - data_size_pos;
	file.seek(data_size_pos);
	file.write((char*)&i32, 4);
	
	i32 = file.size() - 8;//filesize - 8
	file.seek(4);
	file.write((char*)&i32, 4);
	
	file.close();
	
	QApplication::restoreOverrideCursor();
	
	//setCurrentFile(filename);
	//statusBar()->showMessage(tr("File saved"), 2000);
}
*/
void MainWindow::timerEvent(QTimerEvent *event)
{
/*
	if(!m_player.playing()){
		m_act_play->setIcon(m_play_icon);
		m_main_atc->setPlayPosition( 0 );
	}
	else{
		m_main_atc->setPlayPosition( m_player.currentPosition() );
	}
*/
}


void MainWindow::loadSettings()
{
	m_settings.beginGroup("main_window");
	QByteArray state = m_settings.value("state", "").toByteArray();
	int x = m_settings.value("x").toInt();
	int y = m_settings.value("y").toInt();
	int prev_width = m_settings.value("width", 640).toInt();
	int prev_height = m_settings.value("height", 480).toInt();
	m_settings.endGroup();
	
	restoreState(state);
	move(x,y);
	resize(prev_width, prev_height);

	m_settings.beginGroup("player");
	m_player_jump_len = float( m_settings.value("player_jump", 5.0).toDouble() );
	m_settings.endGroup();
	
	m_settings.beginGroup("sources");
	int size = m_settings.beginReadArray("sources");
	for(int i=0; i<size; ++i){
		m_settings.setArrayIndex(i);
/*
		QString location = m_settings.value("location").toString();
		int bps = m_settings.value("sample_bytes").toInt();
		int freq = m_settings.value("freq").toInt();
		bool format_signed = m_settings.value("sample_format_signed").toBool();
		bool swap_samples_order = m_settings.value("swap_samples_order").toBool();
		
		AudioSource *au = new AudioSource(location, bps, freq, format_signed);
		au->setSwapSamplesOrder(swap_samples_order);
		addAudioSource(au);
*/
	}
	m_settings.endArray();
	m_settings.endGroup();
}

void MainWindow::saveSettings()
{
	m_settings.beginGroup("main_window");
	m_settings.setValue("state", QVariant(saveState()) );
	m_settings.setValue("x", QVariant(x()) );
	m_settings.setValue("y", QVariant(y()) );
	m_settings.setValue("width", QVariant(width()) );
	m_settings.setValue("height", QVariant(height()) );
	m_settings.endGroup();
	
	m_settings.beginGroup("player");
	m_settings.setValue("player_jump", QVariant(m_player_jump_len) );
	m_settings.endGroup();
	
	m_settings.beginGroup("sources");
	m_settings.remove("sources");
	m_settings.beginWriteArray("sources");
/*
	int index=0;
	foreach(ATCWidget * atc, m_atc_list){
		AudioSource * au = atc->getAudioSource();
		if(!au)continue;
		
		m_settings.setArrayIndex(index);
		m_settings.setValue("location", au->getDirectory());
		m_settings.setValue("sample_bytes", au->getBytesInSample());
		m_settings.setValue("freq", au->getFreq());
		m_settings.setValue("sample_format_signed", au->getFormatSigned());
		m_settings.setValue("swap_samples_order", au->getSwapSamplesOrder());
		++index;
	}
*/
	m_settings.endArray();
	m_settings.endGroup();

	m_settings.sync();
}
