import Map as GameMap
import Metrics

box = lambda x: x == 2
level = open("level/test.txt", "r").read()

game_map = GameMap.Map(level)
game_map.mapProduction()
game_map.mapPrinting()

boxArray = game_map.getBoxArray()
print("BOX ARRAY: ", boxArray)

#algo = AssignmentAlgorithms.AssignmentAlgorithms(game_map.width, game_map.height, game_map.targetArray, game_map.getClearedMap())
#algo.nearestNeighbour(boxArray, 0)

metrics = Metrics.Metrics(game_map.width, game_map.height, game_map.getTargetsArray(), game_map.getClearedMap(), Metrics.PULL_METRIC)

print("PLAYER->BOX: ", metrics.distance(7, 16))
print("BOX->TARGET: ", metrics.distance(13, 19))