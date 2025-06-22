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

""" rebuild images """

import os
import sys
import logging
import datetime
from .utils_shell import UtilsShell
from .pm_config import PmConfig

logging.basicConfig()


class CommandRebuildEnvironmentImages:
    """ CommandRebuildEnvironmentImages """

    def __init__(self, config: PmConfig):
        self.__log = logging.getLogger("CommandRebuildEnvironmentImages")
        self.__log.setLevel(logging.DEBUG)
        self.__config = config
        now = datetime.datetime.now()
        self.__dt_tag = now.strftime("%Y-%m-%d")
        self.__subcomamnd_name = "rebuild-environment-images"

    def get_name(self):
        """ return subcommand name """
        return self.__subcomamnd_name

    def do_registry(self, subparsers):
        """ registring sub command """
        descr = "Rebuild environment images"
        descr += " (Dockerfile.build-environment && Dockerfile.release-environment)"
        _parser_rebuild_env_images = subparsers.add_parser(
            name=self.__subcomamnd_name,
            description=descr
        )
        _parser_rebuild_env_images.set_defaults(subparser=self.__subcomamnd_name)

    def __build_docker_image(self, tag, filename):
        cmd = "docker build --rm --tag " + tag + " -f " + filename + " ."
        ret = os.system(cmd)
        if ret != 0:
            self.__log.error("ERROR: Could not build image by command: %s", cmd)
            sys.exit(1)

    def __has_image(self, fulltag):
        retcode, output = UtilsShell.run_command_get_output(self.__log, [
            "docker", "images", fulltag, "--format", "{{json . }}"
        ])
        if retcode != 0:
            # self.__log.info("retcode: %s", retcode)
            self.__log.error("Could not execute command 'docker images...', output %s", output)
            sys.exit(-1)
        # self.__log.info("output: %s", output)
        return output != ""  # output not empty... so has image

    def execute(self, _):
        """ executing """
        self.__log.info("Rebuild environment images...")
        os.chdir(self.__config.get_root_dir())

        build_env = "Dockerfile.build-environment"
        release_env = "Dockerfile.release-environment"
        tag_build = "sea5kg/fhq-server-build-environment"
        tag_build_today = tag_build + ":" + self.__dt_tag
        tag_build_latest = tag_build + ":latest"
        tag_release = "sea5kg/fhq-server-release-environment"
        tag_release_today = tag_release + ":" + self.__dt_tag
        tag_release_latest = tag_release + ":latest"

        if self.__has_image(tag_build_today):
            self.__log.info("Found image %s, try removing...", tag_build_today)
            ret = os.system("docker rmi " + tag_build_today)
            if ret != 0:
                sys.exit("")
        self.__build_docker_image(tag_build_today, build_env)

        if self.__has_image(tag_build_latest):
            self.__log.info("Found image %s, try removing...", tag_build_latest)
            ret = os.system("docker rmi " + tag_build_latest)
            if ret != 0:
                sys.exit("")
        self.__build_docker_image(tag_build_latest, build_env)

        if self.__has_image(tag_release_today):
            self.__log.info("Found image %s, try removing...", tag_release_today)
            ret = os.system("docker rmi " + tag_release_today)
            if ret != 0:
                sys.exit("")
        self.__build_docker_image(tag_release_today, release_env)

        if self.__has_image(tag_release_latest):
            self.__log.info("Found image %s, try removing...", tag_release_latest)
            ret = os.system("docker rmi " + tag_release_latest)
            if ret != 0:
                sys.exit("")
        self.__build_docker_image(tag_release_latest, release_env)
        sys.exit(0)
