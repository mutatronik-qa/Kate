#include <WiFi.h>
#include <WebServer.h>

#define PIN_SUBIR 16
#define PIN_BAJAR 26
#define PIN_SALIDA 27

// WiFi
const char* ssid = "HONOR X7c"; 
const char* password = "toto1025";

WebServer server(80);

// Claves para botones
String claveContadores = "1234";   
String clavePenalizacion = "abcd"; 

// Variables pulsos
volatile int contadorSubir = 0;
volatile int contadorBajar = 0;

bool estadoAnteriorSubir = HIGH; // NC
bool estadoAnteriorBajar = HIGH; // NC

// Variables salida
bool salidaActiva = false;
unsigned long tiempoActivacionSalida = 0;

// Contadores de tiempo
unsigned long tiempoSubir = 0;
unsigned long tiempoBajar = 0;
unsigned long tiempoTotal = 0;

bool contarSubir = false;
bool contarBajar = false;

unsigned long startSubir = 0;
unsigned long startBajar = 0;

// ---------- Función formatear tiempo ----------
String formatTime(unsigned long tiempoMs){
  unsigned long totalSeg = tiempoMs / 1000;
  unsigned int horas = totalSeg / 3600;
  unsigned int minutos = (totalSeg % 3600) / 60;
  unsigned int segundos = totalSeg % 60;

  char buffer[9];
  sprintf(buffer, "%02u:%02u:%02u", horas, minutos, segundos);
  return String(buffer);
}

// ---------- Contar pulsos (solo flanco de entrada) ----------
void contarPulso(int pin){
  bool estado = digitalRead(pin);
  if(pin == PIN_SUBIR){
    if(estadoAnteriorSubir == HIGH && estado == LOW){
      contadorSubir++;
      Serial.println("Pulso Subir");
    }
    estadoAnteriorSubir = estado;
  }
  if(pin == PIN_BAJAR){
    if(estadoAnteriorBajar == HIGH && estado == LOW){
      contadorBajar++;
      Serial.println("Pulso Bajar");
    }
    estadoAnteriorBajar = estado;
  }
}

// ---------- Ruta JSON para actualizar datos sin recargar ----------
void handleData(){
  String json = "{";
  json += "\"subir\":\"" + String(contadorSubir) + "\",";
  json += "\"bajar\":\"" + String(contadorBajar) + "\",";
  json += "\"tSubir\":\"" + formatTime(tiempoSubir) + "\",";
  json += "\"tBajar\":\"" + formatTime(tiempoBajar) + "\",";
  json += "\"tTotal\":\"" + formatTime(tiempoTotal) + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

// ---------- Página web ----------
void handleRoot(){
  String page = "<!DOCTYPE html><html><head><meta charset='utf-8'>";
  page += "<title>Monitoreo Polipasto</title></head><body>";
  page += "<h2>Monitoreo Polipasto</h2>";

  page += "<p><b>Pulsos Subir:</b> <span id='pulsosSubir'></span></p>";
  page += "<p><b>Pulsos Bajar:</b> <span id='pulsosBajar'></span></p>";
  page += "<p><b>Tiempo Subir:</b> <span id='tiempoSubir'></span></p>";
  page += "<p><b>Tiempo Bajar:</b> <span id='tiempoBajar'></span></p>";
  page += "<p><b>Tiempo Total:</b> <span id='tiempoTotal'></span></p>";

  page += "<h3>Reiniciar Contadores</h3>";
  page += "<input type='password' id='claveContadores' placeholder='Ingrese clave'>";
  page += "<button onclick='reiniciarContadores()'>Reiniciar</button>";

  page += "<h3>Reiniciar Penalización</h3>";
  page += "<input type='password' id='clavePenalizacion' placeholder='Ingrese clave'>";
  page += "<button onclick='reiniciarPenalizacion()'>Reiniciar</button>";

  // Script AJAX para actualizar datos
  page += "<script>";
  page += "function actualizarDatos(){";
  page += "fetch('/getData').then(r=>r.json()).then(data=>{";
  page += "document.getElementById('pulsosSubir').innerText = data.subir;";
  page += "document.getElementById('pulsosBajar').innerText = data.bajar;";
  page += "document.getElementById('tiempoSubir').innerText = data.tSubir;";
  page += "document.getElementById('tiempoBajar').innerText = data.tBajar;";
  page += "document.getElementById('tiempoTotal').innerText = data.tTotal;";
  page += "});}";
  page += "setInterval(actualizarDatos,1000);";

  page += "function reiniciarContadores(){";
  page += "let clave = document.getElementById('claveContadores').value;";
  page += "fetch('/reiniciarContadores?clave='+clave);}";
  page += "function reiniciarPenalizacion(){";
  page += "let clave = document.getElementById('clavePenalizacion').value;";
  page += "fetch('/reiniciarPenalizacion?clave='+clave);}";
  page += "</script>";

  page += "</body></html>";
  server.send(200,"text/html",page);
}

// ---------- Manejo botones ----------
void handleReiniciarContadores(){
  String clave = server.arg("clave");
  if(clave == claveContadores){
    contadorSubir = 0;
    contadorBajar = 0;
    Serial.println("✅ Contadores reiniciados");
    server.send(200,"text/plain","Contadores reiniciados");
  } else server.send(403,"text/plain","Clave incorrecta");
}

void handleReiniciarPenalizacion(){
  String clave = server.arg("clave");
  if(clave == clavePenalizacion){
    salidaActiva = false;
    digitalWrite(PIN_SALIDA,LOW);
    Serial.println("⚠️ Penalización reiniciada");
    server.send(200,"text/plain","Penalización reiniciada");
  } else server.send(403,"text/plain","Clave incorrecta");
}

// ---------- Setup ----------
void setup(){
  Serial.begin(115200);
  pinMode(PIN_SUBIR, INPUT);
  pinMode(PIN_BAJAR, INPUT);
  pinMode(PIN_SALIDA, OUTPUT);
  digitalWrite(PIN_SALIDA,LOW);

  WiFi.begin(ssid,password);
  Serial.print("Conectando a WiFi...");
  while(WiFi.status() != WL_CONNECTED){ delay(500); Serial.print("."); }
  Serial.println("\nWiFi conectado");
  Serial.println(WiFi.localIP());

  server.on("/",handleRoot);
  server.on("/getData",handleData);
  server.on("/reiniciarContadores",handleReiniciarContadores);
  server.on("/reiniciarPenalizacion",handleReiniciarPenalizacion);

  server.begin();
}

// ---------- Loop ----------
void loop(){
  server.handleClient();
  unsigned long ahora = millis();

  // Contar pulsos
  contarPulso(PIN_SUBIR);
  contarPulso(PIN_BAJAR);

  // Contadores de tiempo
  bool estadoSubir = digitalRead(PIN_SUBIR) == LOW; 
  bool estadoBajar = digitalRead(PIN_BAJAR) == LOW; 

  // Subir
  if(estadoSubir){
    if(!contarSubir){ startSubir = ahora; contarSubir = true; }
    tiempoSubir = ahora - startSubir;
  } else contarSubir = false;

  // Bajar
  if(estadoBajar){
    if(!contarBajar){ startBajar = ahora; contarBajar = true; }
    tiempoBajar = ahora - startBajar;
  } else contarBajar = false;

  // Tiempo total solo al terminar ciclo
  if(!estadoSubir && !estadoBajar && (tiempoSubir>0 || tiempoBajar>0)){
    tiempoTotal += tiempoSubir + tiempoBajar;
    tiempoSubir = 0;
    tiempoBajar = 0;
  }

  delay(50);
}
