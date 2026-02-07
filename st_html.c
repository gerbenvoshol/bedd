#include <string.h>
#include <syntax.h>
#include <ctype.h>

static int is_ident(char chr) {
  return isalnum(chr) || strchr("_-", chr);
}

enum {
  st_html_default,
  st_html_tag_open,
  st_html_tag_name,
  st_html_tag_attr_name,
  st_html_tag_attr_value,
  st_html_tag_close,
  st_html_comment,
  st_html_comment_exit1,
  st_html_comment_exit2,
  st_html_string,
  st_html_string_escape,
};

int st_html_color(int prev_state, int *state, const char *text, int length) {
  if (prev_state == st_html_comment_exit2) {
    *state = st_html_default;
    return st_color_comment;
  }
  
  if (prev_state == st_html_comment_exit1) {
    if (text[0] == '>') {
      *state = st_html_comment_exit2;
    } else {
      *state = st_html_comment;
    }
    return st_color_comment;
  }
  
  if (prev_state == st_html_comment) {
    if (text[0] == '-' && length >= 2 && text[1] == '-') {
      *state = st_html_comment_exit1;
    }
    return st_color_comment;
  }
  
  if (prev_state == st_html_string_escape) {
    *state = st_html_string;
    return st_color_string;
  }
  
  if (prev_state == st_html_string) {
    if (text[0] == '\\') {
      *state = st_html_string_escape;
    } else if (text[0] == '"' || text[0] == '\'') {
      *state = st_html_tag_attr_name;
    }
    return st_color_string;
  }
  
  if (prev_state == st_html_tag_attr_value) {
    if (text[0] == '"' || text[0] == '\'') {
      *state = st_html_string;
      return st_color_string;
    } else if (is_ident(text[0])) {
      *state = st_html_tag_attr_name;
      return st_color_default;
    } else if (text[0] == '>') {
      *state = st_html_default;
      return st_color_symbol;
    }
    *state = st_html_tag_attr_name;
    return st_color_default;
  }
  
  if (prev_state == st_html_tag_attr_name) {
    if (text[0] == '=') {
      *state = st_html_tag_attr_value;
      return st_color_symbol;
    } else if (text[0] == '>') {
      *state = st_html_default;
      return st_color_symbol;
    } else if (isspace(text[0])) {
      *state = st_html_tag_attr_name;
      return st_color_default;
    } else if (is_ident(text[0])) {
      return st_color_function;
    }
    *state = st_html_tag_attr_name;
    return st_color_default;
  }
  
  if (prev_state == st_html_tag_name) {
    if (text[0] == '>') {
      *state = st_html_default;
      return st_color_symbol;
    } else if (isspace(text[0])) {
      *state = st_html_tag_attr_name;
      return st_color_default;
    } else if (is_ident(text[0])) {
      return st_color_keyword;
    }
    *state = st_html_tag_attr_name;
    return st_color_default;
  }
  
  if (prev_state == st_html_tag_open) {
    if (is_ident(text[0]) || text[0] == '/') {
      *state = st_html_tag_name;
      return st_color_keyword;
    }
    *state = st_html_default;
    return st_color_default;
  }
  
  if (prev_state == st_html_tag_close) {
    *state = st_html_default;
    return st_color_symbol;
  }
  
  if (text[0] == '<') {
    if (length >= 4 && text[1] == '!' && text[2] == '-' && text[3] == '-') {
      *state = st_html_comment;
      return st_color_comment;
    }
    *state = st_html_tag_open;
    return st_color_symbol;
  }
  
  *state = st_html_default;
  return st_color_default;
}
