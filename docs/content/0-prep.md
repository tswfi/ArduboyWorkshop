---
title: Perusteet
nav: true
---

# Mikä ihmeen arduboy?

[Arduboy](https://arduboy.com/){:target="_blank"} on [Arduino](https://www.arduino.cc/){:target="_blank"} pohjainen pelialusta. Sen sydämenä pyörii [Atmega 32u4](https://www.microchip.com/wwwproducts/en/ATmega32u4){:target="_blank"} prosessori.

Ohjelmointi kielenä toimii [c++](https://en.wikipedia.org/wiki/C%2B%2B){:target="_blank"} kieli.

Iso osa Arduboyn hienoudesta on kirjasto joka helpottaa pelien kirjoittamista huomattavasti. Tämän kirjaston dokumentaatio löytyy [täältä](https://mlxxxp.github.io/documents/Arduino/libraries/Arduboy2/Doxygen/html/index.html){:target="_blank"} ja kirjaston lähdekoodi löytyy [githubista](https://github.com/MLXXXp/Arduboy2){:target="_blank"} kuten tämä [sivustokin](https://github.com/tswfi/ArduboyWorkshop){:target="_blank"}.

# Tarvittavat ohjelmistot

Arduboyta kuten muitakin Arduino pohjaisia laitteistoja voi koodata monilla erityökaluilla. [Arduino IDE](https://www.arduino.cc/en/main/software){:target="_blank"} oli alkuperäinen ja nykyään on myös [arduino web editori](https://create.arduino.cc/){:target="_blank"}.

Tässä workshopissa käytetään kuitenkin toisia ohjelmia. Näiden etuna on se että ne toimivat myös muiden kielien ja järjestelmien kanssa jatkossa.

## VSCode

[VSCode](https://code.visualstudio.com/){:target="_blank"} Yksi parhaista koodi editoreista tällä hetkelllä
[PlatformIO](https://platformio.org/){:target="_blank"} Alusta monien eri sulautettujen laitteistojen ohjelmointiin.

Ensin asennetaan VSCode. Valitse alla olevasta linkistä oma käyttöjärjestelmäsi ja lataa ja asenna paketti:

[https://code.visualstudio.com/download](https://code.visualstudio.com/download){:target="_blank"}

## PlatformIO

Kun vscode on asennettu voidaan käyttää VSCodea asentamaan PlatformIO laajennos. Valitse oikeasta reunasta "extensions" ja kirjoita hakuun PlatformIO ja paina "asenna" nappia.

Tässä kestää hetken, sillä välin kun järjestelmä asentuu niin otetaan pikaoppia :)

# C++ pika oppi

Hyvin yksinkertaistettu pikaoppi asioista joita tarvitaan tämän workshopin aikana.

Internetissä on useita [c++ pika ohjeita](https://www.tutorialspoint.com/cplusplus/cpp_quick_guide.htm) joista voi lueskella lisää.

## Muuttujat

Muuttuja on arvo jota voidaan muuttaa ohjelman aikana. Esim:

```c
int x = 10;
x = x + 5;
print(x); // tulostaa 15
```

Muuttujia on eri tyyppisiä:

| tyyppi | kuvaus |
| int | kokonaisluku |
| float | desimaaliluku |
| char | yksi "tavu" |
| bool | totuusarvo (true tai false) |

jne.

jos muuttujan tyypille sanoo alkuun "const" on tuo muuttuja sellainen jota ei voi enää muuttaa (tämä helpottaa huomaamaan virheitä).

## Operaatiot

Normaalit matematiikan operaatiot toimivat myös ohjelmointikielissä.

Seuraavissa esimerkeissä A = 10 ja B = 20

| operaatio | kuvaus | esimerkki |
| + | summa | A+B tulos on 30 |
| - | vähentäminen | A - B tulos on -10 |
| * | kertolasku | A * B tulos on 200 |
| / | jakolasku | A / B tulos on 2 |
| % | modulo eli jakojäännös | A % B tulos on 0 |
| ++ | plus yksi | A++ tulos on 11 |
| -- | miinus yksi | A-- tulos on 9 |

Lisäksi koodia voi yksinkertaistaa useissa tapauksissa käyttämällä "asetusoperaatioita"

```c
x = x +10
```

voidaan kirjoittaa myös

```c
x += 10
```

## Vertailulausekkeet

palautttavat "totuuksia".

| operaatio | kuvaus | esimerkki |
| == | on yhtäsuurikuin | A == B tulos on false (epätosi) |
| != | on erisuurikuin | A != B tulos on true |
| > | on suurempikuin | A > B tulos on false |
| < | on pienempikuin | A < B tulos on tosi |
| >= | suurempi tai yhtäsuurikuin | A >= B tulos on false |
| <= | pienempi tai yhtäsuurikuin | A <= B tulos on true |

## Loogiset operaatiot

Yhdistävät totuusarvoja. Jos A = true ja B = false

| operaatio | kuvaus | esimerkki |
| && | Looginen JA. Jos kumpikin on tosi niin tulos on tosi | A && B tulos on false |
| || | Looginen TAI. Jos jompikumpi on tosi niin tulos on tosi | A || B tulos on true | 
| ! | Looginen EI. Kääntää tuloksen toisinpäin | !B tulos on tosi |

## Ehtolauseke

Ehtolausekkeella voidaan tehdä eri asioita riippuen siitä mikä esim muuttujan arvo on:

```c
int x = 5;
if(x > 3) {
    print("x on suurempi kuin viisi");
} else {
    print("x on pienempi kuin viisi");
}
```

Toinen ehtolauseke rakenne on switch rakenne.

```c
int x = 2;
switch (x)
{
case 0:
  print("x on nolla");
  break;
case 1:
  print("x on yksi");
  break;
case 2:
  print("x on kaksi");
  break;
default:
  break;
}
```

Palauttaa "x on kaksi"


## Funktiot

Funktio on pätkä koodia jota voidaan kutsua monta kertaa ja se voi tehdä erilaisia asioita ja palauttaa tietoja seuraavien käsiteltäväksi:

```c
int add(int a, int b) {
    return a + b;
}
print(add(1,2));
```

Ensin määriteltiin funktio nimeltään "add" joka ottaa kaksi kokonaisluku muuttujaa ja joka palauttaa näiden summan.

Viimeisellä rivillä ensin kutsutaan add funtiota arvoilla 1 ja 2 jolloin ohjelman suoritus siirtyy riville 2 jossa lasketaan summa ja palautetaan tieto return arvolla. Tämän jälkeen print tulostaa tiedon ruudulle.

## Loopit

Loopit ovat todella hyödyllisiä rakenteita joilla voidaan toistaa koodia useita kertoja.

```c
for(int x = 0; x < 10; x++) {
    print("Lähtölaskenta: ");
    print(x);
}
```

Lisäksi on olemassa myös while loop ja do..while loop. Loopista voi myös poistua kesken kaiken break lausekkeella tai hypätä seuraavalle kierrokselle continue lausekkeella.

```c
for(int x = 0; x < 10; x++) {
    if(x % 2 == 0) {
        continue;
    }
    if(x > 5) {
        break;
    }
    print(x);
}
```

Looppi aloittaa x arvolla 0 ja tarkistaa jakojäännöksen ja jos jakojäännös on 0 hypätään seuraavalle kierrokselle. Jos jakojäännös on 1 jatketaan tarkistamaan onko x suurempi kuin viisi. Lopuksi tulostetaan x arvo ja aloitetaan seuraava kierros.

Mitkä luvut ohjelma tulostaa?

<div class="spoiler">
1
3
5
</div>


## Taulukot



# Lisäksi

C++ kielessä on paljon asioita jotka on tässä jätetty käsittelemättä. Muistinkäsittely ja viittaukset ja bitwise operaatiot on jätetty kokonaan pois yksinkertaistamisen vuoksi.