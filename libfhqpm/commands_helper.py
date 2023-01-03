#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (C) 2011-2022, Evgenii Sopov <mrseakg@gmail.com>

""" run all tests """

import sys
import subprocess
import glob2

class CommandsHelper:
    def __init__(self):
        pass
    
    def run_command(self, _command, _output=None):
        """ run_command """
        print("Run command: " + " ".join(_command))
        if _output is not None:
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
                    if _output is not None:
                        _output.write(_line + "\n")
            while _line:
                _line = _proc.stdout.readline()
                if _line:
                    _line = _line.decode("utf-8").strip()
                    print(_line)
                    if _output is not None:
                        _output.write(_line + "\n")
                else:
                    break
            if _returncode != 0:
                print("ERROR: returncode " + str(_returncode))
                if _output is not None:
                    _output.write("ERROR: returncode " + str(_returncode) + "\n")
                sys.exit(_returncode)
            return
        sys.exit("Could not start process")