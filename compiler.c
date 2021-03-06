#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reclass.h"
#include "compiler.h"
#include "helper.h"
#include "symbol.h"

FILE *yyin;
buffer *code;
dllist *symbols_list;
int current_method;
int current_max_stack;
int current_max_locals;
extern int line;

/* Opcode compilation */

/**
 * nop
 * OPC: 0x00
 */
int nop() {
    unsigned char byte;
    
    byte = 0x00;
    buffer_append(code, &byte, 1);

    return CF_OK;   
}

/**
 * Increments stack by 1
 * 
 * aconst_null
 * OPC: 0x01
 */
int aconstnull() {
    unsigned char byte;
    
    byte = 0x01;
    buffer_append(code, &byte, 1);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * iconst_m1
 * OPC: 0x02
 */
int iconstm1() {
    unsigned char byte;

    byte = 0x02;
    buffer_append(code, &byte, 1);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * iconst_0
 * OPC: 0x03
 */
int iconst0() {
    unsigned char byte;

    byte = 0x03;
    buffer_append(code, &byte, 1);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * iconst_1
 * OPC: 0x04
 */
int iconst1() {
    unsigned char byte;

    byte = 0x04;
    buffer_append(code, &byte, 1);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * iconst_2
 * OPC: 0x05
 */
int iconst2() {
    unsigned char byte;

    byte = 0x05;
    buffer_append(code, &byte, 1);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * iconst_3
 * OPC: 0x06
 */
int iconst3() {
    unsigned char byte;

    byte = 0x06;
    buffer_append(code, &byte, 1);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * iconst_4
 * OPC: 0x07
 */
int iconst4() {
    unsigned char byte;

    byte = 0x07;
    buffer_append(code, &byte, 1);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * iconst_5
 * OPC: 0x08
 */
int iconst5() {
    unsigned char byte;

    byte = 0x08;
    buffer_append(code, &byte, 1);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * lconst_0
 * OPC: 0x09
 */
int lconst0() {
    unsigned char byte;

    byte = 0x09;
    buffer_append(code, &byte, 1);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * lconst_1
 * OPC: 0x0A
 */
int lconst1() {
    unsigned char byte;

    byte = 0x0A;
    buffer_append(code, &byte, 1);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * fconst_0
 * OPC: 0x0B
 */
int fconst0() {
    unsigned char byte;

    byte = 0x0B;
    buffer_append(code, &byte, 1);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * fconst_1
 * OPC: 0x0C
 */
int fconst1() {
    unsigned char byte;

    byte = 0x0C;
    buffer_append(code, &byte, 1);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * fconst_2
 * OPC: 0x0D
 */
int fconst2() {
    unsigned char byte;

    byte = 0x0D;
    buffer_append(code, &byte, 1);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * dconst_0
 * OPC: 0x0E
 */
int dconst0() {
    unsigned char byte;

    byte = 0x0E;
    buffer_append(code, &byte, 1);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * dconst_1
 * OPC: 0x0F
 */
int dconst1() {
    unsigned char byte;

    byte = 0x0F;
    buffer_append(code, &byte, 1);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * bipush BYTE
 * OPC: 0x10 BYTE
 */
int bipush_byte(unsigned char byte) {
    unsigned char bytes[2];

    bytes[0] = 0x10;
    bytes[1] = byte;
    buffer_append(code, bytes, 2);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * bipush IDENTIFIER
 * OPC: 0x10 IDENTIFIER
 */
int bipush_identifier(char *identifier) {
    symbol    *s;
    symbol    *tmp;
    
    tmp = symbol_new(identifier);
    
    s = dllist_find(symbols_list, tmp);
    
    symbol_free(tmp);
    
    if(s == NULL) {
        debug(DBG_ERROR, "bipush_identifier: identifier %s not found", identifier);
        return CF_NOTOK;
    }

    if(((symbol *)s)->type != SYM_BYTE) {
        debug(DBG_ERROR, "bipush_identifier: identifier %s not of type BYTE", identifier);
        return CF_NOTOK;
    }
    
    return bipush_byte(((symbol *)s)->value.byte_val);
}

/**
 * Increments stack by 1
 *
 * sipush INT
 * OPC: 0x11 INT
 */
int sipush_short(int value) {
    unsigned char bytes[3];

    if((value < 0) || (value > 65535)) {
        debug(DBG_ERROR, "sipush_short: Invalid  value %d. Short expected", value);
        return CF_NOTOK;
    }

    bytes[0] = 0x11;
    bytes[1] = (value & 0x0000FF00) >> 8;
    bytes[2] = value & 0x000000FF;
    buffer_append(code, bytes, 3);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * sipush IDENTIFIER
 * OPC: 0x11 IDENTIFIER
 */
int sipush_identifier(char *identifier) {
    symbol    *s;
    symbol    *tmp;
    
    tmp = symbol_new(identifier);
    
    s = dllist_find(symbols_list, tmp);
    
    symbol_free(tmp);
    
    if(s == NULL) {
        debug(DBG_ERROR, "sipush_identifier: identifier %s not found", identifier);
        return CF_NOTOK;
    }

    if(((symbol *)s)->type != SYM_INT) {
        debug(DBG_ERROR, "sipush_identifier: identifier %s not of type INT", identifier);
        return CF_NOTOK;
    }
    
    return sipush_short(((symbol *)s)->value.int_val);
}

/**
 * Increments stack by 1
 *
 * ldc BYTE
 * OPC: 0x12 BYTE
 */
int ldc_byte(ClassFile *cf, unsigned char byte) {
    unsigned char bytes[2];

    if(byte > cf->constant_pool_count) {
        debug(DBG_WARN, "ldc_byte: Byte %d for ldc is not a valid index inside" 
                        " the constant pool", byte);
    }

    bytes[0] = 0x12;
    bytes[1] = byte;
    buffer_append(code, bytes, 2);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * ldc IDENTIFIER
 * OPC: 0x12 IDENTIFIER
 */
int ldc_identifier(ClassFile *cf, char *identifier) {
    symbol    *s;
    symbol    *tmp;
    
    tmp = symbol_new(identifier);
    
    s = dllist_find(symbols_list, tmp);
    
    symbol_free(tmp);
    
    if(s == NULL) {
        debug(DBG_ERROR, "ldc_identifier: identifier %s not found", identifier);
        return CF_NOTOK;
    }

    if((((symbol *)s)->type != SYM_INT) && (((symbol *)s)->type != SYM_STR)) {
        debug(DBG_ERROR, "ldc_identifier: identifier %s not of type INT or "
                         "STRING", identifier);
        return CF_NOTOK;
    }

    if(((symbol *)s)->cp_index > 255) {
        debug(DBG_ERROR, "ldc_identifier: identifier %s is in an index greater"
                         " than 255 in the constant pool. Use ldc_w instead",
                         identifier);
        return CF_NOTOK;
    }
    
    return ldc_byte(cf, ((symbol *)s)->cp_index);
}

/**
 * Increments stack by 1
 *
 * ldc *IDENTIFIER
 * OPC: 0x12 *IDENTIFIER
 */
int ldc_deref_identifier(ClassFile *cf, char *identifier) {
    symbol    *s;
    symbol    *tmp;
    
    tmp = symbol_new(identifier);
    
    s = dllist_find(symbols_list, tmp);
    
    symbol_free(tmp);
    
    if(s == NULL) {
        debug(DBG_ERROR, "ldc_deref_identifier: identifier %s not found", identifier);
        return CF_NOTOK;
    }

    if((((symbol *)s)->type != SYM_INT) && (((symbol *)s)->type != SYM_BYTE)) {
        debug(DBG_ERROR, "ldc_deref_identifier: identifier %s not of type INT or BYTE", identifier);
        return CF_NOTOK;
    }

    if(((symbol *)s)->type == SYM_INT) {
        if(((symbol *)s)->value.int_val > 255) {
            debug(DBG_ERROR, "ldc_deref_identifier: identifier %s has a value "
                             "greater than 255. Use ldc_w instead", identifier);

            return CF_NOTOK;
        }
        
        return ldc_byte(cf, (unsigned char)((symbol *)s)->value.int_val);
    }
    
    return ldc_byte(cf, ((symbol *)s)->value.byte_val);
}

/**
 * Increments stack by 1
 *
 * ldc_w SHORT
 * OPC: 0x13 SHORT
 */
int ldcw_short(ClassFile *cf, int value) {
    unsigned char bytes[3];

    if((value < 0) || (value > 65535)) {
        debug(DBG_ERROR, "ldcw_short: Invalid  value %d. Short expected", value);
        return CF_NOTOK;
    }

    if(value > cf->constant_pool_count) {
        debug(DBG_WARN, "ldcw_short: Short %d for ldcw is not a valid index" 
                        " inside the constant pool", value);
    }

    bytes[0] = 0x13;
    bytes[1] = (value & 0x0000FF00) >> 8;
    bytes[2] = value & 0x000000FF;
    buffer_append(code, bytes, 3);

    current_max_stack += 1;

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * ldc_w IDENTIFIER
 * OPC: 0x13 IDENTIFIER
 */
int ldcw_identifier(ClassFile *cf, char *identifier) {
    symbol    *s;
    symbol    *tmp;
    
    tmp = symbol_new(identifier);
    
    s = dllist_find(symbols_list, tmp);
    
    symbol_free(tmp);
    
    if(s == NULL) {
        debug(DBG_ERROR, "ldcw_identifier: identifier %s not found", identifier);
        return CF_NOTOK;
    }

    if((((symbol *)s)->type != SYM_INT) && (((symbol *)s)->type != SYM_STR)) {
        debug(DBG_ERROR, "ldcw_identifier: identifier %s not of type INT or "
                         "STRING", identifier);
        return CF_NOTOK;
    }

    return ldcw_short(cf, ((symbol *)s)->cp_index);
}

/**
 * Increments stack by 1
 *
 * ldc_w *IDENTIFIER
 * OPC: 0x13 *IDENTIFIER
 */
int ldcw_deref_identifier(ClassFile *cf, char *identifier) {
    symbol    *s;
    symbol    *tmp;
    
    tmp = symbol_new(identifier);
    
    s = dllist_find(symbols_list, tmp);
    
    symbol_free(tmp);
    
    if(s == NULL) {
        debug(DBG_ERROR, "ldc_deref_identifier: identifier %s not found", identifier);
        return CF_NOTOK;
    }

    if((((symbol *)s)->type != SYM_INT) && (((symbol *)s)->type != SYM_BYTE)) {
        debug(DBG_ERROR, "ldc_deref_identifier: identifier %s not of type INT or BYTE", identifier);
        return CF_NOTOK;
    }

    if(((symbol *)s)->type == SYM_INT) {
        if(((symbol *)s)->value.int_val > 65535) {
            debug(DBG_ERROR, "ldc_deref_identifier: identifier %s has a value "
                             "greater than 65535.", identifier);

            return CF_NOTOK;
        }
        
        return ldc_byte(cf, (unsigned char)((symbol *)s)->value.int_val);
    }
    
    return ldc_byte(cf, ((symbol *)s)->value.byte_val);
}

/**
 * aaload
 * OPC: 0x32
 */
int aaload() {
    unsigned char bytes[1];
    
    bytes[0] = 0x32;
    buffer_append(code, bytes, 1);

    return CF_OK;
}

/*****************************************************************************/

/**
 * return
 * OPC: 0xB1
 */
int jreturn() {
    unsigned char byte;
    
    byte = 0xB1;
    buffer_append(code, &byte, 1);

    return CF_OK;
}

/**
 * This function generates the opcode for the getstatic mnemonic
 * It will check if the index specified is valid into the constant_pool
 * according to the Classfile documentation and warn the user if 
 * it isn't. This check WILL NOT prevent the code from being compiled
 * as this value might have been intentional (looking for vulns for
 * example)
 *
 * getstatic int
 * OPC: 0xB2 HI_ADDR LO_ADDR
 */
int getstatic_int(ClassFile *cf, int index) {
    unsigned char bytes[3];
    
    if(index > cf->constant_pool_count) {
        debug(DBG_WARN, "Index for getstatic %d is greater than the " 
                        "number of entries in the constant pool", index);
                         
    } else {
        if(cf->constant_pool[index].tag != CONSTANT_FIELDREF) {
            debug(DBG_WARN, "Index for getstatic %d does not point to "
                            "a CONSTANT_Fieldref into the constant_pool",
                             index);
        }
    }
    
    bytes[0] = 0xB2;
    bytes[1] = (index & 0x0000FF00) >> 8;
    bytes[2] = (index & 0x000000FF);
    
    if(buffer_append(code, bytes, 3) != 3) {
        debug(DBG_ERROR, "Cannot append code to buffer... Aborting");
    }
    
    current_max_stack += 1;

    return CF_OK;

}

/**
 * getstatic identifier
 * OPC: 0xB2 HI_ADDR LO_ADDR
 */
int getstatic_identifier(ClassFile *cf, char *identifier) {
    
    symbol    *s;
    symbol    *tmp;
    
    tmp = symbol_new(identifier);
    
    s = dllist_find(symbols_list, tmp);
    
    symbol_free(tmp);
    
    if(s == NULL) {
        debug(DBG_ERROR, "getstatic: identifier %s not found", identifier);
        return CF_NOTOK;
    }
    
    return getstatic_int(cf, ((symbol *)s)->cp_index);
    
}

/**
 * newarray type
 * OPC: 0xBC TYPE
 */
int newarray_int(int type) {
    unsigned char bytes[2];
    
    bytes[0] = 0xBC;
    bytes[1] = (type & 0x000000FF);
    buffer_append(code, bytes, 2);

    return CF_OK;
}

/**
 * anewarray index
 * OPC: 0xBD INDEXBYTE1 INDEXBYTE2
 */
int anewarray_int(int index) {

    // Not sure yet...

    return CF_OK;

}

/**
 * pop
 * OPC: 0x57
 */
int pop() {
    unsigned char bytes[1];
    
    bytes[0] = 0x57;
    buffer_append(code, bytes, 1);

    return CF_OK;
}

/**
 * pop2
 * OPC: 0x58
 */
int pop2() {
    unsigned char bytes[1];
    
    bytes[0] = 0x58;
    buffer_append(code, bytes, 1);

    return CF_OK;
}

/**
 * Increments stack by 1
 *
 * dup
 * OPC: 0x59
 */
int dup() {
    unsigned char bytes[1];
    
    bytes[0] = 0x59;
    buffer_append(code, bytes, 1);

    current_max_stack += 1;

    return CF_OK;
}

/* Assembler special directives */

int method_start(ClassFile *cf, char *identifier, char *params) {
    method_info *MyMethod;
    symbol *s;
    
    s = symbol_new(identifier);
    s->type = SYM_METHOD;
    s->signature = strdup(params);
    
    current_method = RC_GetMethodIndex(cf, identifier, params);
    
    if(current_method == -1) {
        RC_AddMethod(cf, identifier, params, ACC_PUBLIC | ACC_STATIC, &MyMethod);
        
        current_method = RC_GetMethodIndex(cf, identifier, params);
        if(current_method == -1) {
            debug(DBG_ERROR, "Impossible to create method %s [%s]", identifier, params);
            return CF_NOTOK;
        }
    }

    return CF_OK;
}

int method_end(ClassFile *cf) {
    
    RC_ChangeMethodCodeAttribute(cf, &cf->methods[current_method] , 
                                 code->bytes, code->size, 
                                 current_max_stack, 
                                 current_max_locals);
    
    
    current_method = 0;
    current_max_stack = 0;
    current_max_locals = 0;
    buffer_clear(code);
    
    return CF_OK;
}

int create_int(ClassFile *cf, char *identifier, int value) {
    
    unsigned short index;
    symbol *s;
    
    s = symbol_new(identifier);
    s->type = SYM_INT;
    
    if(dllist_find(symbols_list, s)) {
        debug(DBG_ERROR, "Symbol %s already declared. Aborting...",
              identifier);
        
        symbol_free(s);
        
        return CF_NOTOK;
    }


    index = RC_CPAddInteger(cf, (u4)value);

    if(index == CF_NOTOK) {
        debug(DBG_ERROR, "Could not create symbol %s. Too many values inside "
                         "the constant_pool.");
        return CF_NOTOK;
    }
    
    s->cp_index = index;
    s->value.int_val = value;
    
    dllist_add(symbols_list, (void *)s);
    
    return CF_OK;
}

int create_byte(ClassFile *cf, char *identifier, unsigned char value) {
    
    symbol *s;
    
    s = symbol_new(identifier);
    s->type = SYM_BYTE;
    
    if(dllist_find(symbols_list, s)) {
        debug(DBG_ERROR, "Symbol %s already declared. Aborting...\n",
              identifier);
        
        symbol_free(s);
        
        return CF_NOTOK;
    }
 
    s->value.byte_val = value;
    
    dllist_add(symbols_list, (void *)s);
    
    return CF_OK;
}

/* Entry point */
int compile(ClassFile *cf, char *file_path) {
    int ret = CF_OK;
    FILE *f;

    if((f = fopen(file_path, "r")) == NULL) {
        printf("Error opening file: %s\n", file_path);
        return CF_NOTOK;
    }
    
    yyin = f;
    current_method = 0;
    current_max_stack = 0;
    current_max_locals = 0;
    code = buffer_new(0);
    symbols_list = dllist_new(0, symbol_free, symbol_cmp); 
    
    do {
        if(yyparse(cf) > 0) {
            ret = CF_NOTOK;
            break;
        }
    } while (!feof(yyin));


    buffer_free(code);
    dllist_free(symbols_list);
    fclose(f);

    return ret;
}

void yyerror(const char *s) {
    printf("Line %d -> Error on token: %s\n", line, s);
}
