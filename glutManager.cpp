#include"glutManager.h"

double MAX_FPS = 30;
double timeStep = 1000/30;
int timeTick = 0;
unsigned int WIDTH = 1920;
unsigned int HEIGHT= 1080;
Camera *camera;

Vector3d ballPosition(0.2, 1.8, 1.0);
Vector3d ballVelocity(3.5, 0.0, 0.7);
Vector3d gravity(0.0, -9.8, 0.0);
const double ballRad = 0.2;

//stores the distance of the ball from each plane
double oldpHeight[6] = {0};
double pHeight[6] = {0};

double sleepTimer = 0;
double totalTime = 0;
double lastTime = 0;

cglLight* keyL = NULL;
cglLight* fillL = NULL;
cglLight* rimL = NULL;
plane box[6];

//Constructor calls all initialization routines
glutManager::glutManager(int argc, char** argv) :
    eye( 6, 3, 6),
    view( 1, .5, 1),
    up( 0, 1, 0)
{
    //I really need to switch to SDL because glut is 18 years old and garbage
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE|GLUT_ACCUM);
	glutInitWindowSize(WIDTH,HEIGHT);
	glutInitWindowPosition(200, 50);
	glutCreateWindow("817 Homework 1");
	//GLEW MUST be initialized AFTER creating the GLUT window- otherwise, will receive "missing GL version" error
	GLenum err = glewInit();
	if(GLEW_OK != err){
		printf("ERROR: %s\n", glewGetErrorString(err));
	}

    camera = new Camera(eye, view, up);
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClearAccum(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

    setLights();
    setShaders();
    setMaterial();
    
    //initializing the box geometry
    GLfloat ftop[] = {0.0,2.0,0.0, 0.0,2.0,2.0, 2.0,2.0,2.0, 2.0,2.0,0.0};
    GLfloat fbot[] = {0.0,0.0,0.0, 2.0,0.0,0.0, 2.0,0.0,2.0, 0.0,0.0,2.0};
    GLfloat fleft[] = {0.0,0.0,2.0, 0.0,2.0,2.0, 0.0,2.0,0.0, 0.0,0.0,0.0};
    GLfloat fright[] = {2.0,0.0,2.0, 2.0,0.0,0.0, 2.0,2.0,0.0, 2.0,2.0,2.0};
    GLfloat ffront[] = {0.0,0.0,2.0, 2.0,0.0,2.0, 2.0,2.0,2.0, 0.0,2.0,2.0};
    GLfloat fback[] = {0.0,0.0,0.0, 0.0,2.0,0.0, 2.0,2.0,0.0, 2.0,0.0,0.0};
    box[0] = ftop;
    box[1] = fbot;
    box[2] = fleft; 
    box[3] = fright;
    box[4] = ffront;
    box[5] = fback;
    //initViewport();
}

