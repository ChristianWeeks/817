#include"plane.h"

plane::plane(){
	for(int i = 0; i < 4; i++){
		points[i].x = 0;
		points[i].y = 0;
		points[i].z = 0;
	}
}

//The primary method of instantiating a plane will be to give it an array of values
plane::plane(const point pointsb[]){
	for(int i = 0; i < 4; i++){
		points[i].x = pointsb[i].x;
		points[i].y = pointsb[i].y;
		points[i].z = pointsb[i].z;
	}
	calcNormal();
    calcCentroid();
}

plane::plane(const GLfloat values[]){
	for(int i = 0; i < 4; i++){
		points[i].x = values[i*3];
		points[i].y = values[i*3 + 1];
		points[i].z = values[i*3 + 2];
	}
	calcNormal();
    calcCentroid();
}

plane::plane(const plane &planeb){
	for (int i = 0; i < 4; i++){
		points[i].x = planeb.points[i].x;
		points[i].y = planeb.points[i].y;
		points[i].z = planeb.points[i].z;
	}
	normal = planeb.normal;
}

const plane& plane::operator=(const GLfloat values[]){
	for(int i = 0; i < 4; i++){
		points[i].x = values[i*3];
		points[i].y = values[i*3 + 1];
		points[i].z = values[i*3 + 2];
	}
	calcNormal();
    calcCentroid();
    return *this;
}

//no pointers, no need to delete
plane::~plane(){}

//calculate the normal based on two edges of the plane
void plane::calcNormal(){
	Vector3d v1(points[0].x - points[1].x, points[0].y - points[1].y, points[0].z - points[1].z);
	Vector3d v2(points[0].x - points[2].x, points[0].y - points[2].y, points[0].z - points[2].z);
	normal = v1%v2;
    normal = normal.normalize();
    normal = -1*normal;
}

void plane::calcCentroid(){
    centroid.x = (points[0].x + points[1].x + points[2].x + points[3].x) / 4;
    centroid.y = (points[0].y + points[1].y + points[2].y + points[3].y) / 4;
    centroid.z = (points[0].z + points[1].z + points[2].z + points[3].z) / 4;
}


//Draw the actual geometry
void plane::draw() const{
    glBegin(GL_QUADS);
    for(int i = 0; i < 4; i++){
        glVertex3f(points[i].x, points[i].y, points[i].z);
        glNormal3f(normal.x, normal.y, normal.z);
    }
    glEnd();
}

//draw the edges around the plane
void plane::drawContour() const{

    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < 4; i++){
        glVertex3f(points[i].x, points[i].y, points[i].z);
    }
    glVertex3f(points[0].x, points[0].y, points[0].z);
    glEnd();
}

//draw normals for each vertex
void plane::drawNormals() const{
    glBegin(GL_LINES);
    for(int i = 0; i < 4; i++){
        glVertex3f(points[i].x, points[i].y, points[i].z);
        glVertex3f(points[i].x + normal.x, points[i].y + normal.y, points[i].z + normal.z);
    }
    glEnd();
}


std::ostream& operator<< (std::ostream& os, const plane& p){
	os << "Points:\n";
	for(int i = 0; i < 4; i++)
		std::cout << "\t[" << p.points[i].x << ", " << p.points[i].y << ", " << p.points[i].z << "]\n";
	os << "Norm:\n";
	os << "\t" << p.normal << "\n";
	return os;
}



