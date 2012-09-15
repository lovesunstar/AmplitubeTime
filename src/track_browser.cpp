#include "track_browser.h"
#include "audio_track.h"
#include "directory_stream.h"
//
TrackBrowser::TrackBrowser( QWidget * parent ) 
	: QWidget(parent)
{
	QBoxLayout * main_layout = new QVBoxLayout;

	main_layout->addLayout(createCalendarLayout());
	main_layout->setContentsMargins(2,2,2,2);
	main_layout->addSpacing(6);

	m_track_box = new QVBoxLayout;
	main_layout->addLayout(m_track_box);
	m_track_box->setSpacing(0);
	m_track_box->setContentsMargins(0,0,0,0);

	m_ruler = new TimeRuler;
	m_track_box->addWidget(m_ruler);
	m_track_box->addWidget(new AudioTrack(new AudioSource(new DirectoryStream("source2"))));

	m_ruler->setBegin(time(0));
	m_ruler->setRange(30);

	setLayout(main_layout);
}
//
TrackBrowser::~TrackBrowser()
{
}
//
QLayout * TrackBrowser::createCalendarLayout()
{
	QPushButton * day_forward = new QPushButton(QIcon(":images/forward.png"), "");
	QPushButton * day_backward = new QPushButton(QIcon(":images/backward.png"), "");
	QSizePolicy size_policy;
	size_policy = day_forward->sizePolicy();
	//size_policy.setHorizontalStretch(0);
	size_policy.setHorizontalPolicy( QSizePolicy::Minimum );
	day_backward->setSizePolicy(size_policy);
	day_forward->setSizePolicy(size_policy);
	day_backward->setToolTip(tr("Day backward"));
	day_forward->setToolTip(tr("Day forward"));

//	connect(day_backward, SIGNAL(clicked()), this, SLOT(dayBackward()));
//	connect(day_forward, SIGNAL(clicked()), this, SLOT(dayForward()));
	
	m_date_edit = new QDateTimeEdit(this);
	m_date_edit->setCalendarPopup(true);
	m_date_edit->calendarWidget()->setFirstDayOfWeek(Qt::Monday);
//	connect(m_date_edit, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(setSourcesOffset(QDateTime)) );
	size_policy = m_date_edit->sizePolicy();
	//size_policy.setHorizontalStretch(1);
	size_policy.setHorizontalPolicy ( QSizePolicy::Expanding );
	m_date_edit->setSizePolicy(size_policy);

	m_autoscroll_button = new QPushButton(QIcon(":images/double_arrow_right.png"), tr("Autoscroll"));
	m_autoscroll_button->setCheckable(true);
	
	QBoxLayout *calendar_box = new QHBoxLayout;
	calendar_box->addWidget(day_backward);
	calendar_box->addSpacing(2);
	calendar_box->addWidget(m_date_edit);
	calendar_box->addSpacing(2);
	calendar_box->addWidget(day_forward);
	calendar_box->addSpacing(6);
	calendar_box->addWidget(m_autoscroll_button);

	return calendar_box;
}

//void TrackBrowser::addSource(Source * source)
//{
//}

//void TrackBrowser::removeSource(Source * source)
//{
//}
