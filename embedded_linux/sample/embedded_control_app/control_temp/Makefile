FUN_NAME=control_temp

${FUN_NAME}:${FUN_NAME}.o
	${CC} -o ${FUN_NAME} ${FUN_NAME}.o -lpthread

${FUN_NAME}.o:${FUN_NAME}.c
	${CC} -c ${FUN_NAME}.c


clean:
	rm *.o ${FUN_NAME}

