1.- (7.5 Puntos) Escribe la función moverBloque con SSE2, que copie un bloque de una 
imagen A en otro bloque de la imagen B, y además borre el bloque A (queda en color 
negro).

void moverBloque(Iplimage *imgA, int filaA, int colA, iplimage *imgB, int filaB, int 
colB)

Haz el efecto de transición en el cual se van moviendo todos los bloques de la imagen 
A a la imagen B. Se deben visualizar las 2 imágenes. Hacer una pausa de 100 ms después 
de visualizar cada movimiento.

Escribid el código en el fichero ejercicio1.c, mostrad el resultado al profesor cuando 
el programa funcione y entregad el proyecto al final del examen.

2.- (4 Puntos). Modifica el ejercicio anterior para realizarlo con 2 threads. El 
primero realiza la mitad izquierda, y el segundo la mitad derecha
Escribid el código en el fichero ejercicio2.c, mostrad el resultado al profesor cuando 
el programa funcione y entregad el proyecto al final del examen.
