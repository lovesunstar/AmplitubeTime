#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H
//
#include <QtGui>
#include <aspell.h>
//
class SpellChecker : public QObject
{
Q_OBJECT
	AspellConfig * m_spell_config_ru;
	AspellSpeller * m_spell_checker_ru;
	AspellConfig * m_spell_config_en;
	AspellSpeller * m_spell_checker_en;

	QTextCodec * m_codec;

protected:

public:
	SpellChecker(QObject * parent=0);
	~SpellChecker();
	
	bool check(QString word);
	QStringList suggestions(QString word);
};
#endif
