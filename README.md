# bedd: the bar editor

bedd is an open source, multifunctional editor and environment, with purposes ranging from file exploring to coding and note taking. Its
name comes from 'bar editor', due to the tab bar present on the top of the terminal, while also being a word play on the word "bed". It
uses keybindings and shortcuts commonly found in graphical text editors, in order to provide the flattest learning curve as possible. The
terminal UI is also optimized for simple terminals with limited color support, while being able to provide a "modern" interface. It also
provides builtin help in the form of a welcome screen with all the shortcuts present in bedd.

![(Screenshot of bedd - Welcome screen)](https://raw.githubusercontent.com/segfaultdev/bedd/master/image_01.png)
![(Screenshot of bedd - Editing bedd.c)](https://raw.githubusercontent.com/segfaultdev/bedd/master/image_02.png)

## How to build

bedd can be built using either the provided shell script or the Makefile.

### Using Makefile (Recommended)

```sh
# Normal build (dynamically linked, ~174KB)
make

# Static build (statically linked, ~1.1MB, works on many systems)
make STATIC=1

# Debug build (with debug symbols and sanitizers)
make DEBUG=1

# Install to /usr/local/bin
sudo make install

# Clean build artifacts
make clean
```

The Makefile supports the following targets:
- `make` or `make all` - Build bedd (default)
- `make clean` - Remove build artifacts
- `make install` - Install bedd to /usr/local/bin
- `make uninstall` - Remove bedd from /usr/local/bin
- `make help` - Show help message with all options

**Static Build Benefits:**
- A static build creates a single binary with no external dependencies
- Works on systems without the exact same glibc version
- Can be copied to older or newer Linux systems and run immediately
- Ideal for deployment, containers, or when you need maximum portability

### Using build.sh

```sh
$ sh build.sh # Building it
$ ./bedd      # Running it afterwards
```

## Key Features

### Text Editing

bedd provides a familiar text editing experience with standard keybindings:

- **Indentation**: Use `Ctrl+]` to increase indentation or `Ctrl+[` to decrease indentation on the current line or selected lines. `Tab` and `Shift+Tab` also work for selected text.
- **Selection**: Hold `Shift` while using arrow keys to select text
- **Undo/Redo**: `Ctrl+Z` and `Ctrl+Y`
- **Copy/Cut/Paste**: `Ctrl+C`, `Ctrl+X`, `Ctrl+V`
- **Find/Replace**: `Ctrl+F`
- **Save**: `Ctrl+S`
- **Reload**: `Ctrl+U` to reload the current file from disk (works for text files, images, and directory listings)

Press `Ctrl+W` in bedd to see the welcome screen with all available shortcuts.

## Configuration

bedd stores its configuration in `~/.beddcfg` using a human-readable text format. The configuration file uses simple key=value pairs:

```
# bedd configuration file
indent_width=2
indent_spaces=1
theme=17
shell_path=/usr/bin/sh
```

See `example.beddcfg` for a complete configuration example with all available options.

### Editing Configuration

You can edit the configuration in two ways:

1. **Configuration Editor (Recommended)**: Press `Ctrl+E` in bedd to open the interactive configuration editor. Use arrow keys to navigate, left/right arrows to change values, and `Ctrl+S` to save.

2. **Text File**: Edit `~/.beddcfg` directly with any text editor.

### Terminal Shell

The terminal shell can be customized by setting the `shell_path` option. In the configuration editor:
1. Navigate to "Terminal shell path" 
2. Press `Enter` to open the edit dialog
3. Enter your preferred shell (e.g., `/bin/bash`, `/bin/zsh`, `/usr/bin/fish`)
4. Press `Enter` to confirm and `Ctrl+S` to save

### Migration from Binary Format

If you have an existing binary configuration file from an older version of bedd, it will be automatically converted to the text format on the first run.

## Clipboard Support

bedd automatically detects and uses the best available clipboard mechanism:

1. **xclip** (X11) - If available, uses the X11 clipboard
2. **wl-clipboard** (Wayland) - If available, uses the Wayland clipboard
3. **File-based fallback** - Always available as `~/.bedd_clipboard`

This means bedd's clipboard functionality works even without X11 or Wayland, making it suitable for use over SSH, in containers, or in minimal environments.

## Licensing

This project is licensed with the MIT license, check LICENSE for further details.
