#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <vector>
#include <cstdio>
#include<iostream>
// Game variables
struct Bullet {
    float x;
	float y;
};

struct Enemy {
    float x;
	float y;
	// Movement direction
    float dx;
	float dy;  
    bool alive;
    std::vector<Bullet> bullets;
};
 // Player jet position
float playerX = 0.0f;
float playerY = -0.8f;
std::vector<Enemy> enemies;
std::vector<Bullet> playerBullets;

float bulletSpeed = 0.05f;
float enemyBulletSpeed = 0.02f;
float enemyspeedm=0.7f;//speed multiplexer based on level
int score = 0;
int level = 1;
int lives = 3;
bool gameOver = false;
float enemySpeed = 0.0002f;  
const float MAX_Y = 0.5f;  
int maxBulletsAllowed = 1;  
int maxEnemiesAtLevel = 1 ; 
int spawnInterval = 1000;
int enemiesDefeated = 0; 
bool isPlayerHit = false;
float playerJetColor[3] = {0.0f, 1.0f, 0.0f};  
int totalEnemiesDefeated = 0; 

// Function to create a new enemy with synced movement
void addEnemy() {
    Enemy newEnemy;
    // Random x between -1.0 and 1.0
    newEnemy.x = (rand() % 200 - 100)/ 100.0f; 
    newEnemy.y = 0.8f;
    // Slower horizontal movement
    newEnemy.dx = (rand() % 100 - 50)/ 2000.0f*enemyspeedm; 
    // Slower upward/downward movement
    newEnemy.dy = 0.0005f*enemyspeedm; 
    newEnemy.alive = true;
    enemies.push_back(newEnemy);
}

// Draw text on screen
void drawText(const char* text, float x, float y) {
    glRasterPos2f(x, y);
    for (size_t i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

// Draw fighter jet
void drawJet(float x, float y, bool isPlayer) {
    glColor3f(isPlayer ? playerJetColor[0] : 1.0f, isPlayer ? playerJetColor[1] : 0.0f, isPlayer ? playerJetColor[2] : 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y);
    glVertex2f(x - 0.05f, y - 0.1f);
    glVertex2f(x + 0.05f, y - 0.1f);
    glEnd();
}

// Draw bullets
void drawBullets(const std::vector<Bullet>& bullets, float r, float g, float b) {
    glColor3f(r, g, b);
    for (size_t i = 0; i < bullets.size(); ++i) {
        glBegin(GL_QUADS);
        glVertex2f(bullets[i].x - 0.01f, bullets[i].y);
        glVertex2f(bullets[i].x + 0.01f, bullets[i].y);
        glVertex2f(bullets[i].x + 0.01f, bullets[i].y + 0.05f);
        glVertex2f(bullets[i].x - 0.01f, bullets[i].y + 0.05f);
        glEnd();
    }
}

// Update enemy behavior
void updateEnemies() {
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (!enemies[i].alive) continue;

        // Update position (side-to-side with slight vertical movement)
        enemies[i].x += enemies[i].dx;
        enemies[i].y += enemies[i].dy;

        // Prevent enemies from getting stuck at the edges
        if (enemies[i].x > 1.0f - 0.05f) {
            enemies[i].dx = -fabs(enemies[i].dx);  // Reverse direction
        } else if (enemies[i].x < -1.0f + 0.05f) {
            enemies[i].dx = fabs(enemies[i].dx);   // Reverse direction
        }

        // Keep the enemies within vertical bounds
        if (enemies[i].y > MAX_Y || enemies[i].y < -1.0f) {
            enemies[i].dy = -enemies[i].dy;  // Reverse vertical direction
        }

        // Increase bullet firing rate
        if (rand() % 50 == 0) {  // More frequent bullet firing (chance 1 in 50)
            Bullet newBullet = {enemies[i].x, enemies[i].y - 0.1f};
            enemies[i].bullets.push_back(newBullet);
        }
    }
}

// Check collisions
void checkCollisions() {
    // Check player bullets hitting enemies
    for (size_t j = 0; j < playerBullets.size(); ++j) {
        for (size_t i = 0; i < enemies.size(); ++i) {
            if (enemies[i].alive &&
                playerBullets[j].y >= enemies[i].y - 0.1f &&
                playerBullets[j].x >= enemies[i].x - 0.05f &&
                playerBullets[j].x <= enemies[i].x + 0.05f) {
                enemies[i].alive = false;
                playerBullets.erase(playerBullets.begin() + j);
                score += 10;
                enemiesDefeated++;
                totalEnemiesDefeated++;
                return;
            }
        }
    }

    // Check enemy bullets hitting player
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (!enemies[i].alive) continue;
        for (size_t j = 0; j < enemies[i].bullets.size(); ++j) {
            if (enemies[i].bullets[j].y <= playerY + 0.1f &&
                enemies[i].bullets[j].x >= playerX - 0.05f &&
                enemies[i].bullets[j].x <= playerX + 0.05f) {
                enemies[i].bullets.erase(enemies[i].bullets.begin() + j);
                lives--;
                isPlayerHit = true; // Set hit flag
                playerJetColor[0] = 1.0f; // Change to red color when hit
                playerJetColor[1] = 1.0f;
                playerJetColor[2] = 1.0f;
                if (lives <= 0) gameOver = true;
                return;
            }
        }
    }
}



 bool abel= false;
 

    
		
