type Node = Int
type Edge = (Node, Node)
data Graph = Single Node | Union Graph Graph [Edge] deriving Show

--Question One: Does this Node Appear in this Graph

--findNodeInSingleton simply checks if a node is present in a singleton graph
findNodeInSingleton :: Node -> Graph -> Bool
findNodeInSingleton find (Single node) --check if node is in singleton graph
  | find == node = True --node is in the graph
  | otherwise = False --node is not in the graph

--findNodeInGraph find if a node is present in a graph
findNodeInGraph :: Node -> Graph -> Bool
findNodeInGraph nodeToFind (Single node) --if the graph is a singleton
        = findNodeInSingleton nodeToFind (Single node) --use function to check if node's in the singleton
findNodeInGraph nodeToFind (Union graphOne graphTwo edge) --if the graph is not a singleton
        --use the logical OR to check if either sub-graph contains the node
        --recursively check if either graph contains the node until the graph is exhausted
        = (findNodeInGraph nodeToFind graphTwo) || (findNodeInGraph nodeToFind graphOne)

--Question Two: Does this Edge Appear in this Graph
--Works on: (Union (Union (Single 4) (Union (Union (Single 5) (Single 4) [(5,5), (6,6), (7,7)]) (Union (Single 4) (Single 23) [(1,3), (2,4), (2,3)]) [(22,32)]) [(1,3), (1,3), (1,3)]) (Single 3) [(1, 3), (3, 1), (22, 34)])

checkEdgeInGraph :: Edge -> Graph -> Bool
checkEdgeInGraph edgeToFind (Single node) = False --a singleton graph has no edges, therefore false
checkEdgeInGraph edgeToFind (Union graphOne graphTwo edges)
        = breakEdgeList edgeToFind edges || checkEdgeInGraph edgeToFind graphOne || checkEdgeInGraph edgeToFind graphTwo


breakEdgeList :: Edge -> [Edge] -> Bool
breakEdgeList edgeToFind [] = False
breakEdgeList edgeToFind (firstEdge:remainder) 
        = (checkEdge edgeToFind firstEdge) || (breakEdgeList edgeToFind remainder)

checkEdge :: Edge -> Edge -> Bool
checkEdge (nodeOne, nodeTwo) (nodeThree, nodeFour) --break each edge into pair of nodes
  | nodeOne == nodeThree && nodeTwo == nodeFour = True --assumes directed graph
  | otherwise = False --false if not found

--Question 3: List All Nodes that Appear in this Graph
getNodesInGraph :: Graph -> [Node]
getNodesInGraph (Single singletonNode) = [singletonNode] --get the node from this singleton graph
getNodesInGraph (Union graphOne graphTwo edges)
        = (getNodesInGraph graphOne)++(getNodesInGraph graphTwo)

--Question 4: List All Edges that Appear in this Graph
getEdgesInGraph :: Graph -> [Edge]
getEdgesInGraph (Single singletonNode) = [] --singleton graphs have no edges
getEdgesInGraph (Union graphOne graphTwo edges)
        = edges++(getEdgesInGraph graphOne)++(getEdgesInGraph graphTwo)

--Question 5: Is this Graph a Singleton
isSingleton :: Graph -> Bool
isSingleton (Single singletonNode) = True
isSingleton (Union graphOne graphTwo edges) = False

--Question 6: Compute a Breadth-First Search Traversal 
--this functions finds adjacent edge pairs to nodes, and converts the graph into an adjacency list
adjacentNodes :: Graph -> [(Node, [Node])]
adjacentNodes (Single element) = [(element, [])] --singleton graphs have no edges
adjacentNodes (Union graphOne graphTwo edges)
  = (adjacentNodes graphOne)++(adjacentNodes graphTwo) --find the children of both subgraphs

--adds the edges to each node, such that a node has its children
buildAdjacencyList :: [(Node, [Node])] -> [Edge] -> [(Node, [Node])]
buildAdjacencyList [] edges = [] --exit when the whole list is iterated through
buildAdjacencyList ((element, edges):remainder) list
  --adds each edge to the nodes position in the adjacency list
  = (element,(addElements element list)):(buildAdjacencyList remainder list)

--determines the children of each node
addElements :: Node -> [Edge] -> [Node]
addElements node [] = [] --exit upon iterating through the whole list
addElements node ((first, second):remainder) --if the node is parent of the edge add it, continue otherwise
  | node == first = second:(addElements node remainder)
  | otherwise = addElements node remainder

--finds the smallest element within the adjacency list 
findSmallest :: [(Node, [Node])] -> Node -> Node
findSmallest [] node = node --returns the smallest element found
findSmallest ((first, second):remainder) node --look for an element thats smaller
  | node > first = findSmallest (remainder) first
  | otherwise = findSmallest (remainder) node
  
