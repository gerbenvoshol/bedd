#include <string.h>
#include <syntax.h>
#include <ctype.h>

static int is_ident(char chr) {
  return isalnum(chr) || chr == '_';
}

enum {
  st_py_default,
  st_py_ident,
  st_py_number,
  st_py_string,
  st_py_char,
  st_py_line_comment,
  st_py_string_escape,
  st_py_char_escape,
  st_py_triple_string,
  st_py_triple_string_exit1,
  st_py_triple_string_exit2,
};

int st_py_color(int prev_state, int *state, const char *text, int length) {
  if (prev_state == st_py_line_comment) {
    if (text[0] == '\n') {
      *state = st_py_default;
      return st_color_default;
    }
    return st_color_comment;
  }
  
  if (prev_state == st_py_triple_string_exit2) {
    *state = st_py_default;
    return st_color_string;
  }
  
  if (prev_state == st_py_triple_string_exit1) {
    if (text[0] == '"' || text[0] == '\'') {
      *state = st_py_triple_string_exit2;
    } else {
      *state = st_py_triple_string;
    }
    return st_color_string;
  }
  
  if (prev_state == st_py_triple_string) {
    if (length >= 2 && (text[0] == '"' || text[0] == '\'') && text[0] == text[1]) {
      *state = st_py_triple_string_exit1;
    }
    return st_color_string;
  }
  
  if (prev_state == st_py_string_escape) {
    *state = st_py_string;
    return st_color_string;
  }
  
  if (prev_state == st_py_char_escape) {
    *state = st_py_char;
    return st_color_string;
  }
  
  if (prev_state == st_py_string) {
    if (text[0] == '\\') {
      *state = st_py_string_escape;
    } else if (text[0] == '"') {
      *state = st_py_default;
    }
    return st_color_string;
  }
  
  if (prev_state == st_py_char) {
    if (text[0] == '\\') {
      *state = st_py_char_escape;
    } else if (text[0] == '\'') {
      *state = st_py_default;
    }
    return st_color_string;
  }
  
  if (text[0] == '#') {
    *state = st_py_line_comment;
    return st_color_comment;
  }
  
  if (strchr("+-*/%=&|^!?:.,;<>~@", text[0])) {
    *state = st_py_default;
    return st_color_symbol;
  }
  
  if (isspace(text[0]) || strchr("()[]{}\n", text[0])) {
    *state = st_py_default;
    return st_color_default;
  }
  
  if (text[0] == '"') {
    if (length >= 3 && text[1] == '"' && text[2] == '"') {
      *state = st_py_triple_string;
      return st_color_string;
    }
    *state = st_py_string;
    return st_color_string;
  }
  
  if (text[0] == '\'') {
    if (length >= 3 && text[1] == '\'' && text[2] == '\'') {
      *state = st_py_triple_string;
      return st_color_string;
    }
    *state = st_py_char;
    return st_color_string;
  }
  
  if (prev_state == st_py_default) {
    if (isdigit(text[0])) {
      *state = st_py_number;
      return st_color_number;
    } else if (is_ident(text[0])) {
      int is_func = 1, is_keyword = 0;
      int ident_length = 1;
      
      for (int i = 1; i < length; i++) {
        if (text[i] == '(') {
          break;
        }
        
        if (!is_ident(text[i])) {
          is_func = 0;
          break;
        }
        
        ident_length++;
      }
      
      char buffer[ident_length + 1];
      memcpy(buffer, text, ident_length);
      buffer[ident_length] = '\0';
      
      if (ident_length == 2 && strstr("as,if,in,is,or", buffer)) {
        is_keyword = 1;
      }
      
      if (ident_length == 3 && strstr("and,def,del,for,not,try", buffer)) {
        is_keyword = 1;
      }
      
      if (ident_length == 4 && strstr("else,from,None,pass,True,with", buffer)) {
        is_keyword = 1;
      }
      
      if (ident_length == 5 && strstr("async,await,break,class,False,raise,while,yield", buffer)) {
        is_keyword = 1;
      }
      
      if (ident_length == 6 && strstr("assert,except,global,import,lambda,return", buffer)) {
        is_keyword = 1;
      }
      
      if (ident_length == 7 && strstr("finally", buffer)) {
        is_keyword = 1;
      }
      
      if (ident_length == 8 && strstr("continue,nonlocal", buffer)) {
        is_keyword = 1;
      }
      
      *state = st_py_ident;
      
      if (is_keyword) {
        return st_color_keyword;
      } else if (is_func) {
        return st_color_function;
      } else {
        return st_color_default;
      }
    }
  }
  
  return st_color_none;
}
