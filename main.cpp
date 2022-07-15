#include <iostream>
#include <vector>
#include <unistd.h>
#include <fstream>
#include <termios.h>

#define sizeX 20
#define sizeY 20
#define Table std::vector<std::vector<bool>>

static struct termios old, current;

void initTermios(int echo) 
{
  tcgetattr(0, &old);
  current = old;
  current.c_lflag &= ~ICANON;
  if (echo) {
      current.c_lflag |= ECHO;
  } else {
      current.c_lflag &= ~ECHO;
  }
  tcsetattr(0, TCSANOW, &current);
}

void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

char getch(void) 
{
  return getch_(0);
}

void showInputTable(Table& table, int x, int y){
    for(int i=0;i<5;i++)std::cout<<std::endl;
    std::cout<<"[ using w,a,s,d move the coursor and using [ENTER] change state of choosen cell, press [ESC] to exit table creator ]"<<std::endl;
    for(int i=0;i<sizeX;i++){
        std::cout<<std::string(10,' ');
        for(int j=0;j<sizeY;j++){
            
            if(x==i && y==j) std::cout<<"▮"<<(table[i][j] ? "●" : " ")<<"]";
            else std::cout<<"["<<(table[i][j] ? "●" : " ")<<"]";
        }
        std::cout<<std::endl;
    }
}

void getTable(Table &table){
    char input;
    
    system("clear");
    
    int x=0;
    int y=0; 
    showInputTable(table,x,y);
    input=getch();
    if(input==(char)13){
        table[x][y]=!table[x][y];
    }
    else if(input=='w'){
        if(y>0)x--;
    }
    else if(input=='a'){
        if(x>0)y--;
    }
    else if(input=='s'){
        if(y<sizeY-1)x++;
    }
    else if(input=='d'){
        if(x<sizeX-1)y++;
    }

    system("clear");
    while(input!=(char)27){
        showInputTable(table,x,y);
        std::cout<<"["<<(input==(char)10 ? 'E' : input)<<"]"<<std::endl;
        input=getch();
        
        if(input==(char)10){
            if(table[x][y])table[x][y]=false;
            else table[x][y]=true;
        }
        else if(input=='w'){
            if(y>0)x--;
        }
        else if(input=='a'){
            if(x>0)y--;
        }
        else if(input=='s'){
            if(y<sizeY-1)x++;
        }
        else if(input=='d'){
            if(x<sizeX-1)y++;
        }
        
        
        system("clear");
    }
}

int getAliveNeighbours(Table& table, int x, int y){
    int out=0;
    out+=table[x-1][y];
    out+=table[x+1][y];
    out+=table[x-1][y-1];
    out+=table[x][y-1];
    out+=table[x+1][y-1];
    out+=table[x-1][y+1];
    out+=table[x][y+1];
    out+=table[x+1][y+1];
    return out;
}

void isAlive(Table &tableInput, Table &tableOutput){
    for(int i=1;i<sizeX-1;i++){
        for(int j=1;j<sizeY-1;j++){
            int n=getAliveNeighbours(tableInput, i, j);
            if((n==2 || n==3) && tableInput[i][j])tableOutput[i][j]=1;
            else if(n==3 && !tableInput[i][j])tableOutput[i][j]=1;
            else tableOutput[i][j]=0;
        }
    }
    return;
}

void showTable(Table &table){
    std::cout<<"     ■■■■■ ■■■■■ ■■ ■■ ■■■■■    ■■■■■ ■■■■■    ■     ■ ■■■■■ ■■■■■"<<std::endl;
    std::cout<<"     ■     ■   ■ ■ ■ ■ ■        ■   ■ ■        ■     ■ ■     ■    "<<std::endl;
    std::cout<<"     ■  ■■ ■■■■■ ■   ■ ■■■      ■   ■ ■■■      ■     ■ ■■■   ■■■  "<<std::endl;
    std::cout<<"     ■   ■ ■   ■ ■   ■ ■        ■   ■ ■        ■     ■ ■     ■    "<<std::endl;
    std::cout<<"     ■■■■■ ■   ■ ■   ■ ■■■■■    ■■■■■ ■        ■■■■■ ■ ■     ■■■■■"<<std::endl;
    std::cout<<std::string(10,' ');
    std::cout<<std::string(2*sizeY+2, '=')<<std::endl;
    for(auto& i:table){
        std::cout<<std::string(10,' ');
        std::cout<<'[';
        for(auto j:i)std::cout<<(j ? "■ " : "  ");
        std::cout<<']'<<std::endl;
    }
    std::cout<<std::string(10,' ');
    std::cout<<std::string(2*sizeY+2, '=')<<std::endl;
    return;
}

void gameOfLife(){
    Table table1(sizeX, std::vector<bool>(sizeY, 0));
    bool whichTable=0;
    getTable(table1);
    Table table2=table1;
    while(true){
        if(whichTable){
            showTable(table1);
            isAlive(table1,table2);
            whichTable=!whichTable;
        }
        else{
            showTable(table2);
            isAlive(table2,table1);
            whichTable=!whichTable;
        }
        usleep(250000);
        system("clear");
    }
}

int main(){
    std::ios_base::sync_with_stdio(false);
    gameOfLife();
    return 0;
}