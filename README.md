# bedd: the bar editor

bedd is an open source, multifunctional editor and environment, with purposes ranging from file exploring to coding and note taking. Its
name comes from 'bar editor', due to the tab bar present on the top of the terminal, while also being a word play on the word "bed". It
uses keybindings and shortcuts commonly found in graphical text editors, in order to provide the flattest learning curve as possible. The
terminal UI is also optimized for simple terminals with limited color support, while being able to provide a "modern" interface. It also
provides builtin help in the form of a welcome screen with all the shortcuts present in bedd.

![(Screenshot of bedd - Welcome screen)](https://raw.githubusercontent.com/segfaultdev/bedd/master/image_01.png)
![(Screenshot of bedd - Editing bedd.c)](https://raw.githubusercontent.com/segfaultdev/bedd/master/image_02.png)

## How to build

Building bedd is as easy as running the provided script in the main source folder:

```sh
$ sh build.sh # Building it
$ ./bedd      # Running it afterwards
```

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

## Licensing

This project is licensed with the MIT license, check LICENSE for further details.
