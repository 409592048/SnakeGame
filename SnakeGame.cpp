/*
    2021.1.22 �� 2021.1.26
    ������־��
        ������ӭ���棬������Ϸ��
    2021.1.31
    ������־��
        ���ʳ�����ʱ�������ص���bug               
*/
/*******ͷ  ��  ��*******/
#include<stdio.h>			
#include<windows.h>			//����dos����
#include<stdlib.h>			//malloc���궨��
#include<conio.h>			//���ռ����������
#include<time.h>            //��ʼ�������

/*******��  ��  ��*******/
#define U 1
#define D 2
#define L 3 
#define R 4    //�ߵ�״̬��U���� ��D���£�L:�� R����

/*******��  ��  ȫ  ��  ��  �� *******/
typedef struct snake{ 		//�����һ���ڵ�
    int x;					//�ڵ�x����
    int y;					//�ڵ�y����
    struct snake *next;		//��������һ�ڵ�
}snake;
int score=0;                //�ܵ÷�
int add=10;			        //ÿ�γ�ʳ��÷�
int HighScore = 0;			//��߷�
int status;					//��ǰ��״̬
int sleeptime=200;			//ÿ�����е�ʱ����
snake *head=NULL;			//��ͷָ��
snake *food=NULL;			//ʳ��ָ��
snake *q=NULL;				//�����ߵ�ʱ���õ���ָ��
int endgamestatus=0;		//��Ϸ�����������1��ײ��ǽ��2��ҧ���Լ���3�������˳���Ϸ
HANDLE hOut;				//����̨���

/*******��  ��  ��  �� *******/
void gotoxy(int x,int y);   //���ù��λ��
int color(int c);           //����������ɫ
void welcometogame();       //��ʼ����
void pointSnake();          //���Ƴ�ʼ�������
void createMap();           //���Ƶ�ͼ
void initsnake();           //��ʼ������������
void createfood();          //�������������ʳ��
int biteself();             //�ж��Ƿ�ҧ�����Լ�
void cantcrosswall();       //������ײǽ�����
void speedup();				//����
void speeddown();			//����
void snakemove();           //������ǰ������
void keyboardControl();     //���Ƽ��̰���
void endgame();             //��Ϸ����
void choose();				//��Ϸʧ��֮���ѡ��
void File_out();            //���ļ��ж�ȡ��߷�
void File_in();            	//������߷ֽ��ļ�
void scoreandtips();		//��Ϸ�����Ҳ�ĵ÷ֺ�С��ʾ

/*
    ���Ƴ�ʼ�������
*/
void pointSnake(){
    color(8);
    printf("                  ��    ��                                               \n");
    printf("                  ��    ��                                               \n");
    printf("                  ������奔                                               \n");
    printf("                   �����                                                \n");
    printf("                  �������                                               \n");
    printf("                 ���������                                              \n");
    printf("                �����������                                             \n");
    printf("               �������������                                            \n");
    printf("                 ���������                                              \n");
    printf("                  ���������                                             \n");
    printf("                   ���������                                            \n");
    printf("                    ���������                                           \n");
    printf("                     ���������                                          \n");
    printf("            �������������������                                         \n");
    printf("       �����������������������                                          \n");
    printf("   ��������������������                    ��                            \n");
    printf("  ���������������������                   ���勺                           \n");
    printf("   ���������������                       ����勺                          \n");
    printf("    ��������������������                 ������勺                         \n");
    printf("      ��������������������             ���������勺                        \n");
    printf("       �����������������            ��������������                        \n");
    printf("        �����������������       ����������������奔                        \n");
    printf("          ������������������������������������奔                         \n");
    printf("           ����������������������������������奔                          \n");
    printf("             �������������������������                                   \n");
    printf("==============================================================================================================");

}

/*
	���ù��λ��
*/
void gotoxy(int x,int y){
    COORD cursor;
    cursor.X=x;
    cursor.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cursor);
}

/*
	������ɫ����
*/
int color(int c){
	//SetConsoleTextAttribute��API���ÿ���̨����������ɫ�ͱ���ɫ�ĺ���
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);        //����������ɫ
	return 0;
}

