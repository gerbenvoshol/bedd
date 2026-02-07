#include <string.h>
#include <syntax.h>
#include <ctype.h>

static int is_ident(char chr) {
  return isalnum(chr) || strchr("_.", chr);
}

enum {
  st_asm_default,
  st_asm_ident,
  st_asm_number,
  st_asm_string,
  st_asm_char,
  st_asm_line_comment,
  st_asm_block_comment,
  st_asm_string_escape,
  st_asm_char_escape,
  st_asm_block_comment_exit,
  st_asm_directive,
};

int st_asm_color(int prev_state, int *state, const char *text, int length) {
  if (prev_state == st_asm_line_comment) {
    if (text[0] == '\n') {
      *state = st_asm_default;
      return st_color_default;
    }
    return st_color_comment;
  }
  
  if (prev_state == st_asm_block_comment_exit) {
    *state = st_asm_default;
    return st_color_comment;
  }
  
  if (prev_state == st_asm_block_comment) {
    if (length >= 2 && text[0] == '*' && text[1] == '/') {
      *state = st_asm_block_comment_exit;
    }
    return st_color_comment;
  }
  
  if (prev_state == st_asm_string_escape) {
    *state = st_asm_string;
    return st_color_string;
  }
  
  if (prev_state == st_asm_char_escape) {
    *state = st_asm_char;
    return st_color_string;
  }
  
  if (prev_state == st_asm_string) {
    if (text[0] == '\\') {
      *state = st_asm_string_escape;
    } else if (text[0] == '"') {
      *state = st_asm_default;
    }
    return st_color_string;
  }
  
  if (prev_state == st_asm_char) {
    if (text[0] == '\\') {
      *state = st_asm_char_escape;
    } else if (text[0] == '\'') {
      *state = st_asm_default;
    }
    return st_color_string;
  }
  
  if (text[0] == '#' || text[0] == ';') {
    *state = st_asm_line_comment;
    return st_color_comment;
  }
  
  if (length >= 2 && text[0] == '/') {
    if (text[1] == '/') {
      *state = st_asm_line_comment;
      return st_color_comment;
    } else if (text[1] == '*') {
      *state = st_asm_block_comment;
      return st_color_comment;
    }
  }
  
  if (strchr("+-*/%=&|^!?:,;<>~@[]", text[0])) {
    *state = st_asm_default;
    return st_color_symbol;
  }
  
  if (isspace(text[0]) || strchr("(){}\n", text[0])) {
    *state = st_asm_default;
    return st_color_default;
  }
  
  if (text[0] == '"') {
    *state = st_asm_string;
    return st_color_string;
  }
  
  if (text[0] == '\'') {
    *state = st_asm_char;
    return st_color_string;
  }
  
  // Directives start with . or %
  if ((text[0] == '.' || text[0] == '%') && length >= 2 && isalpha(text[1])) {
    *state = st_asm_directive;
    return st_color_keyword;
  }
  
  // Register names (common ones)
  if (text[0] == '%' || text[0] == '$') {
    *state = st_asm_ident;
    return st_color_type;
  }
  
  if (prev_state == st_asm_directive) {
    if (is_ident(text[0])) {
      return st_color_keyword;
    }
    *state = st_asm_default;
    return st_color_none;
  }
  
  if (prev_state == st_asm_default) {
    if (isdigit(text[0])) {
      *state = st_asm_number;
      return st_color_number;
    } else if (text[0] == '0' && length >= 2 && (text[1] == 'x' || text[1] == 'X')) {
      *state = st_asm_number;
      return st_color_number;
    } else if (is_ident(text[0])) {
      int ident_length = 1;
      int is_keyword = 0;
      
      for (int i = 1; i < length; i++) {
        if (!is_ident(text[i])) {
          break;
        }
        ident_length++;
      }
      
      char buffer[ident_length + 1];
      memcpy(buffer, text, ident_length);
      buffer[ident_length] = '\0';
      
      // Common x86/x64 instructions
      if (ident_length == 3 && strstr("adc,add,and,bsf,bsr,bsw,btr,bts,cbw,clc,cld,cli,cmc,cmp,cwd,daa,das,dec,div,hlt,inc,int,jae,jbe,jge,jle,jmp,jne,lea,mov,mul,neg,nop,not,out,pop,ret,rol,ror,sal,sar,sbb,shl,shr,stc,std,sti,sub,xor", buffer)) {
        is_keyword = 1;
      }
      
      if (ident_length == 4 && strstr("call,cdqe,cmov,cwde,idiv,imul,iret,jcxz,loop,movs,push,repe,repz,test,xchg,xlat", buffer)) {
        is_keyword = 1;
      }
      
      if (ident_length == 5 && strstr("enter,leave,movsx,movzx,repne,retf,scasb,stosb", buffer)) {
        is_keyword = 1;
      }
      
      if (ident_length == 6 && strstr("loopne,loopnz,movsbl", buffer)) {
        is_keyword = 1;
      }
      
      // Check if it's a label (ends with :)
      if (ident_length < length && text[ident_length] == ':') {
        *state = st_asm_ident;
        return st_color_function;
      }
      
      *state = st_asm_ident;
      
      if (is_keyword) {
        return st_color_keyword;
      } else {
        return st_color_default;
      }
    }
  }
  
  return st_color_none;
}
