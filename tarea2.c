#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	char id[100];
	char title[100];
	List *genres;
	List *directors;
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
	return strcasecmp((char *)key1, (char *)key2) == 0;
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

/**
 * Compara dos claves de tipo flotante para determinar si son iguales.
 * Esta función se utiliza en mapas para comparar claves de tipo flotante.
 *
 * @param key1 Primer puntero a la clave flotante.
 * @param key2 Segundo puntero a la clave flotante.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_float(void *key1, void *key2) {
	return *(float *)key1 == *(float *)key2; // Compara valores flotantes directamente
}

/**
 * Imrpime el contenido de un mapa dado como parametro, en el cual, el valor es una lista de pelis.
 * Y muestra el contenido de cada clave y valor en el mapa.
 *
 * @param pelis_byX Mapa que se quiere imprimir.
 */
void imprimir_mapa(Map *pelis_byX) {
	MapPair *pair = map_first(pelis_byX); //Primer pair del mapa
	
	while (pair != NULL) {
		char *clave = (char *)pair->key; //Clave del pair
		List *valor = (List *)pair->value; //Valor del pair (lista de pelis)
		
		Film *peli = list_first(valor); //Obtenemos la peli del primer elemento de la lista
		while (peli != NULL) {
			printf("  - Título: %s, ID: %s, Rating: %.01f, Decade: %d\n", peli->title, peli->id, 
																															peli->rating, peli->decade);
			peli = list_next(valor); //Obtenemos la siguiente peli de la lista
		}
		pair = map_next(pelis_byX); //Pasamos al siguiente pair del mapa
	}
}
/**
 * Imprime los datos/información de una película dad como parametro.
 *
 * @param peli Película cuya información se desea imprimir.
 */
void imprimir_peli(Film *peli){
	short is_first = 1;
	
	//Imprimimos información general de la película
	printf("===============================\n");
	printf("Título: %s\n", peli->title);
	printf("ID: %s\n", peli->id);
	printf("Rating: %.01f\n", peli->rating);
	printf("Año: %d\n", peli->year);
	printf("Década: %ds\n", peli->decade);

	//Imprimimos los géneros de la película
	printf("Géneros:\n");
	List *list_genres = peli->genres; //Obtenemos la lista de géneros de la película
	char *genre = list_first(list_genres); //Obtenemos el primer género de la lista
	while (genre != NULL){
		if (is_first){
			printf("  -  %s\n", genre);
			is_first = 0;
		} else printf("  - %s\n", genre);

		genre = list_next(list_genres); //Obtenemos el siguiente género de la lista
	}

	//Imprimimos los directores de la película
	is_first = 1;
	printf("Director(es):\n");
	List *list_directors = peli->directors;
	char *directors = list_first(list_directors);
	while (directors != NULL){
		if (is_first){
			printf("  -  %s\n", directors);
			is_first = 0;
		}else printf("  - %s\n", directors);
		directors = list_next(list_genres);
	}
	printf("===============================\n");
}

/**
 * Función para eliminar las comillas al inicio y al final de una cadena, si es que existen.
 *
 * @param str Cadena cuyas comillas se desean eliminar.
 */
void borrar_comillas(char *str) { 
	
	int len = strlen(str); //Obtenemos la longitud de la cadena

	
	if (str[0] == '"') {
			memmove(str, str + 1, len); //Movemos los caracteres una posición hacia la izquierda.
			len--; //Reducimos la longitud de la cadena
	}

	if (len > 0 && str[len - 1] == '"') {
		//Reemplazamos el último carácter por un carácter nulo para terminar la cadena.
			str[len - 1] = '\0'; 
	}
}

/**
 * Función para eliminar los espacios en blanco al inicio de una cadena dada como parámetro.
 *
 * @param str Cadena de caracteres.
 * @return Puntero a la primera posición no vacía de la cadena ingresada.
 */
char *eliminar_espacio_inicio(char *str) {
		while (isspace((unsigned char)*str)) str++;
		return str;
}

