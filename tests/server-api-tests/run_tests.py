#!/usr/bin/env python3
# Copyright (C) 2011-2025, Evgenii Sopov <mrseakg@gmail.com>

""" run all tests """

import sys
import os
import subprocess

def recursive_find_all_py_files(_dir, ignores=None):
    """ recursive_find_all_cbproj_files """
    _ret = []
    for _file in os.listdir(_dir):
        _fullpath = os.path.join(_dir, _file)
        if _file == '.git':
            continue
        if not _file.endswith(".py"):
            continue
        if ignores is not None and _file in ignores:
            continue
        if os.path.isdir(_fullpath):
            _ret.extend(recursive_find_all_py_files(_fullpath, ignores))
        else:
            _ret.append(_fullpath)
    return _ret


def run_command(_command, _output):
    """ run_command """
    print("Run command: " + " ".join(_command))
    _output.write("Run command: " + " ".join(_command) + "\n")
    with subprocess.Popen(
        _command,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        shell=False
    ) as _proc:
        _returncode = _proc.poll()
        while _returncode is None:
            _returncode = _proc.poll()
            _line = _proc.stdout.readline()
            if _line:
                _line = _line.decode("utf-8").strip()
                print(_line)
                _output.write(_line + "\n")
        while _line:
            _line = _proc.stdout.readline()
            if _line:
                _line = _line.decode("utf-8").strip()
                print(_line)
                _output.write(_line + "\n")
            else:
                break
        if _returncode != 0:
            print("ERROR: returncode " + str(_returncode))
            _output.write("ERROR: returncode " + str(_returncode) + "\n")
            sys.exit(_returncode)
        return
    sys.exit("Could not start process")

if __name__ == "__main__":
    with open('result_tests.txt', 'w', encoding="utf-8") as _output:
        py_files = recursive_find_all_py_files(".", ignores=[
            "__pycache__",
            ".pytest_cache",
            "freehackquest_libclient_py",
        ])
        try:
            command = [
                'python3', '-m', 'pylint',
                '--rcfile=.pylintrc',
            ]
            command.extend(py_files)
            run_command(command, _output)

            command = [
                'python3', '-m', 'pycodestyle',
            ]
            command.extend(py_files)

            run_command([
                'python3', '-m', 'pytest',
                '-rAs', '-c', 'env-travis.ini',
                # '-k', 'test_server_uuid_info'
            ], _output)
            print('\n\n\nlook result_tests.txt')
        finally:
            print("finally")
