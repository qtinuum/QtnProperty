%option 8bit
%option warn
%option yylineno
%option noyywrap
%option stack
%option bison-bridge bison-locations

%x ASSIGN_STATE
%x LINE_COMMENT_STATE
%x CPP_COMMENT_STATE
%x CPP_STRING_STATE
%x CPP_CHAR_STATE
%x INCLUDE_STATE
%x CPP_CODE_STATE
%x CPP_CODE_STATE2
%x STRING_STATE
%x INITIALIZATION_LIST_STATE

%{
#include "PropertyEnumGeneratorCommon.h"
#include "PropertyEnum.parser.hpp"

int yycolumn = 1;
int yy_bracket_count = 0;

extern void yyerror(/*YYLTYPE *yylloc,*/ char const *s);

#define YY_USER_ACTION yylloc->first_line = yylloc->last_line = yylineno; \
    yylloc->first_column = yycolumn; yylloc->last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;
%}

%%

[ \t]+                      /* skip whitespaces */
[\r\n]                      { yycolumn = 1; }

#include|#include_h         { yy_push_state(INCLUDE_STATE); return INCLUDE_H; }
#include_cpp                { yy_push_state(INCLUDE_STATE); return INCLUDE_CPP; }
code_h                      { return CODE_H; }
code_cpp                    { return CODE_CPP; }
property_set                { return PROPERTY_SET; }
property                    { return PROPERTY; }
extern                      { return EXTERN; }
slot                        { return SLOT; }
delegate                    { return DELEGATE; }
enum                        { return ENUM; }
public|protected|private    { *yylval = yytext; return ACCESS_ID; }
[[:alpha:]_]+[[:alnum:]_]*  { *yylval = yytext; return ID; }
"{"                         { return CPP_BRACKET_OPEN; }
"}"                         { return CPP_BRACKET_CLOSE; }
"("                         { return '('; }
")"                         { return ')'; }
";"                         { return SEMICOLON; }
":"                         { return COLON; }
"::"                        { return DBL_COLON; }
","                         { return COMMA; }
"."                         { return DOT; }
"~"                         { return TILDE; }
"="                         { yy_push_state(ASSIGN_STATE); return ASSIGN; }
[+-]?[[:digit:]]+           { *yylval = yytext; return NUMBER; }
[LR]?\"                     { yymore(); yy_push_state(STRING_STATE); yy_push_state(CPP_STRING_STATE); }

.                           {
    printf( "Unrecognized character: %s\n", yytext ); yyerror("scan error"); exit(1);
}

<STRING_STATE>{
.                           {
                                yy_pop_state();
                                char lastChar = yytext[yyleng-1];
                                yytext[yyleng-1] = '\0';
                                *yylval = yytext;
                                unput(lastChar);
                                return STR;
                            }
}

<INCLUDE_STATE>{
[^\n]*                      {
                                yy_pop_state();
                                *yylval = yytext;
                                return INCLUDE_NAME;
                            }
}

<ASSIGN_STATE>{
"/*"                        { yy_push_state(CPP_COMMENT_STATE); }
"}"                         { printf( "Unbalanced } bracket\n"); yyerror("scan error"); exit(1); }
"{"                         { yymore(); yy_push_state(CPP_CODE_STATE2); yy_bracket_count = 1; }
[^\";{}]+                   { yymore(); }
[LR]?\"                     { yymore(); yy_push_state(CPP_STRING_STATE); }
;                           {
                                yy_pop_state();
                                // skip ';' char
                                yytext[yyleng-1] = '\0';
                                *yylval = yytext;
                                *yylval = yylval->trimmed();
                                return VALUE_TO_ASSIGN;
                            }
}

<CPP_STRING_STATE>{
[^\\\"]+                    { yymore(); }
"\\\""                      { yymore(); }
"\\"[^\"]                   { yymore(); }
"\""[ \t]*"\""              { yymore(); }
"\""[\r\n]*"\""             { yycolumn = 1; yymore(); }
\"                          { yymore(); yy_pop_state(); }
}

<INITIAL,CPP_CODE_STATE,CPP_CODE_STATE2,ASSIGN_STATE>"//" { yy_push_state(LINE_COMMENT_STATE); }

<LINE_COMMENT_STATE>{
[^\r\n]*                    { /* skip line comment */ yy_pop_state(); }
}

<INITIAL,CPP_CODE_STATE,CPP_CODE_STATE2,ASSIGN_STATE,INITIALIZATION_LIST_STATE>"/*" { yy_push_state(CPP_COMMENT_STATE); }

<CPP_COMMENT_STATE>{
[^*]*                       { /* skip cpp comment */ }
"*"+[^/]                    { /* skip cpp comment */ }
"*"+"/"                     { yy_pop_state(); }
}

<CPP_CODE_STATE>{
[^{}]*                      { yymore(); }
"{"                         { yymore(); ++yy_bracket_count; }
"}"                         {
                                --yy_bracket_count;
                                if (yy_bracket_count == 0) {
                                    yy_pop_state();
                                    // skip close bracked
                                    yytext[yyleng-1] = '\0';
                                    *yylval = yytext;
                                    return CPP_CODE;
                                } else {
                                    yymore();
                                }
                            }
}

<CPP_CODE_STATE2>{
[^{}]*                      { yymore(); }
"{"                         { yymore(); ++yy_bracket_count; }
"}"                         {
                                yymore();
                                --yy_bracket_count;
                                if (yy_bracket_count == 0)
                                    yy_pop_state();
                            }
}

<INITIALIZATION_LIST_STATE>{
[^{]*                       { yymore(); }
"{"                         {
                                yy_pop_state();
                                // skip open bracked
                                yytext[yyleng-1] = '\0';
                                *yylval = yytext;
                                unput('{');
                                return INITIALIZATION_LIST;
                            }
}

%%

void yy_push_state_cpp_code()
{
    yy_push_state(CPP_CODE_STATE);
    yy_bracket_count = 1;
}

void yy_push_state_initialization_list()
{
    yy_push_state(INITIALIZATION_LIST_STATE);
    yy_bracket_count = 1;
}
