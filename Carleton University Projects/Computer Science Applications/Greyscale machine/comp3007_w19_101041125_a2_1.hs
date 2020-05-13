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

showAsASCIIArt :: [[Char]] -> IO ()
showAsASCIIArt pixels = Prelude.putStr (unlines pixels)

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

--converts an array of char integers (negative values) into their corresponding ASCII character
--for example: since "." is the first character in ".-+*#", any value of -1 in the array will be converted to a "." in the corresponding char array
convertToCharRepresentation :: [[Int]] -> [Char] -> [[Char]]
convertToCharRepresentation bitMap shadeArray = convertImageToChar (findGrayscaleRange bitMap (strLength shadeArray) 1) shadeArray

--converts an individual integer pixel into its corresponding char representation
convertIntToChar :: Int -> [Char] -> Char
convertIntToChar element shadeArray = shadeArray!!((-1)-element)

--converts a whole row of negative integer values into their char representation
convertRowToChar :: [Int] -> [Char] -> [Char]
convertRowToChar [] shadeArray = []
convertRowToChar (element:remainder) shadeArray = (convertIntToChar element shadeArray) : (convertRowToChar remainder shadeArray)

--converts the whole image of negative integer values into their corresponding char representation
convertImageToChar :: [[Int]] -> [Char] -> [[Char]]
convertImageToChar [] shadeArray = []
convertImageToChar (row:remainder) shadeArray = (convertRowToChar row shadeArray):(convertImageToChar remainder shadeArray)

--finds the luminosity which corresponds to the current character in the pixel array
--for example: if ".-+*#" is imputed, then element "-" will have an index value of 2
--this will result in the equation evaluating 255-255*0.4 (153)
--this means all pixels with a luminosity of 153 or greater will correspond to this symbol
findRange :: Int -> Int -> Int
findRange shades iteration = round(fromIntegral 255 - (fromIntegral iteration/fromIntegral shades) * fromIntegral 255)
  
--converts an image into a grey-scale representation of itself, using all of the above functions
--the grey-scale image is outputted to the user
convertImageToGrayscale :: [Char] -> Bool -> [[(Int, Int, Int)]] -> [[Char]]
convertImageToGrayscale shades inverted bitMap = convertToCharRepresentation (findRow bitMap) (verifyReversal shades inverted)