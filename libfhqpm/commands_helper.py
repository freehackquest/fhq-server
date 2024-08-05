#!/usr/bin/env python3
##################################################################################
#    __ _
#   / _| |__   __ _       ___  ___ _ ____   _____ _ __
#  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
#  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
#  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
#                |_|
#
# Copyright (c) 2011-2024 FreeHackQuest <freehackquest@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
##################################################################################

""" run all tests """

import sys
import subprocess


class CommandsHelper:
    """ Helper """

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
