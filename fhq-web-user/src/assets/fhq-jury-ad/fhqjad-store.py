#!/usr/bin/python2
import sys
import math
import socket
import random
import time
import errno
import os
import requests
import re
import json
import stat
import StringIO
import zipfile
import shutil
import re

version = '0.0.1'
fhq_jury_ad_use_version = 'latest'

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    DARKGRAY = '\033[90m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def usage():
    print("""
Helper's tool for fhq-jury-ad

Usage: \r\n\t""" + sys.argv[0] + """ [command] [name]

Issues: https://github.com/freehackquest/fhq-server/issues

Commands:
    version - version of current script and for fhq-jury-ad
    info - installed services and teams list
    init - will be init game in current directory
    urls - list of urls
    urls-add [url] - will be added url with vulnbox.store
    urls-rm [url] - will be added url with vulnbox.store
    update - update list information about services
    search [search] - search services by keywords and names
    download [name] - download service and checker to folder ./.fhqjad-store/[name]
    install [name] - will be installed ./cond.d/[name] and to vulnbox
    remove [name] - will be removed from conf.d and from vulnbox
    teams-search [search] - list of teams (from vulnbox.store)
    team-add [name] - add the team
    team-rm [name] - remove the team

Directories:
    jury.d - folder with configs for fhq-jury-ad
    docker_jury - folder with Dockerfile for build jury docker
    vulnbox - folder with services and script for vulnbox

""")
    exit(1)

if len(sys.argv) < 2:
    usage()

command = sys.argv[1]
name = ''
if len(sys.argv) > 2:
    name = sys.argv[2]

urls = ["https://vulnbox.store/"]

if os.path.isfile("./.fhqjad-store/urls"):
    with open("./.fhqjad-store/urls") as f:
        urls = f.readlines()
    urls = [x.strip() for x in urls]

def preapareCacheUrl(u, suffix):
    u2 = re.sub(r"[^a-zA-Z0-9]", "_", u)
    return "./.fhqjad-store/cache/" + u2 + "_" + suffix +".json"

def downloadServicesJSON(u):
    print("Download services.json from " + u)
    u2 = preapareCacheUrl(u, "services")
    # TODO check allow
    r = requests.get(u + '/services.json', verify=False)
    f = open(u2, "w")
    f.write(json.dumps(r.json(), indent=4))
    f.close()

def removeServicesJSON(u):
    print("Remove services.json from cache foor " + u)
    cache2 = preapareCacheUrl(u, 'services')
    if os.path.exists(cache2):
        os.remove(cache2)


def cmd_update():
    for u in urls:
        print("Download services.json from " + u)
        u2 = preapareCacheUrl(u, "services")
        r = requests.get(u + '/services.json', verify=False)
        f = open(u2, "w")
        f.write(json.dumps(r.json(), indent=4))
        f.close()
    for u in urls:
        print("Download teams.json from " + u)
        u2 = preapareCacheUrl(u, "teams")
        r = requests.get(u + '/teams.json', verify=False)
        f = open(u2, "w")
        f.write(json.dumps(r.json(), indent=4))
        f.close()
    exit(0)

def cmd_download():
    name_ = name.lower()
    download_url = ''
    for u in urls:
        u2 = preapareCacheUrl(u, "services")
        with open(u2) as f:
            services_json = json.load(f)
        for s in services_json:
            if s['id'] == name_:
                if download_url != '':
                    print("Duplicated service id: " + download_url)
                else:
                    download_url = u + s['id'] + '.zip'
    if download_url == '':
        print("Not found: " + name_ + "\n    try 'vbs search'")
        exit(0)
    dirto = './.fhqjad-store/downloads/service_' + name_
    print("Download: \n\tURL: " + download_url + "\n\tDirectory:" + dirto + "\n")

    if not os.path.isdir(dirto):
        os.mkdir(dirto)
    else:
        shutil.rmtree(dirto)
    r = requests.get(download_url, stream=True, verify=False)
    z = zipfile.ZipFile(StringIO.StringIO(r.content))
    # fileslist = z.namelist()
    z.extractall(dirto)

