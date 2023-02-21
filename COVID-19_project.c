/*
Integrantes:
David Hernández Camacho
José Eduardo Hurtado Gómez
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <windows.h>

//████████████████████████████████████████████████████████
/*
 * Definicion de valores usados de codigos de color ANSI C,
 * que se utilizaron para poder darle color al imprimir en la
 * terminal.
 */
//████████████████████████████████████████████████████████
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"
#define BORRAR "\e[1;1H\e[2J"

//████████████████████████████████████████████████████████
/*
 * En esta parte del codigo definimos constantes para poder hacer modificaciones
 * facil y que no se dificultaran al mover porcentajes de las simulaciones.
 */
//████████████████████████████████████████████████████████
#define pocentaje_celdas_llenas 60

#define probabilidad_condicion1 90
#define probabilidad_condicion2 20
#define probabilidad_condicion3 10
#define probabilidad_condicion4 45
#define probabilidad_condicion5 15
#define probabilidad_condicion6 5


// Estas son las probabilidades de contagio que creemos que deberian ser las
// correctas.
/*
#define probabilidad_condicion1 90
#define probabilidad_condicion2 40
#define probabilidad_condicion3 20
#define probabilidad_condicion4 90
#define probabilidad_condicion5 30
#define probabilidad_condicion6 10
*/
//████████████████████████████████████████████████████████

//==============================================================

//████████████████████████████████████████████████████████
/*
 * Definicion de una estructura donde establecemos los siguienes estados del sujeto
 * de la simulacion, si hay alguien en la celda, numero para identificarlo, edad,
 * status de la persona, si esta vacunado, cuantos dias lleva enfermo, y si usa cubrebocas.
 */
//████████████████████████████████████████████████████████
typedef struct {
    int status_celda;   // 1 ocupado 0 libre
    int identificador;  //
    int edad;           // Edad con un numero entre 0 y 90
    int status_persona; // 0: Sano, 1: Enfermo, 2:Recuperado
    int vacunado;       // 0:Falso, 1:Verdadero
    int dias_enfermo;
    int usa_cubrebocas; // 0:Falso, 1:Verdadero

} CELDA;
//████████████████████████████████████████████████████████

//==============================================================

//████████████████████████████████████████████████████████
/*
 * Esta funcion refresca la pantalla del codigo en la terminal.
 */
//----------------------------------------------------------
void clrscr() { 
  system("@cls||clear"); 
  }
//████████████████████████████████████████████████████████

//==============================================================

//████████████████████████████████████████████████████████
/*
 * Esta funcion de -inicializar_posiciones- cumple dos propositos:
 * poder crear una matriz inicializada y poder establecer a personas
 * en las celdas pero en posiciones aleatorias a travez de la matriz.
 */
//████████████████████████████████████████████████████████
void inicializar_posiciones(CELDA *mat) {

    /*
     * Primero recorremos toda la matriz inicializando la matriz en un estado de -1,
     * al igual que de una vez establecimos su estado de salud y su estatus de dias enfermo.
     */
    int i, j;
    for (i = 0; i < 30; i++) {
        for (j = 0; j < 30; j++) {
            (mat + i * 30 + j)->status_celda = -1;
            (mat + i * 30 + j)->status_persona = 0;
            (mat + i * 30 + j)->dias_enfermo = 0;
        }
    }

    //Aqui establecemos una variable que la igualamos a una cierta cantidad de personas en estado de sana,
    //que en este caso son 540.
    int contador = (int) 900 * pocentaje_celdas_llenas * 0.01, random = 0;

    /*
     * Ya sabiendo el numero de personas que estaran ocupadas, entonces establecimos un ciclo while en el que si
     * una posicion aleatoria de celda esta en estado inicializado de -1, entonces se posicionaba ahi una persona.
     * Todo esto hasta que llegaramos al contador en 0 restandolo por cada iteracion.
     */
    while (1) {
        random = (rand() % 900);

        if ((mat + random)->status_celda == -1) {
            (mat + random)->status_celda = 1;
            contador--;
        }
        if (contador == 0)
            break;
    }

    //████████████████████████████████████████████████████████
    /*
     * Ahora para establecer las celdas donde no habra personas hicimos un ciclo for que reccoriera
     * los 900 lugares de la celda y donde encontrara alguna celda inicializada en -1 entonces posicionaria
     * que esta vacia la celda con un 0, donde habria un total de 360 por que son lo que restaban de haber posicionada
     * a las 540 personas. Ademas de ello inicializamos a todas las celdas con un estado de vacunado y de cubrebocas de -1.
     */

    for (i = 0; i < 900; i++) {
        if ((mat + i)->status_celda == -1) {
            (mat + i)->status_celda = 0;
        }
        // Inicializacion de los estados de vacunados y uso de cubrebocas.
        (mat + i)->vacunado = -1;
        (mat + i)->usa_cubrebocas = -1;
    }
    //████████████████████████████████████████████████████████
}
//████████████████████████████████████████████████████████


