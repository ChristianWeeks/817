#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include"Utility.h"
#include"Vector.h"


typedef struct{
	GLfloat x, y, z;
}point;

class plane {
	public:
		plane();
		plane(const point pointsb[]);
		plane(const plane& planeb);
        plane(const GLfloat values[]);
		~plane();
		point points[4];
		Vector3d normal;
        Vector3d centroid;

		//draw - draws to screen
        void draw() const;
        void drawContour() const;
        void drawNormals() const;
		void reverseNormal();

		friend std::ostream& operator<< (std::ostream& os, const plane& v);
        const plane& operator=(const GLfloat values[]);		// assignment
	private:
		void calcNormal();
        void calcCentroid();    
};
