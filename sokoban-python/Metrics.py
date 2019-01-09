import math
import numpy as np
import queue

# Metric codes
PYTHAGOREAN_METRIC = 1
MANHATTAN_METRIC = 2
PULL_METRIC = 3


class Metrics:

    def __init__(self, width, height, targets, clearedBoard, metric_code):
        self.width = width
        self.height = height
        self.size = width * height
        self.metric_code = metric_code
        self.distances = np.full((self.size, self.size), 32000, dtype=int)

        mqueue = queue.Queue()
        for i in range(0, self.size, 1):
            self.distances[i][i] = 0
            mqueue.put(i)
            while not mqueue.empty():
                position = mqueue.get()
                for d in [-1, 1, -self.width, self.width]:
                    player_pos = position + d
                    if not 0 <= player_pos < self.size:
                        continue
                    if self.distances[i][player_pos] == 32000 and not clearedBoard[player_pos] == 4:
                        self.distances[i][player_pos] = self.distances[i][position] + 1
                        mqueue.put(player_pos)


    @staticmethod
    def manhattan_distance(a, b, width):
        a_x = int(a % width)
        a_y = int(a / width)
        b_x = int(b % width)
        b_y = int(b / width)
        return Metrics.manhattan_distance_2d(a_x, a_y, b_x, b_y)

    @staticmethod
    def manhattan_distance_2d(a_x, a_y, b_x, b_y):
        return abs(a_x - b_x) + abs(a_y - b_y)

    def pythagorean_distance(self, a, b):
        a_x = int(a % self.width)
        a_y = int(a / self.width)
        b_x = int(b % self.width)
        b_y = int(b / self.width)
        return Metrics.pythagorean_distance_2d(a_x, a_y, b_x, b_y)

    @staticmethod
    def pythagorean_distance_2d(a_x, a_y, b_x, b_y):
        return math.sqrt((a_x - b_x)**2 + (a_y - b_y)**2)

    def pull_distance(self, to, pos):
        if not 0 <= to < self.size or not 0 <= pos < self.size:
            return -1
        else:
            return self.distances[to][pos]

    def pull_distance_2d(self, target_x, target_y, pos_x, pos_y):
        target = self.width * target_y + target_x
        pos = self.width * pos_x + pos_y
        return self.pull_distance(target, pos)

    def distance(self, target, pos):
        if self.metric_code == PYTHAGOREAN_METRIC:
            return self.pythagorean_distance(target, pos)
        if self.metric_code == MANHATTAN_METRIC:
            return self.manhattan_distance(target, pos, self.width)
        if self.metric_code == PULL_METRIC:
            return self.pull_distance(target, pos)

    def distance_2d(self, target_x, target_y, pos_x, pos_y):
        if self.metric_code == PYTHAGOREAN_METRIC:
            return self.pythagorean_distance_2d(target_x, target_y, pos_x, pos_y)
        if self.metric_code == MANHATTAN_METRIC:
            return self.manhattan_distance_2d(target_x, target_y, pos_x, pos_y)
        if self.metric_code == PULL_METRIC:
            return self.pull_distance_2d(target_x, target_y, pos_x, pos_y)
