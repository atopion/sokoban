from __future__ import absolute_import, division, print_function

# TensorFlow and tf.keras
import tensorflow as tf
from tensorflow import keras

# Helper libraries
import numpy as np
import random
import sys


class BuildModel:

    def __init__(self, trainPath, validationPath, layers, epochs, train_repeats, test_repeats, modelDir):
        self.runPath = trainPath
        self.validPath = validationPath
        self.layers = layers
        self.epochs = epochs
        self.train_repeats = train_repeats
        self.test_repeats = test_repeats
        self.modelDir = modelDir
        self.model = None

        tf.logging.set_verbosity(tf.logging.ERROR)

    def loadDataSet(self, randomTestSamples, testSampleSize):
        file = open(self.runPath, "r")
        levels = file.readlines()

        _train_data = []
        _train_labels = []

        _test_data = []
        _test_labels = []

        if randomTestSamples:
            random.shuffle(levels)

        i = 0
        for level in levels:
            if level.startswith("--"):
                continue
            i += 1
            result = level[3]
            level = level[5:-2]
            lines = level.split('|')

            tmp = []
            for l in lines:
                tmp.append([int(x) for x in l])

            a = 0
            if result == "u":
                a = 0
            elif result == "d":
                a = 1
            elif result == "l":
                a = 2
            elif result == "r":
                a = 3
            else:
                print("ERROR ON LEVEL ", i, ". Result: ", result)

            if i <= testSampleSize:
                _test_data.append(tmp)
                _test_labels.append(a)
            else:
                _train_data.append(tmp)
                _train_labels.append(a)

        train_data = np.array(_train_data)
        train_labels = np.array(_train_labels)
        test_data = np.array(_test_data)
        test_labels = np.array(_test_labels)

        return (train_data, train_labels), (test_data, test_labels)

    def loadTestData(self, randomize, size):
        file = open(self.validPath, "r")
        levels = file.readlines()

        if randomize:
            random.shuffle(levels)

        _test_data = []
        _test_labels = []

        i = 0
        for level in levels:
            if level.startswith("--"):
                continue

            i += 1
            result = level[3]
            level = level[5:-2]
            lines = level.split('|')

            tmp = []
            for l in lines:
                tmp.append([int(x) for x in l])

            a = 0
            if result == "u":
                a = 0
            elif result == "d":
                a = 1
            elif result == "l":
                a = 2
            elif result == "r":
                a = 3
            else:
                print("ERROR ON LEVEL ", i, ". Result: ", result)

            _test_data.append(tmp)
            _test_labels.append(a)

            if i >= size:
                break

        test_data = np.array(_test_data)
        test_labels = np.array(_test_labels)

        return (test_data, test_labels)

    def createModel(self):

        _optimizer = 'Adamax'
        _loss = 'sparse_categorical_crossentropy'
        _metrics = ['acc']
        _activation = tf.nn.sigmoid

        _hiddenLayers = []
        for x in self.layers:
            _hiddenLayers.append(keras.layers.Dense(x, activation=_activation)) #, kernel_regularizer=keras.regularizers.l2(0.001)))
            _hiddenLayers.append(keras.layers.Dropout(0.2))

        model = keras.Sequential()

        model.add(keras.layers.Flatten(input_shape=(10, 10)))
        for layer in _hiddenLayers:
            model.add(layer)
        model.add(keras.layers.Dense(4, activation=tf.keras.activations.softmax))

        model.compile(optimizer=_optimizer, loss=_loss, metrics=_metrics)

        return model

    def train(self):

        cp_callback = tf.keras.callbacks.ModelCheckpoint(self.modelDir, save_weights_only=True, verbose=1, period=5)
        model = self.createModel()
        model.save_weights(self.modelDir.format(epoch=0))

        model.summary()

        for i in range(0, self.train_repeats, 1):
            print("Iteration: ", i +1)
            print("Training: ", i+1, file=sys.stderr)
            (train_data, train_labels), (test_data, test_labels) = self.loadDataSet(True, 4000)
            model.fit(train_data, train_labels, epochs=self.epochs, callbacks=[cp_callback], validation_data=(test_data, test_labels), verbose=2)
            self._test(model, i)
            print("\n")

        model.save(self.modelDir + "model.h5")
        self.model = model

    def _test(self, model, index):

        if model is None:
            return

        total_acc  = 0.0
        total_loss = 0.0

        print("\tTesting: [", index, "]:   ", file=sys.stderr, end='')
        for i in range(0, self.test_repeats, 1):
            print("[", index, "] Iteration: ", i+1)
            (test_data, test_labels) = self.loadTestData(randomize=True, size=500)
            test_loss, test_acc = model.evaluate(test_data, test_labels, verbose=2)
            print("\tAccuracy: ", test_acc, "  Loss: ", test_loss)
            total_acc += test_acc
            total_loss += test_loss

        print("\nTotal Accuracy: ", (total_acc / self.test_repeats), "   Total Loss: ", (total_loss / self.test_repeats))
        print("Accuracy: ", (total_acc / self.test_repeats), "   Loss: ", (total_loss / self.test_repeats), file=sys.stderr)

    def test(self):

        if self.model is None:
            return

        total_acc  = 0.0
        total_loss = 0.0

        for i in range(0, self.test_repeats, 1):
            print("Iteration: ", i+1)
            print("Testing: ", i+1, file=sys.stderr)
            (test_data, test_labels) = self.loadTestData(randomize=True, size=4000)
            test_loss, test_acc = self.model.evaluate(test_data, test_labels, verbose=2)
            print("\tAccuracy: ", test_acc, "  Loss: ", test_loss, "\n")
            total_acc += test_acc
            total_loss += test_loss

        print("\n\nTotal Accuracy: ", (total_acc / self.test_repeats), "   Total Loss: ", (total_loss / self.test_repeats))

    def convert(self):

        if self.model is None:
            return

        converter = tf.lite.TFLiteConverter.from_keras_model_file(self.modelDir + "model.h5")
        lite_model = converter.convert()
        open(self.modelDir + "model.tflite", "wb").write(lite_model)
