#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

// Configuración WiFi
const char* ssid = "Torito2312";
const char* password = "AlfaTorito2312";

// Servidor web
WebServer server(80);

// Configuración de la cámara
void setupCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 25000000;  // Establecido en 25 MHz
  config.pixel_format = PIXFORMAT_JPEG;
  
  // Ajustes de resolución y calidad
  config.frame_size = FRAMESIZE_QVGA;  // Puedes ajustar la resolución según sea necesario
  config.jpeg_quality = 10;  // Calidad de imagen
  config.fb_count = 2;  // Uso de dos framebuffers para la transmisión

  // Iniciar la cámara
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Error al inicializar la cámara: 0x%x", err);
    return;
  }
}

void handleStream() {
  camera_fb_t * fb = NULL;
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "multipart/x-mixed-replace; boundary=frame");

  while (server.client().connected()) {
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Error al capturar la imagen");
      server.send(500, "text/plain", "Error al capturar la imagen");
      break;
    }

    server.sendContent("--frame\r\n");
    server.sendContent("Content-Type: image/jpeg\r\n");
    server.sendContent("Content-Length: " + String(fb->len) + "\r\n\r\n");
    server.sendContent((const char*)fb->buf, fb->len);
    server.sendContent("\r\n");

    esp_camera_fb_return(fb);

    // Manejar otras solicitudes mientras se transmite la cámara
    server.handleClient();

    // Introducir un pequeño retraso para evitar sobrecargar el servidor
    delay(10);  // Puedes ajustar este valor si es necesario
  }
}

void handleForm() {
  Serial.println("Solicitud POST recibida");
  if (server.hasArg("plain")) {
    String message = server.arg("plain");
    Serial.println("Mensaje recibido: " + message);
    server.send(200, "text/plain", "Mensaje recibido: " + message);
  } else {
    Serial.println("No se recibió ningún mensaje");
    server.send(400, "text/plain", "No se recibió ningún mensaje");
  }
}

void handleRoot() {
  String html = "<html><head><title>ESP32-CAM Web Server</title>";
  html += "<meta charset=\"UTF-8\">";
  html += "<style>";
  html += "body { display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; background-color: #000000; color: #1111FF; }";
  html += "div { text-align: center; }";
  html += "img { width: 360px; height: 300px; }";
  html += "button { padding: 5px 30px; font-size: 16px; color: #FFFFFF; background-color: #444444; border: 5px; border-radius: 5px; cursor: pointer; margin-top: 10px; margin : 20px; }";
  html += "button:hover { background-color: #555555; }";
  html += "</style></head><body>";
  html += "<div>";
  html += "<h1>ESP32-CAM Web Server</h1>";
  html += "<img id=\"cameraImage\" src=\"/stream\">";
  html += "<br><br>";
  html += "<button onclick=\"sendCommand('avanzar')\">Avanzar</button>";
  html += "<button onclick=\"sendCommand('retroceder')\">Retroceder</button>";
  html += "<button onclick=\"sendCommand('izquierda')\">Girar Izquierda</button>";
  html += "<button onclick=\"sendCommand('derecha')\">Girar Derecha</button>";
  html += "<button onclick=\"sendCommand('autonomo')\">Modo Autónomo</button>";
  html += "<button onclick=\"sendCommand('automatico')\">Modo Automático</button>";
  html += "<script>";
  html += "function sendCommand(command) {";
  html += "fetch('/submit', { method: 'POST', headers: { 'Content-Type': 'text/plain' }, body: command })";
  html += ".then(response => response.text())";
  html += ".then(data => {";
  html += "console.log(data);";
  html += "setTimeout(() => {";
  html += "location.reload();";
  html += "}, 100);";
  html += "})";
  html += ".catch(error => console.error('Error:', error));";
  html += "}";
  html += "function refreshImage(){";
  html += "const cameraImage = document.getElementById('cameraImage');";
  html += "cameraImage.src = '/stream?' + new Date().getTime();";
  html += "setTimeout(refreshImage, 1000);";
  html += "}";
  html += "window.onload = refreshImage;";
  html += "</script>";
  html += "</div></body></html>";

  server.send(200, "text/html", html);
}



void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  setupCamera();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");

  server.on("/", handleRoot);
  server.on("/stream", HTTP_GET, handleStream);
  server.on("/submit", HTTP_POST, handleForm);

  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  server.handleClient();
}
