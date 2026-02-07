#include <theme.h>
#include <bedd.h>
#include <io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

bd_config_t bd_config = (bd_config_t) {
  .indent_width = 2,
  .indent_spaces = 1,
  .indent_guides = 1,
  .scroll_step = 2,
  .scroll_image_step = 4,
  .scroll_width_margin = 0,
  .undo_edit_count = 48,
  .undo_depth = 64,
  .theme = theme_17,
  .xterm_colors = 1,
  .terminal_count = 2048,
  .column_guide = 0,
  .column_tiny = 0,
  
  .shell_path = "/usr/bin/sh",
  
  .syntax_colors = {
    "",             // st_color_none
    IO_WHITE,       // st_color_default
    IO_BOLD_PURPLE, // st_color_keyword
    IO_YELLOW,      // st_color_number
    IO_BLUE,        // st_color_function
    IO_BOLD_CYAN,   // st_color_type
    IO_GREEN,       // st_color_string
    IO_GRAY,        // st_color_comment
    IO_BOLD_RED,    // st_color_symbol
  },
};

// Load config from text-based format
int bd_config_load(const char *path) {
  io_file_t file = io_fopen(path, 0);
  if (!io_fvalid(file)) {
    return 0;
  }
  
  char buffer[4096];
  int bytes_read = io_fread(file, buffer, sizeof(buffer) - 1);
  io_fclose(file);
  
  if (bytes_read <= 0) {
    return 0;
  }
  
  buffer[bytes_read] = '\0';
  
  // Check if this looks like a text-based config (starts with # or contains =)
  int is_text = 0;
  for (int i = 0; i < bytes_read && i < 100; i++) {
    if (buffer[i] == '#' || buffer[i] == '=') {
      is_text = 1;
      break;
    }
  }
  
  if (!is_text) {
    return 0;  // Not a text-based config
  }
  
  // Parse line by line
  char *line = strtok(buffer, "\n");
  while (line) {
    // Skip comments and empty lines
    while (*line == ' ' || *line == '\t') line++;
    if (*line == '#' || *line == '\0') {
      line = strtok(NULL, "\n");
      continue;
    }
    
    // Find '=' separator
    char *eq = strchr(line, '=');
    if (eq) {
      *eq = '\0';
      char *key = line;
      char *value = eq + 1;
      
      // Trim key
      char *key_end = eq - 1;
      while (key_end >= key && (*key_end == ' ' || *key_end == '\t')) {
        *key_end = '\0';
        key_end--;
      }
      
      // Trim value
      while (*value == ' ' || *value == '\t') value++;
      char *value_end = value + strlen(value) - 1;
      while (value_end >= value && (*value_end == ' ' || *value_end == '\t' || *value_end == '\r')) {
        *value_end = '\0';
        value_end--;
      }
      
      // Parse config values
      if (!strcmp(key, "indent_width")) {
        bd_config.indent_width = atoi(value);
      } else if (!strcmp(key, "indent_spaces")) {
        bd_config.indent_spaces = atoi(value);
      } else if (!strcmp(key, "indent_guides")) {
        bd_config.indent_guides = atoi(value);
      } else if (!strcmp(key, "scroll_step")) {
        bd_config.scroll_step = atoi(value);
      } else if (!strcmp(key, "scroll_image_step")) {
        bd_config.scroll_image_step = atoi(value);
      } else if (!strcmp(key, "scroll_width_margin")) {
        bd_config.scroll_width_margin = atoi(value);
      } else if (!strcmp(key, "undo_edit_count")) {
        bd_config.undo_edit_count = atoi(value);
      } else if (!strcmp(key, "undo_depth")) {
        bd_config.undo_depth = atoi(value);
      } else if (!strcmp(key, "theme")) {
        bd_config.theme = atoi(value);
      } else if (!strcmp(key, "xterm_colors")) {
        bd_config.xterm_colors = atoi(value);
      } else if (!strcmp(key, "terminal_count")) {
        bd_config.terminal_count = atoi(value);
      } else if (!strcmp(key, "column_guide")) {
        bd_config.column_guide = atoi(value);
      } else if (!strcmp(key, "column_tiny")) {
        bd_config.column_tiny = atoi(value);
      } else if (!strcmp(key, "shell_path")) {
        strncpy(bd_config.shell_path, value, sizeof(bd_config.shell_path) - 1);
        bd_config.shell_path[sizeof(bd_config.shell_path) - 1] = '\0';
      } else if (strncmp(key, "syntax_color_", 13) == 0) {
        int index = atoi(key + 13);
        if (index >= 0 && index < st_color_count) {
          // Decode hex string
          int value_len = strlen(value);
          int out_pos = 0;
          for (int i = 0; i < value_len && i + 1 < value_len && out_pos < 63; i += 2) {
            char hex[3] = {value[i], value[i+1], '\0'};
            bd_config.syntax_colors[index][out_pos++] = (char)strtol(hex, NULL, 16);
          }
          bd_config.syntax_colors[index][out_pos] = '\0';
        }
      }
    }
    
    line = strtok(NULL, "\n");
  }
  
  return 1;
}

// Save config in text-based format
int bd_config_save(const char *path) {
  io_file_t file = io_fopen(path, 1);
  if (!io_fvalid(file)) {
    return 0;
  }
  
  char buffer[8192];
  int len = 0;
  int remaining = sizeof(buffer) - 1;
  
  #define SAFE_APPEND(...) do { \
    int written = snprintf(buffer + len, remaining, __VA_ARGS__); \
    if (written < 0 || written >= remaining) return 0; \
    len += written; \
    remaining -= written; \
  } while(0)
  
  SAFE_APPEND("# bedd configuration file\n");
  SAFE_APPEND("# This file is in text format for easy editing\n\n");
  
  SAFE_APPEND("indent_width=%d\n", bd_config.indent_width);
  SAFE_APPEND("indent_spaces=%d\n", bd_config.indent_spaces);
  SAFE_APPEND("indent_guides=%d\n", bd_config.indent_guides);
  SAFE_APPEND("scroll_step=%d\n", bd_config.scroll_step);
  SAFE_APPEND("scroll_image_step=%d\n", bd_config.scroll_image_step);
  SAFE_APPEND("scroll_width_margin=%d\n", bd_config.scroll_width_margin);
  SAFE_APPEND("undo_edit_count=%d\n", bd_config.undo_edit_count);
  SAFE_APPEND("undo_depth=%d\n", bd_config.undo_depth);
  SAFE_APPEND("theme=%d\n", bd_config.theme);
  SAFE_APPEND("xterm_colors=%d\n", bd_config.xterm_colors);
  SAFE_APPEND("terminal_count=%d\n", bd_config.terminal_count);
  SAFE_APPEND("column_guide=%d\n", bd_config.column_guide);
  SAFE_APPEND("column_tiny=%d\n", bd_config.column_tiny);
  
  SAFE_APPEND("\nshell_path=%s\n", bd_config.shell_path);
  
  SAFE_APPEND("\n# Syntax colors (hex-encoded for ANSI escape sequences)\n");
  for (int i = 0; i < st_color_count; i++) {
    SAFE_APPEND("syntax_color_%d=", i);
    // Encode as hex
    for (int j = 0; j < 64 && bd_config.syntax_colors[i][j] != '\0'; j++) {
      SAFE_APPEND("%02x", (unsigned char)bd_config.syntax_colors[i][j]);
    }
    SAFE_APPEND("\n");
  }
  
  #undef SAFE_APPEND
  
  io_fwrite(file, buffer, len);
  io_fclose(file);
  
  return 1;
}
