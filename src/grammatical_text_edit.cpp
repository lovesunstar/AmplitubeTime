#include "grammatical_text_edit.h"
//
GrammaticalTextEdit::GrammaticalTextEdit( QWidget * parent ) 
	: QTextEdit(parent),
	m_icon(":/images/spellcheck.png")
{
	m_spell_highlighter = new SpellHighlighter(&m_sc, this);
}
//

void GrammaticalTextEdit::contextMenuEvent(QContextMenuEvent * event)
{
	QTextCursor begin_cursor = cursorForPosition(event->pos());
	begin_cursor.movePosition(QTextCursor::StartOfWord);
	QTextCursor end_cursor = cursorForPosition(event->pos());
	end_cursor.movePosition(QTextCursor::EndOfWord);

	QString text = toPlainText();
	QString word = QString(text.data()+begin_cursor.position(), end_cursor.position()-begin_cursor.position());

	QMenu * menu = createStandardContextMenu();
	
	if(!m_sc.check(word)){
		QStringList suggestions = m_sc.suggestions(word);
		if(!suggestions.empty()){
			int limit = max_suggestions;
			QMenu * spell_menu = new QMenu(tr("Spelling suggestions"));
			spell_menu->setIcon(m_icon);
	
			foreach(QString w, suggestions){
				QAction * act = spell_menu->addAction(w);
				act->setProperty("word_start", begin_cursor.position());
				act->setProperty("word_end", end_cursor.position());
				act->setProperty("new_word", w);
				connect(act, SIGNAL(triggered()), this, SLOT(selectSuggestion()) );
	
				if(--limit<=0) break;
			}
		
			QAction * insert_pos_act=0;
			if(menu->children().size()>1){
				insert_pos_act = (QAction*)(menu->children()[1]);
			}
			menu->insertMenu(insert_pos_act, spell_menu);
			menu->insertSeparator(insert_pos_act);
		}
	}
	
	menu->exec(event->globalPos());
	delete menu;
}

void GrammaticalTextEdit::selectSuggestion()
{
	int start = sender()->property("word_start").toInt();
	int end = sender()->property("word_end").toInt();
	QString nword = sender()->property("new_word").toString();

	QTextCursor cursor = textCursor();
	cursor.setPosition(start);
	cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, end-start);
	cursor.deleteChar();
	cursor.insertText(nword);
}
