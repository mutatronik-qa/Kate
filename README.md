# Kate
Sensor automatizado para poilipasto.
# Proyecto de Automatización para Monitoreo de Polipasto con ESP32

Este proyecto tiene como objetivo monitorear el estado operativo de un polipasto mediante una placa ESP32, implementando dos prototipos diferentes para la detección de estado: uno basado en optoacopladores y otro utilizando relés de estado sólido (SSR).

---

## Descripción

El polipasto es un equipo esencial en procesos industriales que requiere supervisión continua para garantizar su correcto funcionamiento y prevenir fallas. Este sistema utiliza sensores electrónicos conectados a un ESP32 para detectar y reportar el estado del polipasto en tiempo real.

Se desarrollaron dos prototipos para comparar la eficacia y seguridad entre las dos técnicas de aislamiento y conmutación:

- Prototipo con Optoacoplador: para aislar eléctricamente el microcontrolador de la señal de control.
- Prototipo con Relé de Estado Sólido (SSR): para conmutación silenciosa y mayor durabilidad sin partes móviles.

---

## Características

- Monitorización en tiempo real del estado del polipasto.
- Aislamiento eléctrico seguro mediante optoacoplador o SSR.
- Control y lectura con el microcontrolador ESP32.
- Notificaciones y registros de estado para análisis posterior.
- Fácil integración con otros sistemas de supervisión.

---

## Materiales y Componentes

- ESP32 (modelo recomendado: ESP32 DevKit)
- Optoacoplador (ejemplo: PC817 o equivalente)
- Relé de Estado Sólido (SSR) adecuado para la carga del polipasto
- Fuentes de alimentación según requerimientos
- Cables y protoboard para pruebas
- Sensores/interruptores necesarios para detectar estado del polipasto

---

## Esquema de Conexión

### Prototipo 1 - Optoacoplador  
Se conecta la salida del polipasto al optoacoplador, y la salida del optoacoplador a una entrada digital del ESP32 para detectar el estado sin contacto eléctrico directo.

### Prototipo 2 - Relé de Estado Sólido (SSR)  
El SSR se conecta en serie con la señal del polipasto y el ESP32 detecta la conmutación para determinar el estado.

---

## Instalación y Configuración

1. Conectar el hardware según los esquemas indicados para cada prototipo.
2. Descargar este repositorio y abrir el código en el IDE de Arduino o PlatformIO.
3. Configurar los pines de entrada/salida en el código según la conexión utilizada.
4. Subir el programa a la placa ESP32.
5. Verificar la comunicación y lectura del estado del polipasto desde el monitor serie o sistema de supervisión conectado.

---

## Código Ejemplo

En el archivo `main.ino` se incluye el código base para la lectura digital desde el ESP32 y la lógica para diferenciar el prototipo en uso.

---

## Consideraciones de Seguridad

- Asegurar el aislamiento eléctrico para evitar daños al microcontrolador y riesgos de electrocución.
- Verificar que los componentes soporte las corrientes y voltajes del polipasto.
- Evitar interferencias electromagnéticas en las señales.

---

## Contribuciones

Se aceptan mejoras y sugerencias para ampliar el monitoreo, agregar protocolos de comunicación o implementar alertas automáticas.

---

## Licencia

Este proyecto está bajo licencia MIT.

---

## Contacto

Para más información o consultas, contacta a [tu email o redes].

