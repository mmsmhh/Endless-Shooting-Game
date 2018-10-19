#include <iostream>
#include <irrklang/irrklang.h> 
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <string>

#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <glut.h>
#include <vector> 

#pragma comment(lib, "irrKlang.lib") 


using namespace irrklang;


ISoundEngine *SoundEngine = createIrrKlangDevice();


int backgroundMov = 0;
int backgroundMov1 = 150;
int backgroundMov2 = 300;
int backgroundMov3 = 450;
int defenderRespawnTime = 0;
int rotatePlayer = 2;
int score = 0;
double defenderX = 50;
int speX = 600;
int speY = 50;
int enemyRotate = -4;
int defenderRotate = -4;

int p0[2];
int p1[2];
int p2[2];
int p3[2];
int pEnemy[2];
double t = 0;
bool defenderKilled = false;
int rep = 1;
int enemyFireRate = 0;
int playerFireRate = 0;
double k = 0.001;
double kk = 1;
bool killed = false;
bool meKilled = false;
int enemyHealth = 0;
int respawnTime = 0;
int initialHealth = 1;
int nextStar = 0;
struct point {
	double x;
	double y;
};
struct powerUp {
	point p;
	int  z;
};
int defenderFireRate = 0;
std::vector<point> fire;
std::vector<point> EnemyFire;
std::vector<point> defenderFire;
std::vector<point> stars;
std::vector<powerUp> powerUps;

int powerUpRate = 0;
int myPlayerSize = 50;


bool powerOne = false, powerZero = false;

int cPowerOne = 0, cPowerZero = 0;



int randInRange(int min, int max)
{
	return min + (int)(rand() / (double)(RAND_MAX + 1) * (max - min + 1));
}

void print(int x, int y, char *string)
{
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}


void DrawCircle(float cx, float cy, float r, int num_segments)
{
	float theta = 2 * 3.1415926 / float(num_segments);
	float c = cosf(theta);
	float s = sinf(theta);
	float t;
	float x = r;
	float y = 0;

	glBegin(GL_POLYGON);
	for (int ii = 0; ii < num_segments; ii++)
	{
		glVertex2f(x + cx, y + cy);
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}
	glEnd();
}


