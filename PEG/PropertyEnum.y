%{
#include "PropertyEnumGeneratorCommon.h"
#include "PropertyEnumGenerator.h"
#include <stdio.h>

void yyerror(/*YYLTYPE *yylloc, */char const *);
#define YYLEX_PARAM &yylval, &yylloc
#define YYPRINT(File, Type, Value) fprintf ((File), "%s", (Value).toLatin1().data())

extern void yy_push_state_cpp_code();
extern void yy_push_state_initialization_list();

%}

%locations
%define parse.error verbose
//%define api.pure full
%lex-param {YYLEX_PARAM}


%token  PROPERTY_SET
        PROPERTY
        INCLUDE_H
        INCLUDE_CPP
        INCLUDE_NAME
        EXTERN
        ID
        CPP_BRACKET_OPEN
        CPP_BRACKET_CLOSE
        ASSIGN
        SEMICOLON
        COLON
        DBL_COLON
        COMMA
        DOT
        TILDE
        VALUE_TO_ASSIGN
        ACCESS_ID
        SLOT
        DELEGATE
        CODE_H
        CODE_CPP
        CPP_CODE
        ENUM
        NUMBER
        STR
        INITIALIZATION_LIST

%%

pef_body:         /* empty */
                | pef_body include_h
                | pef_body include_cpp
                | pef_body source_code
                | pef_body property_set
                | pef_body enum
;

include_h:      INCLUDE_H INCLUDE_NAME      {
    peg.code.append(QSharedPointer<Code>(new IncludeCode($2, true)));
}
;

include_cpp:    INCLUDE_CPP INCLUDE_NAME    {
    peg.code.append(QSharedPointer<Code>(new IncludeCode($2, false)));
}
;

source_code:      CODE_H CPP_BRACKET_OPEN { yy_push_state_cpp_code(); }
                    CPP_CODE    {
    QSharedPointer<SourceCode> code(new SourceCode($4, true));
    if (pegContext.currPropertySet)
        pegContext.currPropertySet->sourceCodes.append(code);
    else
        peg.code.append(code);
}
                | CODE_CPP CPP_BRACKET_OPEN { yy_push_state_cpp_code(); }
                    CPP_CODE   {
    QSharedPointer<SourceCode> code(new SourceCode($4, false));
    if (pegContext.currPropertySet)
        pegContext.currPropertySet->sourceCodes.append(code);
    else
        peg.code.append(code);
}
;

property_set:   property_set_head property_set_body { pegContext.commitPropertySet(); }
;

property_set_head:  PROPERTY_SET ID   {
    pegContext.startPropertySet();
    pegContext.currPropertySet->setName($2);
}
;

type_decl:    ID                                { pegContext.currType.namespaceName = "";
                                                  pegContext.currType.name = $1; }
            | namespace_name DBL_COLON ID       { pegContext.currType.namespaceName = $1;
                                                  pegContext.currType.name = $3; }
;

namespace_name:   ID
                | namespace_name DBL_COLON ID
;

optional_semicolon: /* empty */
                    | SEMICOLON
;

property_set_body: CPP_BRACKET_OPEN property_set_declarations CPP_BRACKET_CLOSE optional_semicolon {
}
;

property_set_declarations:    /* empty */
                            | property_set_declarations property_declaration
                            | property_set_declarations value_declaration
                            | property_set_declarations slot_declaration
                            | property_set_declarations sub_property_set
                            | property_set_declarations source_code
                            | property_set_declarations delegate_declaration
                            | property_set_declarations constructor
                            | property_set_declarations destructor
;

sub_property_set:     property_set
                    | property_set_with_id
                    | property_set_extern
;

property_set_with_id:   property_set_head ID property_set_body  {
    pegContext.currPropertySet->name = $2;
    pegContext.commitPropertySet();
    // add sub property set as member
    pegContext.currPropertySet->members.push_back(pegContext.currPropertySet->subPropertySets.last());
}
;

property_set_extern:    property_set_extern_head property_set_extern_tail
;

property_set_extern_head:    EXTERN PROPERTY_SET type_decl ID   {
    pegContext.startPropertySet();
    pegContext.currPropertySet->_extern = true;
    pegContext.currPropertySet->selfType = pegContext.currentPropertySetType();
    pegContext.currPropertySet->name = $4;
}
;

property_set_extern_tail:   sub_item_body           {
    pegContext.commitPropertySet();
    // add sub property set as member
    pegContext.currPropertySet->members.push_back(pegContext.currPropertySet->subPropertySets.last());
}
                            | SEMICOLON             {
    pegContext.commitPropertySet();
    // add sub property set as member
    pegContext.currPropertySet->members.push_back(pegContext.currPropertySet->subPropertySets.last());
}
;

property_declaration:   property_declaration_head property_declaration_tail
;

