import Pathfind_Algorithms2
from StateTree import Node

class PlayerPath:

    def __init__(self, path, map):
        self.path = path
        self.map = map
        self.alg = Pathfind_Algorithms2.Pathfind_Algorithms(map)

    def calculatePlayerPath(self, path):
        result = []
        dirs = []
        for i in range(0, len(path), 1):
            dirs.append(path[i].move[1] - path[i].move[0])

        for i in range(0, len(path), 1):
            result.append(path[i].copy([path[i].move[0] - dirs[i], path[i].move[0]]))

        return result

    def calculateEdgesPath(self, path):
        dirs = []
        for i in range(0, len(path), 1):
            dirs.append(path[i].move[1] - path[i].move[0])

        result = [path[0]]
        for i in range(1, len(path), 1):
            if not path[i].move[0] == path[i-1].move[1]:
                p = self.alg.A_sternchen(path[i-1].move[1], path[i].move[0], path[i-1].box_array)
                p = [[p[i-1], p[i]] for i in range(1, len(p), 1)]
                result.extend(p)
            result.append(path[i])

        return result

    def calculateInitialPath(self, path):
        result = []
        tmp_path = self.alg.A_sternchen(self.map.getPlayerPosition(), path[0].move[0], path[0].box_array)
        for i in range(1, len(tmp_path), 1):
            result.append([tmp_path[i-1].point, tmp_path[i].point])
        result.extend(path)
        return result



    def producePlayerPath(self):
        # Produce direction list
        dirs = []
        for i in range(0, len(self.path), 1):
            dirs.append(self.path[i][1] - self.path[i][0])

        tmp = []
        tmp.append(self.path[0])
        # First: For every edge in result, insert around edge steps
        for i in range(1, len(self.path), 1):
            if not (self.path[i - 1][1] - self.path[i - 1][0]) == (self.path[i][1] - self.path[i][0])\
                    and self.path[i-1][1] == self.path[i][0]:
                # k = self.path[i-1][0]
                # l = self.path[i-1][1]
                # m = self.path[i][1]
                # tmp.pop(-1)                           # Remove edge step
                # tmp.append([k, k-(m-l)])              # Step 1
                # tmp.append([k-(m-l), k-(m-l)-(k-l)])  # Step 2
                # tmp.append([k-(m-l)-(k-l), l])        # Step 3
                print("I-1: ", self.path[i-1])
                print("I:   ", self.path[i])
                print("I-1: ", self.path[i - 1][0])
                print("I:   ", self.path[i][0] - dirs[i])
                p = self.alg.A_sternchen(self.path[i-1][0], self.path[i][0] - dirs[i])
                print("P:   ", p)
            #tmp.append(self.path[i])

        self.path = tmp

        # Produce direction list
        dirs = []
        for i in range(0, len(self.path), 1):
            dirs.append(self.path[i][1] - self.path[i][0])

        result = []
        # Next: initial path from player position to first box move
        tmp_path = self.alg.A_sternchen(self.map.getPlayerPosition(), self.path[0][0] - dirs[0])
        for i in range(1, len(tmp_path), 1):
            result.append([tmp_path[i-1].point, tmp_path[i].point])
        result.append([tmp_path[-1].point, self.path[0][0]])

        print("PATH: ", self.path)

        # Next: For every jump in self.path, insert in-between steps
        result.append(self.path[0])
        for i in range(1, len(self.path), 1):
            if not self.path[i-1][1] == self.path[i][0]:
                tmp_path = self.alg.A_sternchen(self.path[i-1][0], self.path[i][0] - dirs[i])
                for a in range(1, len(tmp_path), 1):
                    result.append([tmp_path[a - 1].point, tmp_path[a].point])
                result.append([tmp_path[-1].point, self.path[i][0]])

            result.append(self.path[i])

        tmp = []
        for i in range(0, len(result), 1):
            if not result[i][1] in self.map.getTargetsArray():
                tmp.append(result[i])
        result = tmp

        print(result)

        return result
