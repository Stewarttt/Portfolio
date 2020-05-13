Name: Connor Stewart
Student Number: 101041125

Installation:
	To run, install bmp-1.2.6.3 and Haskell 8.6.3
	The code was tested using WinGHCi

Question 1:
	In order to run question 1, type:
		showAsASCIIArt (convertImageToGrayscale ".-+*#" True (loadBitmap "sample_image_to_search.bmp"))
	Resources Used:
		"sample_image_to_search.bmp" is the image provided with the assignment
		loadBitmap is included with the code provided by the professor
		convertImageToGrayscale executes the portion of the code I wrote
	Please Note:
			On my machine (and WinGHCI in general), when running the above command (usually the first 
		time) the outputted image will have spaces cutting random sections of it in half. 
		Simply running the function a few times seems to make Haskell output the image as a 
		whole image without any spaces.
		According to professor Collier, WinGHCi sometimes adds these random blank rows. 
		Its not an issue with the code, so it should just be ignored.
	Program Flow:
			The program has three main stages. First, the 2D array of RGB values are converted
		into a 2D array of luminosity values. Second, the 2D array of luminosity values are
		converted into negative numbers corresponding to the array of symbols used to represent
		the grey-scale. The negative number is based on a range of luminosity values corresponding
		to the specific grey-scale range. For example, if the array is [[2,200],[255]], and the 
		grey-scale array is "-+", then '-' will correspond to indexes of -1, and '+' will correspond
		to indexes of -2. Thus, the array will be converted to [[-2,-1], [-1]]. Third, the negative
		values in the array are sent to a final function which creates a 2D char array containing
		the characters which correspond to the negative array from part 2. For example, 
		[[-2,-1], [-1]] give instructions to generate [['+','-'],['-']] because -1 corresponds
		to the '-' symbol, and -2 corresponds to the '+' symbol.
			The purpose of the negative array is it decouples the intermediate step of converting 
		luminosity values into ranges. This allows separating the step where luminosity values
		are converted to grey-scale ranges (i.e 75%-100% Gray) from the step where ASCII characters
		are imputed to represent the grey-scale image.
Question 2:
	In order to run question 1, type: 
		findImage (convertImageToGrayscale " .:-=+*#%@" True (loadBitmap "sample_image_to_search.bmp")) (convertImageToGrayscale " .:-=+*#%@" True (loadBitmap "sample_image_to_find.bmp"))
	Resources Used:
		"sample_image_to_search.bmp" and "sample_image_to_FIND.bmp" are the images provided with the assignment
		loadBitmap is included with the code provided by the professor
		convertImageToGrayscale and findImage execute the portion of the code I wrote
	Please Note:
		ConvertImageToGreyscale does not output a [[Char]] in this version, it uses [[Int]]
		showAsASCIIArt is not included in this version, attempting to use it as if this were question 1 will not work
		This program can take some time to finish execution
	Program Flow:
			This program has two main stages. The array of grey-scale data is taken from the 
		methods created in question one and imported into the first stage. In stage one, 
		the larger image is searched through, element-by-element to find a single 'pivot', 
		which is the x and y coordinate of the currently iterated element. In the second
		stage, the pivot element is used as a entry point to check for the duplicate image. 
		Using the pivot to represent the top left hand corner of the image being found
		the image being searched is checked to see if that point actually is the top left 
		hand corner of the image being found. If it is, the pivot is returned, if it is not
		then the program continues finding and comparing new pivot values to the image it 
		is trying to find. If the program reaches the end of the image, then it outputs
		(-1, -1) to indicate that the image was not found.
	Instructions:
		The program will accept the following format:
			findImage -largerImageBeingSearch -smallerImageToSerchFor
		This assumes that the -largerImageBeingSearch and the -smallerImageToSerchFor 
		are both in [[Int]] format. This format can be gained by using the following:
			convertImageToGrayscale -Grey-scale -Inverted -bitMap
		This assumes that -Grey-scale is a character array (i.e. " .:-=+*#%@"), -Inverted 
		is a boolean, and -bitMap is a [[(Int, Int, Int)]]
			loadBitmap is part of the code provided by the professor
		Please note: 
			when using findImage, you must put the larger image, followed by the smaller image that is being searched for
			If you attempt to do this backwards, (-1, -1) will be returned. 
			Clarification: you can look for a flower in a field, but the application wont work as intended if you tell it to look for a field in a flower			
	Output:
		(-1, -1) - the image being searched for is not within the image being searched
		(-2, -1) OR (-1, -2) OR (-2, -2) - the (-2) means that the image is empty, with the first index representing the first image, and the second index representing the second image
		(x>=0, y>=0) - when the x and y are greater then or equal to zero, then the image being searched for has been found within the image being searched
			The value that is returned represents the pixel coordinates that the image was found at
			Note: pixel (0,0) is the top left hand corner