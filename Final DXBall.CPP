/*
		Computer Graphics Mini Project:
			DX-BALL
		by:
		Nikit Batale -07CE1090 -A1
		Runal Bhabal -07CE1016 -A1
		Rahul Awasthi-07CE1024 -A1
		Sameer Gadne -08CE5001 -A1

*/

#include<iostream.h>
#include<conio.h>
#include<graphics.h>
#include<dos.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

class Ball{
public:
	int cx,cy,leftx,lefty,rightx,righty,upx,upy,downx,downy;
	int upleftx,uplefty,uprightx,uprighty,downleftx,downlefty,downrightx,downrighty;
	int ulimit,llimit,rlimit;
	int xIncre,yIncre;
	int lives;
	int radius;

	//removes the ball from previous frame
	void removePreviousBall(){
		setcolor(0);
		setfillstyle(1,0);
		fillellipse(abs(cx),abs(cy),radius,radius);
		setfillstyle(1,15);
		setcolor(15);
	}

	//draws the ball on the screen as per cx and cy	
	void drawBall(){
		fillellipse(abs(cx),abs(cy),radius,radius);
	}
};

class Bar{
public:
	int left,right,up,down,bar[8],size,movement;
	int llimit,rlimit;

	//draws the bar as per the current location of the bar
	void drawBar(){
	fillpoly(4,bar);
	}

	//removes the bar for the previous frame
	void removePreviousBar(){
		setcolor(0);
		setfillstyle(1,0);
		fillpoly(4,bar);
		setfillstyle(1,15);
		setcolor(15);
	}

	//devices the new position co-ordinates for the bar
	void rePositionBar(int ch){
		up=460;

		if(ch==0)
			left=210;
		right=left+size;
		if(ch=='a'&&left>llimit)
			left=left-movement;
		if(ch=='s'&&right<rlimit)
			left=left+movement;
		bar[0]=bar[6]=left;
		bar[2]=bar[4]=left+size;
		bar[1]=bar[3]=up;
		bar[7]=bar[5]=up+20;
	}
};

class Game{
public:
	int erase[8],score;
	char scoreString[5],livesString[2],livesRemain[3];
	int isBrick[20][8];
	Ball b;
	Bar br;
	int brickLen,brickBred;

	//draws the bricks that are currently alive
	//with reference to the array isBricks
	void drawBricks(){
		int brick[8];
		int k=1;
		setcolor(15);
		for(int i=0;i<20;i++){
			for(int j=0;j<8;j++){
				brick[0]=brick[6]=i*brickLen;
				brick[1]=brick[3]=j*brickBred;
				brick[2]=brick[4]=brick[0]+brickLen;
				brick[5]=brick[7]=brick[1]+brickBred;
				if(k==16)
					k=1;
				setfillstyle(1,k);
				if(isBrick[i][j]==1)
					fillpoly(4,brick);
				k++;
			}
			k=1;
		}
		setcolor(15);
	}