//==============================================================

//████████████████████████████████████████████████████████

/*
 * En esta funcion llamada -status_porcentaje_contagio- lo que hace es que inicializamos a las 540
 * personas de las celdas y que de manera aleatoria les establecemos a 135 personas (o el 25% de personas) diferentes
 * estatus de vacunado y de uso de cubrebocas, donde al azar en las 900 celdas tomamos una celda que esta inicializada
 * en los elementos de -1 y les damos un valor asignado si esque todavia no se llega a los 135. Este proceso de repitio 4 veces.
 */
//████████████████████████████████████████████████████████

void status_porcentaje_contagio(CELDA *mat, int vacunado, int usa_cubrebocas) {
    //Aqui usamos la constante de -porcentaje_celdas_llenas- con la que podiamos manipular los porcentas en las pruebas.
    int contador = (int)900 * pocentaje_celdas_llenas * 0.25 * 0.01;
    int random = 0;

    while (1) {
        random = (rand() % 900);

        if ((mat + random)->status_celda == 1) {

            if (((mat + random)->vacunado == -1) &&
                ((mat + random)->usa_cubrebocas == -1)) {
                (mat + random)->vacunado = vacunado;
                (mat + random)->usa_cubrebocas = usa_cubrebocas;
                contador--;
            }
        }

        if (contador == 0) {
            break;
        }
    }
}

//████████████████████████████████████████████████████████
/*
 * Esta funcio de -inicializar_status_persona- es la que utilizamos para invocar,
 * a la funcion de -status_porcentaje_contagio- donde establecemos con los criterios puestos
 * de: si tiene cubrebocas o no y sus posibles combinaciones, todo esto hasta llegar a 540 personas.
 * En pocas palabras esta funcion solo la usamos para llamar a la otra por temas de optimizacion.
 */
//████████████████████████████████████████████████████████

void inicializar_status_persona(CELDA *mat) {
    // No vacunadas y sin cubrebocas
    status_porcentaje_contagio(mat, 0, 0);
    status_porcentaje_contagio(mat, 0, 1);
    status_porcentaje_contagio(mat, 1, 0);
    status_porcentaje_contagio(mat, 1, 1);
}
//████████████████████████████████████████████████████████

//==============================================================

//████████████████████████████████████████████████████████
/*
 * En esta funcion de -enfermo_aleatorio- establecemos solo 1 enfermo, en toda la matriz
 * en la que la inicializamos teniendo en cuenta que debe haber una persona en esa celda y debe estar sana,
 * y que siendo el caso entonces le establecemos que esté en enfermo y que inicie su contador de dias en 0.
 */
//████████████████████████████████████████████████████████

// Sujeto contagiado:
void enfermo_aleatorio(CELDA *mat) {
    int random = 0;
    for (int i = 0; i < 900; i++) {
        random = (rand() % 900);
        if ((mat + random)->status_persona == 0 &&
            (mat + random)->status_celda == 1) {
            (mat + random)->status_persona = 1;
            (mat + random)->dias_enfermo = 0;
            break;
        }
    }
}
//████████████████████████████████████████████████████████

//==============================================================

