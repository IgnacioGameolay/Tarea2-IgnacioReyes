# Buscador de Películas por distintos Criterios

## Descripción del programa

Este programa cumple la funcionalidad de ser un buscador/filtrador para una Base de De Datos de películas, permitiendo así a los usuarios cargar películas, buscar por id, buscar por director, buscar por género, buscar por década, buscar por rango de calificaciones, buscar por década y género (doble filtro). 
Este programa está enfocado para facilitar la búsqueda y filtraje de películas dada una base de datos. Además, el sistema resulta, ser intuitivo y fácil de utilizar.



## Cómo compilar y ejecutar

Como uno de los objetivos del programa es ser fácil de utilizar y ser sumamente accesible, es recomendable utilizar webs como `Repl.it` para probar y ejecutar el sistema rápidamente. 
Para ello seguir los siguientes pasos:

1. Visitar [Repl.it](https://repl.it/).
2. Crear una nueva cuenta o iniciar sesión si ya tienes una.
3. Estando en el `dashboard`, seleccionar la opción "`New Repl`" y eligir "`Import from GitHub`".
4. Clonar el repositorio de GitHub: `https://github.com/IgnacioGameolay/Tarea2-IgnacioReyes.git`
5. [Repl.it](http://repl.it/) clonará el repositorio y preparará el entorno de ejecución automáticamente.
6. Finalmente, presionar el botón "Run" para compilar y ejecutar la aplicación de manera automática. O bien, compilar el código manualmente utilizando: `gcc tdas/*.c tarea2.c -Wno-unused-result -o tarea2` , y posteriormente ejecutar con: `./tarea2`



## Funcionalidades

### Funcionando correctamente:

- Cargar películas desde la base de datos otorgada.
- Buscar/Mostrar  una película ingresando su ID correspondiente.
- Buscar/Mostrar las películas de un director ingresado.
- Buscar/Mostrar las películas pertenecientes al género ingresado.
- Buscar/Mostrar las películas pertenecientes a una década ingresada.
- Buscar/Mostrar las películas pertenecientes a un intervalo/rango de calificaciones.
- Buscar/Mostrar las películas pertenecientes a una década y género (doble filtro) ingresados.
- Salir del sistema de búsqueda de base de datos de películas.

### Problemas conocidos:

- Al ingresar nombres de películas o géneros demasiado largos **podrían** llegar a haber problemas por posible falta de espacio en memoria para esos campos, pero esto serían muy raros casos, sin embargo existe esa posibilidad.
- Al ingresar un intervalo/rango de calificaciones que no respete el formato solicitado, no se realizará correctamente el filtro, por lo que es probable que mostrarán películas.

### A mejorar:

- Permitir la inserción manual de películas en cualquier momento del código.
- Implementar y desarrollar una interfaz que facilite al usuario trabajar con el sistema mucho más cómodamente que por consola.

## Ejemplo de uso
A continuación, veremos un ejemplo de cómo utilizar el sistema para buscar una(s) película(s) con los distintos filtros disponibles.
Por ello, al iniciar el sistema tendremos el siguiente menú con el cual interactuar:

``` 
========================================
     Base de Datos de Películas
========================================
1) Cargar Películas
2) Buscar por id
3) Buscar por director
4) Buscar por género
5) Buscar por década
6) Buscar por rango de calificaciones
7) Buscar por década y género
8) Salir
```
En este menú, podremos seleccionar una de las 8 opciones cuando se nos solicite `Ingrese su opción: `.

Los nombres de las opciones son bastante descriptivas (hacen lo que mencionan), pero de igual manera a continuación se mostrarán los pasos a seguir en el caso de uso completo de todos los filtros para una película, desde que es cargada hasta que es buscada por cualquiera de las otras 6 opciones.


**Opción 1 - Paso sustancial: Cargar las películas de la base de datos al sistema**

Empezamos por cargar las películas de la base de datos, de otra manera no servirían los filtros. 
Para ello seleccionamos la opción (1).

Luego de seleccionar la opción 1, veremos cómo se van cargando las películas y algunos (no todos por evitar saturación de mensajes por consola) de sus datos útiles como: Título, ID, Rating, y década. A continuación un ejemplo:


```
Ingrese su opción: 1
  - Título: Batman Forever, ID: tt0112462, Rating: 5.4, Decade: 1990
  - Título: Casino Royale, ID: tt0061452, Rating: 5.0, Decade: 1960
  - Título: The Millionairess, ID: tt0054086, Rating: 5.4, Decade: 1960
....
```

**Opción 2 - Filtro 1: Buscar por ID**

Una vez las películas hayan sido cargadas al sistema tendremos que indicar el ID de la película que deseamos mostrar, como se muestra en el siguiente ejemplo:

```
===============================
Ingrese su opción: 2
===============================
Ingrese el id de la película: tt0112462
===============================
===============================
Título: Batman Forever
ID: tt0112462
Rating: 5.4
Año: 1995
Década: 1990s
Géneros:
  -  Action
  -  Adventure
Director(es):
  -  Joel Schumacher
===============================
```

Como se ha visto en el ejemplo, al ingresar una id válida se mostrarán todos los datos de la película asociada, siendo estos su título, id, rating, año, década, géneros, director(es). De no existir la id a buscar, el sistema emitirá una alerta indicando que no se encontró la película deseada.

**Opción 3 - Filtro 2: Buscar por director**

El usuario también tiene la opción de filtrar las películas ingresando el nombre de un director. Al hacer esto, el programa buscará en la base de datos y mostrará todos los datos automáticamente de toda película en la que haya sido partícipe el director.
En el caso de no haber películas del director, el sistema el sistema dará el aviso correspondiente.
Siguiendo con el caso ejemplo:

```
===============================
Ingrese su opción: 3
===============================
Ingrese el Director de la(s) película(s): Joel Schumacher
===============================
Pelicula(s) del director Joel Schumacher:
===============================
Título: Batman Forever
ID: tt0112462
Rating: 5.4
Año: 1995
Década: 1990s
Géneros:
  -  Action
  -  Adventure
Director(es):
  -  Joel Schumacher
===============================
===============================
Título: Falling Down
ID: tt0106856
Rating: 7.6
Año: 1993
Década: 1990s
Géneros:
  -  Action
  -  Crime
  -  Drama
  -  Thriller
Director(es):
  -  Joel Schumacher
===============================
....
```

**Opción 4 - Filtro 3: Buscar por género**

El usuario también tendrá la opción de indicar un género para realizar el filtro/búsqueda de películas, una vez hecho esto, se mostrarán todas aquellas películas que pertenezcan a dicho género. A continuación un ejemplo:

```
===============================
Ingrese su opción: 4
===============================
Ingrese el genero de la película: Action
===============================
Género buscado: Action
===============================
Películas con el género del Action:
===============================
Título: The World Is Not Enough
ID: tt0143145
Rating: 6.4
Año: 1999
Década: 1990s
Géneros:
  -  Action
  -  Adventure
  -  Thriller
Director(es):
  -  Michael Apted
===============================
===============================
Título: Predator
ID: tt0093773
Rating: 7.8
Año: 1987
Década: 1980s
Géneros:
  -  Action
  -  Adventure
  -  Horror
  -  Sci-Fi
Director(es):
  -  John McTiernan
===============================
...
```
De no haber películas con el género indicado, el sistema emitirá una advertencia acorde a ello.

**Opción 5 - Filtro 4: Buscar por década**

Esta opción permite al usuario ver el listado de películas que pertenezcan a una determinada década (ej: 2010s), la cual, será brindada por el mismo usuario. De no haber películas lanzadas durante esa década, se mostrará la alerta correspondiente. 
A continuación un ejemplo:


```
===============================
Ingrese su opción: 5
===============================
Ingrese el año de la película: 2010
===============================
Decada buscada: 2010
===============================
Peliculas de la decada del 2010:
===============================
Título: Blue Caprice
ID: tt2027064
Rating: 5.9
Año: 2013
Década: 2010s
Géneros:
  -  Biography
  -  Crime
  -  Drama
Director(es):
  -  Alexandre Moors
===============================
===============================
Título: Like Crazy
ID: tt1758692
Rating: 6.6
Año: 2011
Década: 2010s
Géneros:
  -  Drama
  -  Romance
Director(es):
  -  Drake Doremus
===============================
...
```

**Opción 6 - Filtro 5: Buscar por rango de calificaciones**

Este filtro permite al usuario mostrar aquellas películas que pertenezcan a un intervalo/rango de ratings/calificaciones. Este rating será brindada por el mismo usuario al escoger una de las opciones que se le darán. De no haber películas lanzadas durante esa década, se mostrará la alerta correspondiente. 
A continuación un ejemplo de esta opción:

```
===============================
Ingrese su opción: 6
===============================
Seleccione un intervalo de rating:
1) 1.0 - 1.4
2) 1.5 - 1.9
3) 2.0 - 2.4
4) 2.5 - 2.9
5) 3.0 - 3.4
6) 3.5 - 3.9
7) 4.0 - 4.4
8) 4.5 - 4.9
9) 5.0 - 5.4
10) 5.5 - 5.9
11) 6.0 - 6.4
12) 6.5 - 6.9
13) 7.0 - 7-4
14) 7.5 - 7.9
15) 8.0 - 8.4
16) 8.5 - 8.9
17) 9.0 - 9.4
18) 9.5 - 10.0
===============================
Ingrese el intervalo deseado: 9
===============================
===============================
Películas con rating 5.0-5.4:
===============================
===============================
Título: Batman Forever
ID: tt0112462
Rating: 5.4
Año: 1995
Década: 1990s
Géneros:
  -  Action
  -  Adventure
Director(es):
  -  Joel Schumacher
===============================
===============================
Título: Casino Royale
ID: tt0061452
Rating: 5.0
Año: 1967
Década: 1960s
Géneros:
  -  Comedy
Director(es):
  -  Robert Parrish
===============================
....
```
**Opción 7 - Filtro 6: Buscar por década y género**

En el último filtro, tendremos la posibilidad de aplicar 2 filtros, en primer lugar filtramos por década, y luego, aplicaremos filtro por género. Para ambos casos, se expuso anteriormente casos de filtraje individual. A continuación un ejemplo de uso:

```
===============================
Ingrese su opción: 7
===============================
Ingrese la década de la película: 2000
===============================
===============================
Ingrese el género de la película: Drama
===============================
===============================
Título: Cassandra's Dream
ID: tt0795493
Rating: 6.6
Año: 2007
Década: 2000s
Géneros:
  -  Crime
  -  Drama
  -  Romance
  -  Thriller
Director(es):
  -  Woody Allen
===============================
===============================
Título: Man on Fire
ID: tt0328107
Rating: 7.7
Año: 2004
Década: 2000s
Géneros:
  -  Action
  -  Crime
  -  Drama
  -  Thriller
Director(es):
  -  Tony Scott
===============================
...
```
Si en cualquiera de los 2 filtros no se hayan películas, se avisará con la alerta correspondiente.

**Opción 8: Salir**
Finalmente, para terminar la ejecución del programa, seleccionamos la opción 8 y finalizará automáticamente.

***Cualquier consulta o duda que pueda surgir, contactarse al siguiente mail: 
ireyestoledo8@gmail.com***
-
