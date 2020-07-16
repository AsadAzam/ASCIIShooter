#include<stdio.h>
#include<windows.h>
#include<time.h>
char grid[20][41]=                                    			   //arena
                      {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", // X: wall
                       "X          c                           X", // c: cpu
                       "X                                      X", // u: user
                       "X                                      X", // b: user's bullet
                       "X                                      X", // B: cpu's bullet
                       "X                                      X", // boomerang(array): boomerang
                       "X                                      X", // wall(array): user's wall
                       "X                                      X",
                       "X                                      X",
                       "X                                      X",
                       "X                                      X",
                       "X                                      X",
                       "X                                      X",
                       "X                                      X",
                       "X                                      X",
                       "X                                      X",
                       "X                                      X",
                       "X                                      X",
                       "X          u                           X",
                       "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"};

//Function Declarations

void titlescreen(void);
void printbuffer(void);
void win(int);
void printborder(int,int,COORD,int);
void score(int,int,int,int,int,int,int);
void printscoreboard(void);
void screenflash(void);

//Main Program

int main()
{
    int i,j,k,upos=11,cpos=11,cmove,winner=0,uhealth=10,chealth=40,bullets=500,qtyofboomerang=20,qtyofwall=50,boost=0,nofboost=1;
    int boomerang[4]={188,187,201,200},wall[4]={219,178,177,176};
    clock_t start=-1,reloadtime=-1;
    SetConsoleTitle("Showdown");
    printf("Controls:\n'a': Left\n'd': Right\n'w': Fire Bullet\n'o': Fire Homing Boomerang\n'p': Place a Wall\n'i': Engage Triple Bullet Firing Mode\n");
    system("pause");
    titlescreen();
    system("cls");
    printbuffer();
    printscoreboard();
    getch();
    srand(time(NULL));         //Seeds Random Numbers Using System Time
    while(1)
    {
        //User's Turn
        if(kbhit()) //listens if User Pressed a Key
        {
            switch(getch()) //Takes the Key Input
            {
            case 'w':
                if(bullets>0)
                {
                    if(boost==0)
                    {
                        grid[17][upos]='b';           //Fires a Bullet
                        bullets--;
                    }
                    else
                    {
                        if(bullets>2)
                        {
                            for(i=-1;i<=1;i++)
                            {
                                if(upos+i>0 && upos+i<39)
                                {
                                    grid[17][upos+i]='b';          //Fires 3 Bullets Via Weapon Boost
                                    bullets--;
                                }
                            }
                        }
                    }
                }
                break;
            
			case 'a':              //Moves User Left
                if(upos>1)
                {
                    grid[18][upos]=' ';
                    upos=upos-1;
                    grid[18][upos]='u';
                }
                break;
            
			case 'd':             //Moves User Right
                if(upos<38)
                {
                    grid[18][upos]=' ';
                    upos=upos+1;
                    grid[18][upos]='u';
                }
                break;
            
			case 'o':
                if(qtyofboomerang>0)
                {
                    grid[17][upos]=boomerang[0];      //Fires Homing Boomerang
                    qtyofboomerang--;
                }
                break;
            
			case 'p':
                if(qtyofwall>0)
                {
                    grid[16][upos]=219;                 //Sets Up A Wall
                    qtyofwall--;
                }
                break;
            
			case 'i':
                if(nofboost>0)
                {boost=1;                              //Starts Weapon Boost
                start=clock();
                nofboost--;
                }
                break;
            }
        }
        
        //CPU's Turn
        
        cmove=rand()%4;        //Randomizing CPU's Move
        switch(cmove)
        {
        case 0:
            if(chealth>10)
                grid[2][cpos]='B';    //Fires a Bullet
            else
                {
                    for(i=-1;i<=1;i++)
                        {
                            if(cpos+i>0 && cpos+i<39)
                                grid[2][cpos+i]='B';          //Fires 3 Bullets When Health Goes 10 or Below 10
                        }
                }
            break;
        case 1:
            if(cpos>1)            //Moves left
            {
                grid[1][cpos]=' ';
                cpos=cpos-1;
                grid[1][cpos]='c';
            }
            break;
        case 2:                 //Moves right
            if(cpos<38)
            {
                grid[1][cpos]=' ';
                cpos=cpos+1;
                grid[1][cpos]='c';
            }
            break;
        }
        
        //Prints the Arena
        printbuffer();
      
	    //Prints Score
        score(uhealth,chealth,bullets,qtyofboomerang,qtyofwall,boost,nofboost);
        
		//Time Delay    
	    Sleep(70);
	    
        //User Bullets   
        for(i=0;i<20;i++)
        {
            for(j=0;j<40;j++)
            {
                if(grid[i][j]=='b' && i>1 && grid[i-1][j]==' ')
                {
                    grid[i][j]=' ';
                    grid[i-1][j]='b';             //Moves the Bullets Forward
                }
                if(grid[i][j]=='b' && i==1)
                    grid[i][j]=' ';   //Bullets Collision with Wall
                if(grid[i][j]=='b' && grid[i-1][j]=='B')
                {
                    grid[i][j]=' ';                      //Bullets Collision
                    grid[i-1][j]=' ';
                }
                if(grid[i][j]=='b' && grid[i-1][j]=='c')
                {
                    grid[i][j]=' ';              //Bullet Hits CPU
                    chealth--;
                    screenflash();
                }
                for(k=0;k<4;k++)
                {
                    if(grid[i][j]==boomerang[k]-256 && (grid[i-1][j]=='c' || grid[i-1][j-1]=='c' || grid[i-1][j+1]=='c'))
                    {
                        grid[i][j]=' ';              //Boomerang Hits CPU
                        chealth-=1;
                        screenflash();
                        screenflash();
                    }
                    if(grid[i][j]==boomerang[k]-256 && i>1 && grid[i-1][j]==' ')
                    {
                        grid[i][j]=' ';
                        if(j>cpos)
                            grid[i-1][j-1]=boomerang[(k+1)%4];         //Boomerang Moves Forward
                        if(j<cpos)
                            grid[i-1][j+1]=boomerang[(k+1)%4];           //Homing Boomerang
                        if(j==cpos)
                            grid[i-1][j]=boomerang[(k+1)%4];
                        break;
                    }
                    if(grid[i][j]==boomerang[k]-256 && grid[i-1][j]=='B')
                    {
                        grid[i][j]=' ';                      //Bullet and Boomerang Collision
                        grid[i-1][j]=boomerang[(k+1)%4];
                    }
                    if(grid[i][j]==boomerang[k]-256 && i==1)
                        grid[i][j]=' ';                       //Boomerang Hits Wall
                }
                for(k=0;k<4;k++)
                {
                    if(grid[i][j]=='b' && grid[i-1][j]==wall[k]-256)
                    {
                        grid[i][j]=' ';
                        grid[i-2][j]='b';                                           //Bullets Collision With User's Wall
                    }
                    if(grid[i][j]==boomerang[0]-256 && grid[i-1][j]==wall[k]-256)
                    {
                        grid[i][j]=' ';
                        grid[i-1][j]=' ';                                       //Boomerang Collision With User's Wall
                        grid[i-2][j]=boomerang[2];
                    }
                }
            }
        }
        
        //CPU bullets
        
        for(i=19;i>-1;i--)
        {
            for(j=0;j<40;j++)
            {
                if(grid[i][j]=='B' && i<18 && grid[i+1][j]==' ')
                {
                    grid[i][j]=' ';                //Moves the Bullets Forward
                    grid[i+1][j]='B';
                }
                if(grid[i][j]=='B' && i==18)
                    grid[i][j]=' ';                 //Bullets Collision with Wall
                if(grid[i][j]=='B' && grid[i+1][j]=='u')
                {
                    grid[i][j]=' ';           //Bullet Hits User
                    uhealth--;
                    screenflash();
                }
                for(k=0;k<4;k++)
                {
                    if(grid[i][j]=='B' && grid[i+1][j]==wall[k]-256)
                    {
                        grid[i][j]=' ';                             //CPU's Bullets Collision with User's Wall
                        if(k<3)
                            grid[i+1][j]=wall[k+1];
                        else
                            grid[i+1][j]=' ';
                    }
                }
            }
        }
        if(clock()-start>5000 && start>0)
        {
            boost=0;                                 //Stops the Weapon Boost
            start=-1;
            reloadtime=clock();
        }
        if(clock()-reloadtime>10000 && reloadtime>0)
        {
            if(nofboost==0)
                nofboost++;                              //Reloads The Weapon Boost
            reloadtime=-1;
        }
        if(chealth<=0)
        {
            winner=1;                               //User Wins
            break;
        }
        if(uhealth<=0 || (bullets<=0 && qtyofboomerang==0))
        {
            winner=2;                                            //CPU Wins
            break;
        }
    }
    win(winner);   //Selects the Winner. 0 : No Winner, 1 : User Wins, 2 : CPU Wins
    return 0; //End of Program
}

