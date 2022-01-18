void motorkontrol(int solmotorpwm, int sagmotorpwm) {
  if (sagmotorpwm <= 0) {
    sagmotorpwm = sagmotorpwm * (-1);
    digitalWrite(sagmotor1, LOW);
    digitalWrite(sagmotor1_2, HIGH);
    analogWrite(sagmotor2, sagmotorpwm);
  }
  else {
    digitalWrite(sagmotor1, HIGH);
    digitalWrite(sagmotor1_2, LOW);
    analogWrite(sagmotor2, sagmotorpwm);
  }
  if (solmotorpwm <= 0) {
    solmotorpwm = solmotorpwm * (-1);
    digitalWrite(solmotor1, LOW);
    digitalWrite(solmotor1_2, HIGH);
    analogWrite(solmotor2, solmotorpwm);
  }
  else {
    digitalWrite(solmotor1, HIGH);
    digitalWrite(solmotor1_2, LOW);
    analogWrite(solmotor2, solmotorpwm);
  }
}

void sagdoksan() {

  delay(50);
  qtra.readLine(sensors, 1, zemin);
  while (sensors[2] > se or sensors[1] > se)
  { qtra.readLine(sensors, 1, zemin);


    digitalWrite(solmotor1, 1);
    digitalWrite(solmotor1_2, 0);
    analogWrite(solmotor2, 110);

    digitalWrite(sagmotor1, 0);
    digitalWrite(sagmotor1_2, 1);
    analogWrite(sagmotor2, 80);


  }


}

void soldoksan()
{
  delay(50);

  qtra.readLine(sensors, 1, zemin);
  while (sensors[5] > se or sensors[6] > se ) //siyah gördüğünde >se
  {
    // Serial.println(sensors[7]);
    digitalWrite(solmotor1, 0);
    digitalWrite(solmotor1_2, 1);
    analogWrite(solmotor2, 80);

    digitalWrite(sagmotor1, 1);
    digitalWrite(sagmotor1_2, 0);
    analogWrite(sagmotor2, 110);
    qtra.readLine(sensors, 1, zemin);
  }

}
void frenle(){
motorkontrol(0, 0);
}




void hafifsagadon() {
  motorkontrol(35, -35);
}
void hafifsoladon() {
  motorkontrol(-35, 35);
}
