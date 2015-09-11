#include"cglLight.h"

cglLight::cglLight(){


}

cglLight::cglLight(const GLenum lID){
	lightID = lID;
}

cglLight::cglLight(const GLenum lID, const GLfloat values[]){

	lightID = lID;
	ambient[0] = values[0];
	ambient[1] = values[1];
	ambient[2] = values[2];
	ambient[3] = 0.0;
	
	diffuse[0] = values[3];
	diffuse[1] = values[4];
	diffuse[2] = values[5];
	diffuse[3] = 0.0;
	
	specular[0] = values[6];
	specular[1] = values[7];
	specular[2] = values[8];
	specular[3] = 0.0;
	
	position[0] = values[9];
	position[1] = values[10];
	position[2] = values[11];
	position[3] = 1.0;	

	direction[0] = values[12];
	direction[1] = values[13];
	direction[2] = values[14];
	direction[3] = 1.0;

	directionV.x = values[12];
	directionV.y = values[13];
	directionV.z = values[14];

	spotExp = values[15];
	spotCutoff = values[16];
	constAtten = values[17];
	linearAtten = values[18];
	quadAtten = values[19];
}

void cglLight::init() const {

	glLightfv(lightID, GL_AMBIENT, ambient);
	glLightfv(lightID, GL_DIFFUSE, diffuse);
	glLightfv(lightID, GL_SPECULAR, specular);

	glLightf(lightID, GL_SPOT_EXPONENT, spotExp);
	glLightf(lightID, GL_SPOT_CUTOFF, spotCutoff);	
	glLightf(lightID, GL_CONSTANT_ATTENUATION, constAtten);
	glLightf(lightID, GL_LINEAR_ATTENUATION, linearAtten);
	glLightf(lightID, GL_QUADRATIC_ATTENUATION, quadAtten);
	glLightfv(lightID, GL_POSITION, position);
	glLightfv(lightID, GL_SPOT_DIRECTION, direction);
    enable();
}

void cglLight::enable() const{
	glEnable(lightID);
}
void cglLight::disable() const{
	glDisable(lightID);
}

void cglLight::drawVector() const{
    glBegin(GL_LINES);
    glVertex3f(position[0], position[1], position[2]);
    glVertex3f(position[0] + direction[0], position[1] + direction[1], position[2] + direction[2]);
    glEnd();
}
