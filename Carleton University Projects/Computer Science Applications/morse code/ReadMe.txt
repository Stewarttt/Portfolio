Name: Connor Raymond Stewart
Student Number: 101041125
Carleton Email: connorrstewart@cmail.carleton.ca
Source File Names:
    morseCode.c
    reverseMorseCode.c
    bitMath.c
Compile Instructions: Open the Terminal to the directory of the source files and enter
    morseCode:
        gcc -Wall -o morseCode -std=c99 morseCode.c
        ./morseCode
    reverseMorseCode:
        gcc -Wall -o reverseMorseCode -std=c99 reverseMorseCode.c
        ./reverseMorseCode
    bitMath:
        gcc -Wall -o bitMath -std=c99 bitMath.c
        ./bitMath
Input Information:
    morseCode:
        any non-characters are converted to ? marks
        words are separated by three spaces, letters separated by one space
        simply enter a sequence of letters to convert to Morse code, example:
            I LOVE C => ..   .-.. --- ...- .   -.-.
            This sentence has 2 #'s and 2 symbols => - .... .. ...   ... . -. - . -. -.-. .   .... .- ...   ?   ? ? ...   .- -. -..   ?   ... -.-- -- -... --- .-.. ... 
            Connor Made This => -.-. --- -. -. --- .-.   -- .- -.. .   - .... .. ... 
            [abcdefghijklmnopqustuvwxyz] => ? .- -... -.-. -.. . ..-. --. .... .. .--- -.- .-.. -- -. --- .--. --.- ..- ... - ..- ...- .-- -..- -.-- --.. ?
    reverseMorseCode:
        ? stay as they are
        adding extra spaces will prompt the program to exit, and raise an error message
        entering invalid Morse code strings will cause the program to exit and raise an error message
        simply enter some Morse code to convert to letters, example:
            ..   .-.. --- ...- .   -.-. => I LOVE C
            - .... .. ...   ... . -. - . -. -.-. .   .... .- ...   ?   ? ? ...   .- -. -..   ?   ... -.-- -- -... --- .-.. ...  => THIS SENTENCE HAS ? ??S AND ? SYMBOLS
            -.-. --- -. -. --- .-.   -- .- -.. .   - .... .. ... => CONNOR MADE THIS
            ? .- -... -.-. -.. . ..-. --. .... .. .--- -.- .-.. -- -. --- .--. --.- ..- ... - ..- ...- .-- -..- -.-- --.. ? => ?ABCDEFGHIJKLMNOPQUSTUVWXYZ?
            
    bitMath:
        entering invalid sized sequences (non 8 character sequences), and invalid characters, will cause the program to exit, and raise an error message
        simple enter a 8-bit sequence of 1's or 0's in order to represent a two's compliment number, example:
            Enter 1st 8-bit sequence: 00001010
            Enter 2nd 8-bit sequence: 00000111
              00001010 (10)
              00000111 (7)
            ----------------
              00010001 (17)
        SEE MORE EXAMPLES IN output.txt

        
