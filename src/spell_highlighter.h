#ifndef SPELL_HIGHLIGHTER_H
#define SPELL_HIGHLIGHTER_H
//
#include <QSyntaxHighlighter>
#include "spell_checker.h"
//
class SpellHighlighter : public QSyntaxHighlighter
{
Q_OBJECT
	SpellChecker * m_sc;

public:
	SpellHighlighter(SpellChecker * sc, QTextEdit * parent);
	
protected:
	void highlightBlock(const QString & text);
};
#endif