void time(int val)
{

	if (powerZero)
	{
		cPowerZero++;
		if (cPowerZero > 4000)
		{
			cPowerZero = 0;
			powerZero = false;
		}
	}

	if (powerOne)
	{
		cPowerOne++;
		if (cPowerOne > 4000)
		{
			cPowerOne = 0;
			powerOne = false;
		}
	}



	if (powerUpRate > 2000)
	{
		powerUpRate = 0;
		powerUp myPowerUp;
		myPowerUp.p.x = (rand() % (350 - 700)) + 350;
		myPowerUp.p.y = 650;
		myPowerUp.z = randInRange(0,1);
		powerUps.push_back(myPowerUp);

	}


	powerUpRate++;

	for (int i = 0; i < powerUps.size(); i++)
	{
		powerUps[i].p.y -= 1;

		if ((powerUps[i].p.y < speY + myPlayerSize && powerUps[i].p.y > speY - myPlayerSize) && (powerUps[i].p.x < speX + myPlayerSize && powerUps[i].p.x > speX - myPlayerSize))
		{

			SoundEngine->play2D("music/up.wav");

			//meKilled = true;

			if (powerUps[i].z == 0)
			{
				if (powerZero)
					cPowerZero = 0;
				else
				powerZero = true;
			}
			else if (powerUps[i].z == 1)
			{
				if (powerOne)
					cPowerOne = 0;
				else
					powerOne = true;
			}

			powerUps.erase(powerUps.begin() + i);

		}
		else
		if (powerUps[i].p.y < 0)
		{
			powerUps.erase(powerUps.begin() + i);
		}
	}




	//Defender

	if (defenderX > 1550 || defenderX < -550)
	{
		defenderRotate = defenderRotate  * -1;
			kk = kk * -1;

	}

	defenderX += kk;



	for (int i = 0; i < defenderFire.size(); i++)
	{
		defenderFire[i].y = defenderFire[i].y - 1;

		if ((defenderFire[i].y < speY + myPlayerSize && defenderFire[i].y > speY - myPlayerSize) && (defenderFire[i].x < speX + myPlayerSize && defenderFire[i].x > speX - myPlayerSize))
		{
			if (!powerZero)
			meKilled = true;


		}
		if (defenderFire[i].y < 0)
		{
			defenderFire.erase(defenderFire.begin() + i);
		}
	}

	if (!defenderKilled)
	{

		defenderFireRate++;
		if (defenderFireRate > 200)
		{
			defenderFireRate = 0;
			point Bullet;
			Bullet.x = defenderX;
			Bullet.y = 250;
			defenderFire.push_back(Bullet);
		}
	}
	else
	{
		defenderRespawnTime++;

		if (defenderRespawnTime > 2000)
		{
			defenderRespawnTime = 0;
			defenderKilled = false;
			defenderX = (rand() % (150 - 1050)) + 50;
		}
	}



	if (!killed)
	{
		if (t > 1 || t < 0)
		{
			k = k * -1;
			enemyRotate = enemyRotate * -1;
			p1[1] = (rand() % (400 - 600)) + 300;
			p2[1] = (rand() % (400 - 600)) + 300;
			p1[0] = (rand() % (150 - 1050)) + 50;
			p1[0] = (rand() % (150 - 1050)) + 50;
		}

		t += k;

		pEnemy[0] = pow((1 - t), 3)*p0[0] + 3 * t*pow((1 - t), 2)*p1[0] + 3 * pow(t, 2)*(1 - t)*p2[0] + pow(t, 3)*p3[0];
		pEnemy[1] = pow((1 - t), 3)*p0[1] + 3 * t*pow((1 - t), 2)*p1[1] + 3 * pow(t, 2)*(1 - t)*p2[1] + pow(t, 3)*p3[1];


		enemyFireRate++;
		if (enemyFireRate > 150)
		{
			enemyFireRate = 0;
			point Bullet;
			Bullet.x = pEnemy[0];
			Bullet.y = pEnemy[1] - 60;
			EnemyFire.push_back(Bullet);
		}
	}
	else
	{
		respawnTime++;

		if (respawnTime > 450)
		{
			respawnTime = 0;
			initialHealth = initialHealth + 2;
			enemyHealth = initialHealth;
			killed = false;
		}
	}

	for (int i = 0; i < fire.size(); i++)
	{
		fire[i].y = fire[i].y + 3;

		if ((fire[i].y < pEnemy[1] + 80 && fire[i].y > pEnemy[1] - 80) && (fire[i].x < pEnemy[0] + 60 && fire[i].x > pEnemy[0] - 60))
		{
			if(!killed)
			SoundEngine->play2D("music/bomb.mp3");


			if(powerOne)
				score += 20;
			else
				score += 10;

			if (enemyHealth <= 0)
			{
				if(!killed)
				enemyHealth--;

				killed = true;
			}
			else
			{

				enemyHealth--;
			}
			fire.erase(fire.begin() + i);
		}
		else if ((fire[i].y < 250 + 25 && fire[i].y > 250 - 25) && (fire[i].x < defenderX + 50 && fire[i].x > defenderX - 50))
		{
			SoundEngine->play2D("music/bomb.mp3");

			fire.erase(fire.begin() + i);
		}
		else
			if (fire[i].y > 700)
			{
				fire.erase(fire.begin() + i);
			}
	}


	for (int i = 0; i < EnemyFire.size(); i++)
	{
		EnemyFire[i].y = EnemyFire[i].y - 1;

		if ((EnemyFire[i].y < speY + myPlayerSize && EnemyFire[i].y > speY - myPlayerSize) && (EnemyFire[i].x < speX + myPlayerSize && EnemyFire[i].x > speX - myPlayerSize))
		{
			if(!powerZero)
			meKilled = true;


		}
		if (EnemyFire[i].y < 0)
		{
			EnemyFire.erase(EnemyFire.begin() + i);
		}
	}

	if (playerFireRate > 25)
		playerFireRate = 0;
	else
		if (playerFireRate != 0)
			playerFireRate++;



	glutPostRedisplay();

	glutTimerFunc(10, time, 0);
}