	//initializes the postion variables for ball
	void initializeVariables(){
		b.cx=b.cx+b.xIncre;
		b.cy=b.cy+b.yIncre;
		b.lefty=b.righty=abs(b.cy);
		b.leftx=abs(b.cx)-b.radius;
		b.rightx=abs(b.cx)+b.radius;
		b.upx=b.downx=abs(b.cx);
		b.upy=abs(b.cy)-b.radius;
		b.downy=abs(b.cy)+b.radius;
		b.downleftx=abs(b.cx)-b.radius*sin(0.785);
		b.downlefty=abs(b.cy)-b.radius*cos(0.785);
		b.downrightx=abs(b.cx)+b.radius*sin(0.785);
		b.downrighty=abs(b.cy)+b.radius*cos(0.785);
		if((b.downrightx==br.left&&b.downrighty==br.up)||(b.downlefty==br.up&&b.downleftx==br.right)){
			b.cx=b.cx*(-1);
			b.cy=b.cy*(-1);
		}
		if(abs(b.leftx)==b.llimit||abs(b.rightx)==b.rlimit)
			b.cx=b.cx*(-1);
		if(((abs(b.downy))==br.up)&&(abs(b.downx)>=br.left&&abs(b.downx)<=br.right))
			b.cy=b.cy*(-1);
		if(abs(b.upy)==b.ulimit)
			b.cy=b.cy*(-1);
		if(abs(b.downy>480)){
			settextstyle(0,0,1);
			b.lives--;
			if(b.lives<0){
				settextstyle(0,0,2);
				outtextxy(230,240,"Game Over");
				getch();
				displayAbout();
				exit(0);
			}
			setLivesString();
			outtextxy(200,240,livesRemain);
			changeLives();
			b.cx=20;
			b.cy=240;
			br.removePreviousBar();
			br.rePositionBar(0);
			br.drawBar();
			setcolor(0);
			setfillstyle(1,0);
			getch();
			erase[0]=erase[6]=200;
			erase[1]=erase[3]=240;
			erase[2]=erase[4]=400;
			erase[5]=erase[7]=260;
			fillpoly(4,erase);
			setcolor(15);
			setfillstyle(1,15);
		}

	}

	//touch function determine the action to be 
	//taken when a brick is touched
	void touchLeft(){
		b.cx=b.cx*(-1);
	}
	void touchRight(){
		b.cx=b.cx*(-1);
	}
	void touchUp(){
		b.cy=b.cy*(-1);
	}
	void touchDown(){
		b.cy=b.cy*(-1);
	}
	void touchCorner(){
		b.cx=b.cx*(-1);
		b.cy=b.cy*(-1);
	}

	//determines whether the ball touches the brick
	//and takes the necessary action
	void whetherBallTouchesBrick(){
		for(int i=0;i<20;i++){
			for(int j=0;j<8;j++){
				if(isBrick[i][j]==1&&b.cy<240){
					int touchCx=abs(b.cx+5);
					int touchCy=abs(b.cy+5);
					int brickLeft=i*brickLen;
					int brickRight=brickLeft+brickLen;
					int brickUp=j*brickBred;
					int brickDown=brickUp+brickBred;
					if((touchCx==brickRight)&&(abs(b.upy-brickUp)<=abs(brickBred-b.radius)&&abs(b.downy-brickDown)<=abs(brickBred-b.radius))){
						touchLeft();
						isBrick[i][j]=0;
					}
					else if((touchCx==brickLeft)&&(abs(b.upy-brickUp)<=abs(brickBred-b.radius)&&abs(b.downy-brickDown)<=abs(brickBred-b.radius))){
						touchRight();
						isBrick[i][j]=0;
					}
					else if((touchCy==brickDown)&&(abs(b.leftx-brickLeft)<=abs(brickLen-b.radius)&&abs(b.rightx-brickRight)<=abs(brickLen-b.radius))){
						touchUp();
						isBrick[i][j]=0;
					}
					else if((touchCy==brickUp)&&(abs(b.leftx-brickLeft)<=abs(brickLen-b.radius)&&abs(b.rightx-brickRight)<=abs(brickLen-b.radius))){
						touchDown();
						isBrick[i][j]=0;
					}
					else if((abs(abs(b.cx)-brickLeft)+abs(abs(b.cx)-brickRight)==brickLen)&&(abs(abs(b.cy)-brickUp)+abs(abs(b.cy)-brickDown)==brickBred)){
						touchCorner();
						isBrick[i][j]=0;
					}

					if(isBrick[i][j]==0){
						score=score+(10*abs(j-8));
						setScoreString();
						int brick[8];
						setcolor(0);
						brick[0]=brick[6]=brickLeft;
						brick[1]=brick[3]=brickUp;
						brick[2]=brick[4]=brickRight;
						brick[5]=brick[7]=brickDown;
						setfillstyle(1,0);
						fillpoly(4,brick);
						rectangle(brick[0],brick[1],brick[2],brick[5]);
						drawBricks();
						i=0,j=0;
					}
				}
			}
		}
	}

