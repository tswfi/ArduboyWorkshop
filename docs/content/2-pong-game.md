---
title: Pong
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
```cpp
#include <Arduboy2.h>
Arduboy2 arduboy;
```

Ja muokataan setup functiota alustamaan arduboy ja asetetaan muutama tärkeä optio.

```cpp
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

```cpp
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

```cpp
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

`draw` functiossa ensimmäisenä määritellään apumuuttuja `spacing` jota käytetään loopissa jotta saadaan helposti piirrettyä katkoviiva. Apumuuttujaa käytetään for loopissa joka alkaa 0:sta ja jatkuu aina `HEIGHT`muuttujaan asti [HEIGHT](https://mlxxxp.github.io/documents/Arduino/libraries/Arduboy2/Doxygen/html/Arduboy2Core_8h.html#aed89bd71aee8be823e8a20ec4e093c1e) on arduboy libraryn määrittämä tieto joka kertoo näytön korkeuden. Tuo HEIGHT tieto käytännössä muuttuu arvoksi 64 kun ohjelma käännetään. Jotta saadaan aikaseksi katkoviiva niin tuota loopin i arvoa ei kasvatetakkaan aina yhdellä vaan kasvatetaan sitä tuolla määritetyllä spacing arvolla.

Loopin sisällä käytetään arduboyn [drawline](https://mlxxxp.github.io/documents/Arduino/libraries/Arduboy2/Doxygen/html/classArduboy2Base.html#a79d9d17254856098f4a38726e64b53ba) funktiota jolle kerrotaan viivan alkupiste ja loppupiste. Alkupisteen X koordinaatti lasketaan `WIDTH / 2 => 128/2 => 64` ja y koordinaatti on loopin i arvo, eli ensimmäisellä kierroksella i=0 toisella i=6 kolmannella i=12 jne kunnes i on suurempi kuin HEIGHT. Loppupisteen x koordinaatti on sama kuin alkupisteen mutta y koordinaatti on spacing arvo jaettuna kahdella.

Eli loopin sisällä piirretään useita viivoja joista ensimmäinen lähtee koordinaateista 64, 0 ja päättyy 64, 3 seuraava kulkee 64, 6 => 64, 9 jne kunnes koko ruudun korkeus on täytetty.

Nyt kun koodin kääntää ja lataa arduboylle on keskelle ilmestynyt viiva.

Hienoa, seuraavaksi voidaan lisätä pallo jotta peliä voidaan pelata :)

# Pallo

Palloa varten meidän tarvitsee tietää pallon sijainti jokaisella hetkellä, pallon koko ja myös pallon "suunta" eli minnepäin pallon pitäisi kulkea. Lisäksi pallon pitää pysyä pelikentällä ja jatkossa myös mailojen välissä.

Pallolle tarvitaan siis muutama muuttuja, lisätään ne ensimmäisenä tiedoston yläosaan niin sanotuiksi globaaleiksi muuttujiksi.

```cpp
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

Pallon piirtämistä varten lisätään `draw` metodiin pallon piirtämiselle oma kohta pelikentän piirtämisen jälkeen.

```cpp
  // draw the ball
  arduboy.fillCircle(ballx, bally, ballr);
```

