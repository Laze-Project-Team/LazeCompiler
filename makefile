compiler: parser.o lexer.o toJson.o frame.o parse.o translate.o tree.o printtree.o temp.o escape.o env.o types.o y.tab.o preprocessor.o preprocessor.tab.o pp.yy.o ppsemant.o absyn.o symbol.o table.o lex.yy.o errormsg.o util.o semantic.o lexAnalyze.o 
	g++ -o compiler -g parser.o lexer.o toJson.o frame.o parse.o translate.o tree.o temp.o escape.o env.o types.o y.tab.o preprocessor.o preprocessor.tab.o pp.yy.o ppsemant.o absyn.o symbol.o table.o lex.yy.o errormsg.o util.o semantic.o printtree.o lexAnalyze.o -lm -ljson-c

lexAnalyze.o: lexAnalyze.cpp errormsg.h util.h
	g++ -g -c lexAnalyze.cpp

parser.o: parser.cpp
	g++ -g -c parser.cpp -lnlohmann_json -std=c++17

lexer.o: lexer.cpp
	g++ -g -c lexer.cpp -lnlohmann_json

toJson.o: toJson.c toJson.h
	g++ -g -c toJson.c -ljson-c

parse.o: parse.c parse.h
	g++ -g -c parse.c

semantic.o: semantic.c semantic.h wasm_frame.c
	g++ -g -c semantic.c wasm_frame.c

translate.o: translate.c translate.h
	g++ -g -c translate.c -lm

tree.o: tree.c tree.h
	g++ -g -c tree.c

printtree.o: printtree.c printtree.h
	g++ -g -c printtree.c

frame.o: wasm_frame.c wasm_frame.h frame.h 
	g++ -o frame.o -g -c wasm_frame.c

temp.o: temp.c temp.h
	g++ -g -c temp.c

escape.o: escape.c escape.h
	g++ -g -c escape.c

env.o: env.c env.h
	g++ -g -c env.c

types.o: types.c types.h
	g++ -g -c types.c

y.tab.o: y.tab.c
	g++ -g -c -DYYERROR_VERBOSE -DYYDEBUG=1 y.tab.c

y.tab.c: lang.y
	yacc -dv -v lang.y

y.tab.h: y.tab.c
	echo "created y.tab.h"

preprocessor.tab.o: preprocessor.tab.c preprocessor.c
	g++ -g -c -DYYERROR_VERBOSE -DYYDEBUG=1 preprocessor.tab.c preprocessor.c

preprocessor.tab.c: preprocessor.y
	yacc --name-prefix=pre -dv -v preprocessor.y -o preprocessor.tab.c 

preprocessor.tab.h: preprocessor.tab.c
	echo "created preprocessor.tab.h"

absyn.o: absyn.c absyn.h
	g++ -g -c absyn.c

table.o: table.c table.h
	g++ -g -c table.c -fpermissive

symbol.o: symbol.c symbol.h table.h
	g++ -g -c symbol.c

errormsg.o: errormsg.c errormsg.h util.h
	g++ -g -c errormsg.c

lex.yy.o: lex.yy.c errormsg.h util.h
	g++ -g -c lex.yy.c

lex.yy.c: lang.l
	lex -w lang.l

pp.yy.o: pp.yy.c errormsg.h
	g++ -g -c pp.yy.c

pp.yy.c: preprocessor.l
	lex --prefix="pre" -o pp.yy.c -w preprocessor.l 

preprocessor.o: preprocessor.c preprocessor.h
	g++ -g -c preprocessor.c

ppsemant.o: ppsemant.cpp ppsemant.hpp
	g++ -g -c ppsemant.cpp

util.o: util.c util.h
	g++ -g -c util.c

clean: 
	rm -f compiler lexer.o parser.o lexAnalyze.o parse.o semantic.o translate.o tree.o printtree.o frame.o temp.o escape.o env.o types.o y.tab.o absyn.o symbol.o table.o lex.yy.o errormsg.o util.o preprocessor.o preprocessor.tab.o ppsemant.o
