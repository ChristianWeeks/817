#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include"Utility.h"
#include"Vector.h"

class cglLight {
	public:
		cglLight();
		cglLight(const GLenum lID);
		cglLight(const GLenum lID, const GLfloat values[]);
		~cglLight();
		void init() const;
		void enable() const;
		void disable() const;
        void drawVector() const;

	private:

		GLenum lightID;

		GLfloat ambient[4],
			  diffuse[4],
			  specular[4],
			  position[4],
			  direction[4];

		//glLightfv accepts parameters of length 4, so a separate variable is needed for our direction vector
		Vector3d directionV;
		GLfloat spotExp;
		GLfloat spotCutoff;
		GLfloat constAtten;
		GLfloat linearAtten;
		GLfloat quadAtten;

};
