#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_Circuit_Playground.h>

#define BINS   32          // The number of FFT frequency bins
#define FRAMES 4           // This many FFT cycles are averaged 

int cnt = 0;
uint32_t col[] = {0xFF0000, 0x808000, 0x00FF00, 0x008080, 0x0000FF, 0xFF0000, 0x808000, 0x00FF00, 0x008080, 0x0000FF};
void setup() {
  CircuitPlayground.begin();  // Set up the board library and serial
  CircuitPlayground.setBrightness(60);
  Serial.begin(9600);
}

void loop() {
  uint8_t i, j;
  uint16_t spectrum[BINS];     // FFT spectrum output buffer
  uint16_t avg[BINS];          // The average of FRAME "listens"

  for (j = 1; j <= FRAMES; j++) {          // We gather data FRAMES times and average it
    CircuitPlayground.mic.fft(spectrum);  // Here is the CP listen and FFT the data routine
    for (i = 0; i < BINS; i++) {          // Add for an average
      if (spectrum[i] > 255) spectrum[i] = 255; // limit outlier data
      if (i == 0)
        avg[i] = spectrum[i];
      else
        avg[i] = avg[i] + spectrum[i];
    }
  }
  for (i = 0; i < BINS; i++) {            // For each output bin average
    avg[i] = avg[i] / FRAMES;            //  divide about the number of values aaveraged
  }
  int maxVal = 0, maxIndex = 0;
  for (i = 0; i < BINS; i++) {            // For each output bin average
    if (avg[i] >= maxVal) {              //  find the peak value
      maxVal = avg[i];
      maxIndex = i;                      //  and the bin that max value is in
    }
  }
  for (j = 0; j < 32; j++) {        // print spectrum 32 bins
    Serial.print(avg[j]);
    Serial.print(" ");
  }
  //Serial.println("");              // and print the highest value and the bin it is in
  //Serial.print("Max Value = "); Serial.print(maxVal);
  //Serial.print(", Index of Max Value = "); Serial.println(maxIndex);
  int cnt = 0;
  if(maxVal > 5)
  {
    cnt = 0;
    int num = 0;
    for(int a = 2; a <= 32; a += 3)
    {
      num = 0;
      for(int b = a - 2; b <= a; b++)
      {
        num += avg[b];
      }
      if(a < 15)
      {
        num += 15;
      }
      else
      {
        num += 30;
      }
      if(num > 255)
      {
        num = 255;
      }
      if(cnt == 0)
      {
        Serial.println(num);
      }
      if(num > 35)
      {
        CircuitPlayground.setBrightness(num);
        CircuitPlayground.setPixelColor(cnt, col[cnt]);
      }
      cnt++;
    }
    CircuitPlayground.clearPixels();
  }
  
}
