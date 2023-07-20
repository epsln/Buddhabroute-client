import csv
from os import listdir
from os.path import isfile, join
from PIL import Image
import numpy as np
import math

PATH = "/tmp/checkpoints_idler/"
backupChecks = [f for f in listdir(PATH) if isfile(join(PATH, f))]

outputImg = np.zeros((1920, 2160))

maxVal = 0

for checkpoint in backupChecks:
    with open(join(PATH, checkpoint), newline='') as csvfile:
        checkpointReader = csv.reader(csvfile, delimiter=',')
        linNum = 0
        rowNum = 0
        try:
            for i, row in enumerate(checkpointReader):
                linNum += 1
                rowNum = 0
                for j, count in enumerate(row):
                    rowNum += 1
                if rowNum != outputImg.shape[1]:
                    continue
            print(rowNum, linNum)
        except UnicodeDecodeError:
            continue

        if linNum != outputImg.shape[0] and rowNum != outputImg.shape[1]:
            continue
        csvfile.seek(0)
        for i, row in enumerate(checkpointReader):
            for j, count in enumerate(row[:-1]):
                outputImg[i][j] += int(count)
                maxVal = max(outputImg[i][j], maxVal)

maxVal = math.log(maxVal + 1)
print(maxVal)
if outputImg.any():
    for i in range(outputImg.shape[0]):
        for j in range(outputImg.shape[1]):
            outputImg[i][j] = math.log(outputImg[i][j] + 1)/maxVal * 255

    im = Image.fromarray(outputImg.astype(np.uint8))
    im.convert("L")
    im.save("output.jpg")
