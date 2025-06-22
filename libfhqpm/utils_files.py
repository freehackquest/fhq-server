#!/usr/bin/env python3
##################################################################################
#    __ _
#   / _| |__   __ _       ___  ___ _ ____   _____ _ __
#  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
#  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
#  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
#                |_|
#
# Copyright (c) 2011-2025 FreeHackQuest <freehackquest@gmail.com>
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


""" Utils Files """

import sys
import os


class UtilsFiles:
    """ UtilsFiles """

    @staticmethod
    def get_all_files(_startdir):
        """ recursive find all files in dir """
        _ret = []
        _rec = [_startdir]
        while len(_rec) > 0:
            _dirpath = _rec[0]
            del _rec[0]
            for _file in os.listdir(_dirpath):
                _filepath = os.path.join(_dirpath, _file)
                if os.path.isdir(_filepath):
                    _rec.append(_filepath)
                    continue
                if os.path.isfile(_filepath):
                    _ret.append(_filepath)
        return _ret

    @staticmethod
    def human_readable_size(size, decimal_places=2):
        """ convert bytes to humanable view """
        for unit in ['B', 'KiB', 'MiB', 'GiB', 'TiB', 'PiB']:
            if size < 1024.0 or unit == 'PiB':
                break
            size /= 1024.0
        return f"{size:.{decimal_places}f} {unit}"

    @staticmethod
    def safe_read_file(_filepath, exit_on_error=True):
        """ __test_utf8_encoding """
        # print(_filepath)
        _lines = []
        try:
            with open(_filepath, "rt", encoding="utf-8") as _file:
                _lines = _file.readlines()
        except UnicodeDecodeError as _err:
            print(_err)
            if exit_on_error:
                sys.exit("Problem with encoding in file: " + _filepath)
            return []
        return _lines

    @staticmethod
    def recoursive_remove_files(_dir):
        """ recoursive_remove_files """
        for _file in os.listdir(_dir):
            _fullpath = os.path.join(_dir, _file)
            if os.path.isfile(_fullpath):
                os.remove(_fullpath)
            elif os.path.isdir(_fullpath):
                UtilsFiles.recoursive_remove_files(_fullpath)
        os.rmdir(_dir)
