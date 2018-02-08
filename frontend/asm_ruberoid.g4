
grammar asm_ruberoid;

program
    : section_code
    ;

section_code
    : section_code_header section_code_body
    ;

section_code_header
    : SECTION_CODE_HEADER
    ;

section_code_body
    : (line? EOL) +
    ;

line
    : comment
    | instruction
    | label
    ;

instruction
    : label? opcode argumentlist? comment?
    ;

label
    : label_id ':'
    ;

label_id
    : '.' NAME
    ;

argumentlist
    : argument (',' argumentlist)?
    ;

argument
    : reference
    | label_id
    | number
    | string
    ;

reference
    : '$' id
    ;

id
    : NAME ('%' NAME)*
    ;

number
    : NUMBER
    ;

string
    : STRING
    ;

comment
    : COMMENT
    ;

opcode
    : OPCODE
    ;

ID_TYPE
    : 'c'
    | 'f'
    ;

HEXID
    : [0-9a-fA-F]+
    ;

SECTION_ID_HEADER
    : '.ID'
    ;

SECTION_CODE_HEADER
    : '.CODE'
    ;

OPCODE
    : 'CTXC'
    | 'CTXF'
    | 'BSKP'
    | 'BALN'
    | 'BRDB'
    | 'BRDU8'
    | 'BRDU16'
    | 'BRDU32'
    | 'BRDU64'
    | 'BRDS8'
    | 'BRDS16'
    | 'BRDS32'
    | 'BRDS64'
    | 'SLOTA'
    | 'SLOTF'
    | 'SLOTE'
    | 'SLOTD'
    | 'CALL'
    | 'RET'
    | 'VAR'
    | 'MOV'
    | 'JL'
    | 'JNL'
    | 'JG'
    | 'JNG'
    | 'JE'
    | 'JNE'
    | 'INC'
    | 'DEC'
    | 'ADD'
    | 'SUB'
    | 'MUL'
    | 'DIV'
    | 'MOD'
    | 'XOR'
    | 'OR'
    | 'AND'
    | 'NOT'
    ;

NAME
    : [a-zA-Z_][a-zA-Z0-9_]*
    ;

NUMBER
    : NUMBER_DECIMAL
    | NUMBER_HEX
    | NUMBER_BINARY
    | NUMBER_OCT
    ;


NUMBER_DECIMAL
    : '-'? [1-9][0-9]+
    ;

NUMBER_HEX
    : '0x' [0-9a-fA-F]+
    ;

NUMBER_BINARY
    : '0b' [0-1]+
    ;


NUMBER_OCT
    : '0' [0-7]+
    ;

COMMENT
    : '//' ~[\r\n]* -> skip
    ;

STRING
    : '"' ~["]* '"'
    ;

EOL
    : '\r'? '\n'
    ;

WS
    : [ \t] -> skip
    ;