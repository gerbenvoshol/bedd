#include <string.h>
#include <syntax.h>
#include <ctype.h>

static int is_ident(char chr) {
  return isalnum(chr) || chr == '_';
}

enum {
  st_json_default,
  st_json_ident,
  st_json_number,
  st_json_string,
  st_json_string_escape,
};

int st_json_color(int prev_state, int *state, const char *text, int length) {
  if (prev_state == st_json_string_escape) {
    *state = st_json_string;
    return st_color_string;
  }
  
  if (prev_state == st_json_string) {
    if (text[0] == '\\') {
      *state = st_json_string_escape;
    } else if (text[0] == '"') {
      *state = st_json_default;
    }
    
    return st_color_string;
  }
  
  if (strchr(":.,", text[0])) {
    *state = st_json_default;
    return st_color_symbol;
  }
  
  if (isspace(text[0]) || strchr("()[]{}\n", text[0])) {
    *state = st_json_default;
    return st_color_default;
  }
  
  if (text[0] == '"') {
    *state = st_json_string;
    return st_color_string;
  }
  
  if (prev_state == st_json_default) {
    if (isdigit(text[0]) || text[0] == '-') {
      *state = st_json_number;
      return st_color_number;
    } else if (is_ident(text[0])) {
      int is_keyword = 0;
      int ident_length = 1;
      
      for (int i = 1; i < length; i++) {
        if (!is_ident(text[i])) {
          break;
        }
        
        ident_length++;
      }
      
      char buffer[ident_length + 1];
      
      memcpy(buffer, text, ident_length);
      buffer[ident_length] = '\0';
      
      if (ident_length == 4 && strstr("true,null", buffer)) {
        is_keyword = 1;
      }
      
      if (ident_length == 5 && strstr("false", buffer)) {
        is_keyword = 1;
      }
      
      *state = st_json_ident;
      
      if (is_keyword) {
        return st_color_keyword;
      } else {
        return st_color_default;
      }
    }
  }
  
  return st_color_none;
}
