import BuildModel
import sys
import os

r = open("runnr", 'r').read()
run = int(r)+1
open("runnr", 'w').write(str(run))

training = "training_data/run_centered_train.txt"
validation = "validation_data/run_centered_test.txt"
modelDir = "model/run" + str(run) + "/"
outputPath = "results/run" + str(run) + "/"
convert = True

if not os.path.exists(modelDir):

    os.makedirs(modelDir)
if not os.path.exists(outputPath):
    os.makedirs(outputPath)

layers = [180, 180, 180, 100, 30]

epochs = 100
train_repeats = 50

test_repeats = 10

sys.stdout = open(outputPath + "log.txt", 'w')

bm = BuildModel.BuildModel(training, validation, layers, epochs, train_repeats, test_repeats, modelDir)
bm.train()
bm.test()

if convert:
    bm.convert()
