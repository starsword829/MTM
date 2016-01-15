void setup (){
	pinMode(6,OUTPUT);
	pinMode(9,OUTPUT);
}

void loop(){
	analogWrite(6,0);
    analogWrite(9,255);
}