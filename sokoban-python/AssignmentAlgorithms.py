import Metrics
import numpy as np
import scipy.optimize as sp
import queue


class AssignmentAlgorithms:

    def __init__(self, width, height, targets, cleared_board):
        self.width = width
        self.targets = targets
        self.metrics = Metrics.Metrics(width, height, targets, cleared_board, Metrics.MANHATTAN_METRIC)

    # BEGIN: Box Assignments

    def closestAssignment(self, box_array):
        res = 0
        for b in box_array:
            min_dist = 1000000
            for t in self.targets:
                d = self.metrics.distance(t, b)
                if d < min_dist:
                    min_dist = d
            res += min_dist
        return res

    # The hungarian linear sum assignment solution: O(n^3)
    def hungarianAssignment(self, box_array):
        costs = np.zeros((len(self.targets), len(box_array)), dtype=int)
        for i in range(0, len(self.targets), 1):
            for j in range(0, len(box_array), 1):
                costs[i][j] = self.metrics.distance(self.targets[i], box_array[j])
        (row_index, col_index) = sp.linear_sum_assignment(costs)
        return costs[row_index, col_index].sum()

    def greedyAssignment(self, box_array):
        priority_queue = queue.PriorityQueue()
        matchedBoxes = []
        matchedTargets = []
        distance_sum = 0

        for i in range(0, len(self.targets), 1):
            for j in range(0, len(box_array), 1):
                priority_queue.put((self.metrics.distance(self.targets[i], box_array[j]), self.targets[i], box_array[j]))

        while not priority_queue.empty():
            (dist, target, box) = priority_queue.get()
            if target not in matchedTargets and box not in matchedBoxes:
                distance_sum += dist
                matchedBoxes.append(box)
                matchedTargets.append(target)

        for b in box_array:
            if b not in matchedBoxes:
                min_dist = 1000000
                for t in self.targets:
                    d = self.metrics.distance(t, b)
                    if d < min_dist:
                        min_dist = d
                distance_sum += min_dist
        return distance_sum

    # END: Box Assignments

    def nearestNeighbour(self, box_array, player_pos):
        costs = np.zeros((len(self.targets), len(box_array)), dtype=int)
        for i in range(0, len(self.targets), 1):
            for j in range(0, len(box_array), 1):
                costs[i][j] = self.metrics.distance(self.targets[i], box_array[j])
        (row_index, col_index) = sp.linear_sum_assignment(costs)

        steps = []
        for i in range(0, len(row_index), 1):
            print("Assigning row ", self.targets[row_index[i]], "to col ", box_array[col_index[i]])
            steps.append([box_array[col_index[i]], self.targets[row_index[i]]])

        p = player_pos
        for i in range(0, len(steps), 1):
            min = 1000000
            for j in range(i, len(steps), 1):
                d = self.metrics.distance(p, steps[j][0])
                if d < min:
                    min = d

        n = 0
        if n not in range(0, len(self.targets), 1):
            print("Not inside")
