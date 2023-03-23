APPNAME = builder 
CC = gcc
FILE = main.c

run: build
	@ ./${APPNAME}
	@ rm ${APPNAME}

build:
	@ ${CC} ${FILE} -o ${APPNAME}