void glutManager::setLights(){
    GLfloat sceneAmbient[] = {0.0, 0.0, 0.0, 0.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,sceneAmbient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
    //KEY LIGHT
    GLfloat KEY[] =        {0.0, 0.0, 0.0, 
    /*diffuse*/             1.0, 0.0, 0.0,
    /*specular*/            1.0, 0.0, 0.0,
    /*position*/            10.0, 1.0, 1.0,
    /*direction*/           -3.0, 0, 0,
    /*spot exp, cutoff*/    10.0, 180.0,
    /*attenuation*/         0.01, 1.0, 0.0};
    GLfloat FILL[] =       {0.0, 0.0, 0.0, 
    /*diffuse*/             0.0, 0.0, 1.0, 
    /*specular*/            0.0, 0.0, 1.0, 
    /*position*/            1.0, 1.0, 10.0,
    /*direction*/           0.0, 0.0, -3.0,
    /*spot exp, cutoff*/    10.0, 180.0,
    /*attenuation*/         0.01, 0.001, 0.0};
    GLfloat RIM[] =        {0.0, 0.0, 0.0,
    /*diffuse*/             0.0, 1.0, 0.0, 
    /*specular*/            0.0, 1.0, 0.0, 
    /*position*/            1.0, 10.0, 1.0,
    /*direction*/           0.0, -3.0, 0.0, 
    /*spot exp, cutoff*/    10.0, 180.0,
    /*attenuation*/         0.01, 0.001, 0.0};

    keyL = new cglLight(GL_LIGHT0, KEY);
    fillL = new cglLight(GL_LIGHT1, FILL);
    rimL = new cglLight(GL_LIGHT2, RIM);
    keyL->init();
    fillL->init();
    rimL->init();
}

glutManager::~glutManager(){}

void glutManager::initViewport(){

    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.78, 0.1, 40.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(this->eye.x, this->eye.y, this->eye.z,
			this->view.x, this->view.y, this->view.z,
			this->up.x, this->up.y, this->up.z);
}

void glutManager::setMaterial(){
	//CHOCOLATE MATERIAL!!!
	float mat_ambient[] = {0.0,0.0,0.0,1.0};
	float mat_diffuse[] = {0.5, 0.5,0.5,1.0};
	float mat_specular[] = {1.0,1.0,1.0,1.0};
	float mat_shininess[] = {7.0};

	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}

//read in the whole file in one system call, parse line by line after
char *readFile(char* fileName){

	//Dr. Geist's file IO code almost verbatim	
	FILE *filePointer;
	char *content = NULL;
	int fd, counter;
	fd = open(fileName, O_RDONLY);
	counter = lseek(fd, 0, SEEK_END);
	close(fd);
	content = (char *)calloc(1,(counter+1));
	filePointer = fopen(fileName, "r");
	//SLURP
	counter = fread(content,sizeof(char),counter,filePointer);
	content[counter] = '\0';
	fclose(filePointer);
	return content;
}
unsigned int glutManager::setShaders(){

	char *vs, *fs;
	GLuint v, f, p;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	vs = readFile((char *)"phongEC.vert");
	fs = readFile((char *)"phongEC.frag");
	glShaderSource(v, 1, (const char **)&vs, NULL);
	glShaderSource(f, 1, (const char **)&fs, NULL);
	free(vs);
	free(fs);
	glCompileShader(v);
	glCompileShader(f);
	p = glCreateProgram();
	glAttachShader(p, f);
	glAttachShader(p, v);
	glLinkProgram(p);
	glUseProgram(p);
	return(p);
}

//----------------------------------------------------------------------------------------------------------
//BALL SIMULATION ROUTINES
//----------------------------------------------------------------------------------------------------------
int collision(Vector3d newBallPos){

    Vector3d V;
    for(int i = 0; i < 6; i++){
        //first we calculate v, the vector from a point on the plane to the ball position
        V = newBallPos - box[i].centroid;
        //now we store the old height and use the dot product to determine the new height
        oldpHeight[i] = pHeight[i];
        pHeight[i] = V*box[i].normal;
        //test to see if there is a sign change (
    }
    for(int i = 0; i < 6; i++){
        if(oldpHeight[i]*pHeight[i] < 0 || (pHeight[i] < ballRad && pHeight[i] > -1.0*ballRad)){

            return i+1;
        }
    }
    return 0;
}

void glutManager::ballSimulation(){

    usleep(30000); 
    Vector3d accel(gravity);
    Vector3d newVelocity, newPosition;
    //getting h, or time since last loop
    double timeSinceStart = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    double deltaTime = timeSinceStart - lastTime;
    //calculating the new position and velocity
    newVelocity = ballVelocity + accel*deltaTime;
    //To calculate a more precise position, we will average the velocities at the beginning (ballVelocity) and end (newVelocity)
    //of the timestep
    newPosition = ballPosition + ((ballVelocity + newVelocity)/2.0)*deltaTime;
    int collide = 0;
    collide = collision(newPosition); 
    if(collide){
        int planeIndex = collide - 1;
        double timeFrac;
        if(oldpHeight[planeIndex] > 0)
            timeFrac = (oldpHeight[planeIndex] - ballRad) / (oldpHeight[planeIndex] - pHeight[planeIndex]); 
        else
            timeFrac = (oldpHeight[planeIndex] + ballRad) / (oldpHeight[planeIndex] - pHeight[planeIndex]); 
        Vector3d vHit = ballVelocity + accel*(timeFrac*deltaTime);
        Vector3d xHit = ballPosition + (ballVelocity)*(timeFrac*deltaTime);

        //force would go here
        //reflect the velocity here
        //cr = coefficient of resitution
        //cf = coefficient of collision friction
        //vn = distance from plane
        //vp = vector perpendicular to plane
        //R = -cr*vn + (1 - cf)*vp
        double cr = 0.7;
        double cf = 0.1;
        Vector3d vn = (vHit*box[planeIndex].normal)*box[planeIndex].normal;
        Vector3d vp = vHit - vn;
       // Vector3d R = vp - vn;
        Vector3d R = -cr*vn + (1-cf)*vp;
        newVelocity = R + accel*((1 - timeFrac)*deltaTime);
        newPosition = xHit + R*((1 - timeFrac)*deltaTime);

        //Now we have to calculate the new height of our ball from the plane, since the array index
        //contains the height prior to the bounce calculations

        Vector3d V = newPosition - box[planeIndex].centroid;
        //pHeight[planeIndex] = V*absNormal;
        pHeight[planeIndex] = V*box[planeIndex].normal;

    }

    //storing our new values
    ballVelocity = newVelocity;
    ballPosition = newPosition;
    lastTime = timeSinceStart;
    timeTick++;
}

//----------------------------------------------------------------------------------------------------------
//DRAW ROUTINES
//These are not part of glutManager because glut is outdated and its callback functions won't accept functions
//that are members of a class
//----------------------------------------------------------------------------------------------------------
void drawLightVectors(){
    glDisable(GL_LIGHTING);
    glLineWidth(2.5);
    //key
    glColor3f(0.9, 0.3, 0.3);
    keyL->drawVector();
    //Fill
    glColor3f(0.3, 0.9, 0.3);
    fillL->drawVector();
    //Key
    glColor3f(0.3, 0.3, 0.9);
    rimL->drawVector();
    glEnable(GL_LIGHTING);
    
}

void drawAxes(){
    glDisable(GL_LIGHTING);
    glLineWidth(2.5);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0.5,0,0);
    glEnd();
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,0,0.5);
    glEnd();
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,0.5,0);
    glEnd();
    glEnable(GL_LIGHTING);
}