// Function Definitions

void titlescreen()         //Displays the Title Screen
{
	system("cls");
	printf("Welcome to Shooter Showdown \n");
	printf("Project by :     Asad   (18070122011) \n");
	system("pause");
}

void printbuffer()         //Prints the Arena
{
    int i,j,ucol=10,ccol=12,color=15;
    CHAR_INFO gridbuffer[800];
    COORD zerozero={0,0},gridsz={40,20};
    SMALL_RECT gridrect={10,2,50,22};
    HANDLE out;
    out=GetStdHandle(STD_OUTPUT_HANDLE);
    for(i=0;i<20;i++)
    {
        for(j=0;j<40;j++)
        {
            gridbuffer[j+40*i].Attributes=color;
            if(grid[i][j]=='X')
                gridbuffer[j+40*i].Char.AsciiChar=219;
            else if(grid[i][j]=='u')
            {
                gridbuffer[j+40*i].Char.AsciiChar=1;
                gridbuffer[j+40*i].Attributes=ucol;
            }
            else if(grid[i][j]==' ')
                gridbuffer[j+40*i].Char.AsciiChar=' ';
            else if(grid[i][j]=='b')
                gridbuffer[j+40*i].Char.AsciiChar=30;
            else if(grid[i][j]=='B')
                gridbuffer[j+40*i].Char.AsciiChar=31;
            else if(grid[i][j]=='c')
            {
                gridbuffer[j+40*i].Char.AsciiChar=2;
                gridbuffer[j+40*i].Attributes=ccol;
            }
            else
            {
                gridbuffer[j+40*i].Char.AsciiChar=grid[i][j];
            }
        }
    }
    WriteConsoleOutput(out,gridbuffer,gridsz,zerozero,&gridrect);
}