def mergeLists(l1, l2):
    for i in l2:
        if i not in l1:
            l1.append(i)
    return l1

def updateDockerfileJury():
    print("Updating ./docker_jury/Dockerfile")
    install_linux_packages = []
    install_run_commands = []

    services = os.listdir("./jury.d/checkers/")
    for s in services:
        sinfo = findServiceById(s)
        if sinfo != None:
            install_linux_packages = mergeLists(install_linux_packages, sinfo['checker-install-linux-packages'])
            install_run_commands = mergeLists(install_run_commands, sinfo['checker-install-run-commands'])
    docker_commands = ['#[begin-checkers-install]']

    if len(install_linux_packages) > 0:
        docker_commands.append('RUN apt install -y "' + '" "'.join(install_linux_packages) + '"')

    for cm in install_run_commands:
        docker_commands.append('RUN ' + cm)

    docker_commands.append('#[end-checkers-install]')

    with open('./docker_jury/Dockerfile') as f:
        dockerfile_jury = f.readlines()

    dockerfile_jury = "".join(dockerfile_jury)
    start_i = dockerfile_jury.find("#[begin-checkers-install]")
    end_i = dockerfile_jury.find("#[end-checkers-install]") + len("#[end-checkers-install]")
    _prev = dockerfile_jury[:start_i]
    _next = dockerfile_jury[end_i:]
    dockerfile_jury = _prev + "\n".join(docker_commands) + _next

    with open('./docker_jury/Dockerfile', 'w') as f:
        f.write(dockerfile_jury)

def updateVulnboxServices():
    print("Updating ./vulnbox/run.sh")
    run_sh = []
    run_sh.append("docker-compose")

    services = os.listdir("./vulnbox/")
    for s in services:
        sinfo = findServiceById(s)
        if sinfo != None:
            run_sh.append("-f " + s + "/docker-compose.yml")

    run_sh.append("up --build")

    with open('./vulnbox/run.sh', 'w') as f:
        f.write("#!/bin/bash\n")
        f.write("\n")
        f.write("# Automaticly generted\n")
        f.write("\n")
        f.write(" \\\n".join(run_sh))
        f.write("\n")


def cmd_install():
    name_ = name.lower()
    pattern = re.compile("^([a-z0-9_]+)$")
    if not pattern.match(name_):
        print("Service name invalid")
        exit(0)
    if not findServiceById(name_):
        print("Service not found")
        exit(0)

    dirto = './.fhqjad-store/downloads/service_' + name_
    if not os.path.isdir(dirto):
        cmd_download()
    if not os.path.isdir(dirto):
        print("Not found: " + name_)
        exit(0)
    dir_checker = './jury.d/checkers/' + name_
    if os.path.isdir(dir_checker):
        shutil.rmtree(dir_checker)
    shutil.copytree('./.fhqjad-store/downloads/service_' + name_ + '/checker', dir_checker)
    # TODO checker.py to chmod +x

    dir_vulnbox = './vulnbox/' + name_
    if os.path.isdir(dir_vulnbox):
        shutil.rmtree(dir_vulnbox)
    shutil.copytree('./.fhqjad-store/downloads/service_' + name_ + '/service', dir_vulnbox)
    updateDockerfileJury()
    updateVulnboxServices()

def cmd_remove():
    name_ = name.lower()
    pattern = re.compile("^([a-z0-9_]+)$")
    if not pattern.match(name_):
        print("Service name invalid")
        exit(0)
    if not findServiceById(name_):
        print("Service not found")
        exit(0)

    dir_checker = './jury.d/checkers/' + name_
    if os.path.isdir(dir_checker):
        shutil.rmtree(dir_checker)
    dir_vulnbox = './vulnbox/' + name_
    if os.path.isdir(dir_vulnbox):
        shutil.rmtree(dir_vulnbox)
    updateDockerfileJury()

def findServiceById(service_id):
    res = None
    for u in urls:
        u2 = preapareCacheUrl(u, 'services')
        with open(u2) as f:
            services_json = json.load(f)
        for s in services_json:
            if s['id'] == service_id:
                if not res:
                    s['from'] = u
                    res = s
                else:
                    print("ERROR: duplicate serviceid " + service_id)
    return res

