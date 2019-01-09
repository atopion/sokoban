import Map as GameMap
import AssignmentAlgorithms

box = lambda x: x == 2
level = open("level/level1.txt", "r").read()

game_map = GameMap.Map(level)
map = game_map.getGameMap()
print("GAME MAP: ", map)

boxArray = game_map.getBoxArray()
print("BOX ARRAY: ", boxArray)

print("NEAREST NEIGHBOUR")

algo = AssignmentAlgorithms.AssignmentAlgorithms(game_map.width, game_map.height, game_map.targetArray, game_map.getClearedMap())
algo.nearestNeighbour(boxArray, 0)