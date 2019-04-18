#!/usr/bin/env python
# -*- coding: utf-8 -*-

import re
import os
import random
import string

updatespath = "../src/storages/updates/"
fileslist = [f for f in os.listdir(updatespath) if os.path.isfile(os.path.join(updatespath, f))]

pattern = r'.*StorageUpdateBase.*\(.*"([a-zA-Z0-9]*)".*,.*"([a-zA-Z0-9]*)".*,.*\).*'

updates = []

for filename in fileslist:
    filepath = os.path.join(updatespath, filename)
    # print(filepath);
    with open(filepath) as f:  
        line = f.readline()
        while line:
            line = line.strip()
            if re.match(pattern, line):
                versions = re.search(pattern, line, re.IGNORECASE)
                if versions:
                    updates.append({
                        "from": versions.group(1),
                        "to": versions.group(2),
                    })
            line = f.readline()

# for tests

'''
updates.append({
    "from": "u0100",
    "to": "615d8fddd",
})

updates.append({
    "from": "615d8fddd",
    "to": "995d8fddd",
})'''

# print all updates
# for v in updates:
#    print("[" + v["from"] + "] -> [" + v["to"] + "]")

# find the ends in graph

end_points = []
max_weight = 0

def recoursive_search_endpoints(spoint, weight):
    global updates, end_points, max_weight
    found = False
    for v in updates:
        if v["from"] == spoint:
            found = True
            recoursive_search_endpoints(v["to"], weight + 1)
    if not found:
        if weight > max_weight:
            max_weight = weight;
        end_points.append({
            "point": spoint,
            "weight": weight
        })

recoursive_search_endpoints("", 0)

print(end_points)

if len(end_points) == 0:
    print("Not found updates")
    exit(-1)

endpoint = ""

for i in end_points:
    if i["weight"] == max_weight and endpoint == "":
        endpoint = i["point"]
    elif i["weight"] == max_weight and endpoint != "":
        print("WARNING: Found points with same weights, will be used first. Ignored: " + i["point"])

print("Found point: " + endpoint + " weight: " + str(max_weight))
newpoint = ''.join(random.choice(string.ascii_lowercase + string.digits) for _ in range(10))

newpoint_upper = newpoint.upper();
endpoint_upper = endpoint.upper();

filename_h = "update_" + endpoint + "_" + newpoint + ".h"
filename_cpp = "update_" + endpoint + "_" + newpoint + ".cpp"

filename_h = os.path.join(updatespath, filename_h)
filename_cpp = os.path.join(updatespath, filename_cpp)

print("Generate header file: " + filename_h)

f_h = open(filename_h, 'w')
f_h.write("#ifndef UPDATE_" + endpoint_upper + "_" + newpoint_upper + "_H\n")
f_h.write("#define UPDATE_" + endpoint_upper + "_" + newpoint_upper + "_H\n")
f_h.write("\n")
f_h.write("#include <storages.h>\n")
f_h.write("\n")
f_h.write("class Update_" + endpoint + "_" + newpoint + " : public StorageUpdateBase {\n")
f_h.write("    public:\n")
f_h.write("        Update_" + endpoint + "_" + newpoint + "();\n")
f_h.write("        virtual bool custom(Storage *pStorage, StorageConnection *pConn, std::string &error);\n")
f_h.write("};\n")
f_h.write("\n")
f_h.write("#endif // UPDATE_" + endpoint_upper + "_" + newpoint_upper + "_H\n")
f_h.close();

print("Generate source file: " + filename_cpp)

f_cpp = open(filename_cpp, 'w')
f_cpp.write("#include \"update_" + endpoint + "_" + newpoint + ".h\"\n")
f_cpp.write("\n")
f_cpp.write("REGISTRY_STORAGE_UPDATE(Update_" + endpoint + "_" + newpoint + ")\n")
f_cpp.write("\n")
f_cpp.write("Update_" + endpoint + "_" + newpoint + "::Update_" + endpoint + "_" + newpoint + "()\n")
f_cpp.write("    : StorageUpdateBase(\"" + endpoint + "\", \"" + newpoint + "\", \"TODO\") {\n")
f_cpp.write("    \n")
f_cpp.write("    // fill the array with struct changes\n")
f_cpp.write("}\n")
f_cpp.write("\n")
f_cpp.write("bool Update_" + endpoint + "_" + newpoint + "::custom(Storage *pStorage, StorageConnection *pConn, std::string &error) {\n")
f_cpp.write("    // here you can migrate data of correction if not just return true;\n")
f_cpp.write("    return true;\n")
f_cpp.write("}\n")
f_cpp.close();
