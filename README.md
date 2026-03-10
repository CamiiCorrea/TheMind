# TheMind
Proyecto Estructura de Datos
Actualización 1:
Para empezar el proyecto, primero analizamos las reglas básicas del juego The Mind y decidimos qué funcionalidades eran necesarias para tener un prototipo viable. Nos enfocamos en lo esencial: repartir cartas, que los jugadores puedan jugarlas en orden, validar si la jugada es correcta, manejar las vidas y verificar si se gana o se pierde.
Con esa base, construimos el diagrama UML. Al inicio lo hicimos simple, con tres clases principales:
- Juego: controla el flujo del juego (niveles, vidas, jugadores, mazo).
- Jugador: representa a cada participante y sus cartas.
- Carta: guarda el valor numérico de cada carta.
  
Después lo fuimos ampliando para que se viera más completo y reflejara mejor la lógica del juego. Agregamos clases auxiliares como Mazo (para barajar y repartir cartas) y Nivel (para definir cuántas cartas recibe cada jugador según el nivel). También añadimos atributos y métodos más detallados, y mostramos las relaciones entre las clases con flechas (por ejemplo, el Juego contiene Jugadores, y cada Jugador tiene Cartas).
En conclusión, el diagrama UML lo hicimos paso a paso: primero sencillo para entender la estructura, y luego lo mejoramos con más clases y relaciones para que se vea más profesional y se acerque mejor al funcionamiento real del juego.
