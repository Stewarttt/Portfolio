"""
Name:
    assignment1.py
Authors:
    Connor Stewart (101041125)
    Gabrielle Latreille (101073284)
    Abdaljabbar Hersi (100919600)
Description:
    Python code solution to the assignment problem
"""

# list of imported libraries
import numpy as np
import heapq
import os


"""
Name:
    Node
Input:
    value - this is the value of the node being created
    heuristic - this is the heuristic being assigned to the node
    x - this is the x coordinate of the node on a graph
    y - this is the y coordinate of the node on a graph
Output:
    Creates a functional node data type
    Can connect other nodes in a path
    Can be used to store search information about the nodes
Description:
    Makes functional nodes used for the A* algorithm
"""
class Node:
    # initializes the node data type
    def __init__(self, value, heuristic, x, y):
        # node cost value
        self.value = value
        # node heuristic value
        self.heuristic = heuristic
        # coordinates of the node
        self.x = x
        self.y = y
        # sets the nodes parent to none by default
        self.parent = None
        # sets the costs based on the A* search as none by default
        self.f_cost = None
        self.g_cost = None
        # by default sets the nodes to be unexplored
        self.frontier = False
        self.explored = False
    # END __init__

    # adds the node to the frontier list of nodes upon update
    def add_to_frontier(self, parent, f_cost, g_cost):
        self.parent = parent
        self.f_cost = f_cost
        self.g_cost = g_cost
        self.frontier = True
    # END add_to_frontier

    # flags the node as having been explored
    def add_to_explored(self):
        self.frontier = False
        self.explored = True
    # END add_to_explored

    # operator overrides the direct comparison of node data types with f_cost values
    def __lt__(self, other):
        return self.f_cost < other.f_cost
    # END __lt__
# END Node


"""
Name:
    read_input_file
Input:
    input_filename - this is the filename of the input for the graph
Output:
    grid - Outputs a grid with the nodes in correct orientation
Description:
    Reads the input and output of a 2D array for the graph grid
"""
def read_input_file(input_filename):
    # create a list to represent the grid
    grid = []

    # open the file with f as a filepointer
    with open(input_filename, "r") as f:
        # let l be each line within the file
        for l in f:
            # format the line of data
            t_arr = l.rstrip("\n").replace(" ", "").split(",")
            # append the formatted data down into the grid
            grid.append(t_arr)
        # END FOR
    # END WITH

    # convert the python list into a numpy array data type
    grid = np.array(grid)

    # return the numpy array
    return grid
# END read_input_file


"""
Name:
    create_heuristic
Input:
    grid - A grid containing the nodes we are searching through
Output:
    g_h - this is the grid with heuristics being returned, with g being the goal node
    s_coords - this is the start of the grid being returned as coordinates
Description:
    Creates a heuristic for the grid based on the minimum manhattan distance between all goals and a node
        (def) manhattan distance - the distance between two points measured along axes at right angles
"""
def create_heuristic(grid):
    # gets the grid with the goal node
    g_coords = np.transpose(np.where(grid == "G"))
    # gets the starting node coordinates
    s_coords = np.transpose(np.where(grid == "S"))[0]

    # gets the grids heuristics
    g_h = np.empty(grid.shape, dtype=Node)

    # for each point in the grid, with i and j representing coordinates
    for i, j in np.ndindex(grid.shape):
        # if there is a wall, we cant pass
        if grid[i, j] == 'X':
            g_h[i, j] = Node('X', np.nan, i, j)
            continue
        # END IF

        # stores the distances between goals and node
        m_dist = []

        # for each goal node in the list of coordinates
        for goal in g_coords:
            # append the formatted data pertaining to manhattan distance to the goal
            m_dist.append(abs(i - goal[0]) + abs(j - goal[1]))
        # END FOR

        # add the data to the grid
        g_h[i, j] = Node(grid[i, j], np.min(m_dist), i, j)
    # END FOR

    # return the heuristic results
    return g_h, s_coords
# END create_heuristic


"""
Name:
    traverse_grid
Input:
    grid - A grid containing the nodes we are searching through
    s_coords - The starting point on the grid in coordinates
Output:
    A traversed path for the grid is returned
Description:
    Traverse the grid with frontier and explored list in mind
"""
def traverse_grid(grid, s_coords):
    # The list of adjacency values
    adjacency = np.array([[0, 1], [1, 0], [0, -1], [-1, 0]])

    # the x and y location of the start node's coordinates
    x, y = s_coords
    
    # adds the start node grid value to the frontier list
    grid[x, y].add_to_frontier(None, grid[x, y].heuristic, 0)

    # makes a list of points to traverse
    q = []
    heapq.heappush(q, grid[x, y])

    # for the list of explored points
    explored = []

    # while the queue is not empty
    while q :
        # set the currently iterated node's information
        current = heapq.heappop(q)
        current.add_to_explored()
        if current not in explored :
            explored.append(current)

        # if the currently iterated node is a goal node, return the explored list adn stop the search
        if current.value == "G":
            return explored
        # END IF

        # set the current adjacency value for the point
        adjacent = adjacency + [current.x, current.y]

        past = []

        # for each adjacency type in the list
        for a in adjacent:
            # set the x and y offset values
            a_x, a_y = a

            # continue if we are moving out of the lower bounds
            if a_x < 0 or a_y < 0:
                continue
            # END IF

            # continue if we are moving out of the upper bounds
            if a_x >= grid.shape[0] or a_y >= grid.shape[1]:
                continue
            # END IF

            # get the new node based on the offset
            adj_node = grid[a_x, a_y]

            # if the node is a wall continue
            if adj_node.value == "X":
                continue
            # END IF

            # if we have already explored the node continue
            if adj_node.explored:
                continue
            # END IF

            # gets the new cost value
            gn = float(current.g_cost) + (float(adj_node.value) if adj_node.value != "G" else 0)
            # adds the cost with the heuristic to get the new estimated value
            fn = gn + float(adj_node.heuristic)

            # if node is in the frontier and the old f_cost is smaller than the new f_cost, don't update
            if adj_node.frontier:
                if adj_node.f_cost < fn:
                    continue
                # END IF
            # END IF

            # gets the current adjacency node and adds it to the frontier list
            adj_node.add_to_frontier(current, fn, gn)
            # add the new nodes to the list
            heapq.heappush(q, adj_node)
        # END FOR
    # END WHILE
