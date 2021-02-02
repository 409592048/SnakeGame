/*
    2021.1.22 至 2021.1.26
    更新日志：
        新增欢迎界面，美化游戏。
    2021.1.31
    更新日志：
        解决食物出现时与蛇身重叠的bug               
*/
/*******头  文  件*******/
#include<stdio.h>			
#include<windows.h>			//控制dos界面
#include<stdlib.h>			//malloc、宏定义
#include<conio.h>			//接收键盘输入输出
#include<time.h>            //初始化随机数

/*******宏  定  义*******/
#define U 1
#define D 2
#define L 3 
#define R 4    //蛇的状态，U：上 ；D：下；L:左 R：右

/*******定  义  全  局  变  量 *******/
typedef struct snake{ 		//蛇身的一个节点
    int x;					//节点x坐标
    int y;					//节点y坐标
    struct snake *next;		//蛇身体下一节点
}snake;
int score=0;                //总得分
int add=10;			        //每次吃食物得分
int HighScore = 0;			//最高分
int status;					//蛇前进状态
int sleeptime=200;			//每次运行的时间间隔
snake *head=NULL;			//蛇头指针
snake *food=NULL;			//食物指针
snake *q=NULL;				//遍历蛇的时候用到的指针
int endgamestatus=0;		//游戏结束的情况，1：撞到墙；2：咬到自己；3：主动退出游戏
HANDLE hOut;				//控制台句柄

/*******函  数  声  明 *******/
void gotoxy(int x,int y);   //设置光标位置
int color(int c);           //更改文字颜色
void welcometogame();       //开始界面
void pointSnake();          //绘制初始界面的蛇
void createMap();           //绘制地图
void initsnake();           //初始化蛇身，画蛇身
void createfood();          //创建并随机出现食物
int biteself();             //判断是否咬到了自己
void cantcrosswall();       //设置蛇撞墙的情况
void speedup();				//加速
void speeddown();			//减速
void snakemove();           //控制蛇前进方向
void keyboardControl();     //控制键盘按键
void endgame();             //游戏结束
void choose();				//游戏失败之后的选择
void File_out();            //在文件中读取最高分
void File_in();            	//储存最高分进文件
void scoreandtips();		//游戏界面右侧的得分和小提示

/*
    绘制初始界面的蛇
*/
void pointSnake(){
    color(8);
    printf("                  ┃    ┃                                               \n");
    printf("                  ╋    ╋                                               \n");
    printf("                  ┗╋╋╋╋┛                                               \n");
    printf("                   ╋╋╋╋                                                \n");
    printf("                  ╋╋╋╋╋╋                                               \n");
    printf("                 ╋╋╋╋╋╋╋╋                                              \n");
    printf("                ╋╋╋╋╋╋╋╋╋╋                                             \n");
    printf("               ╋╋╋╋╋╋╋╋╋╋╋╋                                            \n");
    printf("                 ╋╋╋╋╋╋╋╋                                              \n");
    printf("                  ╋╋╋╋╋╋╋╋                                             \n");
    printf("                   ╋╋╋╋╋╋╋╋                                            \n");
    printf("                    ╋╋╋╋╋╋╋╋                                           \n");
    printf("                     ╋╋╋╋╋╋╋╋                                          \n");
    printf("            ╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋                                         \n");
    printf("       ╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋                                          \n");
    printf("   ╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋                    ┃                            \n");
    printf("  ╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋                   ┏╋┓                           \n");
    printf("   ╋╋╋╋╋╋╋╋╋╋╋╋╋╋                       ╋╋╋╋┓                          \n");
    printf("    ┗╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋                 ╋╋╋╋╋╋┓                         \n");
    printf("      ┗╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋             ╋╋╋╋╋╋╋╋╋┓                        \n");
    printf("       ┗╋╋╋╋╋╋╋╋╋╋╋╋╋╋            ╋╋╋╋╋╋╋╋╋╋╋╋┫                        \n");
    printf("        ┗╋╋╋╋╋╋╋╋╋╋╋╋╋╋       ╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋┛                        \n");
    printf("          ┗╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋┛                         \n");
    printf("           ┗╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋┛                          \n");
    printf("             ┗╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋                                   \n");
    printf("==============================================================================================================");

}

/*
	设置光标位置
*/
void gotoxy(int x,int y){
    COORD cursor;
    cursor.X=x;
    cursor.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cursor);
}