/**
 * Carga películas desde un archivo CSV y las almacena en diferentes mapas.
 * En esta función cada mapa almacena películas por distintos criterios.
 *
 * @param pelis_byid Mapa que almacena las películas por ID.
 * @param pelis_byGenre Mapa que almacena las películas por género.
 * @param pelis_byDirector Mapa que almacena las películas por director.
 * @param pelis_byRating Mapa que almacena las películas por rating.
 * @param pelis_byYear Mapa que almacena las películas por año.
 */
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

	//Se crean intervalos definiendo el rango de rating de las películas.
	int cant_intervalos = 18;
	float rating_intervalos[18][2] = { {1.0, 1.4}, {1.5, 1.9}, {2.0, 2.4}, {2.5, 2.9},
															{3.0, 3.4}, {3.5, 3.9}, {4.0, 4.4}, {4.5, 4.9},
															{5.0, 5.4}, {5.5, 5.9}, {6.0, 6.4}, {6.5, 6.9},
															{7.0, 7.4}, {7.5, 7.9}, {8.0, 8.4}, {8.5, 8.9},
															{9.0, 9.4}, {9.5, 10.0}
	};
	
	//Se inserta cada intervalo como clave en el mapa de rating.
	//Y se crea e inserta una lista de películas a cada intervalo de rating como valor.
	for (int rate = 0; rate < cant_intervalos; rate++){
		List *intervalo_peliculas = list_create(); //Lista de peliculas del intervalo
		char clave_intervalo[10]; //La clave para cada intervalo dentro del map
		sprintf(clave_intervalo, "%.1f-%.1f", rating_intervalos[rate][0],                                                                   rating_intervalos[rate][1]);
		map_insert(pelis_byRating, strdup(clave_intervalo), intervalo_peliculas);
	}
	
	// Lee cada línea del archivo CSV hasta el final
	while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
		// Crea una nueva estructura Film y almacena los datos de cada película
		Film *peli = (Film *)malloc(sizeof(Film));
		strcpy(peli->id, campos[1]);        // Asigna ID
		strcpy(peli->title, campos[5]);     // Asigna título
		peli->directors = list_create();  //Inicializa lista para directores
		peli->genres = list_create();       // Inicializa la lista de géneros
		peli->year = atoi(campos[10]); // Asigna año, convirtiendo de cadena a entero
		peli->decade = peli->year / 10 * 10; //Calcular la década de la película
		
		float peli_rating = atof(campos[8]); //Convertir rating de cadena a flotante

		map_insert(pelis_byid, peli->id, peli); //Insertar película por ID en el mapa
		peli->rating = peli_rating; //Asignar rating a la película	

		// Agregar el género a la lista de géneros de la película
		char *tokenDiv = strtok(campos[11], ","); // Dividir los géneros por comas
		while (tokenDiv != NULL){
			//Guardamos cada género en la list de géneros de la película
			list_pushBack(peli->genres, strdup(tokenDiv)); //strdup() para guardar una copia
			tokenDiv = strtok(NULL, ",");
		}

		//Inserción de la película en el mapa por género
		char *genre_token = list_first(peli->genres); //Obtenemos el primer género de la lista
		while(genre_token != NULL){
			//Limpieza de los géneros
			borrar_comillas(genre_token);
			genre_token = eliminar_espacio_inicio(genre_token);
			
			//Buscar el género en el mapa
			MapPair *genre_pair = map_search(pelis_byGenre, strdup(genre_token));
			List *genre_peliculas = NULL; //Lista auxiliar de películas para el género
			
			//Si el género ya existe en el mapa, se obtiene la lista de películas
			if (genre_pair != NULL){
				genre_peliculas = (List *)genre_pair->value;
			} else {
				//Si el género no existe en el mapa, se crea una nueva lista y se añade al mapa
				genre_peliculas = list_create();
				map_insert(pelis_byGenre, strdup(genre_token), genre_peliculas);
			}
			//En cualquiera de los 2 casos se inserta la película en la lista de películas del género
			list_pushFront(genre_peliculas, peli); 
			genre_token = list_next(peli->genres);//[pasar al] siguiente género
		}
		
		//Inserta la película en el mapa por decada
		List *decada_peliculas = NULL; //Lista auxiliar de películas para la década
		
		MapPair *decada_pair = map_search(pelis_byYear, &peli->decade); //Buscar la década en el mapa

		//Si la década ya existe en el mapa, se obtiene la lista de películas
		if (decada_pair != NULL) {
			decada_peliculas = (List *)decada_pair->value;
		} else {
			//Si la década no existe en el mapa, se crea una nueva lista y se añade al mapa
			decada_peliculas = list_create();
			map_insert(pelis_byYear, &peli->decade, decada_peliculas);
		}
		//En cualquiera de los 2 casos se inserta la película en la lista de películas de la década
		list_pushFront(decada_peliculas, peli);


		//Inserción de la película en el mapa por director
		tokenDiv = strtok(campos[14], ","); // Dividir los directores por comas
		while (tokenDiv != NULL){
			//Guardamos cada director en la lista de directores de la película 
			list_pushBack(peli->directors, strdup(tokenDiv)); //strdup() para guardar por referencia
			tokenDiv = strtok(NULL, ",");
		}
		char *director_token = list_first(peli->directors); //Obtener primer director de la lista
		List *director_peliculas = NULL; //Lista auxiliar de películas para el director
		
		while(director_token != NULL){
			//Limpieza de la cadena de los directores
			borrar_comillas(director_token);
			director_token = eliminar_espacio_inicio(director_token);
			
			//Buscar el director en el mapa
			MapPair *director_pair = map_search(pelis_byDirector, director_token);

			//Si el director ya existe en el mapa, se obtiene la lista de películas
			if (director_pair != NULL) {
				director_peliculas = (List *) director_pair->value;

			} else{
				//Si el director no existe en el mapa, se crea una nueva lista y se añade al map
				director_peliculas = list_create();
				map_insert(pelis_byDirector, strdup(director_token), director_peliculas);
			}
			//En cualquiera de los 2 casos se inserta la película en la lista de películas del director
			list_pushBack(director_peliculas, peli);
			director_token = list_next(peli->directors); //Pasar al siguiente director
		}

		//Recorrer los intervalos de ratings predefinidos
		for (int rate = 0; rate < cant_intervalos; rate++){
			//Comprobar si el rating de la pelicula se comprende en alg[un] intervalo
			if (peli->rating >= rating_intervalos[rate][0] && 
					peli->rating <= rating_intervalos[rate][1])
			{
				char clave_intervalo[10]; //La clave para cada intervalo dentro del map
				//Obtener la clave del intervalo encontrado
				sprintf(clave_intervalo, "%.1f-%.1f", rating_intervalos[rate][0],                                                                   rating_intervalos[rate][1]);
				//Buscar el intervalo en el mapa de rating
				MapPair *rating_pair = map_search(pelis_byRating, clave_intervalo);
				//Obtener la lista de películas del intervalo
				List *rating_peliculas = (List *) rating_pair->value;
				//Agregar la pelicula a la lista de películas del intervalo
				list_pushBack(rating_peliculas, peli); 
			}
		}
	}
			
		  
	
	fclose(archivo); // Cierra el archivo después de leer todas las líneas

	// Mostrar las películas cargadas
	imprimir_mapa(pelis_byRating);
}

