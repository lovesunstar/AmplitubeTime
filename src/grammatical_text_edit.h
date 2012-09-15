#ifndef GRAMMATICAL_TEXT_EDIT_H
#define GRAMMATICAL_TEXT_EDIT_H
//
#include <QtGui>
#include "spell_highlighter.h"
//
class GrammaticalTextEdit : public QTextEdit
{
Q_OBJECT

	enum {max_suggestions=10};
	SpellChecker m_sc;
	SpellHighlighter * m_spell_highlighter;
	
	QIcon m_icon;

private slots:
	void selectSuggestion();

public:
	GrammaticalTextEdit(QWidget * parent=0);
	
protected:
	void contextMenuEvent(QContextMenuEvent * event);
};
#endif
