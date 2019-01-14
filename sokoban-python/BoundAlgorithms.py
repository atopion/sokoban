import Metrics

class BoundAlgorithms:

    def __init__(self, map):
        self.targets = map.getTargetsArray()
        self.metrics = Metrics.Metrics(map.width, map.height, map.getTargetsArray(), map.getClearedMap(), Metrics.PULL_METRIC)

    def greedyBound(self, box_array, pos):

        # IDEA: Use shortest ways first.
        matchedBoxes = []
        matchedTargets = []
        distance_sum = 0

        b = 0
        for i in range(0, len(box_array), 1):
            for j in range(0, len(self.targets), 1):
                if box_array[i] == self.targets[j]:
                    matchedTargets.append(self.targets[j])
                    matchedBoxes.append(box_array[i])
                    b += 1

        pos_b, dist_b = 0, 0
        pos_t, dist_t = pos, 0

        for i in range(0, len(box_array) - b, 1):
            # TODO distance at box minimum needs to use the before position


            pos_b, dist_b = BoundAlgorithms.minimum(box_array, matchedBoxes, lambda x : self.metrics.distance(x, pos_t))
            pos_t, dist_t = BoundAlgorithms.minimum(self.targets, matchedTargets, lambda x : self.metrics.distance(x, pos_b))
            matchedBoxes.append(pos_b)
            matchedTargets.append(pos_t)

            if pos_b == -1 or pos_t == -1:
                print("No more box or target found")
                break
            else:
                distance_sum += dist_b + dist_t

        return distance_sum

# HELPERS
    @staticmethod
    def minimum(array, exclude, func):
        _min = 10000000
        pos = -1
        for i in range(0, len(array), 1):
            if array[i] not in exclude:
                if func(array[i]) < _min:
                    _min = func(array[i])
                    pos = array[i]
        return pos, _min

    #@staticmethod
    #def

