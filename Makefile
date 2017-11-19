#Makefile for ViolaJones face detector CLI program

all:
	gcc pngfileio.c facedetector.c drawing.c pixel.c imageprocessing.c ui.c -o vjfd.out -lpng

clean:
	rm vjfd.out
