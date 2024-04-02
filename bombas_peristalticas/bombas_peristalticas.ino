// Definir los pines utilizados para controlar la bomba
const int pinControl1 = 18; // Pin de control 1
const int pinControl2 = 19; // Pin de control 2
const int pinControl3 = 21; // Pin de control 2


int TimeON = 2000;
int power = 255;
int powerM = 200;

int Turbidity = 0;  // variable to store the value coming from the sensor
int PinTurbidity = 34;
float Volt = 0;
float ntu;
float concent;

int volumen;
int bombaB;
int bombaC;

void setup() {
  // Inicializar los pines como salida
  Serial.begin(115200);
  pinMode(pinControl1, OUTPUT);
  pinMode(pinControl2, OUTPUT);
  pinMode(pinControl3, OUTPUT);

  volumen = 0;
  bombaB = 0;
  bombaC = 0;


}

void loop() {
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    Serial.print("--> ");
    Serial.print(char(inByte));
    Serial.print(": ");
    switch (inByte) {
      case 'M':
        Serial.println("Bomba 1 ON");
        encenderBomba1();

        delay(TimeON);
        apagarBomba1();
        inByte = 0;
        break;

      case 'p':
        power = (Serial.parseInt()); //dataIn now holds 314
        Serial.print("Power = ");
        Serial.println(power);
        //Serial.println("");
        break;

      case 'B':
        Serial.println("Bomba 2 ON");
        encenderBomba2();
        delay(TimeON);
        apagarBomba2();
        inByte = 0;
        break;

      case 'C':
        Serial.println("Bomba 3 ON");
        encenderBomba3();
        delay(TimeON);
        apagarBomba3();
        inByte = 0;
        break;

      case 't':  //reset
        TimeON = Serial.parseInt(); //dataIn now holds 314
        Serial.print("time ON = ");
        Serial.print(TimeON);
        Serial.println(" ms");
        break;

      case 'L':

        volumen = 0;
        bombaB = 0;
        bombaC = 0;

        volumen = Serial.parseInt();

        // extracción de medio
        bombaB = ((volumen - 33.699) / 0.0059) + 16949.152; // bomba b
        Serial.print("Tiempo extracción bomba B: ");
        Serial.print(bombaB);
        Serial.println("s ");

        encenderBomba2();
        delay(bombaB);
        apagarBomba2();



        //ingreso de medio

        bombaC = (volumen -   3.1456) / 0.0059;   // bomba C
        Serial.print("Tiempo ingreso bomba C: ");
        Serial.println(bombaC);
        Serial.println("s ");
        encenderBomba3();
        delay(bombaC);
        apagarBomba3();


        break;

      case 'N':
        inByte = 0;
        ntu = 0;

        Volt = measureTurbidity();

        Serial.print("NTU= ");
        Serial.print(ntu);
        Serial.print(" ,  ");
        Serial.print(Volt);
        Serial.print(" V , ADC= ");
        Serial.println(analogRead(PinTurbidity));

        concent = calcularConcent(Volt);
        Serial.print("concentración: ");
        Serial.println(concent);
        /*

          CALCULAR CONCENTRACIÓN

        */
        break;


      default:
        // Serial.println("Default");
        break;
    }

  }


  //FIN LOOP
}



float measureTurbidity() {

  // inByte = 0;  // what is this?
  float volt = 0;

  for (int i = 0; i < 800; i++) {
    volt += ((float)analogRead(PinTurbidity) / 4093) * 3.3; //3.3
  }
  volt = volt / 800;
  volt = round_to_dp(volt, 3);
  if (volt < 2.5) {
    ntu = 3000;
  } else {
    ntu = -1120.4 * sq(volt) + 5742.3 * volt - 4353.8;
  }

  return volt;

}

float calcularConcent(float voltaje) {
  return (-0.5815 * pow(voltaje, 5)) + (5.2798 * pow(voltaje, 4)) - (17.42 * pow(voltaje, 3)) + (25.408 * pow(voltaje, 2)) - (16.801 * voltaje) + 6.1209;
}


float round_to_dp(float in_value, int decimal_place) {
  float multipler = powf(10.0f, decimal_place);
  in_value = roundf(in_value * multipler) / multipler;
  return in_value;
}

// Función para encender la bomba
void encenderBomba1() {
  analogWrite(pinControl1, powerM);
}


// Función para apagar la bomba
void apagarBomba1() {
  analogWrite(pinControl1, LOW);
}

void encenderBomba2() {
  analogWrite(pinControl2, power);
}


// Función para apagar la bomba
void apagarBomba2() {
  analogWrite(pinControl2, LOW);
}

void encenderBomba3() {
  analogWrite(pinControl3, power);
}


// Función para apagar la bomba
void apagarBomba3() {
  analogWrite(pinControl3, LOW);
}
