#include <AGV_Core_Utils.h>
using AGV_Core::Utils::CircularBuffer;

CircularBuffer<int> buf;

void setup() {
    Serial.begin(115200);

    buf.Init(5);   // buffer de 5 enteros

    for (int i = 1; i <= 10; ++i) {
        buf.Push(i);
        
        Serial.print("Push ");
        if(i<10){Serial.print(" ");}
        Serial.print(i);
        Serial.print(" -> contenido: ");

        for (size_t k = 0; k < buf.Size(); k++) {
            Serial.print(buf.Get(k));
            Serial.print(" ");
        }
        Serial.println();
    }
}

void loop() {}
