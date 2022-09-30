#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (C) 2011-2022, Evgenii Sopov <mrseakg@gmail.com>

""" run all tests """

import sys
import subprocess
import glob2

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
    with open('result_tests.txt', 'w') as _output:
        try:
            command = [
                'python3', '-m', 'pylint',
                '--rcfile=.pylintrc',
            ]
            command.extend(glob2.glob("*.py"))
            run_command(command, _output)
            run_command([
                'python3', '-m', 'pytest',
                '-rAs', '-c', 'env-travis.ini'
            ], _output)
            print('\n\n\nlook result_tests.txt')
        finally:
            print("finally")