property_declaration_head:   type_decl ID               {
    pegContext.startProperty();
    pegContext.currProperty->selfType = pegContext.currentPropertyType();
    pegContext.currProperty->name = $2;
}
;

property_declaration_tail:    sub_item_body {
    pegContext.commitProperty();
}
                            | SEMICOLON     {
    pegContext.commitProperty();
}
;

sub_item_body:  CPP_BRACKET_OPEN sub_item_declarations CPP_BRACKET_CLOSE {
}
;

sub_item_declarations:    /* empty */
                        | sub_item_declarations value_declaration
                        | sub_item_declarations slot_declaration
                        | sub_item_declarations delegate_declaration
;

value_declaration:      name_to_assign ASSIGN VALUE_TO_ASSIGN   {
    AssignInfo info;
    info.member = pegContext.currAssignMember;
    info.value = $3;
    pegContext.current()->assignments.insert(pegContext.currAssignFunc, info);
}
;

name_to_assign:   ID                        {
    pegContext.currAssignFunc = $1;
    pegContext.currAssignMember = "";
}
                | name_to_assign DOT ID     {
    pegContext.currAssignFunc = $3;
    pegContext.currAssignMember = $1;
}
;

slot_declaration:   SLOT name_to_assign            {
    if (!pegContext.checkSlotName(pegContext.currAssignFunc)) {
        QString error = peg.tr("Unrecognized slot name <%1>.").arg(pegContext.currAssignFunc);
        yyerror(error.toLatin1().data());
        exit(1);
    }
}
                        CPP_BRACKET_OPEN    { yy_push_state_cpp_code(); }
                        CPP_CODE            {
    AssignInfo info;
    info.member = pegContext.currAssignMember;
    info.value = $6;
    pegContext.current()->slots_code.insert(pegContext.currAssignFunc, info);
}
;

delegate_declaration:     DELEGATE {
    if (pegContext.current()->delegateInfo.data()) {
        QString error = peg.tr("Delegate for object <%1> already defined.").arg(pegContext.current()->name);
        yyerror(error.toLatin1().data());
        exit(1);
    }

    pegContext.current()->delegateInfo.reset(new DelegateInfo());
}
                                delegate_tail
;

delegate_tail:            ID delegate_body {
    pegContext.current()->delegateInfo->name = $1;
}
                        | delegate_body
;

delegate_body:       SEMICOLON
                    | CPP_BRACKET_OPEN delegate_attributes CPP_BRACKET_CLOSE
;

delegate_attributes:      /*empty*/
                        | delegate_attributes delegate_attribute
;

delegate_attribute:      ID ASSIGN VALUE_TO_ASSIGN {
    pegContext.current()->delegateInfo->attributes[$1] = $3;
}

constructor: ID '(' ')'
                        initialization_list
                        CPP_BRACKET_OPEN { yy_push_state_cpp_code(); } CPP_CODE {
    pegContext.currPropertySet->setConstructorCode($1, $4, $7);
}
;

initialization_list:      /*empty*/
                        | COLON { yy_push_state_initialization_list(); }
                          INITIALIZATION_LIST {
    $$ = $3;
}
;

destructor: TILDE ID '(' ')' CPP_BRACKET_OPEN { yy_push_state_cpp_code(); }
                        CPP_CODE {
    pegContext.currPropertySet->setDestructorCode($2, $7);
}
;

enum:   ENUM ID             {
    pegContext.enumCode.reset(new EnumCode($2));
}
            CPP_BRACKET_OPEN
            enum_values_list CPP_BRACKET_CLOSE optional_semicolon {
    peg.code.append(pegContext.enumCode);
    pegContext.enumCode.reset();
}
;

enum_values_list:        /* none */
                        | enum_values_list2
;

enum_values_list2:        enum_value
                        | enum_values_list2 COMMA enum_value
;

enum_value:         ID '(' NUMBER COMMA STR ')'     {
    EnumCode::EnumItemCode item;
    item.name = $1;
    bool success = false;
    item.id = $3.toInt(&success);
    if (!success) {
        QString error = peg.tr("Unrecognized enum item id <%1>.").arg($3);
        yyerror(error.toLatin1().data());
        exit(1);
    }
    item.text = $5;

    pegContext.enumCode->items.append(item);
}
                 enum_states_list
;

enum_states_list:         /* none */
                        | enum_states_list ID   {
    pegContext.enumCode->items.last().states.append($2);
}
;

%%

void yyerror(/*YYLTYPE *yylloc, */char const *s)
{
    fprintf(stderr, "Parser error: (%d,%d)-(%d,%d) '%s'\n"
                    ,yylloc.first_line
                    ,yylloc.first_column
                    ,yylloc.last_line
                    ,yylloc.last_column
                    ,s);
}

