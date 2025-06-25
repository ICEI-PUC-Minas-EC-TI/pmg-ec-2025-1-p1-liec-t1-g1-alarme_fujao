
# Materiais

Os materiais utilizados no projeto foram:
- 2x ESP32.
- 2x Jumpers Macho
- 2x Protoboard's.
- Buzzer.
- Botão.
- Módulo Sensor de Distância Ultrassônico (HC-SR04).
- Kit Chassi 4WD (4 rodas) Robô para Arduino.
- Led Verde.
- Display de 7 segmentos (4 digitos).
- Módulo RTC.
- Caixa para o Alarme (imprimida em 3D).

# Desenvolvimento

O desenvolvimento do projeto foi dividido entre os integrantes do grupo e envolveu etapas de planejamento, construção da interface do aplicativo, montagem do hardware e integração dos sistemas.

A ideia do “Alarme Fujão” surgiu da proposta de criar um despertador interativo, que obrigasse o usuário a se mover para desligá-lo. Para isso, desenvolvemos um aplicativo no MIT App Inventor com uma interface simples, capaz de definir o horário do alarme e se comunicar via Bluetooth com o ESP32 presente no carrinho robô.

Paralelamente, realizamos a montagem do circuito com sensores, buzzer, display e motores, utilizando o kit 4WD. O código do ESP32 foi desenvolvido na Arduino IDE, sendo responsável por interpretar os comandos recebidos do app e acionar os componentes físicos.

Após a montagem, realizamos testes para garantir o funcionamento correto de todo o sistema e fizemos ajustes tanto na programação quanto na interface. No entanto, infelizmente o carrinho não funcionou conforme o esperado, apresentando falhas em sua movimentação. Além disso, não foi possível finalizar completamente a integração do alarme com o sistema físico. Apesar dessas limitações, o projeto apresentou avanços importantes e demonstrou, de forma funcional em partes, a proposta de integração entre hardware e software.

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
O botão remoto foi feito, separadamente, por dois participantes do grupo.

### Desenvolvimento do Código

Desenvolvidos via Arduino IDE, com o auxilio de conteúdos encontrados em forúms, conhecimentos adquiridos nas aulas, e tutoriais na internet.

## Comunicação entre App e Hardware

A comunicação entre o aplicativo e o hardware (ESP32) é realizada via Bluetooth serial.

  -> O aplicativo envia comandos por meio do BluetoothClient do App Inventor, utilizando comunicação em formato de texto simples.
  
  -> O ESP32 fica constantemente escutando na porta serial Bluetooth e, ao receber o comando (ex: '1'), executa ações como ativar os motores do carrinho ou iniciar a movimentação.
  
  -> Para a conexão inicial, o usuário clica no botão "Conectar com ESP", que abre uma lista de dispositivos Bluetooth disponíveis. Após selecionar o ESP32, o app tenta parear e confirmar a conexão.
  
  -> O status da conexão é exibido no app em tempo real, ajudando o usuário a saber se a comunicação está ativa ou se falhou.

Essa integração direta entre o app e o ESP32 é o que permite a experiência única do "alarme fujão", onde o despertador literalmente se move para obrigar o usuário a acordar.

