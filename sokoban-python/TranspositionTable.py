import random
import numpy as np


class TranspositionTable:

    def __init__(self, width, height, box_count):
        self.table = {}
        self.zobristTable = np.zeros((int(width*height), int(box_count)+1), dtype=np.longlong)
        self.box_count = box_count
        for i in range(0, int(width*height), 1):
            for j in range(0, box_count+1, 1):
                self.zobristTable[i][j] = random.randint(0, 4294967296)

    def empty(self):
        return self.table == {}

    def _insert(self, key):
        self.table[key] = True

    def _remove(self, key):
        del self.table[key]

    def _lookup(self, key):
        return key in self.table

    def _computeHash(self, box_array, player_position):
        h = 0
        for i in range(0, len(box_array), 1):
            h ^= self.zobristTable[box_array[i]][i]
        h ^= self.zobristTable[player_position][self.box_count]
        return h

    def insert(self, box_array, player_position):
        h = self._computeHash(box_array, player_position)
        self.table[h] = True

    def lookup(self, box_array, player_position):
        h = self._computeHash(box_array, player_position)
        return self._lookup(h)

    def remove(self, box_array, player_position):
        h = self._computeHash(box_array, player_position)
        self._remove(h)
