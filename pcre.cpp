#include "pcre.hpp"

// what is empty string??

myPCRE::myPCRE( const std::string &pattern )
:re(NULL)
{
	compile( pattern  );
}

myPCRE::~myPCRE( )
{
	if(re){
	pcre_free(re);
	}
}

void
myPCRE::compile(const std::string &pattern  )
{
	std::cout << "pattern: (" << pattern << ");" << std::endl;
	
	const char *error;
	int erroffset;
	const unsigned char *tables = NULL;

	if(re){
	pcre_free(re);
	}

	// создание таблицы перекодировки для локали ru
	setlocale (LC_CTYPE, (const char *) "ru.");
	tables = pcre_maketables();

	// компилирование регулярного выражения во внутреннее представление
	
	int options = 0;

	re = pcre_compile (pattern.c_str(), options, &error, &erroffset, NULL);
}

std::string
myPCRE::test(const std::string &str)
{
	char *msg = myPCRE::test(str.c_str());
	std::string res;
	if(msg != NULL){
		res = std::string(msg);
		delete[] msg;
		msg = NULL;
	}
	else{
		res = std::string("");
	}
	return res;
}

char *
myPCRE::test(const char *str)
{
	if (str == NULL){
		return NULL;
	}

//	std::cout << ">>" << str.c_str() << ";" << std::endl;
	if (!re){ // if compile error
		std::cout << "Failed regexp" << std::endl;
	}
	else{
		unsigned int count = 0;
		int ovector[30];
 
		count = pcre_exec (re, NULL, str, strlen(str), 0, NULL, ovector, 30);
		// выполнение сопоставления с образцом
		if (!count){ // если нет совпадений
		// FIXME Я сейчас пишу на русском - я делаю плохо :)
		// В боевом коде мы никогда не используем русский - ни как есть ни в транслитерации
		// (Stroka - никогда, String - всегда)
		//! no match
			std::cout << "No match" << std::endl;
			return NULL;
		}
		else{
			if ((ovector[0] < 0)or(ovector[1] > strlen(str) )){
				// или <unset> для несопоставившихся подвыражений
				std::cout << "<unset>" << ovector[0] << " " << ovector[1] << std::endl;
				return NULL;
			}
			else{
				std::cout << "regular:" << ovector[0] << " " << ovector[1] << " ";

				int len = ovector[1] - ovector[0];
				char *msg = new char[len+1];
				memcpy(msg, (str + ovector[0]), len + 1 );
				msg[len] = '\0';

				std::cout << msg << std::endl;
				return msg;
			}
		}
	}
	return NULL;
}
