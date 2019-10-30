//30    27    29
void setup()
{
	Serial.begin(115200);
	pinMode(A0, INPUT);
	pinMode(A1, INPUT);
	pinMode(A2, INPUT);
}

void loop()
{
	delay(50);
	Serial.print(analogRead(A0));   
	Serial.print(",");
	Serial.print(analogRead(A1));
	Serial.print(",");
	Serial.println(analogRead(A2));
}
