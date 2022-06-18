void setups() {
  pinMode(D4,OUTPUT);
}
void loops() {
  digitalWrite(D4,1);
  delay(1000);
  digitalWrite(D4,0);
  delay(1000);
}