getFirstElement :: [(Node, [Node])] -> Node
getFirstElement ((first, second):remainder) = first --gets the first element from the adjacency list
  
searchSmallest :: [(Node, [Node])] -> Node -> Bool -> [Node] --searches the edges of the smallest node available
searchSmallest [] startingPoint firstRun = [] --exit upon completing the list
--we simply search through each element in the adjacency list to determine the next one to use
searchSmallest list startingPoint firstRun
  | firstRun == False = (removeElement (startSort (currentEdges list (findSmallest list (getFirstElement list)))))
  ++ (searchSmallest (removeSmallest (findSmallest list (getFirstElement list)) list) startingPoint False)
  | otherwise = (removeElement (startSort (currentEdges list startingPoint)))
  ++ (searchSmallest (removeSmallest (findSmallest list (getFirstElement list)) list) startingPoint False)

--removes the smallest element from a list
removeSmallest :: Node -> [(Node, [Node])] -> [(Node, [Node])]
removeSmallest element [] = [] --exit upon full iteration
removeSmallest element ((first, second):remainder) --look for smallest element
  | first == element = removeSmallest element remainder
  | otherwise = (first, second):(removeSmallest element remainder)

--look for the current edges in the adjacency list
currentEdges :: [(Node, [Node])] -> Node -> [Node]
currentEdges [] current = [] --exit upon full list iteration
currentEdges ((first, second):remainder) current --looks for valid edges for the node
  | first == current = second++(currentEdges remainder current)
  | otherwise = (currentEdges remainder current)

--sorts the list n times
sortList :: [Int] -> [Int]
sortList [] = [] --exit if array is empty
sortList (element:[]) = [element] --return the only element if theres only one element left
sortList (first:second:rest) --take the first, second, and remaining elements
  | rest == [] && first > second = (second):(first):[] --check which of the last two elements is larger
  | rest == [] && second > first = (first):(second):[] --check which of the last two elements is larger
  | second > first = [first] ++ (sortList (second:rest)) --check if this element is larger then the next
  | otherwise = [second] ++ (sortList (first:rest)) --check if this element is larger then the next

--runs the sortList function
startSort :: [Node] -> [Int]
startSort [] = [] --exit upon full iteration
startSort list --run if and only if the array is more then 1 unit long, sort n elements
  | (lengthList list) > 1 = sort list (lengthList list) --list is too short
  | otherwise = list --list no longer too short
  
sort :: [Int] -> Int -> [Int] --sort the list n times
sort [] runs = [] --exit upon full iteration
sort list runs
  | runs == 0 = list --return the list upon n runs
  | otherwise = sort (sortList list) (runs-1) --run the sort and count minus 1 to n runs

--finds the length of an integer array
lengthList :: [Int] -> Int
lengthList [] = 0 --return zero if the lists empty
lengthList (first:rest) = 1 + lengthList rest --remove an element, add one, and continue checking

--removes duplicates, takes the first element, then compares it to all others
removeElement :: [Int] -> [Int]
removeElement [] = [] --end upon full iteration 
removeElement (first:remainder)
  = first:(removeElement (remove first remainder)) --check for duplicates, and remove them

--entry point for the breadth first search
bfsEntry :: Graph -> Node -> [Int]
bfsEntry graph startingNode --initializes all the peace's to make a BFS
  | findNodeInGraph startingNode graph == False =  error "Error: node not in graph" --check if the node is in the graph
  | isSingleReturn (getEdgesInGraph graph) startingNode == False = [startingNode] --check if the element connects to any edges, or if its just a single return
  | otherwise = startingNode:(removeElement(searchSmallest (buildAdjacencyList (adjacentNodes (graph)) (getEdgesInGraph (graph))) startingNode True))

--checks if an element has no valid edges
isSingleReturn :: [Edge] -> Node -> Bool
isSingleReturn [] node = False --if the list ends return false
isSingleReturn ((first, second):remainder) node 
  | first == node = True --return true
  | otherwise = False || (isSingleReturn remainder node) --keep looking for an element that matches
  
  
--removes a given element from an array
remove :: Int -> [Int] -> [Int]
remove element [] = [] --end upon full list iteration
remove element (first:others) --take the first element
  | element == first = remove element others --check if the first element equals the deletion element
  | otherwise = first:(remove element others) --if the element is not the deletion element, keep it
   
