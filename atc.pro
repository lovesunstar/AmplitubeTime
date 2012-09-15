TEMPLATE = app
QT = gui core opengl
CONFIG += qt \
 release \
 warn_on \
 console \
 stl
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
HEADERS = src/mainwindow.h \
# src/atcwidget.h \
# src/sourceslist.h \
# src/audiosource.h \
# src/audioplayer.h \
# src/source_edit_dialog.h \
# src/volume_edit_widget.h \
# src/equalizer.h \
# src/config.h \
 src/track.h \
 src/time_ruler.h \
 src/audio_track.h \
 src/track_browser.h \
 src/source.h \
 src/audio_source.h \
 src/data_stream.h \
 src/directory_stream.h \
 src/apu_chain.h \
 src/apu.h \
 src/apu_lowpass.h \
 src/apu_highpass.h \
 src/apu_bandpass.h \
 src/apu_peak.h \
 src/apu_ladspa.h \
 src/FFT.h \
 src/apu_spectrum_view.h \
 src/filters.h \
 src/linear_filter.h \
 src/afc_widget.h \
 src/afc_widget_peak.h \
 src/afc_widget_lhpf.h \
 src/afc_widget_bpf.h \
 src/spell_checker.h \
 src/spell_highlighter.h \
 src/grammatical_text_edit.h
SOURCES = src/mainwindow.cpp \
 src/main.cpp \
# src/atcwidget.cpp \
# src/sourceslist.cpp \
# src/audiosource.cpp \
# src/audioplayer.cpp \
# src/source_edit_dialog.cpp \
# src/volume_edit_widget.cpp \
# src/equalizer.cpp \
 src/track.cpp \
 src/time_ruler.cpp \
 src/audio_track.cpp \
 src/track_browser.cpp \
 src/source.cpp \
 src/audio_source.cpp \
 src/data_stream.cpp \
 src/directory_stream.cpp \
 src/apu_chain.cpp \
 src/apu.cpp \
 src/apu_lowpass.cpp \
 src/apu_highpass.cpp \
 src/apu_bandpass.cpp \
 src/apu_peak.cpp \
 src/apu_ladspa.cpp \
 src/FFT.cpp \
 src/apu_spectrum_view.cpp \
 src/filters.cpp \
 src/linear_filter.cpp \
 src/afc_widget.cpp \
 src/afc_widget_peak.cpp \
 src/afc_widget_lhpf.cpp \
 src/afc_widget_bpf.cpp \
 src/spell_checker.cpp \
 src/spell_highlighter.cpp \
 src/grammatical_text_edit.cpp
RESOURCES += resources.qrc
INCLUDEPATH += P:\include
LIBS += -lopenal32 -laspell-15 -LP:\lib
TRANSLATIONS = tr/ru_RU.ts