/*
	文字颜色函数
*/
int color(int c){
	//SetConsoleTextAttribute是API设置控制台窗口字体颜色和背景色的函数
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);        //更改文字颜色
	return 0;
}

/*
	开始界面
*/
void welcometogame(){
    int n;
    gotoxy(42,3);
	color(11);
	printf("贪 吃 蛇 游 戏");
    color(3);
	gotoxy(37, 12);
	printf("1.开始游戏");
	gotoxy(57, 12);
	printf("2.退出游戏");
	gotoxy(35,27);
	color(9);
	printf("请选择[1 2]:[ ]\b\b");        //\b为退格，使得光标处于[]中间
	color(14);
    scanf("%d", &n);    		//输入选项
    switch (n)
    {
    	case 1:					//选择开始游戏
    		system("cls");
			createMap();        //创建地图
			initsnake();        //初始化蛇身
			createfood();		//初始化食物
			keyboardControl();	//控制键盘按钮
        	break;
    	case 2:					//选择退出游戏
        	exit(0);     		//退出游戏
        	break;
		default:				//输入非1~2之间的选项
			color(12);
			gotoxy(40,28);
			printf("请输入1~2!");
			getch();			//输入任意键
			system("cls");		//清屏
            pointSnake();
			welcometogame();
    }
}

/*
    当前得分
*/
void newScore(){
    gotoxy(64,7);
	color(14);
	printf("当前得分：%d",score);
	gotoxy(64,9);
	printf("每个食物得分：%d分",add);
}
/*
	游戏界面右侧的得分和小提示
*/
void scoreandtips(){
	File_out();				//调用File_out()，读取文件SnakeGamesave.txt中的内容
	gotoxy(64,4);			//确定打印输出的位置
	color(11);				//设置颜色
	printf("◆最高记录◆：%d",HighScore);	//打印最高分
	gotoxy(64,7);
	color(14);
	printf("当前得分：%d  ",score);
	color(15);
	gotoxy(73,11);
	printf("小 提 示");
	gotoxy(60,13);
	printf("*************************************");
	gotoxy(60,25);
	printf("*************************************");
    color(6);
	gotoxy(64,15);
	printf("不能撞墙，不能咬到自己");
	gotoxy(64,17);
	printf("用↑ ↓ ← →分别控制蛇的移动");
	gotoxy(64,19);
	printf("F1键加速，F2键减速");
	gotoxy(64,21);
	printf("空格键暂停游戏");
	gotoxy(64,23);
    printf("Esc键退出游戏");
}

/*
	创建地图
*/
void createMap(){
    int i,j;
    for(i=0;i<58;i+=2){		//打印上下边框
        gotoxy(i,0);
		color(6);			//橙色的边框
        printf("□");
        gotoxy(i,26);
        printf("□");
    }
    for(i=1;i<26;i++){		//打印左右边框
        gotoxy(0,i);
        printf("□");                        
        gotoxy(56,i);
        printf("□");        
    }
	for(i = 2;i<56;i+=2){	//打印中间网格
		for(j = 1;j<26;j++){
			gotoxy(i,j);
			color(8);
			printf("■\n\n");
		}
	}
}

/*
	初始化蛇身，画蛇身
*/
void initsnake(){
    snake *tail;
    int i;
    tail=(snake*)malloc(sizeof(snake));	//从蛇尾开始，头插法，以x,y设定开始的位置
    tail->x=20;				//蛇的初始位置（20,8）
    tail->y=8;
    tail->next=NULL;
    for(i=1;i<=2;i++){       //设置蛇身，长度为3
        head=(snake*)malloc(sizeof(snake)); //初始化蛇头
        head->next=tail;    //蛇头的下一位为蛇尾
        head->x=20+2*i;     //设置蛇头位置
        head->y=8;
        tail=head;          //蛇头变成蛇尾，然后重复循环
    }
    while(tail!=NULL){		//从头到尾，输出蛇身
        gotoxy(tail->x,tail->y);
		color(14);
        printf("●");       //输出蛇身，蛇身使用●组成
        tail=tail->next;    //蛇头输出完毕，输出蛇头的下一位，一直输出到蛇尾
    }
}