// Draw instructions
    void enemies1(){
char info[100];	
sprintf(info,"Home  ");
	glColor3f(0.0f, 0.0f, 0.0f);
	drawText("Well Come to Flight fighter Game ", -0.5f, 0.6f);
        glColor3f(1.0f, 1.0f, 1.0f);
    drawText("Up Button: Move Up ", -0.2f,0.6f);
    drawText("Down Button: Move Down  ", -0.2f, 0.5f);
    drawText("Left Button: Move Left  ", -0.2f, 0.4f);
    drawText("Right Button: Move Right ", -0.2, 0.3f);
    drawText("Space: Shoot", -0.2f, 0.2f);
    drawText("To start the game  press : G", -0.2f, 0.1f);
drawText(info,-0.2f,0.8f);

    // Draw player jet



	if(abel == true ){
		
    glClear(GL_COLOR_BUFFER_BIT);
    sprintf(info, "Score: %d  Level: %d  Lives: %d", score, level, lives);
    drawText(info, -0.9f, 0.8f);
		  drawJet(playerX, playerY, true);
		
//     Draw enemies
    
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i].alive) {
            drawJet(enemies[i].x, enemies[i].y, false);
            drawBullets(enemies[i].bullets, 1.0f, 0.0f, 0.0f);
        }
    }
//    Draw player bullets

drawBullets(playerBullets, 1.0f, 1.0f, 0.0f);

	}

//starter();

    glutSwapBuffers();
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT);


    if (gameOver) {
        glColor3f(1.0f, 0.0f, 0.0f);
        drawText("Game Over! Press Esc to Exit.", -0.5f, 0.0f);

        // Display final score
        char scoreText[100];
        sprintf(scoreText, "Final Score: %d", score);
        drawText(scoreText, -0.5f, -0.1f);
        glColor3f(0.0f, 0.0f, 1.0f);
        drawText("PRESS ENTER TO RESTART", -0.5f, 0.1f);
        glColor3f(0.0f, 1.0f, 0.0f);
        drawText("DEVLOPED BY: 1. NATAN MESERET", -0.5f,- 0.2f);
        drawText("               : 2.ABEL TADESSE ", -0.5f,- 0.3f);
        drawText("               : 3.GIRMA WAKEYO ", -0.5f,- 0.4f);
        drawText("               : 4.MIKIYAS SINTAYEHU ", -0.5f,- 0.5f);
        drawText("CopyRight@2025GC ", -0.5f,- 0.6f);

        glutSwapBuffers();
        return;
    }
    
    enemies1();
}
    

// Update function
void update(int value) {
    if (!gameOver) {
        // Update player bullets
        for (size_t i = 0; i < playerBullets.size(); ++i) {
            playerBullets[i].y += bulletSpeed;
            if (playerBullets[i].y > 1.0f) playerBullets.erase(playerBullets.begin() + i);
        }

        // Update enemies
        updateEnemies();

        // Update enemy bullets
        for (size_t i = 0; i < enemies.size(); ++i) {
            for (size_t j = 0; j < enemies[i].bullets.size(); ++j) {
                enemies[i].bullets[j].y -= enemyBulletSpeed;
                if (enemies[i].bullets[j].y < -1.0f) enemies[i].bullets.erase(enemies[i].bullets.begin() + j);
            }
        }

        // Check for collisions
        checkCollisions();
    }

    // If level progression is needed
    if (enemiesDefeated >= level ){
        // Proceed to the next level
        level++;
        enemyspeedm+=0.2f;
        maxEnemiesAtLevel = level; // Increase max enemies per level
        enemiesDefeated = 0; // Reset the enemy counter

        // Spawn new enemies for the next level
        for (int i = 0; i < level; ++i) {
            addEnemy();
        }
    }
 
    // Spawn new enemies based on level
    if (enemies.size() < maxEnemiesAtLevel) {
        addEnemy();
    }

    glutTimerFunc(1000 / 60, update, 0);
    glutPostRedisplay();
}


// Keyboard function to control player
void keyboard(unsigned char key, int x, int y) {
    if (gameOver) {
        if (key == 27) {
            exit(0);  // Exit the game on 'Esc' key
        }
        if (key == 13) {
           gameOver=false;  // restart the game on 'Enter' key
           score=0;
           level=1;
           lives=3;
           enemies.clear();
           playerBullets.clear();
           enemiesDefeated=0;
           totalEnemiesDefeated=0;
		   playerX=0.0f;
		   playerY=-0.8f;
		   float enemyspeedm=0.7f;
		   
           
        }
        return;
    }

    // Player movement controls
    if (key == 'g'||key =='G') {
    	abel = true;
		
		}
		
		 // Fire bullets
    if (key == 32 && playerBullets.size() < maxBulletsAllowed) { // Space key
        Bullet newBullet = {playerX, playerY + 0.1f};
        playerBullets.push_back(newBullet);
    }
}
    void specialKeys(int key,int x,int y){
	switch (key){
		case GLUT_KEY_LEFT:
			if (playerX > -1.0f) playerX -= 0.05f;
			break;	
		case GLUT_KEY_RIGHT:
			if (playerX < 1.0f) playerX += 0.05f;
			break;
		case GLUT_KEY_UP:
			if (playerY < 0.9f) playerY += 0.05f;
			break;
		case GLUT_KEY_DOWN:
			if (playerY > -0.9f) playerY -= 0.05f;
			break;
	}

}


// Reshape function
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
}



// Main function
int main(int argc, char** argv) {
    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Fighter Jet Game");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyboard);
    
    glutTimerFunc(1000 / 60, update, 0);  // Start the update function

    glutMainLoop();

    return 0;
}

