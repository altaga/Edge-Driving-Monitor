#include <Arduino.h>
#include <WiFi.h>
#include <EdgeDriverMonitor_inferencing.h>
#include "esp_timer.h"
#include "img_converters.h"
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "esp_camera.h"
#include "camera_pins.h"

// raw frame buffer from the camera
#define FRAME_BUFFER_COLS           240
#define FRAME_BUFFER_ROWS           240
#define CUTOUT_COLS                 EI_CLASSIFIER_INPUT_WIDTH
#define CUTOUT_ROWS                 EI_CLASSIFIER_INPUT_HEIGHT
#define PART_BOUNDARY "123456789000000000000987654321"
#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 2 /* Time ESP32 will go to sleep (in seconds) */

camera_fb_t * fb = NULL;
uint8_t * _jpg_buf = NULL;
camera_config_t config;

const int cutout_row_start = (FRAME_BUFFER_ROWS - CUTOUT_ROWS) / 2;
const int cutout_col_start = (FRAME_BUFFER_COLS - CUTOUT_COLS) / 2;

String classify();
void cameraSetup();
void r565_to_rgb(uint16_t color, uint8_t *r, uint8_t *g, uint8_t *b);
int cutout_get_data(size_t offset, size_t length, float *out_ptr);

void setup() {
  Serial.begin(115200, SERIAL_8E2);
  Serial.println("Start");
  cameraSetup();
  fb = esp_camera_fb_get();
  if (fb) {
    classify();
  }
  // Drop First Classification
  fb = esp_camera_fb_get();
  if (!fb) {
    esp_deep_sleep_start();
  }
  else {
    Serial.println(classify());
    Serial.flush();
  }
  delay(1);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void loop() {
  // This loop never start
}

void cameraSetup() {
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
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  if (psramFound()) {
    config.frame_size = FRAMESIZE_240X240;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_240X240;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    //Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  sensor_t * s = esp_camera_sensor_get();
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, 0); // lower the saturation
  }
  s->set_framesize(s, FRAMESIZE_240X240);
}

void r565_to_rgb(uint16_t color, uint8_t *r, uint8_t *g, uint8_t *b) {
  *r = (color & 0xF800) >> 8;
  *g = (color & 0x07E0) >> 3;
  *b = (color & 0x1F) << 3;
}

int cutout_get_data(size_t offset, size_t length, float *out_ptr) {
  // so offset and length naturally operate on the *cutout*, so we need to cut it out from the real framebuffer
  size_t bytes_left = length;
  size_t out_ptr_ix = 0;

  // read byte for byte
  while (bytes_left != 0) {
    // find location of the byte in the cutout
    size_t cutout_row = floor(offset / CUTOUT_COLS);
    size_t cutout_col = offset - (cutout_row * CUTOUT_COLS);

    // then read the value from the real frame buffer
    size_t frame_buffer_row = cutout_row + cutout_row_start;
    size_t frame_buffer_col = cutout_col + cutout_col_start;

    uint16_t pixelTemp = fb->buf[(frame_buffer_row * FRAME_BUFFER_COLS) + frame_buffer_col];

    uint16_t pixel = (pixelTemp >> 8) | (pixelTemp << 8);

    uint8_t r, g, b;
    r565_to_rgb(pixel, &r, &g, &b);
    float pixel_f = (r << 16) + (g << 8) + b;
    out_ptr[out_ptr_ix] = pixel_f;

    out_ptr_ix++;
    offset++;
    bytes_left--;
  }

  // and done!
  return 0;
}

String classify() {
  ei_impulse_result_t result = { 0 };

  // Convert to RGB888
  //fmt2rgb888(fb->buf, fb->len, PIXFORMAT_RGB888, _jpg_buf);

  // Set up pointer to look after data, crop it and convert it to RGB888
  signal_t signal;
  signal.total_length = CUTOUT_COLS * CUTOUT_ROWS;
  signal.get_data = &cutout_get_data;

  // Feed signal to the classifier
  EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false /* debug */);

  // Returned error variable "res" while data object.array in "result"
  if (res != 0) return "Error";

  // Print short form result data
  String returnResult = "";
  for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
    returnResult+=String(result.classification[ix].value);
    if(ix!= EI_CLASSIFIER_LABEL_COUNT - 1){
      returnResult+=String(",");
    }
  }
  return String("Star"+returnResult+"Stop");
}