void glutManager::drawCube(){
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    Vector3d planeNormal;
    Vector3d viewVector(1,.5,1);

    //draw the planes
  //  for(int i = 0; i < 6; i++){
  //      box[i].draw();
  //  }
    box[1].draw();
    box[2].draw();
    box[5].draw();
    glDisable(GL_DEPTH_TEST);
    box[0].drawContour();
    box[3].drawContour();
    box[4].drawContour();
    glEnable(GL_DEPTH_TEST);
    //draw meta data

    for(int i = 0; i < 6; i++){ 
        glColor3f(1.0, 0.0, 0.0);
        glLineWidth(2.5);
//        box[i].drawContour();
        glColor3f(1.0, 0.0, 1.0);
        box[i].drawNormals();
    }

}

void drawBall(){

    glColor3f(1.0, 1.0, 1.0);
    GLUquadricObj *quadric;
    quadric = gluNewQuadric();
    glPushMatrix();
    glTranslatef(ballPosition.x, ballPosition.y, ballPosition.z);
    gluSphere(quadric, ballRad, 25, 25);
    glPopMatrix();
    gluDeleteQuadric(quadric);

}
void glutManager::draw(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    camera->PerspectiveDisplay(WIDTH, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(-1.0, -1.0, -1.0); 
    ballSimulation();
    glEnable(GL_DEPTH_TEST);
    drawCube();
    drawBall();
    glDisable(GL_DEPTH_TEST);
//	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, (void*)0);
    glDisable(GL_LIGHTING);
  //  drawLightVectors();
    glDisable(GL_DEPTH_TEST);
  //  drawAxes();
    glPopMatrix();
    glutSwapBuffers();
    glutPostRedisplay();
}

GLfloat genRand(float high){
    GLfloat r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/high);
    return r;
}
//----------------------------------------------------------------------------------------------------------
//MISC ROUTINES
//----------------------------------------------------------------------------------------------------------
void glutManager::rotate(int angle, int x, int y, int z) {
	glRotatef(angle, x, y, z);
    draw();
    glutSwapBuffers();
	glutPostRedisplay();
}

void mouseEventHandler(int button, int state, int x, int y) {
  // let the camera handle some specific mouse events (similar to maya)
  camera->HandleMouseEvent(button, state, x, y);
  glutPostRedisplay();
}

void motionEventHandler(int x, int y) {
  // let the camera handle some mouse motions if the camera is to be moved
  camera->HandleMouseMotion(x, y);
  glutPostRedisplay();
}

void glutManager::keyBindingsFunc(unsigned char key, int x, int y)
{
    x = 0;
    y = 0;
    switch(key) {
        case 'q':               
            exit(1);
            break;
        case 'r': case 'R':
            camera->Reset();
            break;
        case 'f': case 'F':
            camera->SetCenterOfFocus(Vector3d(0,0,0));
            break;
        //reset ball sim
        case 'g':
            ballPosition.x = genRand(1.5);
            ballPosition.y = genRand(1.5);
            ballPosition.z = genRand(1.5);
            ballVelocity.x = genRand(8.0);
            ballVelocity.y = genRand(8.0);
            ballVelocity.z = genRand(8.0);
            break;
        default:
            break;
    }
}
void glutManager::run(){

    glutDisplayFunc(draw);    
    glutKeyboardFunc(keyBindingsFunc);
    glutMouseFunc(mouseEventHandler);
    glutMotionFunc(motionEventHandler);
    glutMainLoop();

}