def findTeamById(team_id):
    res = None
    for u in urls:
        u2 = preapareCacheUrl(u, 'teams')
        with open(u2) as f:
            teams_json = json.load(f)
        for s in teams_json:
            if s['id'] == team_id:
                if not res:
                    s['from'] = u
                    res = s
                else:
                    print("ERROR: duplicate teamid " + team_id)
    return res

def cmd_search():
    print("Searching.... '" + name.lower() + "'")
    nFound = 0
    for u in urls:
        u2 = preapareCacheUrl(u, 'services')
        with open(u2) as f:
            services_json = json.load(f)
        for s in services_json:
            combined_text = ''
            combined_text = combined_text + s["name"] + '\n'
            combined_text = combined_text + s["id"] + '\n'
            combined_text = combined_text + s["game"] + '\n'
            combined_text = combined_text + "\n".join(s["keywords"]) + '\n'
            combined_text = combined_text.lower()

            if combined_text.find(name.lower()) != -1:
                nFound = nFound + 1
                print(' ' + bcolors.OKGREEN + s['id'] + bcolors.ENDC + " - " + s['game'] + " / " + s['name'] + " (" + ",".join(s["keywords"]) + ")")
                print("\ton " + bcolors.DARKGRAY + u + bcolors.ENDC)
    print("")
    print(" *** Found: " + str(nFound))
    print("")

def cmd_teams_search():
    print("Searching teams... '" + name.lower() + "'")
    nFound = 0
    for u in urls:
        u2 = preapareCacheUrl(u, 'teams')
        with open(u2) as f:
            teams_json = json.load(f)
        for s in teams_json:
            combined_text = ''
            combined_text = combined_text + s["name"] + '\n'
            combined_text = combined_text + s["id"] + '\n'
            combined_text = combined_text.lower()

            if combined_text.find(name.lower()) != -1:
                nFound = nFound + 1
                print(' ' + bcolors.OKGREEN + s['id'] + bcolors.ENDC + " - " + s['name'] + "  (from " + bcolors.DARKGRAY + u + bcolors.ENDC + ")")
    print("")
    print(" *** Found: " + str(nFound))
    print("")

def cmd_team_add():
    name_ = name.lower()
    pattern = re.compile("^([a-z0-9_]+)$")
    if not pattern.match(name_):
        print("Team name invalid")
        exit(0)
    team_info = findTeamById(name_)
    if team_info != None:
        logo_url = 'https://vulnbox.store/teams/' + team_info['logo']
        logo_path = './jury.d/html/images/teams/' + team_info['logo']
        print("Download logo " + logo_url)
        if not os.path.isfile(logo_path):
            r = requests.get(logo_url, allow_redirects=True, verify=False)
            open(logo_path, 'wb').write(r.content)
    else:
        team_info = {}
        team_info['id'] = name_
        team_info['name'] = name_
        team_info['name'] = raw_input('Team - Name [' + team_info['name']  + ']: ') or team_info['name']
        team_info['logo'] = name_ + '.svg'
        if not os.path.isfile('./jury.d/html/images/teams/' + name_ + '.svg'):
            r = requests.get('https://vulnbox.store/teams/unknown.svg', verify=False, allow_redirects=True)
            open('./jury.d/html/images/teams/' + name_ + '.svg', 'wb').write(r.content)

    team_info['ip_address'] = '127.0.0.1'
    team_info['ip_address'] = raw_input('Team - IP Address [' + team_info['ip_address']  + ']: ') or team_info['ip_address']

    team_conf_path = "./jury.d/teams/" + name_ + ".conf"
    print("Write to " + team_conf_path)
    with open(team_conf_path, 'w') as f:
        f.write("teams." + name_ + ".active = yes\n")
        f.write("teams." + name_ + ".name = " + team_info['name'] + "\n")
        f.write("teams." + name + ".logo = images/teams/" + team_info['logo'] + "\n")
        f.write("teams." + name_ + ".ip_address = " + team_info['ip_address'] + "\n")
        f.close()

