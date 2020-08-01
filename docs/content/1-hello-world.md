---
title: Hei maailma
nav: true
---

# Hello World!

Ensimmäisiä asioita joita koodatessa on hyvä tehdä on kirjoittaa niin sanottu Hello World ohjelma.

Hello World ohjelma ei itsessään tee mitään järkevää, vilkuttaa lediä tai kirjoittaa ruudulle jotain mutta se todistaa yhden tärkeän asian. Olet pystynyt kirjoittamaan koodin oikein ja käännöstympäristö joka ohjelman tuottamiseen tarvitaan on oikein asennettu ja kaikki toimii.

Aloitetaan ensin uusi projekti. Etsi VSCodesta PlatformIO kuvake ja valitse sieltä "PIO Home" -> "Open". Valitse "Quick Access" valikosta "New Project"

<center>
{% include figure.html img="pio_new_project.png" alt="" caption="" width="100%" %}
</center>

Tämän jälkeen annetaan ohjelmalle nimi, esim HelloWorld ja valitaan oikea laitteisto jolle ohjelmaa ollaan kirjoittamassa (Arduboy)

<center>
{% include figure.html img="pio_new_project_arduboy.png" alt="" caption="" width="100%" %}
</center>

Pienen odottelun jälkeen projektin kansio aukeaa vasempaan reunaan.

<center>
{% include figure.html img="pio_new_project_started.png" alt="" caption="" width="100%" %}
</center>

Projektin kansio rakenteessa on paljon valmista:

```
.
├── include
│   └── README
├── lib
│   └── README
├── platformio.ini
├── src
│   └── main.cpp
└── test
    └── README
```

"include" kansiossa on projektin "header" tiedostot. Tämä hello world ohjelma on niin yksinkertainen että tätä ei tarvita.

"lib" kansioon voi lisätä omia tai muiden libraryitä joita käyttää projektissa

"src" kansiossa on ohjelman varsinainen lähdekoodi. Oletuksena luodaan main.cpp tiedosto jota muokkaamme kohta

"test" kansioon voi kirjoittaa testi caset ohjelman toimivuuden tarkistamiseksi

"platformio.ini" tiedostossa määritellään projektin asetuksia PlatformIO:ta varten.

# Arduboy libraryn lisääminen

[Arduboy libraryn dokumentaatio](https://mlxxxp.github.io/documents/Arduino/libraries/Arduboy2/Doxygen/html/classArduboy2.html)

Koska laitteistomme on arduboy emmekä halua tehdä ylimääräistä työtä näytön ohjelmointia varten otamme aivan ensimmäisenä käyttöön arduboy libraryn tätä varten.

Muutetaan platformio.ini tiedostoa niin että kerrotaan että haluamme arduboy2 nimisen libraryn mukaan ohjelmaamme. Lisätään tiedoston loppuun nämä rivit:

```ini
lib_deps =
  Arduboy2
```
Tämän jälkeen muutetaan hieman main.cpp tiedostoa

```cpp
#include <Arduino.h>
#include <Arduboy2.h>

// instantiate our arduboy object
Arduboy2 arduboy;

void setup() {
  // begin, as per arduino guidelines
  arduboy.begin();
}

void loop() {
  // clear screen
  arduboy.clear();
  // print our text to the buffer
  arduboy.print("Hello World");
  // and display the buffer
  arduboy.display();
  // and do it again
}
```

Nyt voimme kääntää ohjelman ja ladata sen arduboyhin. Alareunassa on nuoli nappi joka tekee kääntämisen ja lataa ohjelman arduboylle.

<center>
{% include figure.html img="hello_world.png" alt="" caption="" width="100%" %}
</center>

Nyt Arduboyn näytöllä näkyy "Hello World" ja tiedämme että ohjelmointi ympäristö ja laitteisto ovat toimivia!

Hienoa, seuraavaksi voidaankin siirtyä taas hieman haastavampaan juttuun.

[Aloitetaan pelin kirjoittaminen](2-pong-game)