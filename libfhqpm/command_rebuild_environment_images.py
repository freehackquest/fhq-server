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
        self.__debian_version = "12"
        self.__node_version = "24"
        self.__build_packages = [
            "make",
            "cmake",
            "gcc",
            "g++",
            "curl",
            "pkg-config",
            "libcurl4-openssl-dev",
            "zlib1g-dev",
            "libpng-dev",
            "default-libmysqlclient-dev",
            "libwebsockets-dev",
            "apt-utils",
            "build-essential",
            "nodejs",
        ]
        self.__release_packages = [
            "libcurl4",
            "zlib1g",
            "libpng16-16",
            "libmariadb3",
            "libpthread-stubs0-dev",
            "locales",
        ]

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

    def __update_dockerfile_build_env(self):
        _filename = "Dockerfile.build-environment"
        self.__log.info("Update file %s", _filename)
        os.chdir(self.__config.get_root_dir())
        command_node = "curl -sL https://deb.nodesource.com/setup_" + self.__node_version + ".x"
        command_node += " -o setup_node.sh && bash setup_node.sh"

        with open(_filename, "wt", encoding="utf-8", newline="\n") as _file:
            _file.write("""FROM debian:""" + self.__debian_version + """
WORKDIR /root/

LABEL "maintainer"="Evgenii Sopov <mrseakg@gmail.com>"
LABEL "repository"="https://github.com/freehackquest/fhq-server"

RUN apt-get update && \\
    apt-get install -y curl && \\
    apt-get clean

RUN """ + command_node + """

# basic libs
RUN apt-get update && apt-get install -y --no-install-recommends \\
    """ + " \\\n    ".join(self.__build_packages) + """ \\
    """ + " \\\n    ".join(self.__release_packages) + """

# RUN node --version
# RUN npm --version

RUN apt-get install -y --no-install-recommends \\
    libqt5sql5-mysql \\
    libqt5websockets5 \\
    libqt5websockets5-dev \\
    qtchooser

# prepare cache for build
RUN mkdir /root/node_modules_cache
COPY web-user/package.json /root/node_modules_cache
COPY web-user/package-lock.json /root/node_modules_cache
COPY web-user/package-lock.json /root/node_modules_cache
WORKDIR /root/node_modules_cache
RUN npm install
""")
        return _filename

    def __update_dockerfile_release_env(self):
        _filename = "Dockerfile.release-environment"
        self.__log.info("Update file %s", _filename)
        os.chdir(self.__config.get_root_dir())
        with open(_filename, "wt", encoding="utf-8", newline="\n") as _file:
            _file.write("""FROM debian:""" + self.__debian_version + """

LABEL "maintainer"="Evgenii Sopov <mrseakg@gmail.com>"
LABEL "repository"="https://github.com/freehackquest/fhq-server"

RUN apt-get update && \\
    apt-get install -y \\
    """ + " \\\n    ".join(self.__release_packages) + """

# RUN locale-gen en_US.UTF-8
RUN sed -i -e "s/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/" /etc/locale.gen && \\
    echo 'LANG="en_US.UTF-8"'>/etc/default/locale && \\
    dpkg-reconfigure --frontend=noninteractive locales && \\
    update-locale LANG=en_US.UTF-8
# RUN update-locale LANG=en_US.UTF-8

RUN apt-get install -y \\
    libqt5sql5-mysql \\
    libqt5websockets5 \\
    libqt5core5a \\
    libqt5concurrent5

# RUN apt-get install -y qt5-default qtchooser \\

RUN apt-get clean
""")
        return _filename

    def __silent_remove_image(self, image_tag):
        if self.__has_image(image_tag):
            self.__log.info("Found image %s, try removing...", image_tag)
            ret = os.system("docker rmi " + image_tag)
            if ret != 0:
                self.__log.error("Could not remove %s", image_tag)
                sys.exit(1)

    def execute(self, _):
        """ executing """
        os.chdir(self.__config.get_root_dir())
        build_env = self.__update_dockerfile_build_env()
        release_env = self.__update_dockerfile_release_env()
        self.__log.info("Rebuild environment images...")

        tag_build = "sea5kg/fhq-server-build-environment"
        tag_build_today = tag_build + ":" + self.__dt_tag
        tag_build_latest = tag_build + ":latest"
        tag_release = "sea5kg/fhq-server-release-environment"
        tag_release_today = tag_release + ":" + self.__dt_tag
        tag_release_latest = tag_release + ":latest"

        self.__silent_remove_image(tag_build_today)
        self.__build_docker_image(tag_build_today, build_env)

        self.__silent_remove_image(tag_build_latest)
        self.__build_docker_image(tag_build_latest, build_env)

        self.__silent_remove_image(tag_release_today)
        self.__build_docker_image(tag_release_today, release_env)

        self.__silent_remove_image(tag_release_latest)
        self.__build_docker_image(tag_release_latest, release_env)
        sys.exit(0)