def cmd_team_remove():
    name_ = name.lower()
    pattern = re.compile("^([a-z0-9_]+)$")
    if not pattern.match(name_):
        print("Team name invalid")
        exit(0)

    team_conf_path = './jury.d/teams/' + name_ + '.conf'

    if not os.path.isfile(team_conf_path):
        print("Team not found")
        exit(0)

    cnf = parseConf(team_conf_path)


    if os.path.isfile('./jury.d/html/' + cnf['teams.' + name_ + '.logo']):
        os.remove('./jury.d/html/' + cnf['teams.' + name_ + '.logo'])

    os.remove(team_conf_path)
    print("Removed")

def cmd_info():
    services = os.listdir("./jury.d/checkers/")
    services.sort()
    print(" *** INFORMATION ***\n")
    print("Checkers:\n")
    n = 0
    for s in services:
        sinfo = findServiceById(s)
        if sinfo != None:
            n = n + 1
            print(bcolors.OKGREEN + ' * installed ' + sinfo['id'] + bcolors.ENDC + " - " + sinfo['game'] + " / " + sinfo['name'] + " (" + ",".join(sinfo["keywords"]) + ")")
            print("\tfrom " + bcolors.DARKGRAY + sinfo['from'] + bcolors.ENDC)
    if n == 0:
        print(" " + bcolors.WARNING + " (!) Search and choose: 'vbs search <filter>'"
        "\n (!) and then try install 'vbs install [name]'" + bcolors.ENDC)

    print("\nTeams:\n")
    n = 0
    teams = os.listdir("./jury.d/teams/")
    teams.sort()
    for t in teams:
        n = n + 1
        team_id = t.split(".")[0]
        team_info = findTeamById(team_id)
        cnf = parseConf("./jury.d/teams/" + t)
        if team_info == None:
            team_info = {}
            team_info['id'] = team_id
        frm = ''

        team_info['name'] = cnf['teams.' + team_id + '.name']
        team_info['logo'] = cnf['teams.' + team_id + '.logo']

        if 'from' in team_info:
            frm = bcolors.DARKGRAY + " (from: " + team_info['from'] + ")" + bcolors.ENDC
        print(bcolors.OKGREEN + ' * defined team ' + team_info['id'] + bcolors.ENDC + " - " + team_info['name'] + ', logo: ./jury.d/html/' + team_info['logo'] + frm)

    if n == 0:
        print(bcolors.WARNING
            + " (!) Add new team: 'vbs team-add [name]'"
            + "\n (!) Alse you can look preconfigured list of teams: 'vbs teams'"
            + bcolors.ENDC)
    print("")

def parseConf(path):
    if not os.path.isfile(path):
        return {}

    with open(path) as f:
        content = f.readlines()
    content = [x.strip() for x in content]
    conf = {}
    for lin in content:
        if lin.find('=') != -1:
            param_name = lin[0:lin.find('=')]
            param_value = lin[lin.find('=')+1:]
            param_name = param_name.strip()
            param_value = param_value.strip()
            conf[param_name] = param_value

    return conf