	//setString and change functions update score and lives display
	void setScoreString(){
		itoa(score,scoreString,10);
	}
	void setLivesString(){
		livesString[1]='\0';
		livesString[0]=48+b.lives;
		livesRemain[0]=48+b.lives;
	}
	void changeScore(){
		erase[0]=erase[6]=535;
		erase[1]=erase[3]=230;
		erase[2]=erase[4]=620;
		erase[5]=erase[7]=270;
		setcolor(0);
		setfillstyle(1,0);
		fillpoly(4,erase);
		setcolor(15);
		setfillstyle(1,15);
		settextstyle(0,0,2);
		outtextxy(550,230,scoreString);
	}
	void changeLives(){
		erase[0]=erase[6]=535;
		erase[1]=erase[3]=350;
		erase[2]=erase[4]=600;
		erase[5]=erase[7]=390;
		setcolor(0);
		setfillstyle(1,0);
		fillpoly(4,erase);
		setcolor(15);
		setfillstyle(1,15);
		settextstyle(0,0,2);
		outtextxy(560,350,livesString);
	}

	//displays the list of Programmers in this project
	void displayAbout(){
		cleardevice();
		settextstyle(0,0,1);
		char q=48;
		do{
			outtextxy(280,200,"Programmed By:");
			outtextxy(280,220,"Nikit Batale");
			outtextxy(280,230,"Rahul Awasthi");
			outtextxy(280,240,"Runal Bhabal");
			outtextxy(280,250,"Sameer Gadne");
			outtextxy(250,470,"Press Space to Exit");
			q=getch();
		}
		while(q!=' ');
	}

	//function which centrally controls the execution of game
	void gameHandler(){
		settextstyle(0,0,6);
		outtextxy(180,200,"DX-Ball");
		getch();
		clearviewport();
		brickLen=25,brickBred=15;
		for(int i=0;i<20;i++)
			for(int j=0;j<8;j++)
				isBrick[i][j]=1;
		drawBricks();
		b.cx=20;
		b.cy=240;
		b.xIncre=1;
		b.yIncre=1;
		b.radius=5;
		br.size=80;
		br.movement=10;
		score=0;
		livesRemain[0]=' ';
		livesRemain[1]=' ';
		strcat(livesRemain,"live(s) remaining.");
		b.lives=3;
		line(500,0,500,480);
		settextstyle(0,0,2);
		outtextxy(535,200,"Score:");
		settextstyle(0,0,2);
		setScoreString();
		outtextxy(535,230,scoreString);
		outtextxy(535,320,"Lives:");
		setLivesString();
		outtextxy(565,350,livesString);
		b.ulimit=0;
		b.llimit=0;
		b.rlimit=500;
		br.llimit=0;
		br.rlimit=500;
		br.rePositionBar(0);
		br.drawBar();
		int ch='a';
		while(1){
			while(!kbhit()){
			delay(4);
			b.removePreviousBall();
			changeScore();
			initializeVariables();
			whetherBallTouchesBrick();
			b.drawBall();
			br.removePreviousBar();
			br.drawBar();
			line(500,0,500,480);
			}
			ch=getch();
			if(ch=='a'){
				br.removePreviousBar();
				br.rePositionBar('a');
				br.drawBar();
			}
			if(ch=='s'){
				br.removePreviousBar();
				br.rePositionBar('s');
				br.drawBar();
			}
			if(ch=='p')
				ch=getch();
			if(ch=='q'){
				displayAbout();
				getch();
				exit(0);
			}
		}
	}
};

void main(){
	int gDriver=DETECT,gMode;
	detectgraph(&gDriver,&gMode);
	initgraph(&gDriver,&gMode,"");
	Game g;
	g.gameHandler();
	getch();
}