/*
	��ʼ����
*/
void welcometogame(){
    int n;
    gotoxy(42,3);
	color(11);
	printf("̰ �� �� �� Ϸ");
    color(3);
	gotoxy(37, 12);
	printf("1.��ʼ��Ϸ");
	gotoxy(57, 12);
	printf("2.�˳���Ϸ");
	gotoxy(35,27);
	color(9);
	printf("��ѡ��[1 2]:[ ]\b\b");        //\bΪ�˸�ʹ�ù�괦��[]�м�
	color(14);
    scanf("%d", &n);    		//����ѡ��
    switch (n)
    {
    	case 1:					//ѡ��ʼ��Ϸ
    		system("cls");
			createMap();        //������ͼ
			initsnake();        //��ʼ������
			createfood();		//��ʼ��ʳ��
			keyboardControl();	//���Ƽ��̰�ť
        	break;
    	case 2:					//ѡ���˳���Ϸ
        	exit(0);     		//�˳���Ϸ
        	break;
		default:				//�����1~2֮���ѡ��
			color(12);
			gotoxy(40,28);
			printf("������1~2!");
			getch();			//���������
			system("cls");		//����
            pointSnake();
			welcometogame();
    }
}

/*
    ��ǰ�÷�
*/
void newScore(){
    gotoxy(64,7);
	color(14);
	printf("��ǰ�÷֣�%d",score);
	gotoxy(64,9);
	printf("ÿ��ʳ��÷֣�%d��",add);
}
/*
	��Ϸ�����Ҳ�ĵ÷ֺ�С��ʾ
*/
void scoreandtips(){
	File_out();				//����File_out()����ȡ�ļ�SnakeGamesave.txt�е�����
	gotoxy(64,4);			//ȷ����ӡ�����λ��
	color(11);				//������ɫ
	printf("����߼�¼����%d",HighScore);	//��ӡ��߷�
	gotoxy(64,7);
	color(14);
	printf("��ǰ�÷֣�%d  ",score);
	color(15);
	gotoxy(73,11);
	printf("С �� ʾ");
	gotoxy(60,13);
	printf("*************************************");
	gotoxy(60,25);
	printf("*************************************");
    color(6);
	gotoxy(64,15);
	printf("����ײǽ������ҧ���Լ�");
	gotoxy(64,17);
	printf("�á� �� �� ���ֱ�����ߵ��ƶ�");
	gotoxy(64,19);
	printf("F1�����٣�F2������");
	gotoxy(64,21);
	printf("�ո����ͣ��Ϸ");
	gotoxy(64,23);
    printf("Esc���˳���Ϸ");
}

/*
	������ͼ
*/
void createMap(){
    int i,j;
    for(i=0;i<58;i+=2){		//��ӡ���±߿�
        gotoxy(i,0);
		color(6);			//��ɫ�ı߿�
        printf("��");
        gotoxy(i,26);
        printf("��");
    }
    for(i=1;i<26;i++){		//��ӡ���ұ߿�
        gotoxy(0,i);
        printf("��");                        
        gotoxy(56,i);
        printf("��");        
    }
	for(i = 2;i<56;i+=2){	//��ӡ�м�����
		for(j = 1;j<26;j++){
			gotoxy(i,j);
			color(8);
			printf("��\n\n");
		}
	}
}

/*
	��ʼ������������
*/
void initsnake(){
    snake *tail;
    int i;
    tail=(snake*)malloc(sizeof(snake));	//����β��ʼ��ͷ�巨����x,y�趨��ʼ��λ��
    tail->x=20;				//�ߵĳ�ʼλ�ã�20,8��
    tail->y=8;
    tail->next=NULL;
    for(i=1;i<=2;i++){       //������������Ϊ3
        head=(snake*)malloc(sizeof(snake)); //��ʼ����ͷ
        head->next=tail;    //��ͷ����һλΪ��β
        head->x=20+2*i;     //������ͷλ��
        head->y=8;
        tail=head;          //��ͷ�����β��Ȼ���ظ�ѭ��
    }
    while(tail!=NULL){		//��ͷ��β���������
        gotoxy(tail->x,tail->y);
		color(14);
        printf("��");       //�����������ʹ�á����
        tail=tail->next;    //��ͷ�����ϣ������ͷ����һλ��һֱ�������β
    }
}

/*
	�������ʳ��
*/
void createfood(){
    srand((unsigned)time(NULL));        	        //��ʼ�������
    snake *food_1=(snake*)malloc(sizeof(snake));    //��ʼ��food_1
    while((food_1->x%2)!=0){  				        //��֤��Ϊż����ʹ��ʳ��������ͷ���룬Ȼ��ʳ����������������
        food_1->x=rand()%52+2;                      //ʳ��������֣�ʳ���x������2~53
    }
    food_1->y=rand()%24+1;					        //ʳ���y������1~24
    q=head;
    while(q->next==NULL){
        if(q->x==food_1->x && q->y==food_1->y){     //�ж������Ƿ���ʳ���غ�
            free(food_1);                           //��������ʳ���غϣ���ô�ͷ�ʳ��ָ��
            createfood();                           //���´���ʳ��
        }
        q=q->next;
    }
    gotoxy(food_1->x,food_1->y);
    food=food_1;
	color(3);
    printf("��");                                    //���ʳ��
}

