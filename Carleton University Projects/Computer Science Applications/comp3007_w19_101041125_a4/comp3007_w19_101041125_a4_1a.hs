--Name: Connor Raymond Stewart
--Student Number: 101041125

countZerofree :: [Int] -> Int --finds the number of zero free integers in the list
countZerofree [] = 0 --if the lists empty return 0
countZerofree array = searchThroughList array 0 --search through the list

searchThroughList :: [Int] -> Int -> Int --find the number of zero free integers
searchThroughList [] count = count --return the total number of non-zero elements
searchThroughList (first:remainder) count --get the first element
  | first == 0 = searchThroughList remainder count --if the first elements just a zero, add nothing
  | otherwise = searchThroughList remainder (count + checkZerofree first) --check if the non-zero number contains a zero

--checkZerofree works because when it divides the number by ten, it truncates the remainder
--if we simply divide by ten, then multiple by ten, then 1234X is converted to 12340
--we can then subtract 1234X by 12340 to isolate for X, we can find if X = 0 and also divide 12340 by ten to 
--repeat the same process to check all the other numbers
checkZerofree :: Int -> Int --check the number to see if it has a zero
checkZerofree 0 = 1 --if this number reaches zero, it is a non zero number
checkZerofree number --check for zeros
  | number < 0 = checkZerofree (-1 * number) --if the numbers negative, make it positive
  | (modulo number 10) == 0 = 0 --if the modulo of the number is zero, then add nothing, as there is a zero there
  | otherwise = checkZerofree (number `div` 10) --divide the number by 10

modulo :: Int -> Int -> Int --get the modulo of the number based on the specified denominator
modulo number denominator = number - ((number `div` denominator)*10)