class StateTree:

    def __init__(self, box_array, player_pos, assignment):
        self.current_node = Node(box_array, player_pos, None, None, assignment.greedyAssignment(box_array))
        self.root_node = self.current_node

    def add_new_node(self, box_array, player_pos, move, lower_bound):
        self.current_node.add_new_node(box_array, player_pos, move, lower_bound)

    def add_node(self, node):
        self.current_node.add_node(node)

        # Handled inside the execute function..
        # lower_bound = assignment.greedyAssignment(box_array)
        # if lower_bound < self.current_node.lower_bound:
        #     self.current_node.add_node(box_array, player_pos, move, lower_bound)
        #     self.current_node = self.current_node.sons[-1]
        #     return 1
        # else:
        #     self.current_node.add_node(box_array, player_pos, move, lower_bound)
        #     return 0


    def get_current_node(self):
        return self.current_node

    def refuse_node(self):
        if self.current_node == self.root_node:
            return
        else:
            self.current_node = self.current_node.farther


class Node:

    def __init__(self, box_array, player_pos, farther, move, lower_bound, depth):
        self.box_array = box_array
        self.player_pos = player_pos
        self.farther = farther
        self.move = move
        self.lower_bound = lower_bound
        self.sons = []
        self.depth = depth

    def copy(self, new_move):
        return Node(self.box_array, self.player_pos, self.farther, new_move, self.lower_bound, self.depth)

    def add_new_node(self, box_array, player_pos, move, lower_bound, depth):
        self.sons.append(Node(box_array, player_pos, self, move, lower_bound, depth))

    def add_node(self, node):
        self.sons.append(node)

    def __str__(self):
        return str(self.move)

    def __repr__(self):
        return self.__str__()


class Move:

    def __init__(self, box_nr, old_pos, new_pos):
        self.box_nr = box_nr
        self.old_pos = old_pos
        self.new_pos = new_pos