void win(int winner) //Prints Winning Message
{
	system("cls");
    COORD wintextpos={37,12};
    switch(winner)
    {
    case 1:
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),wintextpos);
        printf("You win!\n");
        Sleep(10000);
        break;
    case 2:
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),wintextpos);
        printf("You lose!\n");
        Sleep(10000);
        break;
    }
}

void printborder(int _length,int _width,COORD _coordinates,int _color)     //Border Printing Function
{
    int i,j;
    COORD zerozero={0,0},bordersz;
    SMALL_RECT _rect;
    CHAR_INFO _border[_length*_width];
    HANDLE out;
    out=GetStdHandle(STD_OUTPUT_HANDLE);
    bordersz.X=_length;
    bordersz.Y=_width;
    _rect.Left=_coordinates.X;
    _rect.Top=_coordinates.Y;
    _rect.Right=_rect.Left+_length;
    _rect.Bottom=_rect.Top+_width;
    for(i=0;i<_width;i++)
    {
        for(j=0;j<_length;j++)
        {
            if(i==0 || i==_width-1)
            {
                _border[j+_length*i].Char.AsciiChar=205;
                _border[j+_length*i].Attributes=_color;
                continue;
            }
            if(j==0 || j==_length-1)
            {
                _border[j+_length*i].Char.AsciiChar=186;
                _border[j+_length*i].Attributes=_color;
                continue;
            }
            _border[j+_length*i].Char.AsciiChar=' ';
            _border[j+_length*i].Attributes=_color;
        }
    }
    _border[0].Char.AsciiChar=201;
    _border[_length-1].Char.AsciiChar=187;
    _border[_length*_width - 1].Char.AsciiChar=188;
    _border[_length*(_width-1)].Char.AsciiChar=200;
    _border[0].Attributes=_color;
    _border[_length-1].Attributes=_color;
    _border[_length*_width - 1].Attributes=_color;
    _border[_length*(_width-1)].Attributes=_color;
    WriteConsoleOutput(out,_border,bordersz,zerozero,&_rect);
}

