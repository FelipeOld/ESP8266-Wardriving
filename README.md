# US
# ESP8266 Wardriving
A compilation of scripts and resources for wardriving on the [ESP8266 WiFi microcontroller](https://www.espressif.com/en/products/socs/esp8266), including data visualization tools in Jupyter Notebook.

Documentation on this page is still in development.

## Components
Any ESP8266-based board should work with the basic required components, but the D1 mini form factor is highly recommended since using modules (such as for SD logging + battery management) can be done using plug-and-play hardware in a small footprint. All linked components are D1 mini compatible. 

**Required Components:**
| Component | Purpose |
| --- | --- |
| ESP8266 | Gather WiFi data & control hardware modules
| SD Reader | Store data that we can analyze with WiGLE / Python |
| GPS Module | Grab geolocation data + timestamp |

**Optional Components:**
| Component | Purpose | 
| --- | --- |
| 128x64 OLED | Get a visual display of WiFi data being captured |
| LiPo Battery | Power your ESP8266 for portable applications |
| Battery Module | Manage power for your ESP8266 w/ a LiPo battery |
| 100K Ω Resistor | Optional to read in battery level w/ the D1 Mini |
   
## Hardware Setup

**SD Reader Module** 
| SD Reader Pin | ESP8266 GPIO | D1 Mini Pin |
| --- | --- | --- |
| VCC | 3.3V | 3.3V |
| MISO | GPIO12 | D6 |
| MOSI | GPIO13 | D7 |
| SCK | GPIO14 | D5 |
| CS | GPIO15 | D8 |

If you are using SD card shield and intend to use OLED display in this stack I strongly recommend disconnect data pins D1 to D4 to connect OLED and GPS modules directly to Wemos D1 mini. This avoid strange behaviors like dont receive data from GPS or dont send any data to display and misconnect through SD card shield.

**GPS Module** 
| GPS Pin | ESP8266 GPIO | D1 Mini Pin |
| --- | --- | --- |
| VCC | 5V | 5V |
| TX | GPIO2 | D4 |
| RX | GPIO0 | D3 |

**OLED display** 
| GPS Pin | ESP8266 GPIO | D1 Mini Pin |
| --- | --- | --- |
| VCC | 3.3V | 3.3V |
| SCL | --- | D1 |
| SDA | --- | D2 |

## Data Visualization Scripts

Data Visualization Tutorial: https://www.youtube.com/watch?v=pFHUPs51CRQ

# PT-BR
# ESP8266 Wardriving
Um compilado de scripts e recursos para realizar a prática de wardriving com o [ESP8266 WiFi microcontroller](https://www.espressif.com/en/products/socs/esp8266), incluindo ferramentas de visualização de dados com in Jupyter Notebook.

A documentação desta página está em desenvolvimento.

## Componentes
Qualquer placa baseada no microcontrolador ESP8266 deve funcionar com os componentes básicos necessários. Entretanto a placa Wemos D1 mini é altamente recomendada em virtude do pequeno formato e possibilidade de empilhamento com os módulos/shields como SD Card e bateria. A listagem abaico contempla estes componentes.

**Componentes necessários:**
| Componente | Propósito |
| --- | --- |
| ESP8266 | Coletar dados WiFi e controlar os demais módulos
| SD Reader | Armazenar arquivo que poderá ser enviado à plataforma WiGLE ou com a biblioteca Python |
| GPS Module | Coletar geolocalização, data e hora |

**Componentes opcionais:**
| Componente | Propósito | 
| --- | --- |
| 128x64 OLED | Ter um feedback viausl dos dados coletados |
| Bateria LiPo | Alimentar o módulo ESP8266 e demais módulos através do shield |
| Módulo de bateria | Gerenciar energia para o ESP8266 quando usando uma bateria LiPo |
| Resistor 100K Ω | Opcional para realizar a leitura do nível de carga da bateria com o Wemos D1 mini |

Particularmente acho que um powerbank é mais simples de manter e com propósito mais genérico, além de, a depender do kit montado, poder facilmente ter mais capacidade de carga do que um kit de bateria LiPo.
  
## Configuração de Hardware

**Shield/Módulo SD card** 
| Pino do módulo SD | ESP8266 GPIO | Pino D1 Mini |
| --- | --- | --- |
| VCC | 3.3V | 3.3V |
| MISO | GPIO12 | D6 |
| MOSI | GPIO13 | D7 |
| SCK | GPIO14 | D5 |
| CS | GPIO15 | D8 |

Se você estiver usando um shield do cartão SD com o Wemos D1 mini e pretende usar o display OLED neste stack, recomendo fortemente desconectar os pinos de dados D1 a D4 para conectar os módulos OLED e GPS diretamente ao Wemos D1 mini. 

Isso evita comportamentos estranhos, como não receber dados do GPS ou não enviar nenhum dado para exibir no display OLED ou desconexões aleatórias através do shield do cartão SD.

**Módulo GPS** 
| Pino GPS | ESP8266 GPIO | Pino D1 Mini |
| --- | --- | --- |
| VCC | 5V | 5V |
| TX | GPIO2 | D4 |
| RX | GPIO0 | D3 |

**Display OLED** 
| Pino OLED | ESP8266 GPIO | Pino D1 Mini |
| --- | --- | --- |
| VCC | 3.3V | 3.3V |
| SCL | --- | D1 |
| SDA | --- | D2 |

## Scripts para visualização de dados

Tutorial: https://www.youtube.com/watch?v=pFHUPs51CRQ

wardriving_kit.jpg
