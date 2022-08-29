compiler: ./dist/converter.o ./dist/parser.o ./dist/lexer.o ./dist/toJson.o ./dist/frame.o ./dist/parse.o ./dist/translate.o ./dist/tree.o ./dist/printtree.o ./dist/temp.o ./dist/escape.o ./dist/env.o ./dist/types.o ./dist/preprocessor.o ./dist/preprocessor.tab.o ./dist/pp.yy.o ./dist/ppsemant.o ./dist/absyn.o ./dist/symbol.o ./dist/table.o ./dist/errormsg.o ./dist/util.o ./dist/semantic.o ./dist/lexAnalyze.o 
	g++ -O2 -o compiler -g ./dist/converter.o ./dist/parser.o ./dist/lexer.o ./dist/toJson.o ./dist/frame.o ./dist/parse.o ./dist/translate.o ./dist/tree.o ./dist/temp.o ./dist/escape.o ./dist/env.o ./dist/types.o ./dist/preprocessor.o ./dist/preprocessor.tab.o ./dist/pp.yy.o ./dist/ppsemant.o ./dist/absyn.o ./dist/symbol.o ./dist/table.o ./dist/errormsg.o ./dist/util.o ./dist/semantic.o ./dist/printtree.o ./dist/lexAnalyze.o -lm -ljson-c

./dist/lexAnalyze.o: ./src/lexAnalyze.cpp ./src/errormsg.hpp ./src/util.h
	g++ -O2 -g -c ./src/lexAnalyze.cpp -o ./dist/lexAnalyze.o

./dist/converter.o: ./src/converter.cpp
	g++ -O2 -g -c ./src/converter.cpp -lnlohmann_json -std=c++17 -o ./dist/converter.o

./dist/parser.o: ./src/parser.cpp
	g++ -O2 -g -c ./src/parser.cpp -lnlohmann_json -std=c++17 -o ./dist/parser.o

./dist/lexer.o: ./src/lexer.cpp
	g++ -O2 -g -c ./src/lexer.cpp -lnlohmann_json -o ./dist/lexer.o

./dist/toJson.o: ./src/toJson.cpp ./src/toJson.hpp
	g++ -O2 -g -c ./src/toJson.cpp -ljson-c -o ./dist/toJson.o

./dist/parse.o: ./src/parse.cpp ./src/parse.h
	g++ -O2 -g -c ./src/parse.cpp -o ./dist/parse.o

./dist/semantic.o: ./src/semantic.cpp ./src/semantic.hpp ./src/wasm_frame.c
	g++ -O2 -g -c ./src/semantic.cpp -o ./dist/semantic.o 

./dist/translate.o: ./src/translate.c ./src/translate.h
	g++ -O2 -g -c ./src/translate.c -lm -o ./dist/translate.o

./dist/tree.o: ./src/tree.c ./src/tree.h
	g++ -O2 -g -c ./src/tree.c -o ./dist/tree.o

./dist/printtree.o: ./src/printtree.cpp ./src/printtree.hpp
	g++ -O2 -g -c ./src/printtree.cpp -o ./dist/printtree.o

./dist/frame.o: ./src/wasm_frame.c ./src/wasm_frame.h ./src/frame.h 
	g++ -O2 -g -c ./src/wasm_frame.c -o ./dist/frame.o

./dist/temp.o: ./src/temp.c ./src/temp.h
	g++ -O2 -g -c ./src/temp.c -o ./dist/temp.o

./dist/escape.o: ./src/escape.c ./src/escape.h
	g++ -O2 -g -c ./src/escape.c -o ./dist/escape.o

./dist/env.o: ./src/env.cpp ./src/env.hpp
	g++ -O2 -g -c ./src/env.cpp -o ./dist/env.o

./dist/types.o: ./src/types.c ./src/types.h
	g++ -O2 -g -c ./src/types.c -o ./dist/types.o
	
./dist/preprocessor.tab.o: ./src/preprocessor.tab.c ./src/preprocessor.c
	g++ -O2 -g -c -DYYERROR_VERBOSE -DYYDEBUG=1 ./src/preprocessor.tab.c -o ./dist/preprocessor.tab.o

./src/preprocessor.tab.c: ./src/preprocessor.y
	yacc --name-prefix=pre -dv -v ./src/preprocessor.y -o ./src/preprocessor.tab.c 

./src/preprocessor.tab.h: ./src/preprocessor.tab.c
	echo "created preprocessor.tab.h"

./dist/absyn.o: ./src/absyn.c ./src/absyn.h
	g++ -O2 -g -c ./src/absyn.c -o ./dist/absyn.o

./dist/table.o: ./src/table.c ./src/table.h
	g++ -O2 -g -c ./src/table.c -fpermissive -o ./dist/table.o

./dist/symbol.o: ./src/symbol.c ./src/symbol.h ./src/table.h
	g++ -O2 -g -c ./src/symbol.c -o ./dist/symbol.o

./dist/errormsg.o: ./src/errormsg.cpp ./src/errormsg.hpp ./src/util.h
	g++ -O2 -g -c ./src/errormsg.cpp -o ./dist/errormsg.o

./dist/pp.yy.o: ./src/pp.yy.c ./src/errormsg.hpp
	g++ -O2 -g -c ./src/pp.yy.c -o ./dist/pp.yy.o

./src/pp.yy.c: ./src/preprocessor.l
	lex --prefix="pre" -o ./src/pp.yy.c -w ./src/preprocessor.l 

./dist/preprocessor.o: ./src/preprocessor.c ./src/preprocessor.h
	g++ -O2 -g -c ./src/preprocessor.c -o ./dist/preprocessor.o

./dist/ppsemant.o: ./src/ppsemant.cpp ./src/ppsemant.hpp
	g++ -O2 -g -c ./src/ppsemant.cpp -o ./dist/ppsemant.o

./dist/util.o: ./src/util.c ./src/util.h
	g++ -O2 -g -c ./src/util.c -o ./dist/util.o

clean: 
	rm -f compiler ./dist/converter.o ./dist/lexer.o ./dist/parser.o ./dist/lexAnalyze.o ./dist/parse.o ./dist/semantic.o ./dist/translate.o ./dist/tree.o ./dist/printtree.o ./dist/frame.o ./dist/temp.o ./dist/escape.o ./dist/env.o ./dist/types.o ./dist/y.tab.o ./dist/absyn.o ./dist/symbol.o ./dist/table.o ./dist/lex.yy.o ./dist/errormsg.o ./dist/util.o ./dist/preprocessor.o ./dist/preprocessor.tab.o ./dist/ppsemant.o
