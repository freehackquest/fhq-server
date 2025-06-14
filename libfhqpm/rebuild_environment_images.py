#!/usr/bin/env python
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

import re
import os
import sys
import random
import glob2
import string
import datetime


class RebuildEnvironmentImages:
    def __init__(self):
        now = datetime.datetime.now()
        self.__dt_tag = now.strftime("%Y-%m-%d")

    def __build_docker_image(self, tag, filename):
        cmd = "docker build --rm --tag " + tag + " -f " + filename + " ."
        ret = os.system(cmd)
        if ret != 0:
            print("ERROR: Could not build image by command:  " + cmd)
            sys.exit(1)

    def run(self):
        build_env = "Dockerfile.build-environment"
        release_env = "Dockerfile.release-environment"
        tag_build = "sea5kg/fhq-server-build-environment"
        tag_release = "sea5kg/fhq-server-release-environment"

        self.__build_docker_image(tag_build + ":" + self.__dt_tag, build_env)
        # TODO if :latest exist so remove before
        self.__build_docker_image(tag_build + ":latest", build_env)
        self.__build_docker_image(tag_release + ":" + self.__dt_tag, release_env)
        self.__build_docker_image(tag_release + ":latest", release_env)

        return 0
