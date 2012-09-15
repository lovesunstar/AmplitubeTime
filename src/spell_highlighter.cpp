#include "spell_highlighter.h"
//
SpellHighlighter::SpellHighlighter( SpellChecker * sc, QTextEdit * parent ) 
	: QSyntaxHighlighter(parent)
{
	m_sc = sc;
}
//

void SpellHighlighter::highlightBlock(const QString & text)
{
	QTextCharFormat error_format;
	QTextCharFormat normal_format;
	error_format.setUnderlineColor(QColor(Qt::red));
	error_format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);

	for(int i=0; i<text.length(); ++i){
		if(!text[i].isLetter()) continue;
		int end=i;
		while(end<text.length() && text[end].isLetter()) ++end;

		if(m_sc->check(QString(text.data()+i, end-i))){
			setFormat(i, end-i, normal_format);
		}
		else{
			setFormat(i, end-i, error_format);
		}
		
		i=end;
	}
}
