#include "spell_checker.h"
//
SpellChecker::SpellChecker( QObject * parent ) 
	: QObject(parent)
{
	m_spell_config_ru = new_aspell_config();
	aspell_config_replace(m_spell_config_ru, "dict-dir", "./dict");
	aspell_config_replace(m_spell_config_ru, "encoding", "utf-8");
	aspell_config_replace(m_spell_config_ru, "lang", "ru");
	AspellCanHaveError * possible_err = new_aspell_speller(m_spell_config_ru);
	m_spell_checker_ru = 0;
	if (aspell_error_number(possible_err) != 0){
		puts(aspell_error_message(possible_err));
	}
	else{
		m_spell_checker_ru = to_aspell_speller(possible_err); 
	}


	m_spell_config_en = new_aspell_config();
	aspell_config_replace(m_spell_config_en, "dict-dir", "./dict");
	aspell_config_replace(m_spell_config_en, "encoding", "utf-8");
	aspell_config_replace(m_spell_config_en, "lang", "en");
	possible_err = new_aspell_speller(m_spell_config_en);
	m_spell_checker_en = 0;
	if (aspell_error_number(possible_err) != 0){
		puts(aspell_error_message(possible_err));
	}
	else{
		m_spell_checker_en = to_aspell_speller(possible_err); 
	}

	m_codec = QTextCodec::codecForName("UTF-8");
}
//
SpellChecker::~SpellChecker()
{
	delete_aspell_speller(m_spell_checker_ru);
	delete_aspell_config(m_spell_config_ru);
	delete_aspell_speller(m_spell_checker_en);
	delete_aspell_config(m_spell_config_en);
}
//
bool SpellChecker::check(QString word)
{
	if(word.isEmpty()) return true;

	AspellSpeller * checker;
	if(word[0].toLower()>=QChar('a') && word[0].toLower()<=QChar('z')){
		checker = m_spell_checker_en;
	}
	else{
		checker = m_spell_checker_ru;
	}
	if(!checker) return true;
	
	return (bool)aspell_speller_check(checker, m_codec->fromUnicode(word).data(), -1);
}

QStringList SpellChecker::suggestions(QString word)
{
	QStringList ret;

	if(word.isEmpty()) return ret;

	AspellSpeller * checker;
	if(word[0].toLower()>=QChar('a') && word[0].toLower()<=QChar('z')){
		checker = m_spell_checker_en;
	}
	else{
		checker = m_spell_checker_ru;
	}
	if(!checker) return ret;

	const AspellWordList * suggestions = aspell_speller_suggest(checker, m_codec->fromUnicode(word).data(), -1);
	AspellStringEnumeration * elements = aspell_word_list_elements(suggestions);
	const char * suggestion;
	while ( suggestion = aspell_string_enumeration_next(elements) ) {
		ret << m_codec->toUnicode(suggestion);
	}
	delete_aspell_string_enumeration(elements);

	return ret;
}
