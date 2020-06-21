# ArduinoSimPedaly
Připojení 3 pedálů s použitím loadcellů a 3 modulů HX711

Pozor - pro úspěšné dokončení tohoto projektu je nutné znát základy arduina a celkově programování (po pár tutoriálech na netu podle mně zvládne každý). Zapojit se to dá i pomocí propojovacích kablíků pro nepájivá pole a menší svorkovnice, ale lepší je to připájet alespoň na univerzální plošný spoj - takže pájka, cín a nějaká zkušenost s nimi se taky hodí

V souborech je i můj návrh na plošný spoj tvořený v kicadu (Pedaly.zip) a soubor Pedaly.kicad_pcb, který stačí poslat například sem https://www.plosnaky.cz/ a nechat si ho vyrobit.

V aktuální verzi softwaru mám zakomentován spojkový pedál, protože ho zatím nemám hotový.

Použité knihovny:
HX711 multi https://github.com/compugician/HX711-multi
Arduino Joystick 2.0 https://github.com/MHeironimus/ArduinoJoystickLibrary

# Seznam materiálu:
Arduino Micro / Arduino Leonardo (např. https://www.gme.cz/100-kompatibilni-klon-arduino-micro-atmega32u4-5v)
(pro můj návrh plošného spoje je potřeba konkrétně typ Micro!)

HX711 modul (např. https://www.laskarduino.cz/ad-prevodnik-modul-24-bit-2-kanaly-hx711/)

3x loadcell

V případě osazení na plošný spoj navíc:

Svorkovnice (např. 4x https://www.gme.cz/svorkovnice-ptr-akz700-3-5-08-v-green)

Případně dutinkové lišty - arduino a HX711 moduly se pak nepájí přímo na PCB, ale nacvaknou se do těchto lišt (tohle řešení silně doporučuji!) https://www.gme.cz/dutinkova-lista-bl04g 4x, https://www.gme.cz/dutinkova-lista-bl806g 4x, https://www.gme.cz/dutinkova-lista-bl15g 2x (arduino nemá využity všechny piny, proto stačí 15 pinová dutinková lišta, nebo i kratší


# Zapojení:

Viz Schema.png. Na HX711 modulech je potřeba udělat úpravu pro běh na 80Hz (v základu běží na 10 Hz) - viz hx711_uprava.jpg. Svorkovnice označená jako příprava pro ručku je myšlena pro připojení případného čtvrtého HX711 modulu, umístěného v ruční brzdě - zatím jenom příprava do budoucna. Ve výsledku nejspíš ručku zapojím na separátní arduino třeba s button boxem.


# Program:
Soubor loadcellyHX711.ino obsahuje můj aktuální sketch, který použvám. V podstatě jde jenom o přečtení hodnot ze všech HX711 do pole results[], přepočet těchto hodnot do rozsahu pro knihovnu joystick (v mém sketchi je rozsah špatně, přícházím o přesnost. Správně by na výstup mělo jít číslo mezi -32767 a 32767).

Soubor Pedaly.ino obsahuje novější verzi, kde si arduino samo přepočítá hodnoty vyčtené z HX711 modulů do rozsahu -32767 / 32767. Při prvním spuštění je potřeba provést "kalibraci" - program přečte EEPROM pamět arduina, kde jsou uloženy minimální a maximální hodnoty všech pedálů. Pokud tam tyto hodnoty nejsou (po prvním nahrání tohoto sketche), spustí kalibraci - do konzole vypíše "Start kalibrace" (v tu chvíli je potřeba nechat pedály v základní poloze), dále "Minimální hodnoty: Pedál 1: x, pedál 2: y, pedál 3:z", kde x, y a z jsou hodnoty přečtené z HX711 modulů. Následně se do konzole vypíše "Sešlápni opakovaně všechny 3 pedály na doraz do 10 sekund (brzdu dle libosti)". V tu chvíli je potřeba během následujících 10 sekund sešlápnout všechny 3 pedály, klidně opakovaně. Program uloží maximální hodnoty všech pedálů a použije je pro přepočet hodnot pro výstup na jednotlivé osy (to je ten zmiňovaný rozsah -32767 / 32767). Hodnoty jsou uloženy do paměti EEPROM arduina, zůstanou tedy zachované i po restartu / vypnutí napájení. V některých případech i při přehrání softwaru arduina, tam záleží jak je nastaven bootloader. 

"Kalibraci" pedálů lze znovu provést otevřením konzole v arduino IDE (nástroje -> Seriový monitor) a odesláním znaku "k" (stiskem tlačítka "pošli".

Konečnou kalibraci pedálů ve windows (deadzony atd.) lze provést programem DIview stejně jako u Heusinkveld pedálů - viz návod https://www.youtube.com/watch?v=b3Ut4R1O-Sw. 
