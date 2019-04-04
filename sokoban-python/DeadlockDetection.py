import numpy as np


class DeadlockDetection:

    # TODO Missing bounds at self.width

    def __init__(self, game_array, target_array, width, height, game_map):
        self.game_array = game_array
        self.target_array = target_array
        self.width = width
        self.height = height
        self.deadlock_array = np.zeros(self.width*self.height, dtype=int)
        self.detect_pull()


    # Self designed deadlock detection.
    # Sets every corner as deadlocked and then uses the recursive property of deadlocks
    def detect(self):
        generator_array = np.zeros(self.width*self.height, dtype=int)
        for i in range(0, len(self.game_array)):
            # Wall
            if self.game_array[i] == 4:
                self.deadlock_array[i] = 1
            # Player or Empty (Boxes can not be on deadlocks from start and a corner with target is not a deadlock)
            elif self.game_array[i] == 1 or self.game_array[i] == 5:
                # Top-left corner
                if (self.game_array[i-1] == 4 or i-1 < 0) and (self.game_array[i-self.width] == 4 or i-self.width < 0):
                    self.deadlock_array[i] = 1
                # Top-right corner
                elif (self.game_array[i+1] == 4 or i+1 > len(self.game_array)) and (self.game_array[i-self.width] == 4 or i-self.width < 0):
                    self.deadlock_array[i] = 1
                # Bottom-left corner
                elif (self.game_array[i-1] == 4 or i-1 < 0) and (self.game_array[i+self.width] == 4 or i+self.width > len(self.game_array)):
                    self.deadlock_array[i] = 1
                # Bottom-right corner
                elif (self.game_array[i+1] == 4 or i+1 > len(self.game_array)) and (self.game_array[i+self.width] == 4 or i+self.width > len(self.game_array)):
                    self.deadlock_array[i] = 1

            # Without walls its no deadlock
            #elif not self._walls_around(i):
            #    self.deadlock_array[i] = 2
            # Targets are not deadlocks
            #elif self.game_array[i] == 3:
            #    self.deadlock_array[i] = 2

        for i in range(0, len(self.game_array), 1):
            self.deadlock_array[i] = self._reku_deadlock(i, generator_array)

    def _reku_deadlock(self, a, generator_array):
        if a < 0 or a >= len(self.game_array): # or self.game_array[a] == 4:
            return 1
        if self.deadlock_array[a] == 2:
            return 0
        if generator_array[a] == 1:
            return 1
        generator_array[a] = 1
        if self.deadlock_array[a] == 1:
            return 1
        if self.game_array[a] == 4:
            return 1
        if not self._walls_around(a):
            return 0
        if self.game_array[a] == 3:
            return 0
        r = 1
        if a+1 < len(self.game_array):
            r = r & self._reku_deadlock(a+1, generator_array)
        if a-1 >= 0:
            r = r & self._reku_deadlock(a-1, generator_array)
        if a+self.width < len(self.game_array):
            r = r & self._reku_deadlock(a+self.width, generator_array)
        if a-self.width > 0:
            r = r & self._reku_deadlock(a-self.width, generator_array)
        return r


    # Pull Deadlock detection
    def detect_pull(self):
        visited = np.zeros(self.width*self.height, dtype=int)
        for t in range(0, len(self.target_array), 1):
            self._reku_pull(self.target_array[t], visited)
        for a in range(0, len(visited), 1):
            if visited[a] == 0:
                self.deadlock_array[a] = 1

    def _reku_pull1(self, a, visited):
        if visited[a] == 1: return
        if a < 0 or self.game_array[a] == 4 or a >= len(self.game_array): return
        visited[a] = 1
        if a+1 < len(self.game_array) and self.game_array[a+1] != 4: self._reku_pull(a+1, visited)
        if a-1 > 0 and self.game_array[a+1] == 4: self._reku_pull(a-1, visited)
        if a+self.width < len(self.game_array) and self.game_array[a+self.width] != 4: self._reku_pull(a+self.width, visited)
        if a-self.width > 0 and self.game_array[a-self.width] != 4: self._reku_pull(a-self.width, visited)

    def _reku_pull(self, a, visited):
        if visited[a] == 1:
            return
        if a < 0 or a >= len(self.game_array):
            return
        visited[a] = 1
        if self.game_array[a] == 4:
            return
        if a+2 < len(self.game_array) and self.game_array[a+1] != 4 and self.game_array[a+2] != 4:
            self._reku_pull(a+1, visited)
        if a-2 >= 0 and self.game_array[a-1] != 4 and self.game_array[a-2] != 4:
            self._reku_pull(a-1, visited)
        if a + 2*self.width < len(self.game_array) and self.game_array[a+self.width] != 4 and self.game_array[a+2*self.width] != 4:
            self._reku_pull(a+self.width, visited)
        if a - 2*self.width >= 0 and self.game_array[a-self.width] != 4 and self.game_array[a-2*self.width] != 4:
            self._reku_pull(a-self.width, visited)



    # Helpers
    def _walls_around(self, a):
        b = self.game_array[a-1] == 4 or self.game_array[a+1] == 4 or self.game_array[a-self.width] == 4 or self.game_array[a+self.width] == 4
        c = a-1 < 0 or a-self.width < 0 or a+1 > len(self.game_array) or a+self.width > len(self.game_array)
        return b or c
