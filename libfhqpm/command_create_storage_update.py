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

""" preapre update for database - deprecated """

import re
import os
import sys
import random
import string
import logging

from .utils_copyrights import UtilsCopyrights
from .utils_files import UtilsFiles
from .pm_config import PmConfig

logging.basicConfig()


class CommandCreateStorageUpdate:
    """ CommandCreateStorageUpdate """

    def __init__(self, config: PmConfig):
        self.__log = logging.getLogger("CommandCodeCheck")
        self.__log.setLevel(logging.DEBUG)
        self.__config = config
        self.__subcomamnd_name = "create-storage-update"
        self.__updates = []
        self.__end_points = []
        self.__max_weight = 0

    def get_name(self):
        """ return subcommand name """
        return self.__subcomamnd_name

    def do_registry(self, subparsers):
        """ registring sub command """
        _parser_create_storage_update = subparsers.add_parser(
            name=self.__subcomamnd_name,
            description='DEPRECATED: Create storage update (for change struct of database)'
        )
        _parser_create_storage_update.set_defaults(subparser=self.__subcomamnd_name)

    def __recoursive_search_endpoints(self, spoint, weight):
        found = False
        for _item in self.__updates:
            if _item["from"] == spoint:
                found = True
                self.__recoursive_search_endpoints(_item["to"], weight + 1)
        if not found:
            if weight > self.__max_weight:
                self.__max_weight = weight
            self.__end_points.append({
                "point": spoint,
                "weight": weight
            })

    def __get_cpp_files_with_updates(self, updates_dir):
        files = UtilsFiles.get_all_files(updates_dir)
        _cpp_files = []
        for filepath in files:
            if filepath.lower().endswith(".cpp"):
                _cpp_files.append(filepath)
        self.__log.info("Found updates: %s", str(len(_cpp_files)))
        return _cpp_files

    def __find_new_points(self, updates_dir):

        self.__max_weight = 0
        self.__updates = []
        self.__end_points = []

        _cpp_files = self.__get_cpp_files_with_updates(updates_dir)

        pattern = r'.*WsjcppStorageUpdateBase.*\(.*"([a-zA-Z0-9]*)".*,.*"([a-zA-Z0-9]*)".*,.*\).*'

        for filepath in _cpp_files:
            # print(filepath)
            with open(filepath) as _file:
                line = _file.readline()
                while line:
                    line = line.strip()
                    if re.match(pattern, line):
                        versions = re.search(pattern, line, re.IGNORECASE)
                        if versions:
                            self.__updates.append({
                                "from": versions.group(1),
                                "to": versions.group(2),
                            })
                    line = _file.readline()

        # print(self.__updates)
        # print all updates
        for _item in self.__updates:
            print("[" + _item["from"] + "] -> [" + _item["to"] + "]")

        # find the ends in graph
        self.__recoursive_search_endpoints("", 0)

        # print(self.__end_points)

        if len(self.__end_points) == 0:
            print("Not found updates")
            sys.exit(-1)

        endpoint = ""

        for i in self.__end_points:
            if i["weight"] == self.__max_weight and endpoint == "":
                endpoint = i["point"]
            elif i["weight"] == self.__max_weight and endpoint != "":
                print(
                    "WARNING: Found points with same weights, will be used first. " +
                    "Ignored: " + i["point"]
                )

        self.__log.info("Found point: %s, weight: %s", endpoint, str(self.__max_weight))
        newpoint = ''.join(random.choice(string.ascii_lowercase + string.digits) for _ in range(10))
        return endpoint, newpoint

    def execute(self, _):
        """ executing """
        self.__log.info("Creating storage update...")

        updates_dir = os.path.join(self.__config.get_root_dir(), "src", "storages", "updates")

        endpoint, newpoint = self.__find_new_points(updates_dir)

        filename_h = "update_" + endpoint + "_" + newpoint + ".h"
        filename_cpp = "update_" + endpoint + "_" + newpoint + ".cpp"

        filename_h = os.path.join(updates_dir, filename_h)
        filename_cpp = os.path.join(updates_dir, filename_cpp)
        classname_update = "Update_" + endpoint + "_" + newpoint

        self.__log.info("Generate header file: %s", filename_h)
        with open(filename_h, 'wt', encoding="utf-8", newline="\n") as f_h:
            f_h.write(UtilsCopyrights.get_cpp_copyright())
            f_h.write("\n")
            f_h.write("#pragma once\n")
            f_h.write("\n")
            f_h.write("#include <wsjcpp_storages.h>\n")
            f_h.write("\n")
            f_h.write(
                "class " + classname_update + " : public WsjcppStorageUpdateBase {\n" +
                "    public:\n" +
                "        " + classname_update + "();\n" +
                "};\n"
            )
            f_h.write("\n")

        self.__log.info("Generate source file: %s", filename_cpp)

        with open(filename_cpp,  "wt", encoding="utf-8", newline="\n") as f_cpp:
            f_cpp.write(UtilsCopyrights.get_cpp_copyright())
            f_cpp.write("\n")
            f_cpp.write(
                "#include \"" + classname_update.lower() + ".h\"\n" +
                "\n" +
                "REGISTRY_WSJCPP_STORAGE_UPDATE(" + classname_update + ")\n" +
                "\n"
            )
            f_cpp.write(
                classname_update + "::" + classname_update + "()\n"
                "    : WsjcppStorageUpdateBase(\"" + endpoint + "\", \""
                + newpoint + "\", \"TODO\") {\n"
            )
            f_cpp.write("    \n")
            f_cpp.write("    // fill the array with struct changes\n")
            f_cpp.write("}\n")
            f_cpp.write("\n")
