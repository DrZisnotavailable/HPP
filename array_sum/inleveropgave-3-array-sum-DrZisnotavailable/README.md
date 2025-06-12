[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/OXrObDV6)
# Array Sum

Om te bepalen of je voldoende van OpenMP hebt begrepen, gaan we een 'eenvoudig' probleem proberen te parallelliseren. Het probleem is het volgende: tel alle waardes in een array bij elkaar op. Op zich niet lastig, maar aangezien we het over meerdere threads (cores) willen verdelen, komt er wat bij kijken. Om het probleem wat interessanter te maken, gaan we werken met grote arrays.

## Opgave

De file [arraySum.cc](arraySum.cc) is gegeven waarin een sequentiële variant van het programma is geïmplementeerd om waardes in een array bij elkaar te tellen. Het programma [gen_array.py](gen_array.py) is gegeven om zelf (grote) arrays van nummers te kunnen maken; deze gaan we gebruiken om de volgende files te maken (instructies voor gebruik volgen als je `./gen_array.py` draait in je shell):

- 10k.txt met 10.000 items;
- 100k.txt met 100.000 items;
- 1m.txt met 1.000.000 items; en
- 10m.txt met 10.000.000 items.

Compile `arraySum.cc`, en draai met elk van je gegenereerde array-files, en noteer hoe lang het duurt om deze uit te voeren. Gebruik de OpenMP timing functies om de duur te berekenen:

```cpp
double start = omp_get_wtime();
  // ... [hier draait jouw code] ...
double end = omp_get_wtime();
cout << "Elasped time = " << (end - start) << " sec" << endl;
```

Pas het programma aan door het toevoegen van OpenMP directives om een parallelle variant te maken (ompArraySum.cpp); gebruik hiervoor een combinatie van de Master-Worker, Parallel For-loop en Reduction patronen.

Het basis idee is dat de master-thread de array leest, en daarna verdeelt over de verschillende beschikbare workers. Elk van de worker processen zal het totaal van zijn toebedeelde stukje moeten berekenen. Door gebruik van Reduction dienen die stukjes dan weer bij elkaar gebracht te worden tot het eind resultaat.

- Maak een overzicht (spreadsheet) met de data van tests op de files 10k.txt, 100k.txt, 1m.txt, 10m.txt waarbij gevarieerd wordt in de hoeveelheid beschikbare threads (1, 2, 4, 8). Als je wilt mag je een mooie grafiek maken om de performance winst per hoeveelheid threads duidelijk te maken, maar dat hoeft niet.


## Inleveren HPP Opdrachten
Voor de opdrachten van High Performance Programming lever je een verslag in, in PDF formaat.

Begin het verslag met:

- De titel van de opdracht;
- Je naam en studentnummer;
- Een link naar je GitHub Classroom repository met je werk.

- Lees de hele opdracht goed door, stel alvast vragen als iets niet duidelijk is
- Voor ieder deel / vraag:
  -  Vermeld het nummer van het deel of de vraag;
  -  Maak de opdracht en/of beantwoord de vraag;
  -  Kies code snippets en/of screenshots om je oplossing te laten zien;
  -  Beschrijf je oplossing beknopt, waarbij je vooral duidelijk maakt hoe je het hebt aangepakt.

Bewaar / exporteer je verslag als PDF en lever die in.
