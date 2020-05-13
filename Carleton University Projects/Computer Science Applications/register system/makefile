all: textToBinary.o healthAssessment.o
textToBinary.o: textToBinary.c
	gcc -Wall -o textToBinary textToBinary.c
healthAssessment.o: healthAssessment.c
	gcc -Wall -o healthAssessment healthAssessment.c
clean:
	rm healthAssessment textToBinary absenteeism.bin
