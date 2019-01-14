from AssignmentAlgorithms import AssignmentAlgorithms
from BoundAlgorithms import BoundAlgorithms
import TranspositionTable
from DeadlockDetection import DeadlockDetection
from StateTree import Node
import Pathfind_Algorithms2
import Map
import numpy as np


class Execution:

    def __init__(self, transposition_table: TranspositionTable, game_map: Map):
        self.transposition_table = transposition_table
        self.game_map = game_map
        self.width = game_map.width
        self.height = game_map.height
        #self.assignment_algorithms = AssignmentAlgorithms(game_map.width, game_map.height, game_map.getTargetsArray(), game_map.getClearedMap())
        self.clearedMap = game_map.getClearedMap()
        self.bound_algorithms = BoundAlgorithms(game_map)
        self.deadlock_detection = DeadlockDetection(game_map.getGameMap(), game_map.getTargetsArray(), self.width, self.height, self.game_map)
        self.alg = Pathfind_Algorithms2.Pathfind_Algorithms(game_map)
        self.openSet = set()

    # def possible_moves(self, box, cleared_map, boxes):
    #     result = []
    #     for a in [1, -1, self.width, -self.width]:
    #         b1 = cleared_map[box + a] != 4 and cleared_map[box - a] != 4
    #         b2 = not self.cont(boxes, box + a) and not self.cont(boxes, box - a)
    #         b3 = self.deadlock_detection.deadlock_array[box + a] == 0
    #         b4 = 0 <= (box + a) <= len(self.game_map.map)
    #         if b1 and b2 and b3 and b4:
    #             result.append([box, box + a])
    #     return result

    def possible_moves(self, box_array, cleared_map, pos):
        result = []
        for a in [1, -1, self.width, -self.width]:
            # The player cannot walk into walls
            b1 = cleared_map[pos + a] != 4
            # The player cannot move a box into a wall or another box
            b2 = not (self.cont(box_array, pos + a) != -1 and (self.cont(box_array, pos + 2*a) != -1 or cleared_map[pos + 2*a] == 4))
            # The player cannot move a box into a deadlock position
            b3 = not (self.cont(box_array, pos + a) != -1 and self.deadlock_detection.deadlock_array[pos + 2*a] == 1)
            # The player cannot leave the field
            b4 = 0 <= (pos + a) <= len(self.game_map.map)

            if b1 and b2 and b3 and b4:
                result.append([pos, pos + a])
        return result

    def analyse_state(self, node):
        box_array = node.box_array
        pos = node.player_pos

        moves = self.possible_moves(box_array, self.clearedMap, pos)
        for m in moves:

            # a new_box_array only needs to be changed if a box was moved
            i = self.cont(box_array, m[1])
            new_box_array = box_array.copy()
            if i != -1:
                new_box_array[i] = 2*m[1] - m[0]

            if self.transposition_table.lookup(new_box_array, m[1]):
                continue
            self.transposition_table.insert(new_box_array, m[1])

            bound = self.bound_algorithms.greedyBound(new_box_array, m[1])

            if bound == 0:
                # print("BOX_ARRAY: ", new_box_array)
                # self.bound_algorithms.greedyBound(new_box_array, pos)
                # Target found
                return Node(new_box_array, m[1], node, m, 0, node.depth +1)

            new_node = Node(new_box_array, m[1], node, m, bound, node.depth + 1)
            node.sons.append(new_node)

            if bound < node.lower_bound:
                res = self.analyse_state(new_node)
                if res is not None:
                    return res
            else:
                self.openSet.add(new_node)

            '''f = True
            for j in range(0, len(node.sons), 1):
                if bound < node.sons[j].lower_bound:
                    node.sons.insert(j, new_node) #Node(new_box_array, m[0], node, m, bound, node.depth +1))
                    f = False
                    break
            if f:
                node.sons.append(new_node) #Node(new_box_array, m[0], node, m, bound, node.depth + 1))'''

        return None

    def execute(self, current_node, depth=0):

        self.openSet.add(current_node)
        while self.openSet:
            current = min(self.openSet, key = lambda o:o.lower_bound + o.depth)
            self.openSet.remove(current)
            res = self.analyse_state(current)
            if res is not None:
                return res

        '''res = self.analyse_state(current_node)
        if res is not None:
            print("Final depth: ", depth)
            return res
        if not current_node.sons:
            return None
        if depth >= 1000:
            print("Maximum depth reached")
            return None

        for s in current_node.sons:
            r = self.execute(s, depth+1)
            if r is not None:
                return r
        return None'''


# Helpers

    @staticmethod
    def cont(array, item):
        for i in range(0, len(array), 1):
            if array[i] == item:
                return i
        return -1
