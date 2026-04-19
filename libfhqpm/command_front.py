#!/usr/bin/env python3
##################################################################################
#                      Project
#    __ _
#   / _| |__   __ _       ___  ___ _ ____   _____ _ __
#  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
#  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
#  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
#                |_|
#
# MIT License
#
# Copyright (c) 2011-2026 FreeHackQuest <freehackquest@gmail.com>
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
# Original repository: https://github.com/freehackquest/fhq-server
#
##################################################################################

""" Build front end in docker-container """

import os
import sys
import logging
from .pm_config import PmConfig

logging.basicConfig()


class CommandFront:
    """ CommandFront """

    def __init__(self, config: PmConfig):
        self.__log = logging.getLogger("CommandFront")
        self.__log.setLevel(logging.DEBUG)
        self.__config = config
        self.__subcommand_name = "front"

    def get_name(self):
        """ return subcommand name """
        return self.__subcommand_name

    def do_registry(self, subparsers):
        """ registering sub command """
        _parser_frontend = subparsers.add_parser(
            name=self.__subcommand_name,
            description='Build Frontend'
        )
        _parser_frontend.add_argument(
            '-b', '--build',
            dest='build_dist',
            help='Build',
            action='store_true',
        )
        _parser_frontend.add_argument(
            '-r', '--run',
            dest='run_npm_server',
            help='Run',
            action='store_true',
        )
        _parser_frontend.set_defaults(subparser=self.__subcommand_name)

    def execute(self, args):
        """ executing """
        if not args.build_dist and not args.run_npm_server:
            self.__log.info("Please run with argument '--run' or '--build'")
            sys.exit(1)
        if args.build_dist and args.run_npm_server:
            self.__log.info("Please run with one argument '--run' or '--build'")
            sys.exit(1)

        self.__log.info("Start...")
        os.chdir(self.__config.get_root_dir())
        self.__log.info("Try stop container 'fhq_frontend'")
        os.system("docker stop fhq_frontend")

        # os.system("docker rm fhq_frontend")
        docker_command = ""
        docker_command += "docker run --rm "
        docker_command += " --name fhq_frontend "
        docker_command += " -v $(pwd)/web-user:/root/web-user"
        docker_command += " -p 4200:4200 "
        docker_command += " -w /root/web-user "
        docker_command += " sea5kg/fhq-server-build-environment:latest "
        shell_command = [
            "echo clean_dist",
            "rm -rf /root/web-user/dist",
            "echo clean_node_modules",
            "rm -rf /root/web-user/node_modules",
            "echo copy_node_modules_from_cache",
            "cp -rf /root/node_modules_cache/node_modules /root/web-user",
            "echo npm_install",
            "npm install",
        ]
        if args.build_dist:
            shell_command.extend([
                "echo npm_build_prod",
                "npm run build-prod",
            ])
        elif args.run_npm_server:
            shell_command.extend([
                "echo npm_start",
                "npm run start",
            ])
        docker_command += " sh -c '" + " && ".join(shell_command) + "' "
        print(docker_command)
        try:
            os.system(docker_command)
        finally:
            os.system("docker stop fhq_frontend")
