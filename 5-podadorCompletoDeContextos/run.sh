function DeleteUnnecessaryFiles {
    rm -f $(pwd)/lib/src/listaDeContextos.o
    rm -f $(pwd)/podadorCompletoDeContextos.out
}

gcc -c -o lib/src/listaDeContextos.o lib/src/listaDeContextos.c
if [ $? -ne 0 ]; then
    DeleteUnnecessaryFiles
    exit 1
fi

gcc -o podadorCompletoDeContextos.out podadorCompletoDeContextos.c lib/src/listaDeContextos.o -lm
if [ $? -ne 0 ]; then
    DeleteUnnecessaryFiles
    exit 1
fi

./podadorCompletoDeContextos.out
DeleteUnnecessaryFiles