/*
	�ж��Ƿ�ҧ�����Լ�
*/
int biteself(){
    snake *self=head->next;                          //�ȶ���һ������ͷ֮�������һ���ڵ�
    while(self!=NULL){
        if(self->x==head->x && self->y==head->y){    //���self�������ϵĽڵ��غ�
            return 1;                                //����1,ҧ��
        }
        self=self->next;
    }
    return 0;
}

/*
	������ײǽ�����
*/
void cantcrosswall(){  
    if(head->x==0 || head->x==56 ||head->y==0 || head->y==26){ //��ͷ������ǽ��
        endgamestatus=1;        //���ص�һ�����
        endgame();              //������Ϸ��������
    }
}

/*
	���٣��߳Ե�ʳ����Զ����٣���F1���٣��÷�+2/��
*/
void speedup(){
	if(sleeptime>=100){
		sleeptime=sleeptime-10;
		add=add+2;
    }
}

/*
	���٣���F2���٣��÷�-2/��
*/
void speeddown(){
	if(sleeptime<350){               //���ʱ����С��350
        sleeptime=sleeptime+30;     //ʱ��������30
        add=add-2;                  //ÿ��һ��ʳ��ĵ÷ּ�2
    }
}

/*
   λ��ת��
*/
void MOVE(snake *nexthead){
        nexthead->next=head;
        head=nexthead;
        q=head;						//ָ��qָ����ͷ
        if(nexthead->x==food->x && nexthead->y==food->y){	//�����һ����ʳ�� ��һ��λ�õ������ʳ���������ͬ
            while(q!=NULL){
                gotoxy(q->x,q->y);
				color(14);
                printf("��");       //ԭ��ʳ���λ�ã��ӡ����ɡ�
                q=q->next;          //ָ��qָ����������һλҲִ��ѭ����Ĳ���
            }
            score+=add;        //����һ��ʳ����ܷ��ϼ���ʳ��ķ�
			speedup();
            createfood();           //����ʳ��
        }else{
            while(q->next->next!=NULL){	//���û����ʳ��
                gotoxy(q->x,q->y);
                color(14);
                printf("��");           //��������ǰ�ߣ������ǰλ�õ�����
                q=q->next;              //���������������
            }
            gotoxy(q->next->x,q->next->y);  //���������ѭ����qָ����β����β����һλ���������߹�ȥ��λ��
			color(8);
            printf("��");
            free(q->next);			//���������֮���ͷ�ָ����һλ��ָ��
            q->next=NULL;			//ָ����һλָ���
        }
}

/*
	���Ʒ���
*/
void snakemove(){	//��ǰ��,��U,��D,��L,��R
	snake * nexthead=(snake*)malloc(sizeof(snake));		//Ϊ��һ�����ٿռ�
    cantcrosswall();
    if(status==U){
        nexthead->x=head->x;        //����ǰ��ʱ��x���겻����y����-1
        nexthead->y=head->y-1;
        MOVE(nexthead);
    }
    if(status==D){
        nexthead->x=head->x;        //����ǰ��ʱ��x���겻����y����+1
        nexthead->y=head->y+1;
        MOVE(nexthead);
    }
    if(status==L){
        nexthead->x=head->x-2;        //����ǰ��ʱ��x���������ƶ�-2��y���겻��
        nexthead->y=head->y;
        MOVE(nexthead);
    }
    if(status==R){
        nexthead->x=head->x+2;        //����ǰ��ʱ��x���������ƶ�+2��y���겻��
        nexthead->y=head->y;
        MOVE(nexthead);
    }
    if(biteself()==1){       //�ж��Ƿ��ҧ���Լ�
        endgamestatus=2;
        endgame();
    }
}