/**
 * Busca y muestra la información de una película por su ID en un mapa.
 * @param pelis_byid Mapa que contiene las películas por ID.
 */
void buscar_por_id(Map *pelis_byid) {
	char id[100]; // Buffer para almacenar el ID de la película
	printf("===============================\n");
	// Solicita al usuario el ID de la película
	printf("Ingrese el id de la película: ");
	scanf("%s", id); // Lee el ID del teclado
	printf("===============================\n");
	// Busca el par clave-valor en el mapa usando el ID proporcionado
	MapPair *pair = map_search(pelis_byid, id);

	// Si se encontró el par clave-valor, se extrae y muestra la información de la
	// película
	if (pair != NULL) {
		Film *peli =
				pair->value; // Obtiene el puntero a la estructura de la película
		// Muestra el título y el año de la película
		imprimir_peli(peli);
	} else {
		printf("===============================\n");
		// Si no se encuentra la película, informa al usuario
		printf("La película con id %s no existe\n", id);
	}
	printf("===============================\n");
}


/**
 * Busca y muestra la información de una(s) película(s) por su director(es) en un mapa.
 * @param pelis_byid Mapa que contiene las películas por ID.
 */
void buscar_por_director(Map *pelis_byDirector) {
	char director[300]; // Buffer para almacenar el ID de la película
	int contador_pelis = 0; // Contador de películas encontradas
	printf("===============================\n");
	printf("Ingrese el Director de la(s) película(s): ");
	scanf(" %[^\n]", director); //Leer el nombre del director
	printf("===============================\n");
	//Buscar al director en el mapa
	MapPair *director_pair = map_search(pelis_byDirector, director);

	//Si se encuentra el director, se recorre la lista de películas del director
	if (director_pair != NULL) {
		
		List *director_peliculas = (List *) director_pair->value; // Obtener lista
		printf("Pelicula(s) del director %s:\n", director);

		//Recorrer la lista de Películas con ese director
		Film *peliAux = list_first(director_peliculas);
		while(peliAux != NULL){
			//Mostrar info. de la película
			imprimir_peli(peliAux);
			contador_pelis++;
			//Pasar a la siguiente película
			peliAux = list_next(director_peliculas);
		}
		printf("En total se encontraron %d películas del director %s\n", contador_pelis, director);
	} else {
		// Si no se encontraron películas, se emite la alerta correspondiente
		printf("===============================\n");
		printf("El director %s no tiene películas\n", director);
		printf("===============================\n");
	}
}

