--Name: Connor Stewart
--Student Number: 101041125

--imported packages
import Codec.BMP
import Data.ByteString
import Data.Either
import GHC.Word
import System.IO.Unsafe

--provided code
loadBitmap :: FilePath -> [[(Int, Int, Int)]]
loadBitmap filename = repackAs2DList (either returnEmptyOnError processDataOnBMP (unsafePerformIO (readBMP filename)))
  
returnEmptyOnError :: Error -> ([(Int, Int, Int)], (Int, Int))
returnEmptyOnError _ = ([], (0, 0))

processDataOnBMP :: BMP -> ([(Int, Int, Int)], (Int, Int))
processDataOnBMP bmp = ((parseIntoRGBVals (convertToInts (unpack (unpackBMPToRGBA32 bmp)))), (bmpDimensions bmp))
  
convertToInts :: [Word8] -> [Int]
convertToInts [] = []
convertToInts (h:t) = (fromIntegral (toInteger h)) : (convertToInts t)

parseIntoRGBVals :: [Int] -> [(Int, Int, Int)]
parseIntoRGBVals [] = []
parseIntoRGBVals (h:i:j:_:t) = (h,i,j) : (parseIntoRGBVals t)

repackAs2DList :: ([(Int, Int, Int)], (Int, Int)) -> [[(Int, Int, Int)]]
repackAs2DList (pixels, (width, height)) = (Prelude.reverse (repackAs2DList' pixels width height))

repackAs2DList' :: [(Int, Int, Int)] -> Int -> Int -> [[(Int, Int, Int)]]
repackAs2DList' []  width  height = []
repackAs2DList' pixels width height = (Prelude.take width pixels) : (repackAs2DList' (Prelude.drop width pixels) width height)

--my code

--converts a square 2D array of RGB-Tuples into a 2D array of luminosity
findRow :: [[(Int, Int, Int)]] -> [[Int]]
findRow [[]] = []
findRow [] = []
findRow (firstRow:otherRows) = (findPixal firstRow):(findRow otherRows)

--converts a single row of RGB-Tuples into a array of luminosity
findPixal :: [(Int, Int, Int)] -> [Int]
findPixal [] = []
findPixal (pixal:otherPixals) = (convertRGBToLuminosity pixal):(findPixal otherPixals)

--converts a single tuple of RGB values into the luminosity for that pixel
convertRGBToLuminosity :: (Int, Int, Int) -> Int
convertRGBToLuminosity (red, green, blue) = round(fromIntegral red * 0.2126 + fromIntegral green * 0.7152 + fromIntegral blue * 0.0722)

--method simply reverses a string
reverseString :: [char] -> [char]
reverseString [] = []
reverseString (firstElement:remainder) = (reverseString remainder)++[firstElement]

--verifies if a string is to be reversed
verifyReversal :: [char] -> Bool -> [char]
verifyReversal string permission
  | permission == True = (reverseString string) --reverse string and return
  | otherwise = string --don't reverse string, simply return the input string

--changes a picture of luminosity values into a numerical representation corresponding to its char representation
--0-representation converts the current index of the char array representing grey-scale (i.e. ".-+*#") into a negative number
--for example: "-" in ".-+*#" is element number two, thus 0-representation will result in -2 being added to this position in the array
--the negative numbers will later be replaced with characters when the array is converted to ASCII art
--it is safe to swap places of the array to negative numbers, because there is no negative luminosity values possible
exchangeRows :: [[Int]] -> Int -> Int -> [[Int]]
exchangeRows [] representation start = [] --terminate when lists empty
exchangeRows (firstRow:otherRows) representation start = (rowToPsudoChar firstRow (0-representation) start):(exchangeRows otherRows representation start)

--changes a row of luminosity values into a numerical representation corresponding to its char representation
rowToPsudoChar :: [Int] -> Int -> Int -> [Int]
rowToPsudoChar [] representation start = []
rowToPsudoChar (element:otherElements) representation start = (changeElement element representation start):(rowToPsudoChar otherElements (representation) start)

--changes a single luminosity pixel into a numerical representation corresponding to its char representation
changeElement :: Int -> Int -> Int -> Int
changeElement element representation start 
  | element >= start = representation
  | otherwise = element

--iterates for the number of elements in the grey-scale
--for example: ".-+*#" has five elements, thus this will iterate exactly five times
findGrayscaleRange :: [[Int]] -> Int -> Int -> [[Int]]
findGrayscaleRange [] shades iteration = []
findGrayscaleRange bitMap shades iteration
  | shades >= iteration = findGrayscaleRange (exchangeRows bitMap iteration (findRange shades iteration)) shades (iteration+1)
  | otherwise = bitMap

--finds the length of a string
strLength :: [Char] -> Int 
strLength [] = 0 --termination step
strLength (first:remainder) = 1+(strLength remainder) --counts the number of elements until the string is empty

--finds the length of a Int array
intLength :: [Int] -> Int 
intLength [] = 0 --termination step
intLength (first:remainder) = 1+(intLength remainder) --counts the number of elements until the string is empty

--finds the length of a Int matrix
arrayLength :: [[Int]] -> Int 
arrayLength [] = 0 --termination step
arrayLength (first:remainder) = 1+(arrayLength remainder) --counts the number of elements until the string is empty

--finds the luminosity which corresponds to the current character in the pixel array
--for example: if ".-+*#" is imputed, then element "-" will have an index value of 2
--this will result in the equation evaluating 255-255*0.4 (153)
--this means all pixels with a luminosity of 153 or greater will correspond to this symbol
findRange :: Int -> Int -> Int
findRange shades iteration = round(fromIntegral 255 - (fromIntegral iteration/fromIntegral shades) * fromIntegral 255)
  
--converts an image into a grey-scale representation of itself, using all of the above functions
convertImageToGrayscale :: [Char] -> Bool -> [[(Int, Int, Int)]] -> [[Int]]
convertImageToGrayscale shades inverted bitMap = findGrayscaleRange (findRow bitMap) (strLength (verifyReversal shades inverted)) 1

--findImage gets both the bitMap arrays and checks which one is the smaller one
--if one bitmap is smaller on the horizontal axis, its determined to be the smaller image
--if the bitmaps are equivalent on the horizontal axis, then the one that is the smallest on the vertical axis is determined to be the smallest
--if the bitmaps are equal size, bitmap one is arbitrarily chosen to be the smaller bitmap
findImage :: [[Int]] -> [[Int]] -> (Int, Int)
findImage [] [] = (-2, -2) --error, both images are empty
findImage [] smallerBitMap = (-2, -1) --error, image one is empty
findImage largerBitMap [] = (-1, -2) --error, image two is empty
findImage largerBitMap smallerBitMap = checkForSmallerInLarger smallerBitMap largerBitMap (0, 0) (intLength (largerBitMap!!0), arrayLength largerBitMap)

--checks a single row in order to determine if the larger row contains the smaller row
--only checks from index zero, thus if both elements don't start in sequence, it wont determine similarity
checkRows :: [Int] -> [Int] -> Bool
checkRows [] [] = True --smaller list exhausted
checkRows [] largerRow = True --smaller list exhausted
checkRows smallerRow [] = False --it cannot be in here
checkRows (current:remainder) (element:rest) --first element on each row
  | current == element = True && (checkRows remainder rest) --check if all elements are the same
  | otherwise = False --if a single element is not the same, it cannot be here
  
--this checks if a 2D array contains a smaller 2D array as a subset of itself, from index zero only
checkColumn :: [[Int]] -> [[Int]] -> Bool
checkColumn [] [] = True --smaller list exhausted
checkColumn [] largerRow = True --smaller list exhausted
checkColumn smallerRow [] = False --it cannot be in here
checkColumn (rowOne:remainderOne) (rowTwo:remainderTwo) --first element on each matrix
  | checkRows rowOne rowTwo == True = True && (checkColumn remainderOne remainderTwo) --check if all elements are the same
  | otherwise = False --if a single element is not the same, it cannot be here
    
--this iterates through an array until a desired index is found, all elements up to that index are discarded
rowSlicer :: [Int] -> Int -> Int -> [Int]
rowSlicer [] coordinates counter = [] --return nothing if the smaller list is exhausted
rowSlicer (first:remainder) coordinates counter
  | coordinates == counter = first:remainder --we have reached out desired location
  | otherwise = rowSlicer remainder coordinates (counter+1) --iterate though the array by one more element
  
--this iterates through a 2D array until a desired index is found, all rows up to that index are discarded
columnSlicer :: [[Int]] -> Int -> Int -> [[Int]]
columnSlicer [] coordinates counter = [] --return nothing if the smaller list is exhausted
columnSlicer (row:remainder) coordinates counter 
  | coordinates == counter = row:remainder  --we have reached out desired location
  | otherwise = columnSlicer remainder coordinates (counter+1) --iterate though the array by one more element

--this iterates through a 2D array until a desired index is found, all rows and columns up to that index are discarded
arraySlicer :: [[Int]] -> (Int, Int) -> Int -> [[Int]]
arraySlicer [] coordinates counter = [] --return nothing, the array is exhausted
arraySlicer (first:others) (xValue, yValue) counter 
  | yValue > 0 = arraySlicer (columnSlicer (first:others) yValue 0) (xValue, 0) counter --recursively remove extra rows/columns, set the y value to zero
  | counter <= (arrayLength (first:others)) = (rowSlicer first xValue 0):(arraySlicer others (xValue, 0) (counter+1)) --recursively remove extra row elements, iterate the counter by 1
  | otherwise = ((rowSlicer first xValue 0):others) --return the final formatted array
  
--checks to see if a smaller array exists as a subset within a larger array
--this method is used to determine if one photo lies within another photo
--this method works by selecting a pivot, which represents an x/y coordinate of the larger bit map
--if the 'pivot' represents the first element of the bit map were searching for, and all the elements after the 'pivot' represent the bit map
--the we return the coordinates of the pivot as a tuple represented by (Int, Int) where the first Int represents the x coordinate and the second
--Int represents the Y coordinate
checkForSmallerInLarger :: [[Int]] -> [[Int]] -> (Int, Int) -> (Int, Int) -> (Int, Int)
checkForSmallerInLarger smallerBitMap largerBitMap (xVal, yVal) (maxX, maxY)
  | xVal > maxX = checkForSmallerInLarger smallerBitMap largerBitMap (0, yVal+1) (maxX, maxY) --iterate the y value by one, and set the x to zero, when the x value is out of bounds
  | yVal > maxY = (-1, -1) --if the y value is out of bounds, then the whole array has been searched through, return not found value
  | (checkColumn smallerBitMap (arraySlicer largerBitMap (xVal, yVal) 0)) == True = (xVal, yVal) --if the current pivot is the top left hand corner of the image, return the pivot coordinates
  | otherwise = checkForSmallerInLarger smallerBitMap largerBitMap (xVal+1, yVal) (maxX, maxY) --find the next pivot by adding one to the current x value