/*
	���Ƽ��̰���
*/
void keyboardControl(){
	status=R;       //��ʼ�������ƶ�
    scoreandtips(); //��ӡ��ʾ��
    while(1){
        newScore(); //��ӡ��ǰ�÷�
        if(GetAsyncKeyState(VK_UP) && status!=D){            //GetAsyncKeyState���������жϺ�������ʱָ���������״̬
            status=U;                                        //����߲�������ǰ����ʱ�򣬰��ϼ���ִ������ǰ������
        }
        else if(GetAsyncKeyState(VK_DOWN) && status!=U){     //����߲�������ǰ����ʱ�򣬰��¼���ִ������ǰ������
            status=D;
        }
        else if(GetAsyncKeyState(VK_LEFT)&& status!=R){      //����߲�������ǰ����ʱ�򣬰������ִ������ǰ��
            status=L;
        }
        else if(GetAsyncKeyState(VK_RIGHT)&& status!=L){     //����߲�������ǰ����ʱ�򣬰��Ҽ���ִ������ǰ��
            status=R;
        }
        if(GetAsyncKeyState(VK_SPACE)){		//����ͣ����ִ��pause��ͣ����
            while(1){
				Sleep(300); //sleep()������ͷ�ļ�#include <unistd.h>  �������ͣ��ֱ���ﵽ�����趨�Ĳ�����ʱ��
				if(GetAsyncKeyState(VK_SPACE)){      //���ո����ͣ
					break;
				}
			}       
        }
        else if(GetAsyncKeyState(VK_ESCAPE)){
            endgamestatus=3;    //��esc����ֱ�ӵ���������
            break;
        }
        else if(GetAsyncKeyState(VK_F1)){    //��F1��������
            speedup();
        }
        else if(GetAsyncKeyState(VK_F2)){    //��F2��������
        	speeddown();
        }
        Sleep(sleeptime);
        snakemove();
    }
}

/*
	������߷ֽ��ļ�
*/
void File_in(){
	FILE *fp;
	fp = fopen("SnakeGamesave.txt", "w+");       //�Զ�д�ķ�ʽ����һ����ΪSnakeGamesave.txt���ļ�
	fprintf(fp, "%d", score);           //�ѷ���д���ļ���
	fclose(fp);                         //�ر��ļ�
}

/*
	���ļ��ж�ȡ��߷�
*/
void File_out(){
	FILE *fp;
	fp = fopen("SnakeGamesave.txt", "a+");       //���ļ�SnakeGamesave.txt
	fscanf(fp, "%d", &HighScore);       //���ļ��е���߷ֶ�����
	fclose(fp);                         //�ر��ļ�
}

/*
	������Ϸ
*/
void endgame()
{
    system("cls");
    if(endgamestatus==1)
    {
		gotoxy(35,9);
    	color(7);
		printf("�Բ�����ײ��ǽ�ˡ���Ϸ������");
    }
    else if(endgamestatus==2){
        gotoxy(35,9);
    	color(7);
        printf("�Բ�����ҧ���Լ��ˡ���Ϸ������");
    }
    else if(endgamestatus==3){
		gotoxy(40,9);
    	color(7);
        printf("���Ѿ���������Ϸ��");
    }
    gotoxy(43,12);
    color(14);
    printf("���ĵ÷��� %d",score);

	if(score >= HighScore){
		color(10);
		gotoxy(33,16);
		printf("����¼������߷ֱ���ˢ����!");
		File_in();              //����߷�д���ļ�
	}
	else{
		color(10);
		gotoxy(34,16);
		printf("����Ŭ����~ ������߷ֻ��%d",HighScore-score);
	}
	choose();
}

/*
	�߿�����ķ�֧ѡ��
*/
void choose(){
	int n;
	gotoxy(30,23);
	color(12);
	printf("����һ�� [1]");
	gotoxy(55,23);
	printf("�˳���Ϸ [2]");
	gotoxy(45,25);
	color(11);
	printf("ѡ��");
	scanf("%d", &n);
    switch (n){
	case 1:
		system("cls");          //����
		score=0;                //��������
		sleeptime=200;			//�趨��ʼ�ٶ�
		add = 10;				//ʹadd�趨Ϊ��ֵ����һ��ʳ��÷�10��Ȼ���ۼ�
        pointSnake();
		welcometogame();
		break;
	case 2:
		exit(0);                //�˳���Ϸ
		break;
	default:
		gotoxy(35,27);
		color(12);
		printf("�����������������������������");
		system("pause >nul");
		endgame();
		choose();
		break;
	}
}

/*
	������
*/
int main(){
	system("mode con cols=110 lines=30");	//���ÿ���̨���
    File_out();								//��ȡ������Ϣ
    pointSnake();                           //��������
	welcometogame();						//��Ϸ��ʼ
	keyboardControl();						//���Ƽ��̰�ť
	endgame();								//��Ϸ����
	return 0;
}