/*
	随机出现食物
*/
void createfood(){
    srand((unsigned)time(NULL));        	        //初始化随机数
    snake *food_1=(snake*)malloc(sizeof(snake));    //初始化food_1
    while((food_1->x%2)!=0){  				        //保证其为偶数，使得食物能与蛇头对齐，然后食物会出现在网格线上
        food_1->x=rand()%52+2;                      //食物随机出现，食物的x坐标在2~53
    }
    food_1->y=rand()%24+1;					        //食物的y坐标在1~24
    q=head;
    while(q->next==NULL){
        if(q->x==food_1->x && q->y==food_1->y){     //判断蛇身是否与食物重合
            free(food_1);                           //如果蛇身和食物重合，那么释放食物指针
            createfood();                           //重新创建食物
        }
        q=q->next;
    }
    gotoxy(food_1->x,food_1->y);
    food=food_1;
	color(3);
    printf("◆");                                    //输出食物
}

/*
	判断是否咬到了自己
*/
int biteself(){
    snake *self=head->next;                          //先定义一个除蛇头之外蛇身的一个节点
    while(self!=NULL){
        if(self->x==head->x && self->y==head->y){    //如果self和蛇身上的节点重合
            return 1;                                //返回1,咬中
        }
        self=self->next;
    }
    return 0;
}

/*
	设置蛇撞墙的情况
*/
void cantcrosswall(){  
    if(head->x==0 || head->x==56 ||head->y==0 || head->y==26){ //蛇头碰到了墙壁
        endgamestatus=1;        //返回第一种情况
        endgame();              //出现游戏结束界面
    }
}

/*
	加速，蛇吃到食物会自动提速，按F1加速，得分+2/次
*/
void speedup(){
	if(sleeptime>=100){
		sleeptime=sleeptime-10;
		add=add+2;
    }
}

/*
	减速，按F2减速，得分-2/次
*/
void speeddown(){
	if(sleeptime<350){               //如果时间间隔小于350
        sleeptime=sleeptime+30;     //时间间隔加上30
        add=add-2;                  //每吃一次食物的得分减2
    }
}

/*
   位置转变
*/
void MOVE(snake *nexthead){
        nexthead->next=head;
        head=nexthead;
        q=head;						//指针q指向蛇头
        if(nexthead->x==food->x && nexthead->y==food->y){	//如果下一个有食物 下一个位置的坐标和食物的坐标相同
            while(q!=NULL){
                gotoxy(q->x,q->y);
				color(14);
                printf("●");       //原来食物的位置，从◆换成●
                q=q->next;          //指针q指向的蛇身的下一位也执行循环里的操作
            }
            score+=add;        //吃了一个食物，在总分上加上食物的分
			speedup();
            createfood();           //创建食物
        }else{
            while(q->next->next!=NULL){	//如果没遇到食物
                gotoxy(q->x,q->y);
                color(14);
                printf("●");           //蛇正常往前走，输出当前位置的蛇身
                q=q->next;              //继续输出整个蛇身
            }
            gotoxy(q->next->x,q->next->y);  //经过上面的循环，q指向蛇尾，蛇尾的下一位，就是蛇走过去的位置
			color(8);
            printf("■");
            free(q->next);			//进行输出■之后，释放指向下一位的指针
            q->next=NULL;			//指针下一位指向空
        }
}

/*
	控制方向
*/
void snakemove(){	//蛇前进,上U,下D,左L,右R
	snake * nexthead=(snake*)malloc(sizeof(snake));		//为下一步开辟空间
    cantcrosswall();
    if(status==U){
        nexthead->x=head->x;        //向上前进时，x坐标不动，y坐标-1
        nexthead->y=head->y-1;
        MOVE(nexthead);
    }
    if(status==D){
        nexthead->x=head->x;        //向下前进时，x坐标不动，y坐标+1
        nexthead->y=head->y+1;
        MOVE(nexthead);
    }
    if(status==L){
        nexthead->x=head->x-2;        //向左前进时，x坐标向左移动-2，y坐标不动
        nexthead->y=head->y;
        MOVE(nexthead);
    }
    if(status==R){
        nexthead->x=head->x+2;        //向右前进时，x坐标向右移动+2，y坐标不动
        nexthead->y=head->y;
        MOVE(nexthead);
    }
    if(biteself()==1){       //判断是否会咬到自己
        endgamestatus=2;
        endgame();
    }
}

