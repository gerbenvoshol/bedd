# Makefile for bedd editor
# Supports both dynamic and static compilation
# Use: make STATIC=1 for static build
#      make DEBUG=1 for debug build

# Compiler and flags
CC := gcc
CFLAGS := -std=gnu2x -Wall -Wextra -Iinclude

# Optimization flags
ifdef DEBUG
    CFLAGS += -Og -g -fsanitize=undefined,address
else
    CFLAGS += -Os
endif

# Static linking flags
ifdef STATIC
    LDFLAGS += -static
    # For static builds, we need to link pthread statically
    LIBS := -lpthread
else
    LIBS :=
endif

# Strip symbols in release builds (not in debug)
ifndef DEBUG
    LDFLAGS += -s
endif

# Target binary
TARGET := bedd

# Source files
SRCS := bd_config.c \
        bd_dialog.c \
        bd_edit.c \
        bd_explore.c \
        bd_global.c \
        bd_image.c \
        bd_terminal.c \
        bd_text.c \
        bd_view.c \
        bd_welcome.c \
        bedd.c \
        io_linux.c \
        io_new_proj.c \
        match.c \
        st_asm.c \
        st_c.c \
        st_cobol.c \
        st_css.c \
        st_html.c \
        st_js.c \
        st_json.c \
        st_md.c \
        st_py.c \
        st_rs.c \
        st_sh.c \
        syntax.c \
        theme.c

# Object files (in the same directory as source files)
OBJS := $(SRCS:.c=.o)

# Header files for dependency tracking
HEADERS := $(wildcard include/*.h)

# Default target
.PHONY: all
all: $(TARGET)

# Build the target
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $(TARGET)
	@echo "Build complete!"
ifdef STATIC
	@echo "Static build created."
endif

# Compile source files
%.o: %.c $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(OBJS) $(TARGET)
	@echo "Clean complete!"

# Install target
.PHONY: install
install: $(TARGET)
	@echo "Installing $(TARGET) to /usr/local/bin..."
	install -m 755 $(TARGET) /usr/local/bin/
	@echo "Installation complete!"

# Uninstall target
.PHONY: uninstall
uninstall:
	@echo "Uninstalling $(TARGET) from /usr/local/bin..."
	rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstall complete!"

# Help target
.PHONY: help
help:
	@echo "bedd Makefile targets:"
	@echo "  all          - Build bedd (default)"
	@echo "  clean        - Remove build artifacts"
	@echo "  install      - Install bedd to /usr/local/bin"
	@echo "  uninstall    - Remove bedd from /usr/local/bin"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Build options:"
	@echo "  STATIC=1     - Build statically linked binary"
	@echo "  DEBUG=1      - Build with debug symbols and sanitizers"
	@echo ""
	@echo "Examples:"
	@echo "  make                    # Normal build"
	@echo "  make STATIC=1           # Static build"
	@echo "  make DEBUG=1            # Debug build"
	@echo "  make clean all          # Clean and rebuild"
	@echo "  make STATIC=1 install   # Static build and install"

# Phony targets
.PHONY: all clean install uninstall help
