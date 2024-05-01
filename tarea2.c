#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char id[100];
  char title[100];
  List *genres;
  char director[300];
  float rating;
  int year;
  int decade;
} Film;

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Base de Datos de Películas");
  puts("========================================");

  puts("1) Cargar Películas");
  puts("2) Buscar por id");
  puts("3) Buscar por director");
  puts("4) Buscar por género");
  puts("5) Buscar por década");
  puts("6) Buscar por rango de calificaciones");
  puts("7) Buscar por década y género");
  puts("8) Salir");
}

/**
 * Compara dos claves de tipo string para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo string.
 *
 * @param key1 Primer puntero a la clave string.
 * @param key2 Segundo puntero a la clave string.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

/**
 * Compara dos claves de tipo entero para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo entero.
 *
 * @param key1 Primer puntero a la clave entera.
 * @param key2 Segundo puntero a la clave entera.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}
int is_equal_float(void *key1, void *key2) {
  return *(float *)key1 == *(float *)key2; // Compara valores enteros directamente
}
/**
 * Carga películas desde un archivo CSV y las almacena en un mapa por ID.
 */
void imprimir_mapa(Map *pelis_byid);
void cargar_peliculas(Map *pelis_byid,
                      Map *pelis_byGenre,
                      Map *pelis_byDirector,
                      Map *pelis_byRating,
                      Map *pelis_byYear) {
  
  // Intenta abrir el archivo CSV que contiene datos de películas
  FILE *archivo = fopen("data/Top1500.csv", "r");
  if (archivo == NULL) {
    perror(
        "Error al abrir el archivo"); // Informa si el archivo no puede abrirse
    return;
  }

  char **campos;
  // Leer y parsear una línea del archivo CSV. La función devuelve un array de
  // strings, donde cada elemento representa un campo de la línea CSV procesada.
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    // Crea una nueva estructura Film y almacena los datos de cada película
    Film *peli = (Film *)malloc(sizeof(Film));
    strcpy(peli->id, campos[1]);        // Asigna ID
    strcpy(peli->title, campos[5]);     // Asigna título
    strcpy(peli->director, campos[14]); // Asigna director
    peli->genres = list_create();       // Inicializa la lista de géneros
    peli->year = atoi(campos[10]); // Asigna año, convirtiendo de cadena a entero
    peli->decade = peli->year / 10 * 10;
    // Inserta la película en el mapa usando el ID como clave
    map_insert(pelis_byid, peli->id, peli);
    
    
    
    //Inserta la película en el mapa por decada
    //int decada = 
    //printf("Decada//: %d\n", decada);
    List *decada_peliculas = NULL;
    
    MapPair *decada_pair = map_search(pelis_byYear, &peli->decade);

    if (decada_pair != NULL) {
      decada_peliculas = (List *)decada_pair->value;
    } else {
      decada_peliculas = list_create();
      printf("Se inserto la decada//: %d\n", peli->decade);
      map_insert(pelis_byYear, &peli->decade, decada_peliculas);
      
    }

    list_pushBack(decada_peliculas, peli);
    
    /*if (decada_pair != NULL) {
      decada_peliculas = (List *) decada_pair->value;//Lista asociada a la decada
      
    } else {
      decada_peliculas = list_create();
      map_insert(pelis_byYear, &decada, decada_peliculas);
      printf("Se ha agregado la peli: %s",peli->title);
    }*/
    list_pushFront(decada_peliculas, peli);
    

    //Inserta la peli en el mapa usando el director como clave
    MapPair *director_pair = map_search(pelis_byDirector, peli->director);
    List *director_peliculas = NULL;
    if (director_pair != NULL) {
      director_peliculas = (List *) director_pair->value;
      
    } else{
      director_peliculas = list_create();
      map_insert(pelis_byDirector, peli->director, director_peliculas);
    }
    //Agregar peli a la lista de pelis del director
    list_pushBack(director_peliculas, peli);


    //Inserta la película en el mapa por rating
    MapPair *rating_pair = map_search(pelis_byRating, &peli->rating);
    List *rating_peliculas = NULL;
    
    if (rating_pair != NULL) {
      rating_peliculas = (List *) rating_pair->value;
      
    } else {
      rating_peliculas = list_create();
      map_insert(pelis_byRating, &peli->rating, rating_peliculas);
    }
    
    list_pushBack(rating_peliculas, peli);

    
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas

  // Itera sobre el mapa para mostrar las películas cargadas
  imprimir_mapa(pelis_byYear);
}

void imprimir_mapa(Map *pelis_byYear) {
  MapPair *pair = map_first(pelis_byYear);
  while (pair != NULL) {
    int *decada = (int *)pair->key;
    List *peliculas = (List *)pair->value;
    printf("Década: %d\n", *decada);

    /*Film *peli = list_first(peliculas);
    while (peli != NULL) {
      printf("  - Título: %s, Año: %d\n", peli->title, peli->year);
      peli = list_next(peliculas);
    }
    */
    pair = map_next(pelis_byYear);
  }
}

