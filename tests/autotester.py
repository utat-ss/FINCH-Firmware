import fileinput
import os

project = os.path.join("Projects", "STM32H743ZITX", ".cproject")
filesToTest = []

# Inspired by https://www.tutorialspoint.com/python/os_walk.htm
# gets all the files under the Manual_Tests/common folder
for root, dirs, files in os.walk(os.path.join("Manual_Tests", "common"), topdown=False):
    for name in files:
        # print(os.path.join(root, name)) #optionally print out each file that is added
        filesToTest.append(os.path.join(root, name))


for fileToTest in filesToTest:
    serachStr = "<entry flags=\"VALUE_WORKSPACE_PATH\" kind=\"sourcePath\" name=\"Src\"/>"
    addition = "<entry flags=\"VALUE_WORKSPACE_PATH\" kind=\"sourcePath\" name=\"" + fileToTest + "\"/>"

    # specific modifcation in the file using this method https://stackoverflow.com/a/33239613
    for line in fileinput.FileInput(project,inplace=1):
        if serachStr in line:
            line=line.replace(line,serachStr + addition+'\n')
        print(line, end='')

    # at this point, .cproject is set up to compile the specified file


