---
title: PingPong
nav: true
---

# Pong peli

[Pong](https://en.wikipedia.org/wiki/Pong) on klassikko peli vuodelta 1972. Peli on kaksi uloitteinen simulaatio pöytä tenniksestä jossa pelaaja kontrolloi mailaansa ja yrittää saada pallon toisen pelaajan mailan ohi.

<center>
{% include figure.html img="Pong_Game_Test2.gif" alt="" caption="By pong74ls, CC BY 3.0, https://commons.wikimedia.org/w/index.php?curid=79121297
" width="100%" %}
</center>


# Suunnittelu ja pohjat

Pelissä on yksi pallo, kaksi mailaa, pisteet ja pelikenttä. Kun pelaaja saa pallon toisen pelaajan mailan ohi saa hän yhden pisteen ja kun pelaajalla on yli viisi pistettä voittaa hän pelin.

Peli tilanne alkaa pallosta keskellä ruutua ja mailat ovat puolivälissä kenttää. Kun jompikumpi pelaaja painaa nappia niin pallo lähtee matkaan ja pelaajat voivat liikuttaa mailojaan.

Aloitetaan pelin tekeminen luomalla runko peliä varten ja laajennetaan sitten osa kerrallaan toiminnallisuuksilla.

Ensin tehdään uusi projekti. Valitaan taas arduboy alustaksi ja lisätään ensimmäisenä arduboy library käyttöön.

platformio.ini tiedostoon lisätään

```ini
lib_deps =
  Arduboy2
```

Ja main.cpp tiedostoon lisätään alkuun
```
#include <Arduboy2.h>
Arduboy2 arduboy;
```

Ja muokataan setup functiota alustamaan arduboy ja asetetaan muutama tärkeä optio.

```c
void setup()
{
  // start using arduboy library
  arduboy.begin();
  // initialize random number generator
  arduboy.initRandomSeed();
  // set framerate to 60 frames per second
  arduboy.setFrameRate(60);
  // clear the screen just in case
  arduboy.clear();
}
```

ja aloitetaan myös loop function kirjoittaminen

```c
void loop()
{
  // wait untill next frame
   if (!arduboy.nextFrame())
    return;
  // clear the screen
  arduboy.clear();

  // everything we need to do goes here

  // flip the buffer to show what we draw
  arduboy.display();
}
```

Jos koodi nyt käännetään ja ladataan arduboylle niin käynnistyksen jälkeen (arduboy logon jälkeen) näkyy vain musta ruutu ja pääsemme aloittamaan asioiden lisäämisen ruudulle.

# Pelikenttä

Pelikenttä koostuu pistelaskureista ja keskiviivasta. Lisäksi kentän ylä ja alareuna kimmottavat palloa.

Piirretään tuo keskiviiva ensimmäisenä. Tuo katkoviiva on keskellä ruutua. Lisätään uusi funktio tuon viivan piirtämiselle niin saadan pidettyä koodi vähän paremmin järjestyksessä.

```c
void draw()
{
  // spacing for the line
  // half will be white half black
  uint16_t spacing = 6;
  for (int i = 0; i < HEIGHT; i += spacing)
  {
    arduboy.drawLine(
        WIDTH / 2, i,              // start coordinate
        WIDTH / 2, i + spacing / 2 // end coordinate
    );
  }
}

void loop() {
  ...
  // everything we need to do goes here
  // call draw to draw the playfield
  draw();
  ...
}
```

```draw``` functiossa ensimmäisenä määritellään apumuuttuja ```spacing``` jota käytetään loopissa jotta saadaan helposti piirrettyä katkoviiva. Apumuuttujaa käytetään for loopissa joka alkaa 0:sta ja jatkuu aina ```HEIGHT```muuttujaan asti [HEIGHT](https://mlxxxp.github.io/documents/Arduino/libraries/Arduboy2/Doxygen/html/Arduboy2Core_8h.html#aed89bd71aee8be823e8a20ec4e093c1e) on arduboy libraryn määrittämä tieto joka kertoo näytön korkeuden. Tuo HEIGHT tieto käytännössä muuttuu arvoksi 64 kun ohjelma käännetään. Jotta saadaan aikaseksi katkoviiva niin tuota loopin i arvoa ei kasvatetakkaan aina yhdellä vaan kasvatetaan sitä tuolla määritetyllä spacing arvolla.

Loopin sisällä käytetään arduboyn [drawline](https://mlxxxp.github.io/documents/Arduino/libraries/Arduboy2/Doxygen/html/classArduboy2Base.html#a79d9d17254856098f4a38726e64b53ba) funktiota jolle kerrotaan viivan alkupiste ja loppupiste. Alkupisteen X koordinaatti lasketaan ```WIDTH / 2 => 128/2 => 64```ja y koordinaatti on loopin i arvo, eli ensimmäisellä kierroksella i=0 toisella i=6 kolmannella i=12 jne kunnes i on suurempi kuin HEIGHT. Loppupisteen x koordinaatti on sama kuin alkupisteen mutta y koordinaatti on spacing arvo jaettuna kahdella.

Eli loopin sisällä piirretään useita viivoja joista ensimmäinen lähtee koordinaateista 64, 0 ja päättyy 64, 3 seuraava kulkee 64, 6 => 64, 9 jne kunnes koko ruudun korkeus on täytetty.

Nyt kun koodin kääntää ja lataa arduboylle on keskelle ilmestynyt viiva.

Hienoa, seuraavaksi voidaan lisätä pallo jotta peliä voidaan pelata :)

# Pallo

Palloa varten meidän tarvitsee tietää pallon sijainti jokaisella hetkellä, pallon koko ja myös pallon "suunta" eli minnepäin pallon pitäisi kulkea. Lisäksi pallon pitää pysyä pelikentällä ja jatkossa myös mailojen välissä.

Pallolle tarvitaan siis muutama muuttuja, lisätään ne ensimmäisenä tiedoston yläosaan niin sanotuiksi globaaleiksi muuttujiksi.

```c
...
Arduboy2 arduboy;

// ball initial position center of the screen
int16_t ballx = WIDTH / 2;
int16_t bally = HEIGHT / 2;
// ball velocity in x and y direction
int8_t vx = 1;
int8_t vy = 1;
// ball radius
const int8_t ballr = 2;
...
```

ballx ja bally kertovat pallon sijainnin ruudulla, alustetaan pallo olemaan keskellä ruutua laskemalla WIDTH/2 ja HEIGHT/2.

vx ja vy kertovat pallon suunnan, alustetaan ne olemaan 1 ja 1 joka tarkoittaa sitä että pallo olisi liikkeessä oikealle ja alas.

lisäksi pallon halkaisija valitaan olemaan 2.

## Pallon piirtäminen

Pallon piirtämistä varten lisätään ```draw``` metodiin pallon piirtämiselle oma kohta pelikentän piirtämisen jälkeen.

```c
  // draw the ball
  arduboy.fillCircle(ballx, bally, ballr);
```

[fillCircle](https://mlxxxp.github.io/documents/Arduino/libraries/Arduboy2/Doxygen/html/classArduboy2Base.html#aacfa81b7a231fbced5e8419ae0e031b9) on taas arduboyn tarjoama funktio jolla voidaan piirtää ympyrä haluttuun kohtaan ruutua. Ensimmäinen parametri on ympyrän x position ja toinen y positio ja kolman on ympyrän säde.

## Pallo liikkeelle

## Pidetään pallo pelikentällä

# Mailat

# Pisteet ja voittaminen

# Äänet