//████████████████████████████████████████████████████████
/*
 * En este apartado de la funcion de -imprimir_matriz- lo que hacemos es poder imprimir la matriz
 * en un cuadrado y en el que dependiendo de los siguientes estatus es que llega a cambiar el tipo de
 * dato de la persona; si ninguno tiene cubrebocas y no está vacunado entonces es "S", si hay alguna
 * persona con cubrebocas, pero la persona sana está vacunada entonces "s", si hay alguna persona
 * vacunada, pero sin cubrebocas entonces "V", si hay alguna persona vacunada y con cubrebocas "v",
 * si hay una persona enferma sin cubrebocas entonces "E", si hay una persona enferma con cubrebocas "e". Y
 * por último si hay persona recuperada "r".
 *
 * Todo esto realizado con un codigo de colores para poder diferenciar a cada persona en la matriz y que sea claro
 * a la vista su contagio, ademas, que se logro darle color al definirlo al inicio las variables de los codigos de color
 * de ANSI C.

 A través de un ciclo for anidado, que recorre cada columna por renglón, se accede a cada elemento
de la matriz con la primera dirección argumento (mat) y se imprime un caracter para las siguientes
condiciones:
" " - Celda vacía
S - Si la persona está vacunada, está sana y usa cubrebocas
s - Si la persona no está vacunada, está sana y usa cubrebocas
V - Si la persona está vacunada, está sana y no usa cubrebocas
v - Si la persona está vacunada, está sana y usa cubrebocas
E - Si la persona está enferma y no usa cobrebocas 
e - Si la persona está enferma y usa cubrebocas
 */
//████████████████████████████████████████████████████████

void imprimir_matriz(CELDA *mat) {
    clrscr();
    printf("\n");
    printf("\n\t\t\t_________________________________________________________"
           "_\n");
    for (int i = 0; i < 30; i++) {
        printf("\t\t\t|");
        for (int j = 0; j < 30; j++) {

            if (mat->status_celda == 0) {
                printf("  ");
            } else if (mat->vacunado == 0 && mat->status_persona == 0 &&
                       mat->usa_cubrebocas == 0) {
                printf(" " GRN "S" RESET);
            } else if (mat->vacunado == 0 && mat->status_persona == 0 &&
                       mat->usa_cubrebocas == 1) {
                printf(" " GRN "s" RESET);
            } else if (mat->vacunado == 1 && mat->status_persona == 0 &&
                       mat->usa_cubrebocas == 0) {
                printf(" " CYN "V" RESET);
            } else if (mat->vacunado == 1 && mat->status_persona == 0 &&
                       mat->usa_cubrebocas == 1) {
                printf(" " CYN "v" RESET);
            } else if (mat->status_persona == 1 && mat->usa_cubrebocas == 0) {
                printf(" " RED "E" RESET);
            } else if (mat->status_persona == 1 && mat->usa_cubrebocas == 1) {
                printf(" " RED "e" RESET);
            } else if (mat->status_persona == 2) {
                printf(" " YEL "r" RESET);
            }
            mat++;
        }
        printf(" |\n");
    }

    printf(
            "\t\t\t---------------------------------------------------------------\n");
}

//████████████████████████████████████████████████████████

//==============================================================

//████████████████████████████████████████████████████████

/*
La función contagiado tiene la tarea de determina si una de las personas alrededor de la persona
enferma se contagia. Esto se realiza a partir de una probabilidad establecida por las
propiedades de la persona seleccionada al azar. Si una de las ocho celdas alrededor corresponde
a una celda vacía, se repite el ciclo hasta encontrar una persona para aplicar la probabilidad.

En dado caso de que un número al azar está dentro del rango de probabilidad, es decir que la 
persona se contagia, entonces regresa 1. De lo contrario, regresará 0. El entero que regresa
esta función se aplica al estado de la persona, siendo 1 como enfermo y 0 como sano.
*/
int contagiado(CELDA *persona_enferma, CELDA *persona_sana) {
    int random_contagios = (rand() % 100);
    // printf("%d", random_contagios);

    // Condicíon 1:
    if (persona_enferma->usa_cubrebocas == 0 &&
        persona_sana->usa_cubrebocas == 0 && persona_sana->vacunado == 0) {
        if (random_contagios < probabilidad_condicion1) {

            return 1;
        }
    }
        // Condicón 2:
    else if ((persona_enferma->usa_cubrebocas == 1 &&
              persona_sana->usa_cubrebocas == 0 && persona_sana->vacunado == 0) ||
              (persona_enferma->usa_cubrebocas == 0 &&
              persona_sana->usa_cubrebocas == 1 && persona_sana->vacunado == 0)) {
        if (random_contagios < probabilidad_condicion2) {
            return 1;
        }
    }

        // Condición 3:
    else if (persona_enferma->usa_cubrebocas == 1 &&
             persona_sana->usa_cubrebocas == 1 && persona_sana->vacunado == 0) {
        if (random_contagios < probabilidad_condicion3) {
            return 1;
        }
    }

        // Condición 4:
    else if (persona_enferma->usa_cubrebocas == 0 &&
             persona_sana->usa_cubrebocas == 0 && persona_sana->vacunado == 1) {
        if (random_contagios < probabilidad_condicion4) {
            return 1;
        }
    }

        // Condición 5:
    else if ((persona_enferma->usa_cubrebocas == 1 ||
              persona_sana->usa_cubrebocas == 1) &&
             persona_sana->vacunado == 1) {
        if (random_contagios < probabilidad_condicion5) {
            return 1;
        }
    }

    // Condición 6:
    if (persona_enferma->usa_cubrebocas == 1 &&
        persona_sana->usa_cubrebocas == 1 && persona_sana->vacunado == 1) {
        if (random_contagios < probabilidad_condicion6) {
            return 1;
        }
    }

    return 0;
}