/*
	控制键盘按键
*/
void keyboardControl(){
	status=R;       //初始蛇向右移动
    scoreandtips(); //打印提示栏
    while(1){
        newScore(); //打印当前得分
        if(GetAsyncKeyState(VK_UP) && status!=D){            //GetAsyncKeyState函数用来判断函数调用时指定虚拟键的状态
            status=U;                                        //如果蛇不是向下前进的时候，按上键，执行向上前进操作
        }
        else if(GetAsyncKeyState(VK_DOWN) && status!=U){     //如果蛇不是向上前进的时候，按下键，执行向下前进操作
            status=D;
        }
        else if(GetAsyncKeyState(VK_LEFT)&& status!=R){      //如果蛇不是向右前进的时候，按左键，执行向左前进
            status=L;
        }
        else if(GetAsyncKeyState(VK_RIGHT)&& status!=L){     //如果蛇不是向左前进的时候，按右键，执行向右前进
            status=R;
        }
        if(GetAsyncKeyState(VK_SPACE)){		//按暂停键，执行pause暂停函数
            while(1){
				Sleep(300); //sleep()函数，头文件#include <unistd.h>  令进程暂停，直到达到里面设定的参数的时间
				if(GetAsyncKeyState(VK_SPACE)){      //按空格键暂停
					break;
				}
			}       
        }
        else if(GetAsyncKeyState(VK_ESCAPE)){
            endgamestatus=3;    //按esc键，直接到结束界面
            break;
        }
        else if(GetAsyncKeyState(VK_F1)){    //按F1键，加速
            speedup();
        }
        else if(GetAsyncKeyState(VK_F2)){    //按F2键，减速
        	speeddown();
        }
        Sleep(sleeptime);
        snakemove();
    }
}

/*
	储存最高分进文件
*/
void File_in(){
	FILE *fp;
	fp = fopen("SnakeGamesave.txt", "w+");       //以读写的方式建立一个名为SnakeGamesave.txt的文件
	fprintf(fp, "%d", score);           //把分数写进文件中
	fclose(fp);                         //关闭文件
}

/*
	在文件中读取最高分
*/
void File_out(){
	FILE *fp;
	fp = fopen("SnakeGamesave.txt", "a+");       //打开文件SnakeGamesave.txt
	fscanf(fp, "%d", &HighScore);       //把文件中的最高分读出来
	fclose(fp);                         //关闭文件
}

/*
	结束游戏
*/
void endgame()
{
    system("cls");
    if(endgamestatus==1)
    {
		gotoxy(35,9);
    	color(7);
		printf("对不起，您撞到墙了。游戏结束！");
    }
    else if(endgamestatus==2){
        gotoxy(35,9);
    	color(7);
        printf("对不起，您咬到自己了。游戏结束！");
    }
    else if(endgamestatus==3){
		gotoxy(40,9);
    	color(7);
        printf("您已经结束了游戏。");
    }
    gotoxy(43,12);
    color(14);
    printf("您的得分是 %d",score);

	if(score >= HighScore){
		color(10);
		gotoxy(33,16);
		printf("创纪录啦！最高分被你刷新啦!");
		File_in();              //把最高分写进文件
	}
	else{
		color(10);
		gotoxy(34,16);
		printf("继续努力吧~ 你离最高分还差：%d",HighScore-score);
	}
	choose();
}

/*
	边框下面的分支选项
*/
void choose(){
	int n;
	gotoxy(30,23);
	color(12);
	printf("重玩一局 [1]");
	gotoxy(55,23);
	printf("退出游戏 [2]");
	gotoxy(45,25);
	color(11);
	printf("选择：");
	scanf("%d", &n);
    switch (n){
	case 1:
		system("cls");          //清屏
		score=0;                //分数归零
		sleeptime=200;			//设定初始速度
		add = 10;				//使add设定为初值，吃一个食物得分10，然后累加
        pointSnake();
		welcometogame();
		break;
	case 2:
		exit(0);                //退出游戏
		break;
	default:
		gotoxy(35,27);
		color(12);
		printf("※※您的输入有误，请重新输入※※");
		system("pause >nul");
		endgame();
		choose();
		break;
	}
}

/*
	主函数
*/
int main(){
	system("mode con cols=110 lines=30");	//设置控制台宽高
    File_out();								//读取分数信息
    pointSnake();                           //绘制蛇身
	welcometogame();						//游戏开始
	keyboardControl();						//控制键盘按钮
	endgame();								//游戏结束
	return 0;
}