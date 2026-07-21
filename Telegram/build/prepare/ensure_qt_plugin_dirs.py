import pathlib
import sys


def ensure_plugin_directories(qt_root, modules):
    for module in modules:
        module_root = qt_root / module
        if not module_root.is_dir():
            raise NotADirectoryError('Qt module directory was not found: ' + str(module_root))
        target = module_root / 'mkspecs' / 'modules-inst'
        target.mkdir(parents=True, exist_ok=True)
        if not target.is_dir():
            raise NotADirectoryError('Qt plugin metadata path is not a directory: ' + str(target))


def main(argv):
    if len(argv) < 3:
        raise ValueError('Usage: ensure_qt_plugin_dirs.py QT_ROOT MODULE [MODULE ...]')
    ensure_plugin_directories(pathlib.Path(argv[1]), argv[2:])


if __name__ == '__main__':
    main(sys.argv)