/**
 * Busca y muestra la información de películas en un mapa por intervalo de rating.
 * @param pelis_byRating Mapa que contiene las películas por intervalo de rating.
 */
void buscar_por_rating(Map *pelis_byRating) {
	float rating_inferior, rating_superior; //Variables para los límites del intervalo
	char clave_intervalo[100]; //Buffer para almacenar la clave del intervalo

	//Mostrar opciones de intervalo
	printf("===============================\n");
	printf("Seleccione un intervalo de rating:\n");
	printf("1) 1.0 - 1.4\n");
	printf("2) 1.5 - 1.9\n");
	printf("3) 2.0 - 2.4\n");
	printf("4) 2.5 - 2.9\n");
	printf("5) 3.0 - 3.4\n");
	printf("6) 3.5 - 3.9\n");
	printf("7) 4.0 - 4.4\n");
	printf("8) 4.5 - 4.9\n");
	printf("9) 5.0 - 5.4\n");
	printf("10) 5.5 - 5.9\n");
	printf("11) 6.0 - 6.4\n");
	printf("12) 6.5 - 6.9\n");
	printf("13) 7.0 - 7-4\n");
	printf("14) 7.5 - 7.9\n");
	printf("15) 8.0 - 8.4\n");
	printf("16) 8.5 - 8.9\n");
	printf("17) 9.0 - 9.4\n");
	printf("18) 9.5 - 10.0\n");

	//Asignar intervalo segun la opción seleccionada
	int opcion;
	printf("===============================\n");
	printf("Ingrese el intervalo deseado: ");
	int flag = scanf(" %d", &opcion);
	//Verificar si se ingreso una decada v[alida]
	if (flag != 1){
		while(getchar() != '\n');
		printf("===============================\n");
		printf("Intervalo no válido.\n");
		printf("===============================\n");
		return;
	}
	printf("===============================\n");
	switch (opcion) {
		case 1:
			rating_inferior = 1.0;
			rating_superior = 1.4;
			break;
		case 2:
			rating_inferior = 1.5;
			rating_superior = 1.9;
			break;
		case 3:
			rating_inferior = 2.0;
			rating_superior = 2.4;
			break;
		case 4:
			rating_inferior = 2.5;
			rating_superior = 2.9;
			break;
		case 5:
			rating_inferior = 3.0;
			rating_superior = 3.4;
			break;
		case 6:
			rating_inferior = 3.5;
			rating_superior = 3.9;
			break;
		case 7:
			rating_inferior = 4.0;
			rating_superior = 4.4;
			break;
		case 8:
			rating_inferior = 4.5;
			rating_superior = 4.9;
			break;
		case 9:
			rating_inferior = 5.0;
			rating_superior = 5.4;
			break;
		case 10:
			rating_inferior = 5.5;
			rating_superior = 5.9;
			break;
		case 11:
			rating_inferior = 6.0;
			rating_superior = 6.4;
			break;
		case 12:
			rating_inferior = 6.5;
			rating_superior = 6.9;
			break;
		case 13:
			rating_inferior = 7.0;
			rating_superior = 7.4;
			break;
		case 14:
			rating_inferior = 7.5;
			rating_superior = 7.9;
			break;
		case 15:
			rating_inferior = 8.0;
			rating_superior = 8.4;
			break;
		case 16:
			rating_inferior = 8.5;
			rating_superior = 8.9;
			break;
		case 17:
			rating_inferior = 9.0;
			rating_superior = 9.4;
			break;
		case 18:
			rating_inferior = 9.5;
			rating_superior = 10.0;
			break;
		default:
			printf("===============================\n");
			printf("Intervalo no válido.\n");
			printf("===============================\n");
			return;
	}

	// Generar la clave del intervalo
	sprintf(clave_intervalo, "%.1f-%.1f", rating_inferior, rating_superior);

	// Búsqueda del intervalo en el mapa
	MapPair *rating_pair = map_search(pelis_byRating, clave_intervalo);
	int contador_pelis = 0; // Contador de películas encontradas

	//sI se encuentra el intervalo, se recorre la lista de películas del intervalo
	if (rating_pair != NULL) {
		List *rating_peliculas = (List *) rating_pair->value;
		if (rating_peliculas != NULL){
			printf("===============================\n");
			printf("Películas con rating %.1f-%.1f:\n", rating_inferior, rating_superior);
			printf("===============================\n");
			//Recorrer la lista de Películas con ese rating
			Film *peliAux = list_first(rating_peliculas);
			//Si no se encuentran películas, se emite la alerta correspondiente
			if (peliAux == NULL){
				printf("===============================\n");
				printf("No hay películas en el rango de %.1f - %.1f\n", rating_inferior, rating_superior);
				printf("===============================\n");
				return;
			} 

			//Si se encuentras películas, se recorre la lista de películas con ese rating
			while(peliAux != NULL){
				imprimir_peli(peliAux); //Mostrar info. de la peli.
				contador_pelis++; //Incrementar contador de pelis.
				peliAux = list_next(rating_peliculas); //Pasar a la siguiente peli.
			}
			printf("En total se encontraron %d películas en este intervalo\n", contador_pelis);
		} else {
			//Emitir alerta si no se encuentran películas
			printf("===============================\n");
			printf("No hay películas en el rango de %.1f - %.1f\n", rating_inferior, rating_superior);
			printf("===============================\n");
		} 
	} else {
		//Emitir alerta si no se encuentran películas
		printf("===============================\n");
		printf("No hay películas en el rango de %.1f - %.1f\n", rating_inferior, rating_superior);
		printf("===============================\n");
	}
}

