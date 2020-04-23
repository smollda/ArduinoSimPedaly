# ArduinoSimPedaly
Připojení 3 pedálů s použitím loadcellů a 3 modulů HX711

Pozor - pro úspěšné dokončení tohoto projektu je nutné znát základy arduina a celkově programování (po pár tutoriálech na netu podle mně zvládne každý). Zapojit se to dá i pomocí propojovacích kablíků pro nepájivá pole a menší svorkovnice, ale lepší je to připájet alespoň na univerzální plošný spoj - takže pájka, cín a nějaká zkušenost s nimi se taky hodí

# Seznam materiálu:
Arduino Micro / Arduino Leonardo (např. https://www.gme.cz/100-kompatibilni-klon-arduino-micro-atmega32u4-5v)

HX711 modul (např. https://www.laskarduino.cz/ad-prevodnik-modul-24-bit-2-kanaly-hx711/)

3x loadcell

V případě osazení na plošný spoj navíc:

Svorkovnice (např. 4x https://www.gme.cz/svorkovnice-ptr-akz700-3-5-08-v-green)

Případně dutinkové lišty - arduino a HX711 moduly se pak nepájí přímo na PCB, ale nacvaknou se do těchto lišt https://www.gme.cz/dutinkova-lista-bl04g 4x, https://www.gme.cz/dutinkova-lista-bl806g 4x, https://www.gme.cz/dutinkova-lista-bl15g 2x (arduino nemá využity všechny piny, proto stačí 15 pinová dutinková lišta, nebo i kratší


# Zapojení:

Viz Schema.png. Na HX711 modulech je potřeba udělat úpravu pro běh na 80Hz (v základu běží na 10 Hz) - viz hx711_uprava.jpg.
