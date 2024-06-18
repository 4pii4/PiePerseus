

# PiePerseus

"Development environment" to test your Perseus modifications in under one minute.

## Requirements:
* Python with f-string support
* `java` in path
* Android Studio with the following components installed and in `PATH`:
  * NDK: for `ndk-build[.cmd]`
  * Android SDK: for `zipalign[.exe]` and `apksigner[.bat]`

## Usage
```
$ python perseus.py --help
usage: perseus apk builder [-h] [--skip | --no-skip] [--quick-rebuild | --no-quick-rebuild] [--clean | --no-clean]

builds apk for you (this is the default behaviour if called with no arguments)

options:
  -h, --help            show this help message and exit
  --skip, --no-skip     skip decompile and extracting if possible
  --quick-rebuild, --no-quick-rebuild
                        rebuild apk by replacing libs in the apk instead of using apktool (saves 40s)
  --clean, --no-clean   delete built apk, decompiled sources, compiled perseus libs and xapk
```

Recommended usage:

```shell
$ python perseus.py --skip=True --quick-rebuild=True
```

The first build takes 3 to 5 minutes, subsequent builds take about 20-30 seconds.

If the built APK doesn't work for you and you're on Windows, try running the script inside Git Bash/MSYS.

## Features and goals:
- [x] No chat/name censorship
- [x] Config file in `json` (instead of archaic `.ini`)
- [x] Chat commands, **disabled by default**
- [x] Disable low morale warning
- [x] Higher auto repeat limit (instead of 3)
- [ ] Skip animations for even faster battle
  - [x] Skip battleship firing animation
- [ ] Commander name/id/oil/coin/gem spoof
  - [x] Commander name/id/level spoof

## Documentation

* [Chat commands](docs/chat_commands.md)
* [Configuration](docs/configuration.md)

To quote the original creator of Perseus:

![image](https://image.pieland.xyz/file/672d4d4be9f333cb70c49.png)