/**
 * Busca y muestra la información de una película por su ID en un mapa.
 */
void buscar_por_id(Map *pelis_byid) {
  char id[100]; // Buffer para almacenar el ID de la película

  // Solicita al usuario el ID de la película
  printf("Ingrese el id de la película: ");
  scanf("%s", id); // Lee el ID del teclado

  // Busca el par clave-valor en el mapa usando el ID proporcionado
  MapPair *pair = map_search(pelis_byid, id);

  // Si se encontró el par clave-valor, se extrae y muestra la información de la
  // película
  if (pair != NULL) {
    Film *peli =
        pair->value; // Obtiene el puntero a la estructura de la película
    // Muestra el título y el año de la película
    printf("Título: %s, Año: %d\n, Director: %s", peli->title, peli->year, peli->director);
  } else {
    // Si no se encuentra la película, informa al usuario
    printf("La película con id %s no existe\n", id);
  }
}

void buscar_por_director(Map *pelis_byDirector) {
  char director[300]; // Buffer para almacenar el ID de la película
 
  printf("Ingrese el Director de la película: ");
  scanf(" %[^\n]", director);
  
  MapPair *director_pair = map_search(pelis_byDirector, director);
  //director_pair = NULL;
  if (director_pair != NULL) {
    
    List *director_peliculas = (List *) director_pair->value;
    printf("Pelicula del director %s:\n", director);

    //Recorrer la lista de peliculas con ese director
    Film *peliAux = list_first(director_peliculas);
    while(peliAux != NULL){
      
      printf("Título: %s, Año: %d\n", peliAux->title, peliAux->year);
      peliAux = list_next(director_peliculas);
    }
    
  } else {
    printf("El director %s no tiene peliculas\n", director);
  }
}

//Buscar por rating
void buscar_por_rating(Map *pelis_byRating) {
  float rating_inferior, rating_superior; // Buffer para almacenar el ID de la película

  printf("Ingrese el rango de clasificaciones de la película (por ejemplo: 6.0-6.4): ");
  scanf(" %f-%f", &rating_inferior, &rating_superior);

  MapPair *rating_pair = map_first(pelis_byRating);
  //director_pair = NULL;
  while (rating_pair != NULL) {
    float rating = *(float *)rating_pair->key;
    if (rating >= rating_inferior && rating <= rating_superior){
      List *rating_peliculas = (List *) rating_pair->value;
      printf("Peliculas con calificación %.1f - %.1f:\n", rating_inferior, rating_superior);

      //Recorrer la lista de peliculas con ese rating
      Film *peliAux = list_first(rating_peliculas);
      while(peliAux != NULL){
        printf("Título: %s, Año: %d\n", peliAux->title, peliAux->year);
        peliAux = list_next(rating_peliculas);
      }
      break;
    }
    rating_pair = map_next(pelis_byRating);

  } 
  if (rating_pair == NULL){
    printf("No hay pelis rango de %.1f - %.1f\n", rating_inferior, rating_superior);
  }
}

//Buscar por decada
void buscar_por_decada(Map *pelis_byYear) {
  int year; // Buffer para almacenar el ID de la película
  
  printf("Ingrese el año de la película: ");
  scanf(" %d", &year);
  int decada = (year/10)*10;
  
  printf("Decada buscada: %d\n", decada);
  MapPair *decada_pair = map_search(pelis_byYear, &decada);
  //director_pair = NULL;
  if (decada_pair != NULL) {

    List *decada_peliculas = (List *) decada_pair->value;
    printf("Peliculas de la decada del %d:\n", decada);

    //Recorrer la lista de peliculas con ese rating
    Film *peliAux = list_first(decada_peliculas);
    while(peliAux != NULL){

      printf("Título: %s, Año: %d\n", peliAux->title, peliAux->year);
      peliAux = list_next(decada_peliculas);
    }

  } else {
    printf("No hay pelis en esta decada: %d \n", decada);
  }
}

int main() {
  char opcion; // Variable para almacenar una opción ingresada por el usuario
               // (sin uso en este fragmento)

  // Crea un mapa para almacenar películas, utilizando una función de
  // comparación que trabaja con claves de tipo string.
  Map *pelis_byid = map_create(is_equal_str);
  Map *pelis_byGenre = map_create(is_equal_str);
  Map *pelis_byDirector = map_create(is_equal_str);
  Map *pelis_byRating = map_create(is_equal_float);
  Map *pelis_byYear = map_create(is_equal_int);
  // Recuerda usar un mapa por criterio de búsqueda

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_peliculas(pelis_byid,
                        pelis_byGenre,
                        pelis_byDirector,
                        pelis_byRating,
                        pelis_byYear);
      break;
    case '2':
      buscar_por_id(pelis_byid);
      break;
    case '3':
      buscar_por_director(pelis_byDirector);
      break;
    case '4':
      
      break;
    case '5':
      buscar_por_decada(pelis_byYear);
      break;
    case '6':
          buscar_por_rating(pelis_byRating);
      break;
    case '7':
      break;
    default:
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  return 0;
}
