# What is Laze?
Laze is a multilingual programming language that runs in your browser.
It is a language for people who want to start learning programming but are daunted by the difficulties of building an English language or pre-programming environment.

## For beginners
Beginners should start programming at the website [here](https://laze.ddns.net/editor) instead of using the compiler commands in this repository.
[Here](https://laze.ddns.net) is the home page.

## For those who want to use this compiler locally
The OS used is WSL2 on Ubuntu 20.04 with Windows 10.

## Libraries required for Laze

nlohmann

```
sudo apt-get install nlohmann-json3-dev
```

jsonc

```
sudo apt-get install libjson-c-dev
```

lex(flex)

```
sudo apt-get install flex
```

yacc(bison)

```
sudo apt-get install bison
```

## Laze commands

### For compiling

```
./compiler <filename> -c <dir> --mode compile --parse-json <lang-file> --parser-opt <parser-file>
```

| argument name | content |
| :--- | :--- |
| filename | name of the file to compile |
| dir | directory of files (can be omitted if compiler and files to be compiled are in the same folder) |
| lang-file | language file (.json) |
| parser-file | parser file (.parser) |

#### Example

Compiling a Japanese program when compiler and test.lazy are in the same file

```
./compiler test.laze --mode compile --parse-json . /lang_files/jalang.json --parser-opt . /parsers/jalang.parser
```

### For language conversion

```
./compiler <filaname> -c <dir> --mode convert --parse-json <lang-file> --parser-opt <parser-file> --convert-json <convert-lang-file>
```

| argument name | content |
| :--- | :--- |
| filename | the name of the file to compile |
| dir | directory of files (can be omitted if compiler and files to be compiled are in the same folder) |
| lang-file | language file (.json) |
| parser-file | parser file (.parser) |
| convert-lang-file | language file of the language to convert (.json) |

#### Example
  
Convert a Japanese program to an English program when compiler and test.laze are in the same file

```
./compiler test.laze --mode convert --parse-json . /lang_files/jalang.json --parser-opt . /parsers/jalang.parser --convert-json . /lang_files/enlang.json
```

### for parser generation

```
./compiler --mode parserload --parse-json <lang-file> --parser-output <parer-file>
```

| argument-name | argument-content |
| :--- | :--- |
| lang-file | language file (.json) |
| parser-file | destination parser file |

#### Example.

```
./compiler --mode parserload --parse-json . /lang_files/jalang.json --parser-output . /lang_files/jalang.parser
```
