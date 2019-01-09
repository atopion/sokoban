import numpy as np


class Map:

    clear     = lambda self, x : 5 if x == 1 or x == 2 else x

    def __init__(self, mapString):
        self.mapString = mapString
        self.map = []
        self.width = 0
        self.height = 0
        self.mapProduction()
        self.clearedMap = list(map(self.clear, self.map))
        self.player = [i for i, x in enumerate(self.map) if x == 1][0]
        self.boxArray = [i for i, x in enumerate(self.map) if x == 2]
        self.targetArray = [i for i, x in enumerate(self.map) if x == 3]

        self.reachableArray = np.zeros(self.width * self.height, dtype=int)
        self.visited = np.zeros(self.width * self.height, dtype=int)

    def mapProduction(self):
        self.map = []
        tmp = 0
        width = 0
        for c in self.mapString:
            tmp += 1
            if c == "@":  # Player
                self.map.append(1)
            if c == "$":  # Box
                self.map.append(2)
            if c == ".":  # Target
                self.map.append(3)
            if c == "#":  # Wall
                self.map.append(4)
            if c == " ":  # Empty
                self.map.append(5)

            if c == "\n":
                if tmp > width +1:
                    width = tmp -1
                tmp = 0

        self.width  = width
        self.height = int(len(self.map) / width)

    def mapPrinting(self):
        tmp = ""
        count = 0
        pre = 0
        for c in self.map:
            if c == 1:
                tmp += "@"
            elif c == 2:
                tmp += "$"
            elif c == 3:
                tmp += "."
            elif c == 4:
                tmp += "#"
            elif c == 5:
                tmp += " "
            count += 1
            if count == self.width:
                tmp += "   "
                for i in range(0, self.width, 1):
                    tmp += str(pre + i).zfill(2) + " "
                print(tmp)
                tmp = ""
                count = 0
                pre += self.width