void spe(int k, int x, int y)
{
	if (k == GLUT_KEY_RIGHT && speX + 20 < 1195)
	{
		speX += 20;
		rotatePlayer = 1;
	}
	if (k == GLUT_KEY_LEFT && speX - 20 > 5)
	{
		speX -= 20;
		rotatePlayer = 0;
	}
	glutPostRedisplay();
}

void key(unsigned char k, int x, int y)
{
	if (meKilled)
	{
		initialHealth = 0;
		enemyHealth = 0;
		powerZero = false;
		powerOne = false;
		EnemyFire.clear();
		fire.clear();
		t = 0;
		speX = 600;
		speY = 50;
		meKilled = false;
		enemyHealth = 0;
		score = 0;
		killed = true;
		defenderKilled = true;


		fire.clear();
	EnemyFire.clear();
	defenderFire.clear();
	powerUps.clear();
	}
	else if (k == 32 && !meKilled)
	{
		if (playerFireRate == 0)
		{

			playerFireRate++;
			point Bullet;
			Bullet.x = speX;
			Bullet.y =  90;
			fire.push_back(Bullet);

			SoundEngine->play2D("music/laser.wav");

		}
	}
}


void Display() {

	glClearColor(0,0,0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	char str[100];
	sprintf(str, "Score : %d", score);
	
	char enemyH[100];
	sprintf(enemyH, "Enemy Health : %d", enemyHealth+1);



	for (int i = 0; i < stars.size(); i++)
	{
		//moga
		glPushMatrix();//draws a red pixel at the origin
		glTranslated(0, stars[i].y, 0);
		glColor3f(1, 1, 1);
		DrawCircle(stars[i].x, 0, 1, 50);
		glPopMatrix();
	}




	if (!meKilled)
	{

		//Player
		glPushMatrix();//draws a red pixel at the origin
		glTranslated(speX - 67, 40, 0);
		if (rotatePlayer == 0)
			glRotatef(5, 0, 0, 1.0f);
		else if (rotatePlayer == 1)
			glRotatef(-5, 0, 0, 1.0f);

		glColor3f(1, 0, 1);

		glPushMatrix();
		glColor3f(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f);
		glBegin(GL_POLYGON);
		glVertex2d(50, 0);
		glVertex2d(90, 0);
		glVertex2d(90, 60);
		glVertex2d(50, 60);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glColor3f(112.0f / 255.0f, 128.0f / 255.0f, 144.0f / 255.0f);

		glBegin(GL_POLYGON);
		glVertex2d(50, 50);
		glVertex2d(50, 10);
		glVertex2d(20, 30);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glColor3f(112.0f / 255.0f, 128.0f / 255.0f, 144.0f / 255.0f);

		glBegin(GL_POLYGON);
		glVertex2d(90, 50);
		glVertex2d(90, 10);
		glVertex2d(120, 30);
		glEnd();
		glPopMatrix();




		glPushMatrix();
		glColor3f(105.0f / 255.0f, 105.0f / 255.0f, 105.0f / 255.0f);

		glBegin(GL_POLYGON);
		glVertex2d(55, 0);
		glVertex2d(65, 0);
		glVertex2d(65, -10);
		glVertex2d(55, -10);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glColor3f(105.0f / 255.0f, 105.0f / 255.0f, 105.0f / 255.0f);

		glBegin(GL_POLYGON);
		glVertex2d(75, 0);
		glVertex2d(85, 0);
		glVertex2d(85, -10);
		glVertex2d(75, -10);
		glEnd();
		glPopMatrix();





		glPushMatrix();
		glColor3f(1, 0, 0);
		DrawCircle(60, -13, 6, 50);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1, 0, 0);
		DrawCircle(80, -13, 6, 50);
		glPopMatrix();



		glPushMatrix();
		glColor3f(139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f);

		glBegin(GL_POLYGON);
		glVertex2d(65, 60);
		glVertex2d(75, 60);
		glVertex2d(75, 80);
		glVertex2d(65, 80);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glColor3f(1, 1, 1);
		DrawCircle(70, 80, 5, 50);
		glPopMatrix();



		glPopMatrix();


if(!killed)
{

		//Enemy
		glPushMatrix();
		glColor3f(1, 1, 0);
		glTranslated(pEnemy[0] - 80, pEnemy[1] - 80, 0);
		glScaled(6, 6, 0);

		glRotatef(enemyRotate, 0, 0, 1.0f);

		glPushMatrix();
		glColor3f(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f);

		glBegin(GL_POLYGON);
		glVertex2d(8, 4);
		glVertex2d(11, 4);
		glVertex2d(17, 16);
		glVertex2d(17, 20);
		glVertex2d(16, 23);
		glVertex2d(15, 25);
		glVertex2d(14, 26);

		glVertex2d(12, 27);
		glVertex2d(7, 27);
		glVertex2d(5, 26);
		glVertex2d(4, 25);

		glVertex2d(3, 23);
		glVertex2d(2, 20);
		glVertex2d(2, 16);
		glEnd();

		glPopMatrix();


		glPushMatrix();

		glColor3f(0, 0, 0);

		glBegin(GL_POLYGON);
		glVertex2d(4, 19);
		glVertex2d(6, 19);

		glVertex2d(8, 17);
		glVertex2d(9, 14);
		glVertex2d(9, 13);
		glVertex2d(8, 13);


		glVertex2d(6, 14);
		glVertex2d(4, 17);

		glEnd();
		glPopMatrix();



		glPushMatrix();

		glColor3f(0, 0, 0);

		glBegin(GL_POLYGON);
		glVertex2d(15, 19);
		glVertex2d(13, 19);

		glVertex2d(11, 17);
		glVertex2d(10, 14);
		glVertex2d(10, 13);
		glVertex2d(11, 13);


		glVertex2d(13, 14);
		glVertex2d(15, 17);

		glEnd();

		glPopMatrix();


		glPushMatrix();

		glColor3f(0, 0, 0);

		glBegin(GL_POLYGON);
		glVertex2d(7, 11);
		glVertex2d(9, 11);
		glVertex2d(9, 9);
		glEnd();

		glPopMatrix();

		glPushMatrix();

		glColor3f(0, 0, 0);

		glBegin(GL_POLYGON);
		glVertex2d(10, 11);
		glVertex2d(12, 11);
		glVertex2d(10, 9);
		glEnd();

		glPopMatrix();



		glColor3f(0, 0, 0);

		glBegin(GL_POLYGON);
		glVertex2d(8, 7);
		glVertex2d(11, 7);
		glVertex2d(11, 8);
		glVertex2d(8, 8);
		glEnd();

		glPopMatrix();


		glPopMatrix();


	}

	if(!defenderKilled)
	{
		//defender
		glPushMatrix();
		glColor3f(1, 1, 0);
		glTranslated(defenderX - 40, 250, 0);
		glScaled(4, 4, 0);
		glRotatef(defenderRotate, 0, 0, 1.0f);
		glPushMatrix();
		glColor3f(0, 191.0f / 255.0f, 1);
		glBegin(GL_POLYGON);
		glVertex2d(8, 4);
		glVertex2d(11, 4);
		glVertex2d(17, 16);
		glVertex2d(17, 20);
		glVertex2d(16, 23);
		glVertex2d(15, 25);
		glVertex2d(14, 26);
		glVertex2d(12, 27);
		glVertex2d(7, 27);
		glVertex2d(5, 26);
		glVertex2d(4, 25);
		glVertex2d(3, 23);
		glVertex2d(2, 20);
		glVertex2d(2, 16);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glColor3f(0, 0, 0);
		glBegin(GL_POLYGON);
		glVertex2d(4, 19);
		glVertex2d(6, 19);
		glVertex2d(8, 17);
		glVertex2d(9, 14);
		glVertex2d(9, 13);
		glVertex2d(8, 13);


		glVertex2d(6, 14);
		glVertex2d(4, 17);

		glEnd();
		glPopMatrix();



		glPushMatrix();

		glColor3f(0, 0, 0);

		glBegin(GL_POLYGON);
		glVertex2d(15, 19);
		glVertex2d(13, 19);

		glVertex2d(11, 17);
		glVertex2d(10, 14);
		glVertex2d(10, 13);
		glVertex2d(11, 13);


		glVertex2d(13, 14);
		glVertex2d(15, 17);

		glEnd();

		glPopMatrix();


		glPushMatrix();

		glColor3f(0, 0, 0);

		glBegin(GL_POLYGON);
		glVertex2d(7, 11);
		glVertex2d(9, 11);
		glVertex2d(9, 9);
		glEnd();

		glPopMatrix();

		glPushMatrix();

		glColor3f(0, 0, 0);

		glBegin(GL_POLYGON);
		glVertex2d(10, 11);
		glVertex2d(12, 11);
		glVertex2d(10, 9);
		glEnd();

		glPopMatrix();



		glColor3f(0, 0, 0);

		glBegin(GL_POLYGON);
		glVertex2d(8, 7);
		glVertex2d(11, 7);
		glVertex2d(11, 8);
		glVertex2d(8, 8);
		glEnd();

		glPopMatrix();


		glPopMatrix();

	}

		for (int i = 0; i < fire.size(); i++)
		{
			//fire
			glPushMatrix();//draws a red pixel at the origin
			glColor3f(1, 0, 0);
			glTranslated(fire[i].x, fire[i].y, 0);

			glPushMatrix();
			glColor3f(139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f);
			glBegin(GL_QUADS);
			glVertex2d(0, 0);
			glVertex2d(5, 0);
			glVertex2d(5, 15);
			glVertex2d(0, 15);
			glEnd();
			glPopMatrix();

			glPushMatrix();

			glColor3f(1, 1, 1);
			glBegin(GL_QUADS);
			glVertex2d(-2, 15);
			glVertex2d(7, 15);
			glVertex2d(7, 30);
			glVertex2d(-2, 30);
			glEnd();
			glPopMatrix();


			glPushMatrix();
			glColor3f(1, 1, 1);
			glBegin(GL_TRIANGLES);
			glVertex2d(-2, 30);
			glVertex2d(7, 30);
			glVertex2d(2.5, 40);
			glEnd();
			glPopMatrix();



			glPopMatrix();
		}


		
			for (int i = 0; i < EnemyFire.size(); i++)
			{
				//fire
				glPushMatrix();//draws a red pixel at the origin
				glTranslated(EnemyFire[i].x, EnemyFire[i].y, 0);

				glPushMatrix();
				glColor3f(1, 0, 0);
				DrawCircle(0, 0, 10, 50);
				glPopMatrix();

				glPushMatrix();
				glColor3f(1, 1, 1);
				glBegin(GL_QUADS);
				glVertex2d(-1, 8);
				glVertex2d(1, 8);
				glVertex2d(1, 14);
				glVertex2d(-1, 14);
				glEnd();
				glPopMatrix();

				glPushMatrix();
				glColor3f(1, 0, 0);
				DrawCircle(0, 15, 2, 50);
				glPopMatrix();

				glPopMatrix();
			}
		

		
			//defenderFire
			for (int i = 0; i < defenderFire.size(); i++)
			{
				//fire
				glPushMatrix();//draws a red pixel at the origin
				glTranslated(defenderFire[i].x, defenderFire[i].y, 0);

				glPushMatrix();
				glColor3f(1, 0, 0);
				DrawCircle(0, 0, 10, 50);
				glPopMatrix();

				glPushMatrix();
				glColor3f(1, 1, 1);
				glBegin(GL_QUADS);
				glVertex2d(-1, 8);
				glVertex2d(1, 8);
				glVertex2d(1, 14);
				glVertex2d(-1, 14);
				glEnd();
				glPopMatrix();

				glPushMatrix();
				glColor3f(1, 0, 0);
				DrawCircle(0, 15, 2, 50);
				glPopMatrix();

				glPopMatrix();
			}

		
		glPushMatrix();//draws a red pixel at the origin
		glColor3f(1, 1, 1);
		print(15, 620, str);
		print(15, 590, enemyH);
		glPopMatrix();

		
		


		if (powerOne)
		{
			char str1[100];
			sprintf(str1, "Double score is active for : %d s", 40 - (cPowerOne/100));
			print(930, 620, str1);

		}
		if (powerZero)
		{
			char str2[100];
			sprintf(str2, "No damge is active for : %d s", 40 - (cPowerZero / 100));
			print(930, 590, str2);


		}




		for (int i = 0; i < powerUps.size(); i++)
		{
			//fire
			glPushMatrix();//draws a red pixel at the origin
			glTranslated(powerUps[i].p.x, powerUps[i].p.y, 0);
			glScaled(4, 4, 0);

			glPushMatrix();


			glPushMatrix();
			glColor3f(powerUps[i].z, 0, 1);
			glBegin(GL_QUADS);
			glVertex2d(0, 0);
			glVertex2d(6, 0);
			glVertex2d(6, 6);
			glVertex2d(0, 6);
			glEnd();
			glPopMatrix();


			glPushMatrix();
			glColor3f(1, 1, 1);
			glBegin(GL_QUADS);
			glVertex2d(2, 0);
			glVertex2d(3, 0);
			glVertex2d(3, 6);
			glVertex2d(2, 6);
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glColor3f(1, 1, 1);
			glBegin(GL_QUADS);
			glVertex2d(0, 3);
			glVertex2d(0, 4);
			glVertex2d(6, 4);
			glVertex2d(6, 3);
			glEnd();
			glPopMatrix();

			glPopMatrix();

			glPopMatrix();
		}


		

		
	}
	else
	{

		print(550, 350, "Game Over");
		
		
		print(550, 300, str);

		print(550, 250, "Press space to play again!");

	}



	glFlush();

}


