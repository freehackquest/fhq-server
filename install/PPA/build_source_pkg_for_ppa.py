#!/usr/bin/env python
# -*- coding: utf-8 -*-

import fileinput
import re
import subprocess
import os
import sys

print ("Welcome to preapre ppa package...")


print("\nPlease choose dist name:\n\t1. xenial\n\t4. artful\n\t5. bionic");
dist_name_ = raw_input()
ppa_name_ = "";

if dist_name_ == "1":
	dist_name_ = "xenial";
	ppa_name_ = "0ppa0";
elif dist_name_ == "4":
	dist_name_ = "artful";
	ppa_name_ = "0ppa3";
elif dist_name_ == "5":
	dist_name_ = "bionic";
	ppa_name_ = "0ppa4";
else:
	print("Wrong dist name... ");
	exit(0);
print("Dist Name: " + dist_name_)

cmakelist="../../fhq-server/CMakeLists.txt"
f = open(cmakelist,'r')
filedata = f.read()
f.close()

# add_definitions(-DVERSION_STRING="0.2.4")
m = re.search('DFHQSRV_VERSION=\"(.+)\"', filedata)
if m:
    current_version = m.group(1)

print ("Current version: " + current_version);

'''
new_version = raw_input("New version: ")
newdata = filedata.replace(current_version,new_version)
f = open(cmakelist,'w')
f.write(newdata)
f.close()'''

# parse CHANGELOG.md
changelog_list = []
version_logs = {'version': '', 'dt': '', 'logs': []}
lines = [line.rstrip('\n') for line in open('../../CHANGELOG.md')]
for li in lines:
	m = re.search('[ ]*##[ ]+\[(\d+\.\d+\.\d+)\][ ]*\((.*)\).*', li)
	if m:
		if version_logs['version'] != '':
			changelog_list.append(version_logs)
			version_logs = {'version': '', 'dt': '', 'logs': []}
		ver = m.group(1)
		dt = m.group(2)
		version_logs['version'] = ver;
		version_logs['dt'] = dt
	
	m = re.search('[ ]*-[ ]*(.*)', li)
	if m:
		line_log = m.group(1)
		version_logs['logs'].append(line_log)

if version_logs['version'] != '':
	changelog_list.append(version_logs)
	version_logs = {'version': '', 'dt': '', 'logs': []}


# print(li)
# print(lines)
# m = re.search('DFHQSRV_VERSION=\"(.+)\"', filedata)
#if m:
#  current_version = m.group(1)


# Prepare changelog
changelog="../../fhq-server/debian/changelog"
f = open(changelog,'w')
li_count = 0;
for li in changelog_list:
	if li_count != 0:
		f.write("\n")
		f.write("\n")
	li_count = li_count + 1
	f.write("fhq-server (" + li['version'] + "-" + ppa_name_ + ") " + dist_name_ + "; urgency=low\n\n")
	for li_log in li['logs']:
		li_log = li_log.strip()
		if li_log != '':
			f.write("  * " + li_log + "\n");
	f.write("\n")
	#if li['dt'] == '?':
	#	li['dt'] = subprocess.Popen(['date', '-R'], stdout=subprocess.PIPE).communicate()[0]
	f.write(" -- Free Hack Quest <freehackquest@gmail.com>  " + li['dt']) # 2 space!!!
f.close()

'''
f.write("fhq-server (" + current_version + "-" + ppa_name_ + ") " + dist_name_ + "; urgency=low\n\n")
changelog_message = raw_input("Changelog message (or empty - end): ");
while changelog_message != "":
	f.write("  * " + changelog_message + "\n");
	changelog_message = raw_input("Changelog message (or empty - end): ");
f.write("\n")

dt=subprocess.Popen(['date', '-R'], stdout=subprocess.PIPE).communicate()[0]

f.write(" -- Free Hack Quest <freehackquest@gmail.com>  " + dt)
'''

f.close()

subprocess.call("./clean_sources_ppa.sh");
os.system("cd ../../ && tar -acf fhq-server_" + current_version + "-" + ppa_name_ + ".orig.tar.gz fhq-server");
os.system("cd ../../fhq-server && debuild -S -sa");


sys.stdout.write("Are you want try upload source package to ppa.launchpad? [y/n]: ")
ask_upload_ = raw_input().lower()

if ask_upload_ == "y":
	os.system("cd ../../ && dput ppa:freehackquest/fhq-server fhq-server_" + current_version + "-" + ppa_name_ + "_source.changes");
