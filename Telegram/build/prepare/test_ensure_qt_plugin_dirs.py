import ast
import pathlib
import re
import subprocess
import sys
import tempfile
import unittest


script_path = pathlib.Path(__file__).with_name('ensure_qt_plugin_dirs.py')
prepare_path = pathlib.Path(__file__).with_name('prepare.py')


def load_win_fail_on_each(prepare):
    tree = ast.parse(prepare)
    function = next(
        node for node in tree.body
        if isinstance(node, ast.FunctionDef) and node.name == 'winFailOnEach')
    namespace = {'re': re}
    exec(compile(
        ast.Module(body=[function], type_ignores=[]),
        str(prepare_path),
        'exec'), namespace)
    return namespace['winFailOnEach']


class EnsureQtPluginDirsTest(unittest.TestCase):
    def run_helper(self, root, *modules):
        return subprocess.run(
            [sys.executable, str(script_path), str(root), *modules],
            capture_output=True,
            text=True,
            check=False)

    def test_creates_missing_plugin_directories(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = pathlib.Path(temporary)
            modules = ('qtimageformats', 'qtsvg')
            for module in modules:
                (root / module / 'mkspecs').mkdir(parents=True)

            result = self.run_helper(root, *modules)

            self.assertEqual(result.returncode, 0, result.stderr)
            for module in modules:
                self.assertTrue((root / module / 'mkspecs' / 'modules-inst').is_dir())

    def test_returns_failure_for_invalid_target(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = pathlib.Path(temporary)
            target = root / 'qtimageformats' / 'mkspecs' / 'modules-inst'
            target.parent.mkdir(parents=True)
            target.write_text('not a directory', encoding='utf-8')

            result = self.run_helper(root, 'qtimageformats')

            self.assertNotEqual(result.returncode, 0)
            self.assertIn('FileExistsError', result.stderr)

    def test_prepare_runs_guard_before_parallel_qt_build(self):
        prepare = prepare_path.read_text(encoding='utf-8')
        guard = 'python "%PREPARE_SCRIPT_DIR%\\\\ensure_qt_plugin_dirs.py" . qtimageformats qtsvg'
        first_build = 'jom -j%NUMBER_OF_PROCESSORS%'
        qt_stage = prepare.index("stage('qt_' + qt")
        qt_stage_end = prepare.index('""")', qt_stage)
        guard_position = prepare.index(guard, qt_stage, qt_stage_end)
        first_build_position = prepare.index(first_build, qt_stage, qt_stage_end)

        runtime_guard = guard.replace('\\\\', '\\')
        generated = load_win_fail_on_each(prepare)(runtime_guard + '\n' + first_build)
        fail_fast = '\r\nif %errorlevel% neq 0 exit /b %errorlevel%\r\n'
        expected = 'call ' + runtime_guard + fail_fast + 'call ' + first_build + fail_fast

        self.assertLess(guard_position, first_build_position)
        self.assertEqual(generated, expected)


if __name__ == '__main__':
    unittest.main()
