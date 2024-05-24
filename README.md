
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
usage: perseus apk builder [-h] [--skip SKIP] [--clean CLEAN] [--quick-rebuild QUICK_REBUILD]

builds apk for you (this is the default behaviour if called with no arguments)

options:
  -h, --help            show this help message and exit
  --skip SKIP           skip decompile and extracting if possible
  --clean CLEAN         delete built apk, decompiled sources, compiled perseus libs and xapk
  --quick-rebuild QUICK_REBUILD
                        rebuild apk by replacing libs in the apk instead of using apktool (saves 40s)
```

### Recommended usage:
```shell
$ python perseus.py --skip=True --quick-rebuild=True
```

The first build takes 3 to 5 minutes, subsequent builds take about 20 seconds