//████████████████████████████████████████████████████████

//==============================================================

//████████████████████████████████████████████████████████
/* En la siguiente función "Encontrar_persona_sana" se regresa un apuntador hacia la dirección
donde se detecta, aleatoriamente, una persona con el estatus de sano alrededor de la persona enferma.
Asimismo, se evalúa que la dirección esté dentro de los límites de la representación de las
celdas repitiendo el algoritmo de encontrar una persona hasta que se cumpla esta condición
y además conserve el estatus de sana.


*/

int existen_sanos(CELDA *mat, int renglon, int columna)
{
  CELDA *posicion_sano;
  int pos_x, pos_y;
  int sanos = 0;

  for (int i = 0; i < 8; i++)
  {
    if (i >= 0 && i <= 2)
        pos_x = -1;

    else if (i >= 3 && i <= 4)
        pos_x = 0;

    else if (i >= 5 && i <= 7)
        pos_x = 1;

    // Asignar el valor de la posición y del contagiado
    if (i == 0 || i == 3 || i == 5)
        pos_y = -1;

    else if (i == 1 || i == 6)
        pos_y = 0;

    else if (i == 2 || i == 4 ||
              i == 7)
        pos_y = 1;
    // Si está dentro de los límites
    posicion_sano = ((mat + (renglon + pos_x) * 30) + (columna + pos_y));
    if ((renglon + pos_x) >= 0 && (renglon + pos_x <= 29) && 
        (columna + pos_y >= 0) && (columna + pos_y <= 29))
      {
      if (posicion_sano->status_celda == 1 && posicion_sano->status_persona == 0)
        sanos++;
      }
  }

  return sanos;
}

CELDA *encontrar_persona_sana(CELDA *mat, int renglon, int columna) {
    int random_posicion = 0;
    int pos_x, pos_y;
    CELDA *posicion_sano;

/* Las variables pos_x y pos_y representan el desplazamiento matricial considerando 
a mat como la persona enferma
*/
    if(existen_sanos(mat, renglon, columna) == 0)
      return (((mat + (renglon) * 30) + (columna)));
  
    while (1) {
        random_posicion = (rand() % 8);
        // Asignar el valor de la posición x del contagiado
        if (random_posicion >= 0 && random_posicion <= 2)
            pos_x = -1;

        else if (random_posicion >= 3 && random_posicion <= 4)
            pos_x = 0;

        else if (random_posicion >= 5 && random_posicion <= 7)
            pos_x = 1;

        // Asignar el valor de la posición y del contagiado
        if (random_posicion == 0 || random_posicion == 3 || random_posicion == 5)
            pos_y = -1;

        else if (random_posicion == 1 || random_posicion == 6)
            pos_y = 0;

        else if (random_posicion == 2 || random_posicion == 4 ||
                 random_posicion == 7)
            pos_y = 1;
/* Una vez se detecta que está dentro de los límites y corresponde a una persona, entonces
se regresa el apuntador a la dirección encontrada
*/
        posicion_sano = ((mat + (renglon + pos_x) * 30) + (columna + pos_y));

        if ((renglon + pos_x) >= 0 && (renglon + pos_x <= 29) &&
            (columna + pos_y >= 0) && (columna + pos_y <= 29)) {
            if (posicion_sano->status_celda == 1 && posicion_sano->status_persona == 0) {
                return (posicion_sano);
            }
        }
    }
}

//████████████████████████████████████████████████████████

/*
La siguiente función será empleada para iterar un ciclo sobre cada día y el desplazamiento
de las personas contagiadas con base en las funciones previamente definidas. Por cada elemento de la
matriz se evaluará:

  1. Detectar si tal persona es enferma
    a. En caso de serlo, seleccionar aleatoriamente una de las personas alrededor del enfermo
      y aplicar la función de contagiado para determina si la persona seleccionada se contagia
    b. Si es contagiada, entonces el día con la enfermedad equivale a 0 y adquiere la propiedad
      de contagiar para el siguiente día, es decir, iteración del while.
*/

