# Homeprompt ESP32 Firmware

Dit is de firmware voor de ESP32 sensor module

Op dit moment is het geconfigueerd voor verbinding met een InfluxDB database. In de toekomst kan dit omgewisseld worden voor de productiedatabase.

## Configuratie

Voor het compileren moet het bestand `config.h.example` hernoemd worden naar `config.h`

De waardes moeten aangepast worden, WiFi secrets en InfluxDB connectie in het bijzonder.
Als voorbeeld kan je de iotroam netwerk nemen die op school beschikbaar is nadat je ESP module aan de hand van de MAC adres aangemeld hebt op https://iotroam.nl

Zodra het project is opgezet en de Influx database is aangesloten zal de ESP elke verversingsperiode data sturen naar de database wat direct te visualizeren valt in een dashboard.

## Organisatie code

Ik heb voor deze project gekozen om object georienteerd te programmeren. Voor elke taak is er een aparte klasse, met zijn eigen verantwoordelijkheid en variabelen.

Elke taak heeft een functie `RunTask` welke periodiek uitgevoerd wordt. Op het moment wordt de timing nog geregeld door main.cpp maar het idee is dat hiervoor een task scheduler gebruikt kan worden. Er hoeft namelijk niet voor elke sensor refresh een wifi/MQTT/Database te werken.