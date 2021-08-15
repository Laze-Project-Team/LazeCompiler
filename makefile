compiler: frame.o parse.o translate.o tree.o printtree.o temp.o escape.o env.o types.o y.tab.o preprocessor.o preprocessor.tab.o pp.yy.o ppsemant.o absyn.o symbol.o table.o lex.yy.o errormsg.o util.o semantic.o lexAnalyze.o 
	cc -o compiler -g frame.o parse.o translate.o tree.o temp.o escape.o env.o types.o y.tab.o preprocessor.o preprocessor.tab.o pp.yy.o ppsemant.o absyn.o symbol.o table.o lex.yy.o errormsg.o util.o semantic.o printtree.o lexAnalyze.o -lm

lexAnalyze.o: lexAnalyze.c errormsg.h util.h
	cc -g -c lexAnalyze.c

parse.o: parse.c parse.h
	cc -g -c parse.c

semantic.o: semantic.c semantic.h wasm_frame.c
	cc -g -c semantic.c wasm_frame.c

translate.o: translate.c translate.h
	cc -g -c translate.c -lm

tree.o: tree.c tree.h
	cc -g -c tree.c

printtree.o: printtree.c printtree.h
	cc -g -c printtree.c

frame.o: wasm_frame.c wasm_frame.h frame.h 
	cc -o frame.o -g -c wasm_frame.c

temp.o: temp.c temp.h
	cc -g -c temp.c

escape.o: escape.c escape.h
	cc -g -c escape.c

env.o: env.c env.h
	cc -g -c env.c

types.o: types.c types.h
	cc -g -c types.c

y.tab.o: y.tab.c
	cc -g -c -DYYERROR_VERBOSE -DYYDEBUG=1 y.tab.c

y.tab.c: lang.y
	yacc -dv -v lang.y

y.tab.h: y.tab.c
	echo "created y.tab.h"

preprocessor.tab.o: preprocessor.tab.c preprocessor.c
	cc -g -c -DYYERROR_VERBOSE -DYYDEBUG=1 preprocessor.tab.c preprocessor.c

preprocessor.tab.c: preprocessor.y
	yacc --name-prefix=pre -dv -v preprocessor.y -o preprocessor.tab.c 

preprocessor.tab.h: preprocessor.tab.c
	echo "created preprocessor.tab.h"

absyn.o: absyn.c absyn.h
	cc -g -c absyn.c

table.o: table.c table.h
	cc -g -c table.c

symbol.o: symbol.c symbol.h table.h
	cc -g -c symbol.c

errormsg.o: errormsg.c errormsg.h util.h
	cc -g -c errormsg.c

lex.yy.o: lex.yy.c errormsg.h util.h
	cc -g -c lex.yy.c

lex.yy.c: lang.l
	lex -w lang.l

pp.yy.o: pp.yy.c errormsg.h
	cc -g -c pp.yy.c

pp.yy.c: preprocessor.l
	lex --prefix="pre" -o pp.yy.c -w preprocessor.l 

preprocessor.o: preprocessor.c preprocessor.h
	cc -g -c preprocessor.c

ppsemant.o: ppsemant.c ppsemant.h
	cc -g -c ppsemant.c

util.o: util.c util.h
	cc -g -c util.c

clean: 
	rm -f compiler lexAnalyze.o parse.o semantic.o translate.o tree.o printtree.o frame.o temp.o escape.o env.o types.o y.tab.o absyn.o symbol.o table.o lex.yy.o errormsg.o util.o preprocessor.o preprocessor.tab.o ppsemant.o
