# ESP32-S3-CAM Quick Test 📷

Repositório de teste, experimentação e validação de hardware para o microcontrolador **ESP32-S3 com sensor de câmera OV2640**.

---

## 🎯 Objetivo do Projeto

1. **Validação Rápida de Hardware**:
   - Testar o funcionamento da câmera OV2640 acoplada ao ESP32-S3.
   - Validar a alocação e detecção de **PSRAM OPI** (essencial para buffers de imagem de alta resolução e streaming sem travamentos).
   - Verificar a conectividade Wi-Fi e tempo de resposta de captura de frames.

2. **Base Modular para Projetos Futuros**:
   - Servir como ponto de partida (boilerplate) limpo para implementações mais avançadas, como:
     - Streaming de vídeo MJPEG contínuo.
     - Reconhecimento facial ou detecção de objetos (ESP-WHO / TinyML / Edge Impulse).
     - Integração com MQTT / WebSockets / Home Assistant / API de visão computacional.
     - Captura de fotos sob demanda via botão físico ou sensor infravermelho/PIR.

3. **Multiplataforma**:
   - Totalmente compatível com **PlatformIO** (VS Code / CLI) e **Arduino IDE 2.x**.

---

## 📁 Estrutura do Repositório

```text
esp32s3-cam-quicktest/
├── platformio.ini         # Configuração de build (PSRAM, Flash 16MB, USB CDC)
├── src/
│   ├── camera_pins.h      # Mapeamento oficial dos pinos do barramento da câmera (S3-EYE)
│   └── main.cpp           # Servidor Web embarcado para captura instantânea e teste de snapshot
├── .gitignore
└── README.md              # Documentação completa de uso e setup
```

---

## ⚡ Guia de Setup e Teste

### 1. Drivers e Reconhecimento USB (macOS / Linux / Windows)
- **USB CDC Nativo**: A maioria dos módulos ESP32-S3 possui USB nativo direto ao chip. No macOS e Linux modernos, o driver é reconhecido automaticamente sem necessidade de instalação.
- **Placas com chip ponte (CH340 / CP2102)**: Instale o respectivo driver serial do fabricante caso o dispositivo não apareça em `/dev/cu.*` ou `COM*`.
- **Modo Bootloader (Download Mode)**: Se o upload falhar por falta de sincronização com o bootloader:
  1. Mantenha o botão **BOOT** pressionado.
  2. Pressione e solte o botão **RST (Reset)**.
  3. Solte o botão **BOOT**.
  4. Dispare a gravação novamente.

---

### 2. Configurações Críticas do ESP32-S3

| Parâmetro | Valor Recomendado | Motivo |
|---|---|---|
| **PSRAM** | `OPI PSRAM` / `-DBOARD_HAS_PSRAM` | Permite usar resoluções SVGA/XGA/HD e manter buffers em DMA |
| **USB CDC On Boot** | `Enabled` / `-DARDUINO_USB_CDC_ON_BOOT=1` | Redireciona a saída do `Serial.print` para a USB Type-C nativa |
| **Flash Partition** | `default_16MB` ou `Huge APP` | O binário com suporte a Wi-Fi e driver de câmera é pesado |
| **Pinout Header** | `CAMERA_MODEL_ESP32S3_EYE` | O ESP32-S3 possui barramento de pinos diferente do ESP32 clássico |

---

### 3. Como Rodar com PlatformIO

1. Clone ou abra a pasta local do projeto:
   ```bash
   cd esp32s3-cam-quicktest
   ```
2. Abra `src/main.cpp` e configure suas credenciais Wi-Fi:
   ```cpp
   const char* ssid = "NOME_DO_SEU_WIFI";
   const char* password = "SENHA_DO_SEU_WIFI";
   ```
3. Conecte a placa na porta USB do computador.
4. Compile, envie e abra o monitor serial:
   ```bash
   pio run -t upload -t monitor
   ```
5. Anote o IP exibido no terminal (ex: `http://192.168.1.150`) e abra no navegador web.

---

### 4. Como Rodar na Arduino IDE

1. Instale o pacote de placas **esp32 by Espressif Systems** (>= 2.0.11 ou 3.x) no Board Manager.
2. Selecione a placa: **ESP32S3 Dev Module**.
3. Em **Tools**, configure:
   - **USB CDC On Boot**: *Enabled*
   - **PSRAM**: *OPI PSRAM*
   - **Flash Size**: *16MB* (ou o tamanho da sua placa)
   - **Partition Scheme**: *Huge APP (3MB No OTA)*
4. Abra o exemplo nativo da Espressif em:
   `File -> Examples -> ESP32 -> Camera -> CameraWebServer`
5. No topo de `CameraWebServer.ino`, comente o modelo default e ative:
   ```cpp
   #define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
   ```
6. Insira seu Wi-Fi e grave na placa.