# END traverse_grid


"""
Name:
    write_explored_list
Input:
    explored - this is the explored nodes in the grid
    explored_list_filename - this is the filename containing the list of explores nodes
Output:
    Nodes are stored inside the specified text file
Description:
    Creates/Clears the text file of the specified name
    Appends a list of explored nodes to a text file
"""
def write_explored_list(explored, explored_list_filename):
    # clears the contents of the text file
    open(explored_list_filename, "w").close()

    # open the text file to append data into it
    f = open(explored_list_filename, "a")

    # for each node in the explored list, write to the file
    for node in explored:
        f.write(f"({node.x}, {node.y})\n")
    # END FOR

    # close the file
    f.close()
# END write_explored_list


"""
Name:
    write_optimal_path
Input:
    explored - this is the explored nodes in the grid
    optimal_path_filename - This is a list of the optimal paths in the grid
Output:
    Writes the optimal paths into the specified file
Description:
    Creates/Clears the text file of the specified name
    Appends a list of optimal paths to a text file
"""
def write_optimal_path(explored, optimal_path_filename):
    # clears the contents of the text file
    open(optimal_path_filename, "w").close()

    # open the text file to append data into it
    f = open(optimal_path_filename, "a")

    # set default values
    optimal = []
    current_node = explored[-1]

    # while there are points left to write down
    while True:
        # insert the node at the front of the list of optimals
        optimal.insert(0, current_node)

        # if the current node is the starting node, S
        if current_node.parent is None:
            break
        # END IF

        # gets the currently iterated nodes parent-node
        current_node = current_node.parent
    # END WHILE

    # for each node in the optimal list, write formatted node data to the file
    for node in optimal:
        f.write(f"({node.x}, {node.y})\n")
    # END FOR

    # close the file
    f.close()
# END write_optimal_path


"""
Name:
    pathfinding
Input:
    input_filename - This is the name of the CSV-file being used as input to setup the grid
    optimal_path_filename - This is the file name being used to store optimal paths in
    explored_list_filename - This is the file name being used to store the explored nodes in
Output:
    Returns the optimal paths and the explored nodes in the specified text files
    Returns the optimal paths and explored nodes into the command line
Description:
    Entry point for the implemented A* pathfinding algorithm
"""
def pathfinding(input_filename, optimal_path_filename, explored_list_filename):
    # gets the grid array data
    grid_array = read_input_file(input_filename)
    # gets the grid heuristic and coordinate values
    grid, s_coords = create_heuristic(grid_array)

    # gets the traversal results
    explored_list = traverse_grid(grid, s_coords)

    # writes the explored nodes and optimal values to the specified files
    write_explored_list(explored_list, explored_list_filename)
    write_optimal_path(explored_list, optimal_path_filename)

    # returns the results to the command line interface for quick review
    #print(explored_list[-1].g_cost)
    return explored_list[-1].g_cost
# END pathfinding


"""
Name:
    getFolder
Input:
    folderName - name of the subdirectory within the current working directory we want to look into
Output:
    Returns a valid folder name
Description:
    Gets user input for a folder/subdirectory within the current working directory of the python file
    Verifies the existence of the folder and relivently named files within
"""
def getFolder(folderName):
    # if the directory exists
    if os.path.isdir(folderName) == True:
        # if all the files exist
        dataExists = all(
            [os.path.isfile(folderName + "/astar_explored_list.txt"),
             os.path.isfile(folderName + "/optimal_path.txt"),
             os.path.isfile(folderName + "/input.txt")])

        # returns the result of the search
        if dataExists == True:
            return True
        else:
            # return an error message to the user
            print(
                "ERROR: the specified folder does not contain all the information files (explored_list_filename.txt, optimal_path_filename.txt, input.txt)")
        # END IF
    else:
        # return an error message to the user
        print("ERROR: the specified folder does not exist\n")
    # END IF

    # if the data does not exist return false
    return False
# END getFolder


"""
Name:
    __main__
Input:
    input() - inputs the name of the folder where the input data is located
Output:
    Outputs the results of the A* search algorithm to the specified folder
    Places data into explored_list_filename.txt and optimal_path_filename.txt
Description:
    Entry point for the A* search program
"""
# states of the specified data exists
specifiedData = False

# while there is no folder to open
while not specifiedData:
    # gets the name of the folder to open
    folderName = input("Please enter the name of the folder you want to open to read data from: ")

    # checks if the folder is valid
    specifiedData = getFolder(folderName)
# END WHILE

# gets the name of the files to input/output data from
explore_list_filename = folderName + "/astar_explored_list.txt"
optimal_path_filename = folderName + "/optimal_path.txt"
input_filename = folderName + "/input.txt"

# initialize the A* search
pathfinding(input_filename, optimal_path_filename, explore_list_filename)
