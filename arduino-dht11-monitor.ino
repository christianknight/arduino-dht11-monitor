#include <LiquidCrystal_I2C.h>
#include <DHT.h>

/* Using a 20x4 LCD */
#define LCD_NUM_CHARS    20
#define LCD_NUM_ROWS     4

/* Data pin and type for DHT11 temperature/humidity sensor */
#define DHT_TYPE    DHT11
#define DHT_PIN     8

LiquidCrystal_I2C lcd(0x3F, LCD_NUM_CHARS, LCD_NUM_ROWS);
DHT dht = DHT(DHT_PIN, DHT_TYPE);

void setup() {
  lcd.init();
  lcd.backlight();

  dht.begin();

  Serial.begin(11500);
}

void loop() {
  dht_do_all_measurements();
  delay(2000);
}

void
dht_do_all_measurements(void) {
  /* Read humidity as relative % */
  float h = dht.readHumidity();
  /* Read temperature as Celsius (default argument) */
  float t = dht.readTemperature();
  /* Read temperature as Fahrenheit (isFahrenheit = true) */
  float f = dht.readTemperature(true);

  /* Check if any reads failed and exit early (to try again) */
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  /* Compute heat index in Fahrenheit (default argument for "isFahreheit") */
  float hif = dht.computeHeatIndex(f, h);
  /* Compute heat index in Celsius ("isFahreheit" = false) */
  float hic = dht.computeHeatIndex(t, h, false);

  /* Print results to serial */
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  /* Print results to LCD */
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.println("%");
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(f);
  lcd.println(" F");
  lcd.setCursor(0, 2);
  lcd.print("Heat idx: ");
  lcd.print(hif);
  lcd.println(" F");
}