//Buscar por decada
void buscar_por_decada(Map *pelis_byYear) {
	int year; // Buffer para almacenar el ID de la película
	printf("===============================\n");
	printf("Ingrese la década de la película: ");
	int flag = scanf(" %d", &year);
	printf("===============================\n");
	//Verificar si se ingreso una decada v[alida]
	if (flag != 1){
		while(getchar() != '\n');
		printf("Década no válida.\n");
		printf("===============================\n");
		return;
	}
	
	int decada = (year/10)*10; //Asegurar que la decada sea multiplo de 10
	int contador_pelis = 0;
	printf("Década buscada: %d\n", decada);
	MapPair *decada_pair = map_search(pelis_byYear, &decada);
	//director_pair = NULL;
	if (decada_pair != NULL) {

		List *decada_peliculas = (List *) decada_pair->value;
		printf("===============================\n");
		printf("Peliculas de la década del %d:\n", decada);
	
		//Recorrer la lista de Películas con ese rating
		Film *peliAux = list_first(decada_peliculas);
		while(peliAux != NULL){

			imprimir_peli(peliAux);
			contador_pelis++;
			peliAux = list_next(decada_peliculas);
		}
		printf("En total se encontraron %d películas de la decada %d\n", contador_pelis, decada);
	} else {
		printf("===============================\n");
		printf("No hay películas en esta década: %d \n", decada);
		printf("===============================\n");
	}
}

//Buscar por decada
void buscar_por_genre(Map *pelis_byGenre) {
	char genre[300]; // Buffer para almacenar el ID de la película
	int contador_pelis = 0;
	printf("===============================\n");
	printf("Ingrese el género de la película: ");
	scanf(" %[^\n]", genre);
	printf("===============================\n");
	printf("Género buscado: %s\n", genre);
	MapPair *genre_pair = map_search(pelis_byGenre, genre);
	//director_pair = NULL;
	if (genre_pair != NULL) {

		List *genre_peliculas = (List *) genre_pair->value;
		printf("===============================\n");
		printf("Películas con el género de: %s\n", genre);

		//Recorrer la lista de Películas con ese rating
		Film *peliAux = list_first(genre_peliculas);
		while(peliAux != NULL){

			imprimir_peli(peliAux);
			contador_pelis++;
			peliAux = list_next(genre_peliculas);
		}
		printf("En total se encontraron %d películas con el género %s\n", contador_pelis, genre);
	} else {
		printf("===============================\n");
		printf("No hay películas con este género: %s \n", genre);
		printf("===============================\n");
	}
}

