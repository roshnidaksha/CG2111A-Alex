void setupDisplay(void) {
  Serial.begin(115200);
    
  tft.init(240, 240, SPI_MODE2);    // Init ST7789 display 240x240 pixel
  tft.setRotation(2);
  tft.fillScreen(ST77XX_BLACK);
}

void showGreen() {
}

void showRed() {
}
