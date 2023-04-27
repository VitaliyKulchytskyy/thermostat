#include <Arduino.h>
#include <microDS18B20.h>

// Уникальные адреса датчиков - считать можно в примере address_read
uint8_t s1_addr[] = {0x28, 0x91, 0xFF, 0xF9, 0xD,  0x0,  0x0, 0xA3};
uint8_t s2_addr[] = {0x28, 0x3A, 0x2B, 0xDB, 0x58, 0x20, 0x1, 0xE0};
MicroDS18B20<2, s1_addr> sensor1;  // Создаем термометр с адресацией
MicroDS18B20<3, s2_addr> sensor2;  // Создаем термометр с адресацией
void setup() {
    Serial.begin(9600);
}

void loop() {
    // асинхронное чтение нескольких датчиков смотри в примере async_read_many
    sensor1.requestTemp();      // Запрашиваем преобразование температуры
    sensor2.requestTemp();
    delay(1000);           // ожидаем результат

    Serial.print("t1: ");
    if (sensor1.readTemp()) Serial.println(sensor1.getTemp());
    else Serial.println("error");
    Serial.print("t2: ");
    if (sensor2.readTemp()) Serial.println(sensor2.getTemp());
    else Serial.println("error");
}

/*
#define DS_SENSOR_AMOUNT 2
// создаём двухмерный массив с адресами
uint8_t addr[][8] = {
        {0x28, 0x91, 0xFF, 0xF9, 0xD,  0x0,  0x0, 0xA3},
        {0x28, 0x3A, 0x2B, 0xDB, 0x58, 0x20, 0x1, 0xE0}, // з гільзою
};

// указываем DS_ADDR_MODE для подключения блока адресации
// и создаём массив датчиков на пине D2
MicroDS18B20<2, DS_ADDR_MODE> sensor[DS_SENSOR_AMOUNT];

void setup() {
    Serial.begin(9600);
    // устанавливаем адреса
    for (int i = 0; i < DS_SENSOR_AMOUNT; i++) {
        sensor[i].setAddress(addr[i]);
    }
}

void loop() {
    // конструкция программного таймера на 1c
    static uint32_t tmr;
    if (millis() - tmr >= 1000) {
        tmr = millis();

        // выводим показания в порт
        for (auto & i : sensor) {
            Serial.print(i.getTemp());
            Serial.print(',');
        }
        Serial.println();

        // запрашиваем новые
        for (auto & i : sensor) {
            i.requestTemp();
        }
    }
}*/