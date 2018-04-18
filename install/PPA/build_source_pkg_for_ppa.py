#!/usr/bin/env python
# -*- coding: utf-8 -*-

import fileinput
import re
import subprocess
import os

print ("Welcome to preapre ppa package...")

cmakelist="../../fhq-server/CMakeLists.txt"
f = open(cmakelist,'r')
filedata = f.read()
f.close()

# add_definitions(-DVERSION_STRING="0.2.4")
m = re.search('DVERSION_STRING=\"(.+)\"', filedata)
if m:
  current_version = m.group(1)
    
print ("Current version: " + current_version);
new_version = raw_input("New version: ")

newdata = filedata.replace(current_version,new_version)
f = open(cmakelist,'w')
f.write(newdata)
f.close()


# Prepare changelog
changelog="../../fhq-server/debian/changelog"
f = open(changelog,'w')
f.write("fhq-server (" + new_version + "-0ppa0) xenial; urgency=low\n\n")
changelog_message = raw_input("Changelog message (or empty - end): ");
while changelog_message != "":
	f.write("  * " + changelog_message + "\n");
	changelog_message = raw_input("Changelog message (or empty - end): ");
f.write("\n")

dt=subprocess.Popen(['date', '-R'], stdout=subprocess.PIPE).communicate()[0]

f.write(" -- Free Hack Quest <freehackquest@gmail.com>  " + dt)
f.close()


subprocess.call("./clean_sources_ppa.sh");
os.system("cd ../../ && tar -acf fhq-server_" + new_version+ ".orig.tar.gz fhq-server");
os.system("cd ../../fhq-server && debuild -S -sa");
