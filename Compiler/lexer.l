%{
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int linha_atual;
extern int errors_num;
%}

%option noyywrap

%x COMMENT

DIGITO  [0-9]
LETRA   [a-zA-Z_]
ID      {LETRA}({LETRA}|{DIGITO})*
NUM     {DIGITO}+

%%

"/*"            { BEGIN(COMMENT); } /* Inicia o comentário */
<COMMENT>"*/"   { BEGIN(INITIAL); } /* Finaliza o comentário */
<COMMENT>\n     { linha_atual++; }   /* Conta linhas dentro do comentário */
<COMMENT>.      { /* Ignora qualquer caractere dentro do comentário */ }
<COMMENT><<EOF>> { 
    fprintf(stderr, "ERRO LEXICO: Comentario nao fechado. LINHA: %d\n", linha_atual); 
    errors_num++; 
    return 0;      
}

"int"           { return INT; }
"void"          { return VOID; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"return"        { return RETURN; }

"<="            { return LE; }
"<"             { return LT; }
">"             { return GT; }
">="            { return GE; }
"=="            { return EQ; }
"!="            { return NE; }

"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULT; }
"/"             { return DIV; }

"="             { return ASSIGN; }

"("             { return LPAREN; }
")"             { return RPAREN; }
"["             { return LBRACKET; }
"]"             { return RBRACKET; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
";"             { return SEMICOLON; }
","             { return COMMA; }

{ID}            { yylval.id = strdup(yytext); return ID; }
{NUM}           { yylval.id = strdup(yytext); return NUM; }

[ \t\r]+        ; 
\n              { linha_atual++; } 
.               { 
    fprintf(stderr, "ERRO LEXICO: '%s' LINHA: %d\n", yytext, linha_atual); 
    errors_num++;  
    return 0;      
}

%%