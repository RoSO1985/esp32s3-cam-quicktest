# ESP32-S3-CAM Quick Test 📷

Projeto rápido de testes para placa **ESP32-S3 com câmera OV2640** (compatível com PlatformIO e Arduino IDE).

---

## ⚡ Guia Rápido de Teste

### 1. Requisitos e Drivers no Computador
- **Chip USB do ESP32-S3**:
  - A maioria dos módulos ESP32-S3 possui conector **USB Type-C nativo** (CDC OTG). No macOS / Linux moderno, o driver já é **nativo** (não precisa instalar nada).
  - Se sua placa usar um chip serial intermediário (ex: CH340, CP2102 ou CH9102), certifique-se de ter o respectivo driver serial instalado (ex: *CH34xVCPDriver* no Mac).
- **Modo Bootloader (Download Mode)**:
  - Se a IDE não reconhecer ou falhar ao fazer o upload:
    1. Segure o botão **BOOT** (ou GPIO 0).
    2. Pressione e solte o botão **RST** (Reset).
    3. Solte o botão **BOOT**.
    4. Agora a porta USB estará em modo de gravação.

---

### 2. Configurações Críticas (Diferença do ESP32 antigo)
O **ESP32-S3** possui barramento e gerenciamento de memória diferentes do antigo ESP32-CAM (AI-Thinker):
1. **PSRAM Habilitada**: A câmera precisa de PSRAM para resoluções acima de QVGA e buffers de JPEG.
   - No `platformio.ini`: `build_flags = -DBOARD_HAS_PSRAM`, `memory_type = qio_opi`.
   - Na Arduino IDE: Em *Tools -> PSRAM -> "OPI PSRAM"* e *Tools -> USB CDC On Boot -> "Enabled"*.
2. **Pinout**: Os pinos da câmera mudaram em relação ao ESP32 clássico. O arquivo `src/camera_pins.h` já traz o mapeamento padrão do ESP32-S3 (S3-EYE / Freenove / Genérico).

---

### 3. Como Rodar com PlatformIO (VS Code / Terminal)

1. Abra a pasta do projeto:
   ```bash
   cd esp32s3-cam-quicktest
   ```
2. Edite o arquivo `src/main.cpp` e coloque o **SSID** e **Senha** da sua rede Wi-Fi.
3. Conecte o ESP32-S3 na porta USB.
4. Compile e envie:
   ```bash
   pio run -t upload -t monitor
   ```
5. Veja o IP que aparecerá no monitor serial (ex: `http://192.168.1.150`).
6. Abra esse endereço no seu navegador para ver o painel e testar o snapshot da câmera!

---

### 4. Como Rodar na Arduino IDE

Se preferir testar pela **Arduino IDE**:
1. Instale o pacote de placas **esp32 by Espressif Systems** (versão 2.0.x ou superior) no Board Manager.
2. Em **Tools -> Board**, selecione **ESP32S3 Dev Module**.
3. Configure em **Tools**:
   - **USB CDC On Boot**: *Enabled*
   - **Flash Size**: *16MB (ou 8MB de acordo com sua placa)*
   - **Partition Scheme**: *Huge APP (3MB No OTA / 16MB SPIFFS)*
   - **PSRAM**: *OPI PSRAM*
4. Abra o exemplo oficial em:
   `File -> Examples -> ESP32 -> Camera -> CameraWebServer`
5. No arquivo de exemplo, descomente:
   ```cpp
   #define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
   ```
   (comente os outros modelos de câmera).
6. Coloque seu Wi-Fi, faça o Upload e acesse o IP pelo navegador!
