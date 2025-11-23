# 🖥️ Analisador Sintático MicroPascal  
Projeto acadêmico — Linguagens Formais e Autômatos / Compiladores  
Aluno: **Myguel Arcanjo**

---

## 📌 Descrição do Projeto

Este projeto implementa um **analisador sintático para a linguagem MicroPascal**, utilizando a técnica de **descida recursiva**, conforme especificação do trabalho acadêmico.

O sistema contém:

- **Analisador Léxico:** identifica tokens como palavras reservadas, operadores, números e identificadores.  
- **Analisador Sintático:** implementado com funções recursivas seguindo a gramática fornecida.  
- **CasaToken():** valida tokens e exibe mensagens de erro no formato exigido.  
- **AST (Árvore Sintática):** é construída durante o parsing e exibida em formato hierárquico.

---

## 📂 Estrutura do Projeto

- ANALISADOR_LEXI/
- │
- ├── ast.c
- ├── ast.h
- ├── lexer.c
- ├── lexer.h
- ├── parser.c
- ├── parser.h
- ├── token.c
- ├── token.h
- ├── main.c
- ├── Makefile
- │
- └── tests/
- ├── certo1.pas
- ├── certo2.pas
- ├── certo3.pas
- ├── errado1.pas
- ├── errado2.pas
- └── errado3.pas


## ⚙️ Compilação
- **gcc -c token.c
- **gcc -c lexer.c
- **gcc -c ast.c
- **gcc -c parser.c
- **gcc -c main.c
- **gcc -o analisador.exe token.o lexer.o ast.o parser.o main.o

## Execucação 

- **.\analisador.exe tests\certo1.pas
- **.\analisador.exe tests\certo2.pas
- **.\analisador.exe tests\certo3.pas
- **.\analisador.exe tests\errado1.pas
- **.\analisador.exe tests\errado2.pas
- **.\analisador.exe tests\errado3.pas

