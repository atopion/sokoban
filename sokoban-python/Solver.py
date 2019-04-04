#   Solver class, to be started

import Map as GameMap
from StateTree import Node
from Execution import Execution
import TranspositionTable
import AssignmentAlgorithms
import Pathfind_Algorithms2
import PlayerPath
import os
import time

level_name = "level3.txt"
cwd = os.getcwd()
path = os.path.join(cwd, "level", level_name)
level = open(path, "r").read()

game_map = GameMap.Map(level)
game_map.mapProduction()
game_map.mapPrinting()

start = time.time()
#assignment = AssignmentAlgorithms.AssignmentAlgorithms(game_map.width, game_map.height, game_map.getTargetsArray(), game_map.getClearedMap())
# states = StateTree.StateTree(game_map.getBoxArray(), game_map.getPlayerPosition(), assignment)

table = TranspositionTable.TranspositionTable(game_map.width, game_map.height, game_map.getBoxCount())

table.insert(game_map.getBoxArray(), game_map.getPlayerPosition())

root = Node(game_map.getBoxArray(), game_map.getPlayerPosition(), None, None, 100000, 0) #assignment.greedyAssignment(game_map.getBoxArray()), 0)
execution = Execution(table, game_map)
end = time.time()
print("Preparation time: ", end - start)

start = time.time()
solution = execution.execute(root)
end = time.time()
print("Execution time: ", end - start)
result = []
ra = []
res = ""
pathalg = Pathfind_Algorithms2.Pathfind_Algorithms(game_map)
node = solution
if node is None:
    print("No Solution found")
else:
    print("")
    print("Solution found: ")
    while node.farther is not None:
        result.insert(0, node.move)
        #if node.move is not None and node.farther.move is not None and not node.move[0] == node.farther.move[1] and not (node.move[1] - node.move[0]) == (node.farther.move[1] - node.farther.move[0]):
        #    print("POS1: ", node.farther.move[1])
        #    print("POS2: ", node.move[0])
        #    path1 = pathalg.A_sternchen(node.farther.move[1], node.move[0] - (node.move[1] - node.move[0]), node.farther.box_array)
        #    print("PATH: ", path1)
        #    result.insert(0, [path1[-1].point, node.move[0]])
        #    for i in range(len(path1)-1, 0, -1):
        #        result.insert(0, [path1[i-1].point, path1[i].point])
        #if node.farther is not None and node.move is not None and node.farther.move is not None:
        #    if not node.move[0] == node.farther.move[1]:
        #        path1 = pathalg.A_sternchen(node.farther.move[1], node.move[0])
        #        r = []
        #        for i in range(len(path1)-1, 0, -1):
        #            result.insert(0, [-1 if path1[i].parent == None else path1[i].parent.point, path1[i].point])

        n = node.move[1] - node.move[0]
        if n == 1: res = "right," + res
        elif n == -1: res = "left," + res
        elif n < 0: res = "up," + res
        elif n > 0: res = "down," + res
        ra.insert(0, node)
        node = node.farther

    print(result)
    # print("")
    # print("Player Path:")
    # ppath = PlayerPath.PlayerPath(result, game_map)
    # result = ppath.calculatePlayerPath(ra)
    # result = ppath.calculateEdgesPath(result)
    # result = ppath.calculateInitialPath(result)
    # print(result)
    # print("")
    # print("PATH:")
    # print(res)
    # print("")
    os.system("java -classpath \"C:\\Users\\atopi\\Codes\\SokobanRobot\\SokobanRobot\\out\\production\\SokobanRobot\" " +
          "distanceboard.Viewer \"" + path + "\" \"" + str(result) + "\"")
