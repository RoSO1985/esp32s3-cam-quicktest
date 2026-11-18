#include <Arduino.h>
#include <WiFi.h>
#include "esp_camera.h"
#include "camera_pins.h"

// ==========================================
// Configure seu Wi-Fi aqui para testar
// ==========================================
const char* ssid = "SEU_WIFI_NOME";
const char* password = "SUA_SENHA_WIFI";

WiFiServer server(80);

void startCamera() {
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
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    Serial.println("PSRAM detectada com sucesso!");
    config.frame_size = FRAMESIZE_VGA; // ou SVGA, XGA, HD
    config.jpeg_quality = 10;
    config.fb_count = 2;
    config.fb_location = CAMERA_FB_IN_PSRAM;
  } else {
    Serial.println("AVISO: PSRAM nao encontrada. Reduzindo resolucao.");
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    config.fb_location = CAMERA_FB_IN_DRAM;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Falha na inicializacao da camera! Erro: 0x%x\n", err);
    while (true) delay(1000);
  }
  Serial.println("Camera OV2640 inicializada com sucesso!");
}

void handleClient(WiFiClient client) {
  // Le requisicao HTTP
  String request = client.readStringUntil('\r');
  client.readStringUntil('\n');

  if (request.indexOf("GET /capture") >= 0) {
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Falha ao capturar imagem");
      client.println("HTTP/1.1 500 Internal Server Error\r\n\r\n");
      return;
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: image/jpeg");
    client.println("Content-Disposition: inline; filename=capture.jpg");
    client.print("Content-Length: ");
    client.println(fb->len);
    client.println("Connection: close\r\n");
    client.write(fb->buf, fb->len);

    esp_camera_fb_return(fb);
  } else {
    // Pagina HTML de teste simples
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html\r\n");
    client.println("<!DOCTYPE html><html><head><meta charset='utf-8'><title>ESP32-S3 CAM Test</title>");
    client.println("<style>body{font-family:sans-serif;text-align:center;background:#1e1e2e;color:#cdd6f4;padding:20px;}");
    client.println("img{max-width:90%;border-radius:12px;border:2px solid #89b4fa;margin:20px 0;}");
    client.println("button{background:#89b4fa;color:#11111b;border:none;padding:12px 24px;border-radius:8px;font-weight:bold;cursor:pointer;font-size:16px;}");
    client.println("</style></head><body>");
    client.println("<h1>📷 ESP32-S3 CAM - Teste Rápido</h1>");
    client.println("<p>Clique para capturar uma foto instantânea da câmera:</p>");
    client.println("<button onclick=\"document.getElementById('foto').src='/capture?t=' + new Date().getTime()\">Tirar Foto</button><br>");
    client.println("<img id='foto' src='/capture' alt='Preview'>");
    client.println("</body></html>");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- ESP32-S3-CAM Quick Test ---");

  startCamera();

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi Conectado!");
  Serial.print("Acesse no navegador: http://");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    handleClient(client);
    client.stop();
  }
}