def cmd_init():
    # preapre struct
    create_dirs = []
    create_dirs.append("./jury.d")
    create_dirs.append("./jury.d/checkers")
    create_dirs.append("./jury.d/teams")
    create_dirs.append("./jury.d/html") # files will be created by jury
    create_dirs.append("./jury.d/html/images")
    create_dirs.append("./jury.d/html/images/teams")
    create_dirs.append("./jury.d/logs")
    create_dirs.append("./.fhqjad-store")
    create_dirs.append("./.fhqjad-store/cache")
    create_dirs.append("./.fhqjad-store/downloads")
    create_dirs.append("./vulnbox")
    create_dirs.append("./docker_jury")

    for dn in create_dirs:
        if not os.path.isdir(dn):
            print("Create directory " + dn)
            os.mkdir(dn)

    if not os.path.isfile("./.fhqjad-store/urls"):
        f = open("./.fhqjad-store/urls", "w")
        f.write("https://vulnbox.store/")
        f.close()

    game_conf = {}
    game_conf['game.id'] = 'test'
    game_conf['game.name'] = 'Test'
    game_conf['game.start'] = '2018-01-09 11:00:00'
    game_conf['game.end'] = '2018-01-09 12:00:00'
    game_conf['game.flag_timelive_in_min'] = '10'

    if os.path.isfile('./jury.d/game.conf'):
        print("Your already inited game here. Please remove all or create a init new in another place")
        game_conf = parseConf('./jury.d/game.conf')
        #return

    game_conf['game.id'] = raw_input('Game - ID [' + game_conf['game.id'] + ']: ') or game_conf['game.id']
    game_id = game_conf['game.id']

    pattern = re.compile("^([a-z0-9_]+)$")
    if not pattern.match(game_id):
        print("Game ID invalid")
        exit(0)

    game_conf['game.name'] = raw_input('Game - Name [' + game_conf['game.name'] + ']: ') or game_conf['game.name']

    game_conf['game.start'] = raw_input('Game - Start Time (UTC) [' + game_conf['game.start'] + ']: ') or game_conf['game.start']
    # TODO check format time

    game_conf['game.end'] = raw_input('Game - End Time (UTC) [' + game_conf['game.end'] + ']: ') or game_conf['game.end']
    # TODO check format time

    game_conf['game.flag_timelive_in_min'] = raw_input('Game - Flag Timelive (in minutes) [' + game_conf['game.flag_timelive_in_min'] + ']: ') or game_conf['game.flag_timelive_in_min']
    # TODO check int

    print("Prepare ./jury.d/game.conf")
    with open('./jury.d/game.conf', 'w') as f:
        f.write("game.id = " + game_id + "\n")
        f.write("game.name = " + game_conf['game.name'] + "\n")
        f.write("game.start = " + game_conf['game.start'] + "\n")
        f.write("game.end = " + game_conf['game.end'] + "\n")
        f.write("game.flag_timelive_in_min = " + game_conf['game.flag_timelive_in_min'] + "\n")
        f.close()

    scoreboard_conf = {}
    scoreboard_conf['scoreboard.port'] = '8080'

    if os.path.isfile('./jury.d/scoreboard.conf'):
        scoreboard_conf = parseConf('./jury.d/scoreboard.conf')

    scoreboard_conf['scoreboard.port'] = raw_input('Scoreboard - Port [' + scoreboard_conf['scoreboard.port'] + ']: ') or scoreboard_conf['scoreboard.port']
    scoreboard_port = scoreboard_conf['scoreboard.port']

    print("Prepare ./jury.d/scoreboard.conf")
    with open('./jury.d/scoreboard.conf', 'w') as f:
        f.write("scoreboard.port = " + scoreboard_port + "\n")
        f.write("scoreboard.htmlfolder = ./html\n")
        f.write("scoreboard.random = no\n")
        f.close()

    print("Prepare ./jury.d/server.conf")
    with open('./jury.d/server.conf', 'w') as f:
        f.write("server.use_storage = mysql\n")
        f.close()

    print("Prepare ./jury.d/mysql_storage.conf")
    mysql_dbhost = 'ctfgame_' + game_id + "_db"
    mysql_dbname = "ctfgame_" + game_id
    mysql_dbuser = "ctfgame_" + game_id
    mysql_password = "uW2r0V5kpK" # TODO random
    mysql_root_password = "KzhyntJxwt" # TODO random
    with open('./jury.d/mysql_storage.conf', 'w') as f:
        f.write("mysql_storage.dbhost = " + mysql_dbhost + "\n")
        f.write("mysql_storage.dbport = 3306\n")
        f.write("mysql_storage.dbname = " + mysql_dbname + "\n")
        f.write("mysql_storage.dbuser = " + mysql_dbuser + "\n")
        f.write("mysql_storage.dbpass = " + mysql_password + "\n")
        f.close()

    print("Prepare ./docker_jury/build_docker.sh")
    with open('./docker_jury/build_docker.sh', 'w') as f:
        f.write("#!/bin/bash\n")
        f.write("\n")
        f.write("docker build --rm=true -t 'ctfgame-" + game_id + "/fhq-jury-ad:" + fhq_jury_ad_use_version + "' .\n")
    os.chmod('./docker_jury/build_docker.sh', stat.S_IRWXU | stat.S_IRWXG | stat.S_IRWXO)

    print("Prepare ./docker_jury/clean_docker.sh")
    with open('./docker_jury/clean_docker.sh', 'w') as f:
        f.write("#!/bin/bash\n")
        f.write("\n")
        f.write("docker rmi -f 'ctfgame-" + game_id + "/fhq-jury-ad:" + fhq_jury_ad_use_version + "'\n")
    os.chmod('./docker_jury/clean_docker.sh', stat.S_IRWXU | stat.S_IRWXG | stat.S_IRWXO)

    print("Prepare ./docker_jury/Dockerfile")
    with open('./docker_jury/Dockerfile', 'w') as f:
        f.write("FROM freehackquest/fhq-jury-ad:" + fhq_jury_ad_use_version + "\n")
        f.write("\n")
        f.write("### Next lines please don't change manually - because will be automatically overrided \n")
        f.write("#[begin-checkers-install]\n")
        f.write("#[end-checkers-install]\n")
        f.write("\n")
        f.write("EXPOSE " + scoreboard_port + "\n")
        f.write("\n")
        f.write("CMD fhq-jury-ad start\n")

    print("Prepare ./docker-compose.yml")
    with open('./docker-compose.yml', 'w') as f:
        f.write("""version: '2'

services:
  """ + mysql_dbhost + """:
    image: mysql:5.7
    volumes:
      - "./dckr_mysql:/var/lib/mysql"
    restart: always
    environment:
      MYSQL_ROOT_PASSWORD: """ + mysql_root_password + """
      MYSQL_DATABASE: """ + mysql_dbname + """
      MYSQL_USER: """ + mysql_dbuser + """
      MYSQL_PASSWORD: """ + mysql_password + """
    networks:
      - ctfgame_""" + game_id + """_net

  """ + game_id + """_jury:
    depends_on:
      - """ + mysql_dbhost + """
    image: ctfgame-""" + game_id + """/fhq-jury-ad:""" + fhq_jury_ad_use_version + """
    volumes:
      - "./jury.d:/usr/share/fhq-jury-ad/jury.d"
    ports:
      - \"""" + scoreboard_port + """:""" + scoreboard_port + """\"\n
    restart: always
    links:
      - \"""" + mysql_dbhost + """\"
    networks:
      - ctfgame_""" + game_id + """_net

networks:
  ctfgame_""" + game_id + """_net:
    driver: bridge
        """)
        f.close()

    updateDockerfileJury()