void score(int uhealth,int chealth,int ubullets,int uboomerang,int uwall,int boost,int nofboost)   //Prints Score
{
    HANDLE out;
    out=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD ctext={68,4},utext={68,16},utext1={68,17},utext2={68,18},utext3={68,19},utext4={68,20};
    SetConsoleCursorPosition(out,ctext);
    printf("   ");
    SetConsoleCursorPosition(out,utext);
    printf("   ");
    SetConsoleCursorPosition(out,utext1);
    printf("   ");
    SetConsoleCursorPosition(out,utext2);
    printf("  ");
    SetConsoleCursorPosition(out,utext3);
    printf("  ");
    SetConsoleCursorPosition(out,utext4);
    printf("   ");
    SetConsoleCursorPosition(out,ctext);
    printf("%d",chealth);
    SetConsoleCursorPosition(out,utext);
    printf("%d",uhealth);+ 
    SetConsoleCursorPosition(out,utext1);
    printf("%d",ubullets);
    SetConsoleCursorPosition(out,utext2);
    printf("%d",uboomerang);
    SetConsoleCursorPosition(out,utext3);
    printf("%d",uwall);
    if(nofboost==1 && boost==0)
    {
        SetConsoleCursorPosition(out,utext4);
        SetConsoleTextAttribute(out,12);
        printf("OFF");
        SetConsoleTextAttribute(out,7);
    }
    else if(nofboost==0 && boost==1)
    {
        SetConsoleCursorPosition(out,utext4);
        SetConsoleTextAttribute(out,10);
        printf("ON");
        SetConsoleTextAttribute(out,7);
    }
    else if(nofboost==0 && boost==0)
    {
        SetConsoleCursorPosition(out,utext4);
        SetConsoleTextAttribute(out,14);
        printf("...");
        SetConsoleTextAttribute(out,7);
    }
}

void printscoreboard()    //Prints Scoreboard
{
    COORD cpuscoresz={55,3},userscoresz={55,15},ctextpos={56,4},utextpos={56,16};
    HANDLE out;
    out=GetStdHandle(STD_OUTPUT_HANDLE);
    printborder(18,3,cpuscoresz,15);
    printborder(18,7,userscoresz,15);
    SetConsoleCursorPosition(out,ctextpos);
    printf("CPU health: ");
    SetConsoleCursorPosition(out,utextpos);
    printf("USER health: ");
    utextpos.Y++;
    SetConsoleCursorPosition(out,utextpos);
    printf("Bullets: ");
    utextpos.Y++;
    SetConsoleCursorPosition(out,utextpos);
    printf("Boomerang: ");
    utextpos.Y++;
    SetConsoleCursorPosition(out,utextpos);
    printf("Wall: ");
    utextpos.Y++;
    SetConsoleCursorPosition(out,utextpos);
    printf("Boost: ");
}

void screenflash()      //Flashes the Screen
{
    char str[9]="color   ";
    char sym[17]="0123456789ABCDEF";
    str[6]=sym[rand()%17];
    str[7]=sym[rand()%17];
    system(str);
    Sleep(3);
    str[6]=sym[rand()%17];
    str[7]=sym[rand()%17];
    system(str);
    Sleep(3);
    str[6]=sym[rand()%17];
    str[7]=sym[rand()%17];
    system(str);
    Sleep(3);
    system("color 07");
}