void main_simulacion(CELDA *mat) {
    clrscr();
    int enfermos = 0;
    int recuperadas = 0;
    int sanas = 0;
    int dias_pasados = 0;
    char c;
    CELDA *posicion_enfermo;
    CELDA *posicion_sano;

    while (1) {
        // --- Ciclo for importante ---
        for (int renglon = 0; renglon < 30; renglon++) {
            for (int columna = 0; columna < 30; columna++) {

                if ((mat + renglon * 30 + columna)->status_persona == 1 && dias_pasados != 0) {

                    posicion_sano = encontrar_persona_sana(mat, renglon, columna);
                    posicion_enfermo = (mat + renglon * 30 + columna);

                    if ((posicion_sano->status_celda == 1) &&
                        (posicion_sano->status_persona != 2) &&
                        (posicion_sano->status_persona != 1) &&
                        (posicion_sano->dias_enfermo == 0)) {

                        posicion_sano->status_persona = contagiado(posicion_enfermo, posicion_sano);
                        posicion_sano->dias_enfermo = 0;
                    }
                }

            } // For columnas
        }   // For renglones

        /*
        El siguiente ciclo for realiza la cuenta de los días que una persona lleva enferma
        y lo convierte al estado recuperado en caso de cumplir los 8 días
        */
        for (int i = 0; i < 900; i++) {
            if ((mat + i)->status_persona == 1) {
                (mat + i)->dias_enfermo += 1;
            }

            if ((mat + i)->dias_enfermo > 8) {
                (mat + i)->status_persona = 2;
            }
        }

        /*
        Este ciclo for se utiliza para representar en la pantalla la cantidad de personas
        sanas, enfermas y recuperadas
        */
        for (int i = 0; i < 900; i++) {
            if ((mat + i)->status_persona == 0 && (mat + i)->status_celda == 1)
                sanas++;

            else if ((mat + i)->status_persona == 1 && (mat + i)->status_celda == 1)
                enfermos++;
            else if ((mat + i)->status_persona == 2 && (mat + i)->status_celda == 1)
                recuperadas++;
        }

        imprimir_matriz((CELDA *)mat);

        printf("\t\t\tDias transcurridos: %d\n", dias_pasados);
        printf(RED "\t\t\tPersonas enfermas: %d" RESET "\n", enfermos);
        printf(GRN "\t\t\tPersonas sanas: %d" RESET "\n", sanas);
        printf(YEL "\t\t\tPersonas recuperadas: %d" RESET "\n", recuperadas);
        //Se regresa el conteo de cada persona para el siguiente día (ciclo)
        enfermos = 0;
        sanas = 0;
        recuperadas = 0;
        printf("\nPresiona ENTER para poder avanzar en el dia o escribe un espacio para salir del programa: ");
        c = getchar();
        if (c == 10) {
            dias_pasados++;
            printf(BORRAR);
        } else if (c == 32) {
            exit(0);
        }
        clrscr();

    } // While
}
//████████████████████████████████████████████████████████
/*
En main se aplicará progresivamente cada función hasta llegar al ciclo que itera cadadía. 
De entrada, se inicializan las posiciones de las celdas en la matriz como libres o vacías.
Posteriormente, se reinician los valores de las personas y, aleatoriamente, se establece una persona
enferma.
Al entrar a la función main_simulación, se entra en el ciclo principal que representará cada día
hasta que se presione la tecla "Esc" y "Enter" para continuar.

*/

int main(void) {
    system("");
    srand(time(NULL));
    CELDA main_mat[30][30]; // Son 900 celdas en total, 60% son 540 y el 40%
    // restante 360.
  /* En cada celda se aplica un cast tipo CELDA y apuntador para que, al ser pasado como parámetro,
     el argumento actue como una apuntador a estructura CELDA del primer elemento de la matriz
  */
  
    inicializar_posiciones((CELDA *)main_mat);
    inicializar_status_persona((CELDA *)main_mat);
    enfermo_aleatorio((CELDA *)main_mat);
    imprimir_matriz((CELDA *)main_mat);

    printf("\n");
    main_simulacion((CELDA *)main_mat);

    return 0;
}