void resize(int width, int height) {
	glutReshapeWindow(1200, 650);
}




void idle(void)
{
	int factor = 1;
	for (int i = 0; i < stars.size(); i++)
	{
		if (stars[i].y < 650)
			stars[i].y += factor;
		else
			stars[i].y = 0;

		factor++;

		if (factor > 25)
			factor = 0;
	}
}


void speUp(int k, int x, int y)// keyboard special key function is called whenever the special key is released.
{
	rotatePlayer = 2;
}

void main(int argc, char** argr) {


	SoundEngine->play2D("music/breakout.mp3", GL_TRUE);


	p0[0] = 150;
	p0[1] = 600;

	p1[0] = 150;
	p1[1] = 400;

	p2[0] = 1050;
	p2[1] = 400;

	p3[0] = 1050;
	p3[1] = 600;

	pEnemy[0] = 0;
	pEnemy[1] = 0;


	int x = 10;
	for (int i = 0; i < 119; i++)
	{
		point star;
		star.x = x;
		star.y = 0;
		stars.push_back(star);
		x += 10;
	}




	glutInit(&argc, argr);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1200, 650);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - 1200) / 2,(glutGet(GLUT_SCREEN_HEIGHT) - 650) / 2);	
	glutCreateWindow("Endless Shooting Game");
	glutTimerFunc(0, time, 0);		//call the timer function
	glutDisplayFunc(Display);
	glutSpecialFunc(spe);
	glutSpecialUpFunc(speUp);		//call the keyboard special keys up function
	glutIdleFunc(idle);
	glutKeyboardFunc(key);			//call the keyboard function
	gluOrtho2D(0.0, 1200.0, 0.0, 650.0);
	glutReshapeFunc(resize);

	glutMainLoop();

}