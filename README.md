
# PiePerseus

"Development environment" to test your Perseus modifications in ~ 1 minute.

## Requirements:
* Python with f-string support and `pip install loguru`
* `java` in path
* Android Studio with the following components installed and in `PATH`:
  * NDK: for `ndk-build[.cmd]`
  * Android SDK: for `zipalign[.exe]` and `apksigner[.bat]`

## Usage
```shell  
$ python perseus.py --help
usage: perseus apk builder [-h] [--skip SKIP] [--clean CLEAN]

builds apk for you (default behaviour if called with no arguments)

options:
  -h, --help     show this help message and exit
  --skip SKIP    skip decompile and extracting if possible
  --clean CLEAN  delete built apk, decompiled sources and xapk
```
