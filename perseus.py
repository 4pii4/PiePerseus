import argparse
import glob
import multiprocessing
import os
import shutil
import time
import zipfile
from subprocess import Popen, PIPE, STDOUT, run
import logging

logging.basicConfig(format='%(asctime)s,%(msecs)03d %(levelname)-8s [%(filename)s:%(funcName)s:%(lineno)d] - %(message)s',
                    datefmt='%Y-%m-%d:%H:%M:%S',
                    level=logging.DEBUG)

pkg = 'com.YoStarEN.AzurLane'
pkg_version = '0'
rootdir = os.getcwd()

skip = False
clean = False
quick_rebuild = False


def is_windows() -> bool:
    return os.name in ['nt']


def mkcd(d):
    if not os.path.isdir(d):
        os.mkdir(d)
    os.chdir(d)


def executable_path(e, absolute=True):
    e = os.path.join(rootdir if absolute else '', 'bin', e)
    if is_windows():
        e += '.exe'
    return e


def bbox(cmd):
    # peak laziness
    busybox = os.path.join(rootdir, 'bin', 'busybox')
    if is_windows():
        busybox += '.exe'

    logging.info(f'{busybox} {cmd}')
    os.system(f'{busybox} {cmd}')


def get_version():
    logging.info(f'fetching {pkg} versions')
    global pkg_version
    line = ''
    with Popen([executable_path('apkeep'), '-a', pkg, '-l', 'unused.txt'],
               stdout=PIPE, stderr=STDOUT, text=True) as proc:
        for outputline in proc.stdout:
            line = outputline
    v = line.split(' ')[-1].strip()
    logging.info(f'found {pkg} version {v}')
    pkg_version = v


def build_perseus_lib(do_clean=False):
    logging.info('building perseus libs')

    # thx n0k0m3
    os.chdir('PerseusLib')

    cmd =[f'ndk-build{".cmd" if is_windows() else ""}',
          'NDK_PROJECT_PATH=./src',
          'NDK_APPLICATION_MK=./src/Application.mk',
          'APP_BUILD_SCRIPT=./src/Android.mk',
          'APP_PLATFORM=android-21',
          f'-j{multiprocessing.cpu_count()}'] + (['clean'] if do_clean else [])

    ndk_proc = run(cmd, capture_output=True, text=True)

    output = ndk_proc.stdout
    return_code = ndk_proc.returncode

    if return_code != 0:
        logging.error('ndk-build failed')
        logging.error(output)
        exit(1)

    os.chdir('..')


def download_azurlane():
    if skip and os.path.isfile(f'{pkg}.xapk'):
        logging.info(f'{pkg}.xapk already exists, skipping')
        return
    logging.info(f'downloading {pkg}.xapk')
    os.system(f'{executable_path("apkeep")} -a {pkg} .')
    logging.info(f'downloaded {pkg}.xapk')


def extract_xapk():
    if skip and os.path.isfile(f'{pkg}.apk'):
        logging.info(f'{pkg}.apk is already extracted, skipping')
        return

    logging.info(f'extracting {pkg}.xapk')
    bbox(f'unzip -o ../{pkg}.xapk -d . {pkg}.apk')


def decompile_apk():
    if skip and os.path.isdir(pkg):
        logging.info(f'{pkg}.apk is already decompiled, skipping')
        return

    logging.info(f'decompiling {pkg}.apk')
    if os.path.isdir(pkg):
        shutil.rmtree(pkg)
    os.system(f'java -jar {os.path.join(rootdir, "bin", "apktool.jar")} -q -f d {pkg}.apk')


def copy_perseus_libs():
    logging.info('copying Perseus libs')
    bbox(f'sh -c "cp -r ../PerseusLib/src/libs/* {pkg}/lib"')


def patch():
    logging.info('patching decompiled sources')
    bbox(f'sh {os.path.join(rootdir, 'scripts', 'patch.sh')}')


def rebuild():
    newpkg = f'{pkg}-{pkg_version}.patched.apk'
    tmppkg = newpkg + '.working.zip'
    if quick_rebuild and os.path.isfile(newpkg):
        logging.info(f'rebuiling {pkg}.apk quickly')

        shutil.move(newpkg, tmppkg)
        with zipfile.ZipFile(tmppkg) as old:
            with zipfile.ZipFile(newpkg, 'w') as new:
                for zi in old.infolist():
                    if zi.filename.endswith('libPerseus.so'):
                        new.write(f'{pkg}/{zi.filename}', zi.filename)
                    else:
                        new.writestr(zi, old.read(zi.filename))

        os.remove(tmppkg)

        return

    logging.info(f'rebuilding {pkg}.apk with apktool')
    os.system(f'java -jar {os.path.join(rootdir, "bin", "apktool.jar")} -q -f b {pkg} -o {newpkg}')


def sign_apk():
    f = f'{pkg}-{pkg_version}.patched.apk'
    shutil.move(f, f + '.unsigned')

    logging.info('zipaligning apk')
    zipalign = f'zipalign{".exe" if is_windows() else ""}'
    os.system(f'{zipalign} -pf 4 {f}.unsigned {f}')
    os.remove(f'{f}.unsigned')

    logging.info('signing apk')
    apksigner = f'apksigner{".bat" if is_windows() else ""}'
    key = os.path.join(rootdir, 'signing', 'testkey.pk8')
    cert = os.path.join(rootdir, 'signing', 'testkey.x509.pem')
    os.system(f'{apksigner} sign --key {key} --cert {cert} {f}')

    os.remove(f'{f}.idsig')


def main():
    global skip, clean, quick_rebuild

    parser = argparse.ArgumentParser(
        prog='perseus apk builder',
        description='builds apk for you (this is the default behaviour if called with no arguments)')

    parser.add_argument('--skip', help='skip decompile and extracting if possible', default=False)
    parser.add_argument('--clean', help='delete built apk, decompiled sources, compiled perseus libs and xapk',
                        default=False)
    parser.add_argument('--quick-rebuild',
                        help='rebuild apk by replacing libs in the apk instead of using apktool (saves 40s)',
                        default=False)
    args = parser.parse_args()

    skip = args.skip
    clean = args.clean
    quick_rebuild = args.quick_rebuild

    if clean:
        logging.info('cleaning')
        for f in glob.glob('*.xapk'):
            os.remove(path=f)
        if os.path.isdir('apk_build'):
            shutil.rmtree('apk_build')
        build_perseus_lib(do_clean=True)

    else:
        start = time.time()
        get_version()
        build_perseus_lib()
        download_azurlane()
        mkcd('apk_build')
        extract_xapk()
        decompile_apk()
        copy_perseus_libs()
        patch()
        rebuild()
        sign_apk()
        end = time.time()

        logging.info(
            f'built apk in {os.path.join(rootdir, 'apk_build', f'{pkg}-{pkg_version}.patched.apk')}')
        logging.info(f'done in {round(end - start, 2)} seconds')


if __name__ == '__main__':
    main()