if command == "init":
    cmd_init()
    cmd_update()

if command == "urls":
    print("")
    print("URLs: ")
    i = 0
    for u in urls:
        print(str(i) + ": " + u)
        i = i + 1
    print("")
    exit(0)

if command == "urls-add":
    urls.append(name)
    f = open("./.fhqjad-store/urls", "w")
    for u in urls:
        f.write(u + "\r\n")
    f.close()
    downloadServicesJSON(name)
    exit(0)

if command == "urls-rm":
    new_urls = []
    for u in urls:
        if u != name:
            new_urls.append(u)
    f = open("./.fhqjad-store/urls", "w")
    for u in new_urls:
        f.write(u + "\r\n")
    f.close()
    removeServicesJSON(name)
    exit(0)

if command == "version":
    print("Version script: " + version)
    print("Will be used version of fhq-jury-ad: " + fhq_jury_ad_use_version)

if command == "update":
    cmd_update()
    exit(0)

if command == "info":
    cmd_info()
    exit(0)

if command == "search":
    cmd_search()
    exit(0)

if command == "download":
    cmd_download()
    exit(0)

if command == "install":
    cmd_install()
    exit(0)

if command == "remove":
    cmd_remove()
    exit(0)

if command == "teams-search":
    cmd_teams_search()
    exit(0)

if command == "team-add":
    cmd_team_add()
    exit(0)

if command == "team-rm":
    cmd_team_remove()
    exit(0)

usage()
