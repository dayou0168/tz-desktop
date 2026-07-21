import pathlib
import sys


def ensure_plugin_directories(qt_root, modules):
    for module in modules:
        mkspecs = qt_root / module / 'mkspecs'
        if not mkspecs.is_dir():
            raise NotADirectoryError('Qt module mkspecs directory was not found: ' + str(mkspecs))
        target = mkspecs / 'modules-inst'
        target.mkdir(exist_ok=True)
        if not target.is_dir():
            raise NotADirectoryError('Qt plugin metadata path is not a directory: ' + str(target))


def main(argv):
    if len(argv) < 3:
        raise ValueError('Usage: ensure_qt_plugin_dirs.py QT_ROOT MODULE [MODULE ...]')
    ensure_plugin_directories(pathlib.Path(argv[1]), argv[2:])


if __name__ == '__main__':
    main(sys.argv)
