import os
import xml.etree.ElementTree as ET

project = os.path.join("Projects", "STM32H743ZITX", ".cproject")
filesToTest = []

# Inspired by https://www.tutorialspoint.com/python/os_walk.htm
# gets all the files under the Manual_Tests/common folder
for root, dirs, files in os.walk(os.path.join("Manual_Tests", "common"), topdown=False):
    for name in files:
        # print(os.path.join(root, name)) #optionally print out each file that is added
        filesToTest.append(os.path.join(root, name))



serachStr = "<entry flags=\"VALUE_WORKSPACE_PATH\" kind=\"sourcePath\" name=\"Src\"/>"

import fileinput
for line in fileinput.FileInput(project,inplace=1):
    if serachStr in line:
        line=line.replace(line,line)
    print (line)


# cprojectXML = ET.parse(project)
# myroot = cprojectXML.getroot()

# for x in myroot.iter('sourceEntries'):
#     print(x)

# print(next(myroot.iter('sourceEntries')))
# print(myroot[0])
# ET.SubElement(next(myroot.iter('sourceEntries')), 'testtag')





# #ET.dump(myroot)
# #tree = ET.ElementTree(myroot)
# cprojectXML.write(project)
# #tree.write(open('test.xml','w'), encoding='unicode')