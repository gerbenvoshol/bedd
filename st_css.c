#include <string.h>
#include <syntax.h>
#include <ctype.h>

static int is_ident(char chr) {
  return isalnum(chr) || strchr("_-", chr);
}

enum {
  st_css_default,
  st_css_ident,
  st_css_number,
  st_css_string,
  st_css_line_comment,
  st_css_block_comment,
  st_css_string_escape,
  st_css_block_comment_exit,
  st_css_property,
  st_css_value,
};

int st_css_color(int prev_state, int *state, const char *text, int length) {
  if (prev_state == st_css_line_comment) {
    if (text[0] == '\n') {
      *state = st_css_default;
      return st_color_default;
    }
    return st_color_comment;
  }
  
  if (prev_state == st_css_block_comment_exit) {
    *state = st_css_default;
    return st_color_comment;
  }
  
  if (prev_state == st_css_block_comment) {
    if (length >= 2 && text[0] == '*' && text[1] == '/') {
      *state = st_css_block_comment_exit;
    }
    return st_color_comment;
  }
  
  if (prev_state == st_css_string_escape) {
    *state = st_css_string;
    return st_color_string;
  }
  
  if (prev_state == st_css_string) {
    if (text[0] == '\\') {
      *state = st_css_string_escape;
    } else if (text[0] == '"' || text[0] == '\'') {
      *state = st_css_default;
    }
    return st_color_string;
  }
  
  if (length >= 2 && text[0] == '/') {
    if (text[1] == '/') {
      *state = st_css_line_comment;
      return st_color_comment;
    } else if (text[1] == '*') {
      *state = st_css_block_comment;
      return st_color_comment;
    }
  }
  
  if (strchr("{}:;,", text[0])) {
    *state = st_css_default;
    return st_color_symbol;
  }
  
  if (isspace(text[0]) || strchr("()[]\n", text[0])) {
    *state = st_css_default;
    return st_color_default;
  }
  
  if (text[0] == '"' || text[0] == '\'') {
    *state = st_css_string;
    return st_color_string;
  }
  
  if (text[0] == '#' && length >= 2 && isxdigit(text[1])) {
    *state = st_css_number;
    return st_color_number;
  }
  
  if (prev_state == st_css_default) {
    if (isdigit(text[0])) {
      *state = st_css_number;
      return st_color_number;
    } else if (is_ident(text[0]) || text[0] == '.' || text[0] == '#' || text[0] == '@') {
      *state = st_css_ident;
      
      int ident_length = 1;
      for (int i = 1; i < length; i++) {
        if (!is_ident(text[i])) {
          break;
        }
        ident_length++;
      }
      
      // Check for CSS keywords
      char buffer[ident_length + 1];
      memcpy(buffer, text, ident_length);
      buffer[ident_length] = '\0';
      
      // CSS at-rules
      if (text[0] == '@') {
        return st_color_keyword;
      }
      
      // CSS selectors (. and # prefixes)
      if (text[0] == '.' || text[0] == '#') {
        return st_color_type;
      }
      
      return st_color_function;
    }
  }
  
  if (prev_state == st_css_ident || prev_state == st_css_number) {
    if (is_ident(text[0]) || isdigit(text[0]) || text[0] == '%' || text[0] == '#') {
      return st_color_number;
    }
    *state = st_css_default;
    return st_color_none;
  }
  
  return st_color_none;
}