#   Normalize the players position by finding the top left cell the player can reach without moving a box.
#   This decreases the memory usage of the solution tree a lot, but makes a new algorithm necessary to connect
#   the solution paths.
#
#   one_dim: boolean whether x contains the player position in one dimension or x,y contain it in two dimensions
#            if one_dim is true, the y argument will be ignored

    def normalizedPlayerPosition(self, x, y, one_dim):

        if one_dim:
            y = x // self.width
            x = x % self.width
        if x > self.width or y > self.height:
            return None

        tmp_map = np.empty(len(self.map), dtype=int)
        changes = []
        label = 1
        for i in range(0, len(self.map), 1):
            if self.isUnpassable(i):
                tmp_map[i] = 0
            elif (i-1 < 0 or self.isUnpassable(i-1)) and (i-self.width < 0 or self.isUnpassable(i-self.width)):
                tmp_map[i] = label
                label += 1
            elif i-self.width < 0 or self.isUnpassable(i-self.width):
                tmp_map[i] = tmp_map[i-1]
            elif i-1 < 0 or self.isUnpassable(i-1):
                tmp_map[i] = tmp_map[i-self.width]
            else:
                if tmp_map[i-1] < tmp_map[i-self.width]:
                    tmp_map[i] = tmp_map[i-1]
                    changes.append([tmp_map[i-1], tmp_map[i-self.width]])
                else:
                    tmp_map[i] = tmp_map[i-self.width]
                    changes.append([tmp_map[i-self.width], tmp_map[i-1]])

        # Normalize Changes
        for i in range(0, len(changes), 1):
            for j in range(0, len(changes), 1):
                if changes[i][1] == changes[j][0]:
                    changes[j][0] = changes[i][0]

        for i in range(0, len(tmp_map), 1):
            for c in changes:
                if(tmp_map[i] == c[1]):
                    tmp_map[i] = c[0]

        # print("TMP MAP: ")
        # self.print_array(tmp_map)
        # print("")
        playerLabel = tmp_map[y * self.width + x]
        # print("PlayerLabel: ", playerLabel)

        self.reachableArray = np.zeros(self.width * self.height, dtype=int)
        for i in range(0, len(tmp_map), 1):
            if tmp_map[int((i % self.height) * self.width + i // self.height)] == playerLabel:
                self.reachableArray[i] = 1

        # Calculate left-top corner with label: playerLabel
        # IMPORTANT: left goes before top
        for i in range(0, len(tmp_map), 1):
            if tmp_map[int((i % self.height) * self.width + i // self.height)] == playerLabel:
                if one_dim:
                    return int((i % self.height) * self.width + i // self.height)
                else:
                    return [int((i % self.height) * self.width + i // self.height) % self.width,
                            int((i % self.height) * self.width + i // self.height) // self.width]

    def normalizedPlayerPositionMove(self, move):
        x = move[0]
        t = self.map[move[0]]
        self.map[move[0]] = self.map[move[1]]
        self.map[move[1]] = t

        y = x // self.width
        x = x % self.width
        if x > self.width or y > self.height:
            return None

        tmp_map = np.empty(len(self.map), dtype=int)
        changes = []
        label = 1
        for i in range(0, len(self.map), 1):
            if self.isUnpassableMap(i, self.map):
                tmp_map[i] = 0
            elif (i-1 < 0 or self.isUnpassable(i-1)) and (i-self.width < 0 or self.isUnpassable(i-self.width)):
                tmp_map[i] = label
                label += 1
            elif i-self.width < 0 or self.isUnpassable(i-self.width):
                tmp_map[i] = tmp_map[i-1]
            elif i-1 < 0 or self.isUnpassable(i-1):
                tmp_map[i] = tmp_map[i-self.width]
            else:
                if tmp_map[i-1] < tmp_map[i-self.width]:
                    tmp_map[i] = tmp_map[i-1]
                    changes.append([tmp_map[i-1], tmp_map[i-self.width]])
                else:
                    tmp_map[i] = tmp_map[i-self.width]
                    changes.append([tmp_map[i-self.width], tmp_map[i-1]])

        self.map[move[1]] = self.map[move[0]]
        self.map[move[0]] = t

        # Normalize Changes
        for i in range(0, len(changes), 1):
            for j in range(0, len(changes), 1):
                if changes[i][1] == changes[j][0]:
                    changes[j][0] = changes[i][0]

        for i in range(0, len(tmp_map), 1):
            for c in changes:
                if(tmp_map[i] == c[1]):
                    tmp_map[i] = c[0]

        # print("Position: ", (y * self.width + x))
        # print("TMP MAP: ")
        # self.print_array(tmp_map)
        playerLabel = tmp_map[y * self.width + x]
        # print("PlayerLabel: ", playerLabel)

        self.reachableArray = np.zeros(self.width * self.height, dtype=int)
        for i in range(0, len(tmp_map), 1):
            if tmp_map[i] == playerLabel:
                self.reachableArray[i] = 1

        # Calculate left-top corner with label: playerLabel
        # IMPORTANT: left goes before top
        for i in range(0, len(tmp_map), 1):
            if tmp_map[int((i % self.height) * self.width + i // self.height)] == playerLabel:
                return int((i % self.height) * self.width + i // self.height)

    def getBoxArray(self):
        return np.array(self.boxArray, dtype=int)

    def getBoxCount(self):
        return len(self.boxArray)

    def getPlayerPosition(self):
        return self.player

    def getClearedMap(self):
        return self.clearedMap

    def getTargetsArray(self):
        return self.targetArray

    def getGameMap(self):
        return self.map

    def is_reachable(self, index):
        return self.reachableArray[index] == 1

    def print_reachableArray(self):
        tmp = ""
        count = 0
        for c in self.reachableArray:
            if c == 1: tmp += "1"
            else: tmp += "0"

            count += 1
            if count == self.width:
                print(tmp)
                tmp = ""
                count = 0

    def print_array(self, m):
        tmp = ""
        count = 0
        for c in m:
            tmp += str(c)
            count += 1
            if count == self.width:
                print(tmp)
                tmp = ""
                count = 0

    def print_map_tmp(self, move, box_array):
        tmp = ""
        count = 0
        for i in range(0, len(self.map), 1):
            if i == move[0]:
                tmp += "@"
            elif i == move[1]:
                tmp += "$"
            elif i in box_array:
                tmp += "$"
            elif self.clearedMap[i] == 1:
                tmp += " "
            elif self.clearedMap[i] == 2:
                tmp += "$"
            elif self.clearedMap[i] == 3:
                tmp += "."
            elif self.clearedMap[i] == 4:
                tmp += "#"
            elif self.clearedMap[i] == 5:
                tmp += " "
            count += 1
            if count == self.width:
                print(tmp)
                tmp = ""
                count = 0

    def calculate_reachable_area(self, box_array, player_pos):
        visited = np.zeros(self.width * self.height, dtype=int)
        self._calc_reachable_area(visited, box_array, player_pos)
        return visited

    def _calc_reachable_area(self, visited, box_array, pos):
        if pos < 0 or pos > len(self.map) or visited[pos] == 1 or self.map[pos] == 4 or pos in box_array:
            return
        visited[pos] = 1
        self._calc_reachable_area(visited, box_array, pos + 1)
        self._calc_reachable_area(visited, box_array, pos - 1)
        self._calc_reachable_area(visited, box_array, pos + self.width)
        self._calc_reachable_area(visited, box_array, pos - self.width)

    def can_reach(self, box_array, player_pos, index):
        vi = self.calculate_reachable_area(box_array, player_pos)
        return vi[index] == 1

    def new_normalized_player_position(self, box_array, position):
        vi = self.calculate_reachable_area(box_array, position)
        for i in range(0, len(vi), 1):
            if vi[i] == 1:
                return i

#   Helpers
    def isUnpassable(self, x):
        return self.map[x] == 4 #or self.map[x] == 2

    def isUnpassableMap(self, x, map):
        return map[x] == 4 or map[x] == 2

# mapObj = Map("######\n#   .#\n# $  #\n#@   #\n######\n")
# print("Width:  ", mapObj.width)
# print("Height: ", mapObj.height)
# mapObj.mapPrinting()

# pr = cProfile.Profile()
# pr.run('mapObj.normalizedPlayerPosition(4, 3, False)')
# pr.print_stats()