[fillCircle](https://mlxxxp.github.io/documents/Arduino/libraries/Arduboy2/Doxygen/html/classArduboy2Base.html#aacfa81b7a231fbced5e8419ae0e031b9) on taas arduboyn tarjoama funktio jolla voidaan piirtää ympyrä haluttuun kohtaan ruutua. Ensimmäinen parametri on ympyrän x positio ja toinen y positio ja kolmas on ympyrän säde.

## Pallo liikkeelle

Jotta pelissä olisi jotain ideaa pallo pitää saada liikkumaan. Meillä on jo valmiina olemassa vx ja vy mutta niitä pitää käyttää jotta pallo liikkuu. Lisätään pallon käsittelylle uusi funktio joka hoitaa pallon liikkumista.

```cpp
void handleBall()
{
  // move the ball
  ballx += vx;
  bally += vy;
}
```

Jokaisella funktion kutsulla lisätään pallon x ja y positioihin vx ja vy arvo.

Lisäksi meidän pitää tietysti myös kutsua tätä funktiota, lisätään kutsu loop funktioon.

```cpp
...
  // everything we need to do goes here
  handleBall();
  draw();
...
```

Nyt jos lataamme ohjelman arduboylle mitä tapahtuu?

```spoiler
Pallo lähtee heti liikkeelle oikealle ja alas ja lentää ulos ruudulta.
```

## Pidetään pallo pelikentällä

Pallon liike pitää saada pidettyä rajojen sisällä ja huomioida törmäykset mailoihin.

Joka kerta kun pallon x koordinaatti menee ruudun leveydestä meidän pitää vaihtaa suuntaa ja samoin jos y koordinaatti menee yli ruudun ylä tai alareunan pitää vaihtaa y suuntaa.

Lisätään handleball functioon rajat tälle.

```cpp
  // keep the ball inside our playfield
  if (
      bally - ballr <= 0         // bounce from the top
      || bally + ballr >= HEIGHT // and from the bottom
  )
  {
    // reverse up down
    vy = -vy;
  }

  if (
      ballx - ballr <= 0        // bounce from left
      || ballx + ballr >= WIDTH // bounce from right
  )
  {
    // reverse left right
    vx = -vx;
  }
```

Nyt jos ohjelma ladataan arduboylle pallo pomppii ruudun sisällä jatkuvasti kuin dvd screensaver.

# Mailat

Jotta pelissä olisi jotain ohjattavaa tarvitaan pelaajille mailat. Teemme pelistä kaksinpelin jossa ylös alas napeilla ohjataan vasemman puolen mailaa ja a ja b napeilla ohjataan oikean puolen mailaa.

Samoin kuin pallolle mailoille tarvitaan myös muutamia muuttujia, lisätään ne taas alkuun.

```cpp
// player1 paddle
int16_t p1width = 3;
int16_t p1height = 10;
int16_t p1y = HEIGHT / 2 - p1height / 2;
const int16_t p1x = 0;

// player2 paddle
int16_t p2width = 3;
int16_t p2height = 10;
int16_t p2y = HEIGHT / 2 - p2height / 2;
const int16_t p2x = WIDTH - p2width;
```

Ja lisätään niille piirto `draw` funktioon

```cpp
  // draw both paddles
  arduboy.fillRect(p1x, p1y, p1width, p1height);
  arduboy.fillRect(p2x, p2y, p2width, p2height);
```

[fillRect](https://mlxxxp.github.io/documents/Arduino/libraries/Arduboy2/Doxygen/html/classArduboy2Base.html#a69f8864af3b6fd9179a021da8ff0dc90) piirtää neliön x,y ja x2, y2 koordinaattien väliin.

Lisäksi tarvitaan tapa liikuttaa mailoja. Tämä on hyvä lisätä taas uuteen metodiin jotta koodi pysyy helpompana lukea.

```cpp
void handlePaddles()
{
  // player1 paddle
  if (arduboy.pressed(UP_BUTTON))
  {
    if (p1y > 0)
    {
      p1y--;
    }
  }
  if (arduboy.pressed(DOWN_BUTTON))
  {
    if (p1y + p1height < HEIGHT)
    {
      p1y++;
    }
  }
  // player 2 paddle
  if (arduboy.pressed(B_BUTTON))
  {
    if (p2y > 0)
    {
      p2y--;
    }
  }
  // player 2 paddle
  if (arduboy.pressed(A_BUTTON))
  {
    if (p2y + p2height < HEIGHT)
    {
      p2y++;
    }
  }
}
```

[Pressed](https://mlxxxp.github.io/documents/Arduino/libraries/Arduboy2/Doxygen/html/classArduboy2Base.html#a1a294744f7886588b6864f1df75a91ba) funktio tarkistaa onko nappi pohjassa.

Nyt kun ohjelma ladataan arduboylle voi mailoja liikuttaa nuolinäppäimien ylös alas ja a b näppäimien avulla. Palloon nämä eivät vaikuta vielä mitenkään lisätään seuraavaksi käsittely sille.

`handleBall` funktio on sopiva paikka hoitaa mailoihin osumisen käsittely, muutetaan hieman sen koodia. Ensin poistamme kokonaan koodin joka kimmottaa pallon oikeasta ja vasemmasta reunasta ja lisäämme sen tilalle uutta koodia joka tarkistaa että pallo on osunut mailaan.

```cpp
  // check if we hit either paddle
  // player1 paddle
  if (ballx - ballr == p1x + p1width    // ball is at the paddle surface
      && bally + ballr > p1y            // and between the paddle top position
      && bally - ballr < p1y + p1height // and between the paddle bottom position
  )
  {
    vx = -vx;
  }
  // and player2 paddle
  if (ballx + ballr == p2x              // ball is at the paddle surface
      && bally + ballr > p2y            // and between the paddle position
      && bally - ballr < p2y + p2height // and between the paddle position
  )
  {
    vx = -vx;
  }
```

Joudumme hieman laskemaan pallon ja mailan positioita ja tarkistamaan pari asiaa.

1. Pallo on sivusuunnassa mailan kohdalla
2. Pallo on pystysuunnassa mailan kohdalla

Jos nämä säännöt ovat totta kimmotetaan pallo takaisin päin.

Nyt pallo jatkaisi taas matkaansa sivujen yli ikuisesti joten lisätään vielä tarkistus reunan ylittämiselle ja palautetaan pallo takaisin keskelle jos reuna on ylitetty.

```cpp
  // if ball is over the left or right side resetball
  // over right side
  if (ballx + ballr < 0)
  {
    resetBall();
  }
  // over left side
  if (ballx - ballr > WIDTH)
  {
    resetBall();
  }
```

mikä ihmeen `resetBall`? se on taas uusi funktio jonka lisäämme myös

```cpp
void resetBall()
{
  // move the ball to the center of the screen
  ballx = WIDTH / 2;
  bally = HEIGHT / 2;

  // randomize the ball starting direction
  if (random(0, 2))
  {
    vx = -vx;
  }
  if (random(0, 2))
  {
    vy = -vy;
  }
}
```

`resetBall` palauttaa pallon takaisin keskelle ja jotta peli olisi edes jotenkin mielenkiintoinen vaihdetaan pallon suuntaa randomilla. Arduinon random funktio palauttaa arvotun luvun minimi ja maksimi arvojen välistä (poislukien maksimi) eli random(0,2) palauttaa joko 0 tai 1 ja jos se palauttaa yksi vaihdetaan suuntaa.

Nyt kun ohjelman lataa arduboylle niin sillä voi jo pelata. Palloa voi pomputtaa mailojen välissä ja pallo palautuu keskelle kun se menee yli.

Jotain kuitenkin vielä puuttuu jotta peli olisi mielenkiintoinen.

# Pisteet ja voittaminen

Pisteiden lasku ja voiton tarkistaminen ovat peleissä hyvin tärkeitä osia.

Lisätään taas pari muuttujaa koodin yläreunaan joilla voimme pitää kirjaa pisteistä.

```cpp
// score
uint16_t p1score = 0;
uint16_t p2score = 0;
// maxpoints
const uint16_t maxpoints = 5;
```

Pisteiden kasvattaminen on helppoa, mehän jo tarkistamme että menikö pallo reunan yli ja jos meni niin kasvatetaan vain oikean pelaajan pisteitä. Eli muokataan handleBall funktion reunan yli tarkastusta hieman

```cpp
  // if ball is over the left or right side resetball
  // and increase points
  // over right side
  if (ballx + ballr < 0)
  {
    p2score++;
    resetBall();
  }
  if (ballx - ballr > WIDTH)
  {
    p1score++;
    resetBall();
  }
```

Tietysti pistetilanne olisi hyvä näyttää jossain. Lisätään draw funktioon pisteiden näyttäminen:

```cpp
  // print both scores
  // textsize 2 is 12x16 px
  arduboy.setTextSize(2);

  // player1 score on the left side
  arduboy.setCursor(20, 5);
  arduboy.print(p1score);

  // player2 score on the right side
  arduboy.setCursor(WIDTH - 20 - 12, 5);
  arduboy.print(p2score);
```

Ja meidän pitää myös tarkistaa voitto tilanne. Lisätään tälle tarkistus pää looppiimme.

```cpp
  // check if either side won
  if (p1score >= maxpoints || p2score >= maxpoints)
  {
    // TODO: do something reasonable
    p1score = 0;
    p2score = 0;
  }
  handlePaddles();
  handleBall();
  draw();
```

Tämäkään ei ole vielä kovin hyvä, tarvitsemme selkästi "voitit" ruudun ja tavan aloittaa uusi peli ja aloituskin voisi tapahtua niin että pelaajat voivat valmistautua pelin alkuun rauhassa.

# Pelin eri tilat (finite state machine)

[Tilakone (finite state machine)](https://en.wikipedia.org/wiki/Finite-state_machine) on erittäin hyödyllinen tapa pitää koneella eri tiloja joista siirrytään toisiin tiloihin.

<center>
{% include figure.html img="state_machine.png" alt="" caption="" width="100%" %}
</center>

Pelillämme on kolme eri tilaa. Aloitusruutu, Peli tilanne ja Voittoruutu.

Lisätään vähän apumuuttujia tämän tilan käsittelyyn.

```cpp
// gamestate
const int GAME_TITLE = 0;
const int GAME_PLAY = 1;
const int GAME_WON = 2;
uint8_t gamestate = GAME_TITLE;
```

Kuvan mukaisesti aloitetaan tilanne siitä että olemme title screenissä jossa odotetaan että peli voi alkaa.

Muutetaan myös looppiamme ottamaan huomioon nämä tilat.

```cpp
  // clear the screen
  arduboy.clear();

  // everything we need to do goes here
  // always draw the playfield ball and paddles
  draw();

  // dependig on the state we will do different things
  switch (gamestate)
  {
  // title screen, waits for input and moves to GAME_PLAY
  case GAME_TITLE:
    titleScreen();
    break;
  // game is playing untill either side wins, moves to GAME_WON
  case GAME_PLAY:
    handlePaddles();
    handleBall();
    if (p1score >= maxpoints || p2score >= maxpoints)
    {
      // go to win state
      gamestate = GAME_WON;
    }
    break;
  // game won by a player
  case GAME_WON:
    winScreen();
    break;
  }

  // flip the buffer to show what we draw
  arduboy.display();
```

Tässä tulikin paljon muutoksia mutta ei hätää käydään homma läpi rivi riviltä.

clear kutsun jälkeen kutsumme yhä `draw` funktiotamme jotta saamme aina piirrettyä pelikentän ja pistelaskurit jne.

Switch lausekkeella tutkimme mikä pelin tila on ja jos olemme title ruudussa kutsutaan uutta funktiota `titleScreen` (jonka teemme kohta).

Jos olemme peli tilanteessa kutsumme taas normaalisti handle toimintojamme kunnes jompikumpi pelaajista nousee pisteissä yli maksimin jolloin siirrymme voitto tilaan.

Voitto tilassa kutsutaan `winScreen` funktiota (jonka toteutamme ihan kohta).

## titleScreen

Lisätään uusi funktio hoitamaan titleScreenin tehtäviä.

```cpp
void titleScreen()
{
  if (
      arduboy.pressed(UP_BUTTON) || arduboy.pressed(DOWN_BUTTON) || arduboy.pressed(A_BUTTON) ||
      arduboy.pressed(B_BUTTON))
  {
    resetBall();
    gamestate = GAME_PLAY;
  }
}
```

Yksinkertaisesti vain odotetaan että jotain peli napeista painetaan jonka jälkeen voidaan aloittaa uusi peli. Kutsutaan myös `resetBall` jotta saadaan randomoitua pallon suunta heti alusta.

## winScreen

Samoin lisätään voitto tilanteelle oma funktio hoitamaan sen tehtäviä.

```cpp
void winScreen()
{
  // textsize 2 is 12x16 px
  arduboy.setTextSize(2);

  // print who won about center of the screen
  arduboy.setCursor(34, 23);
  if (p1score > p2score)
  {
    arduboy.print("P1");
  }
  else
  {
    arduboy.print("P2");
  }
  arduboy.print(" won");

  // display right away
  arduboy.display();
  // and sleep for a moment so that there is time to react and not start a new game immediately
  delay(500);

  if (
      arduboy.pressed(UP_BUTTON) || arduboy.pressed(DOWN_BUTTON) || arduboy.pressed(A_BUTTON) ||
      arduboy.pressed(B_BUTTON))
  {
    // start a new game
    resetBall();
    p1score = 0;
    p2score = 0;
    gamestate = GAME_PLAY;
  }
}
```

Hienoa nyt pelissä on selkeä alku ja loppu.

Käännä taas koodi ja lataa ohjelma arduboylle. Nyt peliä on hyvä testailla ja kokeilla että kaikki toimii kuten on ajateltu.

# Äänet

Koska arduboyssa on myös pieni kaiutin niin lisätään vielä ääniefekti

Äänet on jaettu omaan libraryynsä joten meidän pitää ensin lisätä se tarvittuihin libraryihin ja alustaa ääni samaan tyyliin kuin arduboy library.

platformio.ini tiedostoon lisätään lib_deps kohtaan `ArduboyTones`
```ini
lib_deps =
  Arduboy2
  ArduboyTones
```

Ja main.cpp tiedostoon lisätään

```cpp
...
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);
...
```

Tämän jälkeen voidaan kutsua [sound](https://github.com/MLXXXp/ArduboyTones){:target="_blank"} funktiota äänien tuottamiseksi.

Lisätään maila osuman tarkistukseen

```cpp
sound.tone(NOTE_A4, 50);
```

# Valmista

Hienoa pääsit tänne asti! Toivottavasti opit jotain ja pääset nyt aloittamaan vaikka oman pelin kirjoittamisen. Hyviä pelejä joita ekana kokeilla ovat breakout ja side scroller tyyppiset pelit.

Täältä voit vielä lukea koko [pong lähdekoodin](2.1-pong-source).

