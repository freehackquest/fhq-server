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

""" code c++ check """

import os
import glob2
import re


class CodeCheck:
    def __init__(self):
        self.__root = os.path.realpath(os.path.dirname(__file__))
        self.__root = os.path.join(self.__root, "..")
        self.__root = os.path.normpath(self.__root)
        # print("Root: " + self.__root)
        print("")
        self.__result = {
            "tabs": [],
            "todo": [],
            "if-format": [],
            "for-format": [],
            "while-format": [],
            "end-brackets": [],
            "start-bracket-else": [],
            "end-bracket-else": [],
            "auto": [],
        }
        self.__processed_files = 0
        self.__processed_lines = 0

    def is_ignore_path(self, _filepath):
        if '/third-party/' in _filepath:
            return True
        if '/tmp/' in _filepath:
            return True
        if '/logs/' in _filepath:
            return True
        if _filepath.endswith("unit-tests"):
            return True
        if _filepath.endswith(".png"):
            return True
        if _filepath.endswith(".sh"):
            return True
        if _filepath.endswith("CMakeLists.txt"):
            return True
        if _filepath.endswith("CMakeLists.user-custom.txt"):
            return True
        return False

    def __check_tabs(self, _line, _filepath, _line_number):
        if '\t' in _line:
            error_msg = "ERROR: Found tab in " + _filepath + ":" + str(_line_number)
            print(error_msg)
            self.__result["tabs"].append(error_msg)

    def __check_todo(self, _line, _filepath, _line_number):
        if 'TODO' in _line:
            error_msg = "WARNING: Found TODO in " + _filepath + ":" + str(_line_number)
            # print(error_msg)
            self.__result["todo"].append(error_msg)

    def __check_if_format(self, _line, _filepath, _line_number):
        _pattern = r'.*[ ]+if[ ]*\(.*'
        if re.match(_pattern, _line) and 'if (' not in _line:
            error_msg = "ERROR: Found wrong if-format (expected 'if (...)' )"
            error_msg += "in " + _filepath + ":" + str(_line_number)
            print(error_msg)
            self.__result["if-format"].append(error_msg)

    def __check_for_format(self, _line, _filepath, _line_number):
        _pattern = r'.*[ ]+for[ ]*\(.*'
        if re.match(_pattern, _line) and 'for (' not in _line:
            error_msg = "ERROR: Found wrong for-format (expected 'for (...)' )"
            error_msg += " in " + _filepath + ":" + str(_line_number)
            print(error_msg)
            self.__result["for-format"].append(error_msg)

    def __check_while_format(self, _line, _filepath, _line_number):
        _pattern = r'.*[ ]+while[ ]*\(.*'
        if re.match(_pattern, _line) and 'while (' not in _line:
            error_msg = "ERROR: Found wrong while-format (expected 'while (...)' )"
            error_msg += " in " + _filepath + ":" + str(_line_number)
            print(error_msg)
            self.__result["while-format"].append(error_msg)

    def __check_end_brackets(self, _line, _filepath, _line_number):
        _pattern = r'.*\)[ ]*\{.*'
        if re.match(_pattern, _line) and ') {' not in _line:
            error_msg = "ERROR: Found wrong end-brackets (expected '...) {...' )"
            error_msg += " in " + _filepath + ":" + str(_line_number)
            print(error_msg)
            self.__result["end-brackets"].append(error_msg)

    def __check_start_bracket_else(self, _line, _filepath, _line_number):
        _pattern = r'.*\}[ ]*else.*'
        if re.match(_pattern, _line) and '} else' not in _line:
            error_msg = "ERROR: Found wrong start-bracket-else (expected '...} else...' )"
            error_msg += " in " + _filepath + ":" + str(_line_number)
            print(error_msg)
            self.__result["start-bracket-else"].append(error_msg)

    def __check_end_bracket_else(self, _line, _filepath, _line_number):
        _pattern = r'.*else[ ]*\{.*'
        if re.match(_pattern, _line) and 'else {' not in _line:
            error_msg = "ERROR: Found wrong end-bracket-else (expected '...else {...' )"
            error_msg += " in " + _filepath + ":" + str(_line_number)
            print(error_msg)
            self.__result["end-bracket-else"].append(error_msg)

    def __check_auto(self, _line, _filepath, _line_number):

        _pattern = r'.*[^\w]+auto[^\w]+.*'
        if re.match(_pattern, _line):
            if ' = new ' in _line:  # skip if used new constructor
                return
            if ' = findWsjcppEmploy<' in _line:  # skip if used findWsjcppEmploy
                return
            error_msg = "WARNING: Found auto in " + _filepath + ":" + str(_line_number)
            print(error_msg)
            self.__result["auto"].append(error_msg)

    def __print_result(self):
        print("\n ---- result ----")
        print("Processed files: ", self.__processed_files)
        print("Processed lines: ", self.__processed_lines)
        for _stat in self.__result:
            print(_stat, len(self.__result[_stat]), "times in code")

    def run(self):
        _dirs = [
            os.path.join(self.__root, "src"),
            os.path.join(self.__root, "unit-tests.wsjcpp"),
        ]
        _files = []
        for _dir in _dirs:
            _files.extend(glob2.glob(os.path.join(_dir, "**", "*")))

        for _filepath in _files:
            if self.is_ignore_path(_filepath):  # skip
                continue
            if os.path.isdir(_filepath):
                continue
            self.__processed_files += 1
            # print(_filepath)
            with open(_filepath, "rt", encoding="utf-8") as _file:
                _lines = _file.readlines()
                _line_number = 0
                for _line in _lines:
                    _line_number += 1
                    self.__processed_lines += 1
                    self.__check_tabs(_line, _filepath, _line_number)
                    self.__check_todo(_line, _filepath, _line_number)
                    self.__check_if_format(_line, _filepath, _line_number)
                    self.__check_for_format(_line, _filepath, _line_number)
                    self.__check_while_format(_line, _filepath, _line_number)
                    self.__check_end_brackets(_line, _filepath, _line_number)
                    self.__check_start_bracket_else(_line, _filepath, _line_number)
                    self.__check_end_bracket_else(_line, _filepath, _line_number)
                    self.__check_auto(_line, _filepath, _line_number)
        self.__print_result()
        _errors = 0
        for _stat in self.__result:
            for _msg in self.__result[_stat]:
                if "ERROR" in _msg:
                    _errors += 1
        return _errors
