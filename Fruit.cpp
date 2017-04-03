#include "Fruit.h"



Fruit::Fruit()
{
	//set values for default constructor
	setType("null");
	setColour(Scalar(0,0,0));

}

Fruit::Fruit(string name){

	setType(name);
	
	if(name=="Green"){

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values
		//hsvmin : 0,72,153
		//hsvmax: 88,130,25

		//green ptr
		//30,44,119
		//61,109,193

		//ambiance
		//26,82,46
		//47,174,256
		setHSVmin(Scalar(26,82,46));
		setHSVmax(Scalar(47,174,256));

		//BGR value for Green:
		setColour(Scalar(0,255,0));

	}
	if(name=="banana"){

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(0,0,0));
		setHSVmax(Scalar(255,255,255));

		//BGR value for Yellow:
		setColour(Scalar(0,255,255));

	}
	if(name=="cherry"){

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(0,0,0));
		setHSVmax(Scalar(255,255,255));

		//BGR value for Red:
		setColour(Scalar(0,0,255));

	}



}

Fruit::~Fruit(void)
{
}

int Fruit::getXPos(){

	return Fruit::xPos;

}

void Fruit::setXPos(int x){

	Fruit::xPos = x;

}

int Fruit::getYPos(){

	return Fruit::yPos;

}

void Fruit::setYPos(int y){

	Fruit::yPos = y;

}

Scalar Fruit::getHSVmin(){

	return Fruit::HSVmin;

}
Scalar Fruit::getHSVmax(){

	return Fruit::HSVmax;
}

void Fruit::setHSVmin(Scalar min){

	Fruit::HSVmin = min;
}


void Fruit::setHSVmax(Scalar max){

	Fruit::HSVmax = max;
}