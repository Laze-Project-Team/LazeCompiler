# LazeCompiler
Lazeのコンパイラ
# Lazeとは
Lazeはブラウザ上で実行する多言語対応プログラミング言語です。
プログラミングを学習し始めたいけど英語やプログラミング前の環境構築が難しくて手こずっている人のための言語です。

##初心者の方へ
初心者の方はこのリポジトリのコンパイラのコマンドを使うのではなく、[こちら](https://laze.ddns.net/editor)のウェブサイトでプログラミングを始めてみてください。
[こちら](https://laze.ddns.net)がホームページです。

##このコンパイラをローカルで使いたい方へ
使用OSはWindows 10でUbuntu 20.04のWSL2です。

# Lazeに必要なライブラリ
・nlohmann
```
sudo apt-get install nlohmann-json3-dev
```
・jsonc
```
sudo apt-get install libjson-c-dev
```
・lex(flex)
```
sudo apt-get install flex
```
・yacc(bison)
```
sudo apt-get install bison
```

# Lazeのコマンド
##コンパイル用
```
./compiler <ファイル名> -c <ファイルのディレクトリ(compilerとコンパイルするファイルが同じフォルダーなら要らない)> --mode compile --parse-json <言語ファイル(.json)> --parser-opt <言語ファイル(.parser)>
```
###例
compilerとtest.lazeが同じファイルにある時、日本語のプログラムをコンパイルする
```
./compiler test.laze --mode compiler --parse-json ./lang_files/jalang.json --parser-opt ./parsers/jalang.parser
```
##言語変換用
```
./compiler <ファイル名> -c <ファイルのディレクトリ(compilerと変換するファイルが同じフォルダーなら要らない)> --mode convert --parse-json <言語ファイル(.json)> --parser-opt <言語ファイル(.parser)> --convert-json <変換先の言語ファイル>
```
###例
compilerとtest.lazeが同じファイルにある時、日本語のプログラムを英語のプログラムに変換する
```
./compiler test.laze --mode convert --parse-json ./lang_files/jalang.json --parser-opt ./parsers/jalang.parser --convert-json ./lang_files/enlang.json
```
##パーサー生成用
```
./compiler --mode parserload --parse-json <言語ファイル(.json)> --parser-output <パーサーの生成先>
```
###例
```
./compiler --mode parserload --parse-json ./lang_files/jalang.json --parser-output ./lang_files/jalang.parser
```
