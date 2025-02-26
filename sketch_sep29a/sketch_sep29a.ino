#include <Wire.h>

#define BQ78350_ADDR    0xb

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.println("-----------");
    Wire.begin();
}

uint8_t reg = 0x31;
char buf[64];

int SBS_read_reg(uint8_t addr, uint8_t reg, uint8_t *data, int len)
{
    int count = 0;

    Wire.beginTransmission(addr);
    Wire.write(reg);

    Wire.requestFrom(addr, len);
    while (count < len && Wire.available())
        *data++ = Wire.read();

    Wire.endTransmission();

    return count;
}

uint16_t vtab[15];

void loop() {
    uint8_t reg;
    int i;

    for (reg=0x31;reg<=0x3f;reg++) {
        uint8_t d[2];
        uint16_t voltage;

        SBS_read_reg(BQ78350_ADDR, reg, d, 2);
        vtab[0x3f-reg] = d[0] | (d[1] << 8);
    }

    for (i=0;i<15;i++) {
        sprintf(buf, "V%02d=%04d ", i, vtab[i]);
        Serial.print(buf);
    }

    Serial.println("");

    delay(1000);
}
