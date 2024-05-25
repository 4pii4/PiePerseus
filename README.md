

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

Recommended usage:

```shell
$ python perseus.py --skip=True --quick-rebuild=True
```

The first build takes 3 to 5 minutes, subsequent builds take about 30-40 seconds


## Features and goals:
- [x] Config file in JSON (instead of archaic INI)
- [ ] Skip animations for even faster battle
- [ ] Chat commands (like minecraft clients)

## Configuration
```json
{
    "OriginalRepo": "github.com/Egoistically/Perseus",
    "PieRepo": "github.com/4pii4/PiePerseus",

    "Aircraft": {
        "Enabled": false,
        "Accuracy": -1,
        "AccuracyGrowth": -1,
        "AttackPower": -1,
        "AttackPowerGrowth": -1,
        "CrashDamage": -1,
        "Hp": -1,
        "HpGrowth": -1,
        "Speed": -1
    },

    "Enemies": {
        "Enabled": false,
        "AntiAir": -1,
        "AntiAirGrowth": -1,
        "AntiSubmarine": -1,
        "Armor": -1,
        "ArmorGrowth": -1,
        "Cannon": -1,
        "CannonGrowth": -1,
        "Evasion": -1,
        "EvasionGrowth": -1,
        "Hit": -1,
        "HitGrowth": -1,
        "Hp": -1,
        "HpGrowth": -1,
        "Luck": -1,
        "LuckGrowth": -1,
        "Reload": -1,
        "ReloadGrowth": -1,
        "RemoveBuffs": false,
        "RemoveEquipment": false,
        "RemoveSkill": false,
        "Speed": -1,
        "SpeedGrowth": -1,
        "Torpedo": -1,
        "TorpedoGrowth": -1
    },

    "Weapons": {
        "Enabled": false,
        "Damage": -1,
        "ReloadMax": -1
    },

    "Misc": {
        "Enabled": false,
        "ExerciseGodmode": false,
        "FastStageMovement": false,
        "Skins": false
    }
}
```
To quote the original creator of Perseus:
![image](https://gist.github.com/assets/138131197/650ac6f0-59da-4f79-b6e5-c839009a6c3d)

