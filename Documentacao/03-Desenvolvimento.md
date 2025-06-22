
# Materiais

Os materiais utilizados no projeto foram:
- 2x ESP32.
- 2x Protoboard's.
- Módulo Sensor de Distância Ultrassônico (HC-SR04).
- Kit Chassi 2WD (2 rodas) Robô para Arduino.
- Botão 12x12 Vermelho
- Display de 7 segmentos (4 digitos).
- Módulo RTC.

# Desenvolvimento

Descreva aqui como foi o desenvolvimento do trabalho, destacando cada uma das etapas necessárias para chegar até a solução final.

## Desenvolvimento do Aplicativo

### Interface

O aplicativo foi desenvolvido no MIT App Inventor, utilizando uma interface simples e intuitiva. A tela principal contém:

  -> Um campo para digitar o horário do alarme, dividido em dois campos de texto (horas e minutos).

  -> Um botão central com o texto “CLIQUE PARA DEFINIR O HORÁRIO”, responsável por salvar o horário desejado para ativar o alarme.

  -> Um botão chamado “Conectar com ESP”, que inicia a conexão Bluetooth com o módulo ESP32 do carrinho.
   * Um rótulo de status na parte inferior da tela, que informa se o dispositivo está ou não conectado.

A interface foi construída com foco na facilidade de uso, permitindo que o usuário configure rapidamente o alarme e conecte o sistema, mesmo sem experiência prévia com tecnologia.


### Código

A lógica do aplicativo foi desenvolvida por meio de blocos de programação visual do App Inventor, que permitem programar comportamentos e eventos sem escrever código textual.

Os principais elementos da lógica do app incluem:

  -> Armazenamento do horário definido em variáveis.
  
  -> Uso de um temporizador (extensão utilizada) que verifica, a cada segundo, se o horário atual corresponde ao horário programado. 
  
  -> Quando o alarme deve ser disparado, o app envia um comando Bluetooth (ex: caractere '1') para o ESP32.
  
  -> A conexão Bluetooth é gerenciada por meio de blocos que tentam parear e comunicar-se com o dispositivo selecionado da lista de dispositivos próximos.

Condições extras foram adicionadas para evitar o envio repetido de comandos ou envio em horários incorretos (horários maiores ou menores que 24h, por exemplo).

A programação aproveita os recursos nativos do App Inventor para garantir que o app funcione de forma responsiva e confiável, mesmo com limitações do ambiente visual.

## Desenvolvimento do Hardware

### Montagem

Foram realizados encontros semanais nos laboratórios da Universidade, para a montagem dos circuitos (Alarme/Robô) com o auxílio de simuladores online como o "Tinkercad" e manuais de montagem como o do kit Chassi 2WD.

### Desenvolvimento do Código

Desenvolvidos via Arduino IDE, com o auxilio de conteúdos encontrados em forúms, conhecimentos adquiridos nas aulas, e tutoriais na internet.

## Comunicação entre App e Hardware

A comunicação entre o aplicativo e o hardware (ESP32) é realizada via Bluetooth serial.

  -> O aplicativo envia comandos por meio do BluetoothClient do App Inventor, utilizando comunicação em formato de texto simples.
  
  -> O ESP32 fica constantemente escutando na porta serial Bluetooth e, ao receber o comando (ex: '1'), executa ações como ativar os motores do carrinho ou iniciar a movimentação.
  
  -> Para a conexão inicial, o usuário clica no botão "Conectar com ESP", que abre uma lista de dispositivos Bluetooth disponíveis. Após selecionar o ESP32, o app tenta parear e confirmar a conexão.
  
  -> O status da conexão é exibido no app em tempo real, ajudando o usuário a saber se a comunicação está ativa ou se falhou.

Essa integração direta entre o app e o ESP32 é o que permite a experiência única do "alarme fujão", onde o despertador literalmente se move para obrigar o usuário a acordar.