/**
 * Busca y muestra la información de películas por década y género en un mapa.
 * Primero se aplica el filtro de década y luego el de género.
 *
 * @param pelis_byYear Mapa que contiene las películas por década.
 * @param pelis_byGenre Mapa que contiene las películas por género.
 */
void buscar_por_decada_y_genre(Map *pelis_byYear, Map *pelis_byGenre){
	int decade;
	printf("===============================\n");
	printf("Ingrese la década de la película: ");
	int flag = scanf("%d", &decade);
	printf("===============================\n");
	
	//Verificar si se ingreso una decada v[alida]
	if (flag != 1){
		while(getchar() != '\n'); //Limpiar el buffer
		printf("Década no válida.\n");
		printf("===============================\n");
		return;
	} else {
		decade = (decade/10)*10; //Asegurar que la decada sea multiplo de 10
	}

	//Buscar peli. por decada
	MapPair *decada_pair = map_search(pelis_byYear, &decade);
	List *decada_peliculas = NULL;

	//Si se encuentran pelis. para la década, se recorre la lista de películas.
	if (decada_pair != NULL) {
		decada_peliculas = (List *) decada_pair->value;
		
		char genre[300]; // Buffer para almacenar el genero de la película
		int contador_pelis = 0;
		printf("===============================\n");
		printf("Ingrese el género de la película: ");
		scanf(" %[^\n]", genre);
		printf("===============================\n");
			
		List *genre_peliculas = NULL; // Lista auxliar de películas del género seleccionado
		
		MapPair *genre_pair = map_search(pelis_byGenre, genre); //Buscar pelis. por género en el map

		//Si se encuentran pelis. para el género, se recorre su lista de pelis.
		if (genre_pair != NULL){
			genre_peliculas = (List *) genre_pair->value; //Obtener la lista de pelis. del género
			
			//Recorrer las peliculas de ese g[enero]
			Film *peliAux = list_first(genre_peliculas);
			if (peliAux == NULL){
				//Si no se encuentran pelis. para el género, se emite el aviso correspondiente
					printf("===============================\n");
					printf("No hay peliculas con este género: %s\n", genre);
					printf("===============================\n");
				
			}
			//Si se encuentran pelis. para el género, se recorre la lista de pelis
			while(peliAux != NULL){
				if (peliAux->decade == decade){
					imprimir_peli(peliAux); //Mostar info. de las pelis.
					contador_pelis++; //Aumentar contador de pelis.
				}
				peliAux = list_next(genre_peliculas); //Pasar a la siguiente peli.
			}
			printf("En total se encontraron %d películas con el género %s y la década %d\n", contador_pelis, genre, decade);
			
		} else {
			//Emitir alerta si no se encuentran películas
			printf("===============================\n");
			printf("No hay películas con este género: %s\n", genre);
			printf("===============================\n");
		}

	} else {
		//Emitir alerta si no se encuentran películas
		printf("===============================\n");
		printf("No hay peliculas para la década: %d\n", decade);
		printf("===============================\n");
	}
}

int main() {
	char opcion; // Variable para almacenar una opción ingresada por el usuario
							 // (sin uso en este fragmento)

	// Creacion de un mapa para cada criterio de filtro/busqueda
	Map *pelis_byid = map_create(is_equal_str);
	Map *pelis_byGenre = map_create(is_equal_str);
	Map *pelis_byDirector = map_create(is_equal_str);
	Map *pelis_byRating = map_create(is_equal_float);
	Map *pelis_byYear = map_create(is_equal_int);

	do {
		mostrarMenuPrincipal();
		printf("===============================\n");
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
			buscar_por_genre(pelis_byGenre);
			break;
		case '5':
			buscar_por_decada(pelis_byYear);
			break;
		case '6':
			buscar_por_rating(pelis_byRating);
			break;
		case '7':
			buscar_por_decada_y_genre(pelis_byYear, pelis_byGenre);
			break;
		default:
		}
		presioneTeclaParaContinuar();

	} while (opcion != '8');

	return 0;
}