--Question 7: Create a Graph from an Adjacency List
--converts adjacency list into a graph
graphFromAdjacencyList :: [(Node, [Node])] -> Graph
--returns an error if an empty adjacency list is inputted
graphFromAdjacencyList []
        = error "Error: Empty adjacency list argument"
--if the adjacency list represents a singleton graph
graphFromAdjacencyList [(element, edges)]
        = (Single element)
--if the adjacency list represents a double-ton graph
graphFromAdjacencyList [(elementOne, edgesOne), (elementTwo, edgesTwo)]
        = (Union (Single elementOne) (Single elementTwo) ((createEdgeList elementOne edgesOne)++(createEdgeList elementTwo edgesTwo)))
--if the adjacency list is neither a singleton nor a double-ton, break it down into smaller elements
graphFromAdjacencyList ((element, edges):remainder)
        = (Union (Single element) (graphFromAdjacencyList remainder) (createEdgeList element edges))

--creates a list of edges from a list of nodes
createEdgeList :: Node -> [Node] -> [Edge]
--if there are no edges
createEdgeList parent [] = []
--break the list down into smaller elements, and convert each element into an edge pair
createEdgeList parent (first:remainder) = (parent, first):(createEdgeList parent remainder)

--Question 8: Create a Graph from an Adjacency Matrix
--converts an adjacency matrix into a graph
graphFromAdjacencyMatrix :: [[Bool]] -> Graph
--if the adjacency matrix is empty return an error
graphFromAdjacencyMatrix [] 
        = error "Error: Empty adjacency matrix argument"
graphFromAdjacencyMatrix matrix
  -- if the adjacency matrix is not a square, return an error prompting to user this information
  | (isSquareMatrix matrix) == False = error "Error: adjacency matrix is not a square"
  --if the adjacency matrix is a square, then convert it into an adjacency list and convert it into a graph
  | otherwise = graphFromAdjacencyList (convertMatrixToList matrix 1)

--converts an adjacency matrix into an adjacency list
convertMatrixToList :: [[Bool]] -> Int -> [(Node, [Node])]
--if the list is empty return an empty list
convertMatrixToList [] element = []
--convert the current row in the adjacency matrix into its corresponding adjacency list representation
convertMatrixToList (first:remainder) element = (element, convertBoolToEdges first 1):(convertMatrixToList remainder (element+1))

--converts a list of booleans into a edge list
convertBoolToEdges :: [Bool] -> Int -> [Node]
--if the list is empty, return an empty list
convertBoolToEdges [] element = []
convertBoolToEdges (first:others) element
  --if the current element is true, then return the current index value and check the other elements in the list
  | first == True = (element):(convertBoolToEdges others (element+1))
  --if the current element is false, then simply check the other elements in the list
  | otherwise = convertBoolToEdges others (element+1)

--checks if the matrix is a square or not
isSquareMatrix :: [[Bool]] -> Bool
isSquareMatrix [] = True --if the list is empty, return true
isSquareMatrix (first:remainder)
  --if all rows in the matrix are the same length, and the hight of the matrix is equivilent to one of the rows then the matrix is a square
  | (allSameLength (first:remainder)) /= -1 && (getLengthMatrix (first:remainder) == getLengthArray first) == True = True
  --if the above is not true, then the matrix is not a square
  | otherwise = False

--gets the length of a matrix by counting its rows
getLengthMatrix :: [[Bool]] -> Int
getLengthMatrix [] = 0 --empty matrix is length zero
getLengthMatrix (first:remainder) = 1 + getLengthMatrix remainder --add one and count the remaining rows

--gets the length of an array by counting its elements
getLengthArray :: [Bool] -> Int
getLengthArray [] = 0 --empty array is length zero
getLengthArray (first:remainder) = 1 + getLengthArray remainder --add one and count the remaining elements

--checks if all elements within the matrix are of the same length
allSameLength :: [[Bool]] -> Int
allSameLength [] = 0 --if the matrix is empty, return a zero
allSameLength [lastElement] = getLengthArray lastElement --if the matrix contains only one row, get the rows length
allSameLength (first:remainder)
  --if the current row has the same length as the next row, return the length of this current row
  | (getLengthArray first) == (allSameLength remainder) = getLengthArray first
  --if the current row does not have the same length as the next row, return -1 (this is a value no row can ever achieve the length of)
  | otherwise = -1

--Questions:
--how should I treat double listed nodes in a graph for question 3? Two ones is fine
--how should I treat double listed edges in a graph for question 4? Add all edges
--should I treat edges as directed or undirected for question 2? The graphs are directed, parent first child second
--is my representation in question 7 a correct way of representing the graph? No problems with the representation
--is it OK to convert an adjacency matrix into an adjacency list as a solution for Question 8? There should be no problem