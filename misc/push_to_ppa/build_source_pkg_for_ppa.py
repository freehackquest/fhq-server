#!/usr/bin/env python
# -*- coding: utf-8 -*-

import fileinput
import re
import subprocess
import os
import sys

print ("Welcome to preapre ppa package...")

dists = []

'''
# Not supported qt5:
dists.append({
	"dist_name": "trusty",
	"ppa_name_suffix": "ppa-ubuntu-14-04-trusty",
	"end": "April 2019",
	"version": "14.04 LTS"
})
'''

dists.append({
	"dist_name": "xenial",
	"ppa_name_suffix": "ppa-ubuntu-16-04-xenial",
	"end": "April 2021",
	"version": "16.04 LTS"
})

dists.append({
	"dist_name": "bionic",
	"ppa_name_suffix": "ppa-ubuntu-18-04-bionic",
	"end": "April 2023",
	"version": "18.04 LTS"
})

dists.append({
	"dist_name": "cosmic",
	"ppa_name_suffix": "ppa-ubuntu-18-10-cosmic",
	"end": "July 2019",
	"version": "18.10"
})

print("Please choose dist name:")

i = 0
for d in dists:
	print('    ' + str(i) + '. ' + d['dist_name'] + ' (' + d['version'] + '), date end: ' + d['end'])
	i = i + 1

dist_num_ = raw_input("Enter number of dist: ")
dist_num_ = int(dist_num_)
if dist_num_ >= len(dists):
	print("Wrong dist number")
	exit(1)

dist_name_ = dists[dist_num_]['dist_name']
ppa_name_ = dists[dist_num_]['ppa_name_suffix']

print("Dist Name: " + dist_name_)

cmakelist="../../fhq-server/CMakeLists.txt"
f = open(cmakelist,'r')
filedata = f.read()
f.close()

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
