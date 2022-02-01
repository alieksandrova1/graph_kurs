#include <winbgim.h>
#include <stdio.h>
#include <math.h>
#include <dos.h>
#include <windows.h>
#define defcolour WHITE
#define PI M_PI//3.14159265
int BCUBE = BLACK;
int BSCUBE = LIGHTGRAY;
//                    0   1   2   3   4   5   6   7
double   cubex[8] = {700, 700, 700, 700, 750, 750, 750, 750},
         cubey[8] = {600, 600, 650, 650, 600, 600, 650, 650},
         cubez[8] = {700, 750, 700, 750, 700, 750, 700, 750},cubec[3],
//
         tetx[4] = {800,900,850, 843.3},
         tety[4] = {683.3,683.3,683.3,620},
         tetz[4] = {800,800,843.3, 820}, tetc[3], comc[3],

         scubex[8] = {700, 700, 700, 700, 750, 750, 750, 750},
         scubey[8] = {600, 600, 650, 650, 600, 600, 650, 650},
         scubez[8] = {700, 750, 700, 750, 700, 750, 700, 750},scubec[3],
//
         stetx[4] = {800,900,850, 843.3},
         stety[4] = {683.3,683.3,683.3,620},
         stetz[4] = {800,800,843.3, 820}, stetc[3], scomc[3];

int K = 1200000000, koef = 1667, center = 250, er = 1;

/*     a ----------------- b                        |\ A
        |                 |                         | \
        |                 |                         |  \
        |       *e        |                       ac|   \ab
        |                 |                         |    \
        |                 |                         | *D  \
        |                 |                         |      \
       d ----------------- c                       C|_______\B
                                                        bc


       1----------5                                      3          012 - RED
       |\         |\                                    /|\         123 - BLUE
       | \          \                                  /   \        013 - YELLOW
       |  0----------4                                /  |  \       023 - LIGHTBLUE
       |  |       |  |                               /   2   \      line 1 = 01 line 2 = 02
       3 -|- - - -7  |                              /   / \   \     line 3 = 03 line 4 = 12
        \ |        \ |                             /           \    line 5 = 13 line 6 = 23
         \|          |                            / /         \ \
          2----------6                           0_______________1    */

//считает площадь треугольника по трем сторонам
double trisquare(double a,double b, double c)
{
    double p = (a+b+c)/2;
    return sqrt(p*(p-a)*(p-b)*(p-c));
}

//определяет, лежит ли точка в треугольнике
isintriangle(double ax, double ay,double bx,double by,double cx,double cy,double dx,double dy)
{

    double ab = sqrt((ax-bx)*(ax-bx)+(ay-by)*(ay-by)), bc = sqrt((cx-bx)*(cx-bx)+(cy-by)*(cy-by)), ac = sqrt((ax-cx)*(ax-cx)+(ay-cy)*(ay-cy)), ad = sqrt((ax-dx)*(ax-dx)+(ay-dy)*(ay-dy)), bd = sqrt((bx-dx)*(bx-dx)+(by-dy)*(by-dy)), cd = sqrt((cx-dx)*(cx-dx)+(cy-dy)*(cy-dy));
    if (round(trisquare(ab, bc, ac)) >= round((trisquare(ab, ad, bd)+trisquare(ac, ad, cd)+trisquare(bc, cd, bd))) &&round(trisquare(ab, bc, ac))!=0) return true; else return false;
}


//Определяет, лежит ли точка в прямоугольнике
isinrectangle(double ax, double ay,double bx,double by,double cx,double cy,double dx,double dy, double ex, double ey)
{
    if ((isintriangle(ax, ay, bx, by, cx, cy, ex, ey) || isintriangle(ax, ay, dx, dy, cx, cy, ex, ey)))
            return true;
    else return false;
}
//функция закраски треугольника
floodtriangle(double x,double y, double ax, double ay, double bx, double by, double cx, double cy, int col)
{
    if (isintriangle(ax,ay,bx,by,cx,cy,x,y))
    {
        putpixel(x,y,col);
        if (getpixel (x,y+1)!=col)floodtriangle(x,y+1,ax,ay,bx,by,cx,cy,col);
        if (getpixel (x-1,y)!=col)floodtriangle(x-1,y,ax,ay,bx,by,cx,cy,col);
        if (getpixel (x+1,y)!=col)floodtriangle(x+1,y,ax,ay,bx,by,cx,cy,col);
        if (getpixel (x,y-1)!=col)floodtriangle(x,y-1,ax,ay,bx,by,cx,cy,col);
    }
}
//функция закраски прямоугольника
floodrectangle(double x,double y, double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy, int col, int basecol)
{
    floodtriangle((ax+bx+cx)/3, (ay+by+cy)/3, ax, ay, bx, by, cx, cy, col);
    floodtriangle((ax+dx+cx)/3, (ay+dy+cy)/3, ax, ay, dx, dy, cx, cy, col);
    setcolor(col);
    line(ax, ay, dx, dy);
    setcolor(basecol);
}
//Функция отрисовки куба
drawcube(int figcolor, bool toggle)
{
    setcolor(figcolor);
    //Определяем центр масс: Х, Y, и Z
    short i,j,k, temp, notneeded = 0;
    double //проекции  точек.
        cubexp[8] = {K*(cubex[0]-cubec[0]) / (cubez[0]-cubec[2]+K) + cubec[0], K*(cubex[1]-cubec[0]) / (cubez[1]-cubec[2]+K) + cubec[0], K*(cubex[2]-cubec[0]) / (cubez[2]-cubec[2]+K) + cubec[0],K*(cubex[3]-cubec[0]) / (cubez[3]-cubec[2]+K) + cubec[0], K*(cubex[4]-cubec[0]) / (cubez[4]-cubec[2]+K) + cubec[0],K*(cubex[5]-cubec[0]) / (cubez[5]-cubec[2]+K) + cubec[0],K*(cubex[6]-cubec[0]) / (cubez[6]-cubec[2]+K) + cubec[0],K*(cubex[7]-cubec[0]) / (cubez[7]-cubec[2]+K) + cubec[0]},
        cubeyp[8] = {K*(cubey[0]-cubec[0]) / (cubez[0]-cubec[2]+K) + cubec[0], K*(cubey[1]-cubec[0]) / (cubez[1]-cubec[2]+K) + cubec[0], K*(cubey[2]-cubec[0]) / (cubez[2]-cubec[2]+K) + cubec[0],K*(cubey[3]-cubec[0]) / (cubez[3]-cubec[2]+K) + cubec[0], K*(cubey[4]-cubec[0]) / (cubez[4]-cubec[2]+K) + cubec[0],K*(cubey[5]-cubec[0]) / (cubez[5]-cubec[2]+K) + cubec[0],K*(cubey[6]-cubec[0]) / (cubez[6]-cubec[2]+K) + cubec[0],K*(cubey[7]-cubec[0]) / (cubez[7]-cubec[2]+K) + cubec[0]};
        //Определяем ненужную точку таким образом: если координата Z максимальна, точку видно не будет
    for (i = 0; i<8; i++) if (cubez[i]>cubez[notneeded]) notneeded = i;
    //Отрисовываем ВЕСЬ кубик, кроме тех граней, которые примыкают к самой дальней по Z точке.
    if (notneeded !=0 && notneeded !=4 || toggle == 0) line(cubexp[0], cubeyp[0], cubexp[4], cubeyp[4]);
    if (notneeded !=0 && notneeded !=2 || toggle == 0) line(cubexp[0], cubeyp[0], cubexp[2], cubeyp[2]);
    if (notneeded !=0 && notneeded !=1 || toggle == 0) line(cubexp[0], cubeyp[0], cubexp[1], cubeyp[1]);
    if (notneeded !=1 && notneeded !=3 || toggle == 0) line(cubexp[1], cubeyp[1], cubexp[3], cubeyp[3]);
    if (notneeded !=1 && notneeded !=5 || toggle == 0) line(cubexp[1], cubeyp[1], cubexp[5], cubeyp[5]);
    if (notneeded !=2 && notneeded !=3 || toggle == 0) line(cubexp[2], cubeyp[2], cubexp[3], cubeyp[3]);
    if (notneeded !=2 && notneeded !=6 || toggle == 0) line(cubexp[2], cubeyp[2], cubexp[6], cubeyp[6]);
    if (notneeded !=3 && notneeded !=7 || toggle == 0) line(cubexp[3], cubeyp[3], cubexp[7], cubeyp[7]);
    if (notneeded !=4 && notneeded !=5 || toggle == 0) line(cubexp[4], cubeyp[4], cubexp[5], cubeyp[5]);
    if (notneeded !=4 && notneeded !=6 || toggle == 0) line(cubexp[4], cubeyp[4], cubexp[6], cubeyp[6]);
    if (notneeded !=5 && notneeded !=7 || toggle == 0) line(cubexp[5], cubeyp[5], cubexp[7], cubeyp[7]);
    if (notneeded !=6 && notneeded !=7 || toggle == 0) line(cubexp[6], cubeyp[6], cubexp[7], cubeyp[7]);
    //красим
    if (toggle)
    {
        if (notneeded !=0 && notneeded !=1 && notneeded !=2 && notneeded !=3) floodrectangle((cubexp[0]+cubexp[1]+cubexp[2]+cubexp[3])/4,(cubeyp[0]+cubeyp[1]+cubeyp[2]+cubeyp[3])/4, cubexp[0], cubeyp[0],cubexp[1], cubeyp[1],cubexp[3], cubeyp[3],cubexp[2], cubeyp[2],WHITE, BCUBE);
        if (notneeded !=0 && notneeded !=4 && notneeded !=6 && notneeded !=2) floodrectangle((cubexp[0]+cubexp[4]+cubexp[6]+cubexp[2])/4,(cubeyp[0]+cubeyp[4]+cubeyp[6]+cubeyp[2])/4, cubexp[0], cubeyp[0],cubexp[4], cubeyp[4],cubexp[6], cubeyp[6],cubexp[2], cubeyp[2],RED, BCUBE);
        if (notneeded !=3 && notneeded !=7 && notneeded !=6 && notneeded !=2) floodrectangle((cubexp[3]+cubexp[7]+cubexp[6]+cubexp[2])/4,(cubeyp[3]+cubeyp[7]+cubeyp[6]+cubeyp[2])/4, cubexp[3], cubeyp[3],cubexp[7], cubeyp[7],cubexp[6], cubeyp[6],cubexp[2], cubeyp[2],BROWN, BCUBE);
        if (notneeded !=0 && notneeded !=4 && notneeded !=1 && notneeded !=5) floodrectangle((cubexp[0]+cubexp[4]+cubexp[1]+cubexp[5])/4,(cubeyp[0]+cubeyp[4]+cubeyp[1]+cubeyp[5])/4, cubexp[0], cubeyp[0],cubexp[4], cubeyp[4],cubexp[5], cubeyp[5],cubexp[1], cubeyp[1],YELLOW, BCUBE);
        if (notneeded !=3 && notneeded !=7 && notneeded !=5 && notneeded !=1) floodrectangle((cubexp[3]+cubexp[7]+cubexp[5]+cubexp[1])/4,(cubeyp[3]+cubeyp[7]+cubeyp[5]+cubeyp[1])/4, cubexp[3], cubeyp[3],cubexp[7], cubeyp[7],cubexp[5], cubeyp[5],cubexp[1], cubeyp[1],GREEN, BCUBE);
        if (notneeded !=5 && notneeded !=4 && notneeded !=6 && notneeded !=7) floodrectangle((cubexp[5]+cubexp[4]+cubexp[6]+cubexp[7])/4,(cubeyp[5]+cubeyp[4]+cubeyp[6]+cubeyp[7])/4, cubexp[5], cubeyp[5],cubexp[4], cubeyp[4],cubexp[6], cubeyp[6],cubexp[7], cubeyp[7],LIGHTBLUE, BCUBE);
    }
}
//Функция отрисовки ТЕНИ куба, повторяющая функцию отрисовки куба. Связано повторение с тем, что переменные указаны глобально.
drawcubesh(int figcolor, bool toggle)
{
    setcolor(LIGHTGRAY);
    short i,j,k, temp, notneeded = 0;
    double
        scubexp[8] = {K*(scubex[0]-scubec[0]) / (scubez[0]-scubec[2]+K) + scubec[0], K*(scubex[1]-scubec[0]) / (scubez[1]-scubec[2]+K) + scubec[0], K*(scubex[2]-scubec[0]) / (scubez[2]-scubec[2]+K) + scubec[0],K*(scubex[3]-scubec[0]) / (scubez[3]-scubec[2]+K) + scubec[0], K*(scubex[4]-scubec[0]) / (scubez[4]-scubec[2]+K) + scubec[0],K*(scubex[5]-scubec[0]) / (scubez[5]-scubec[2]+K) + scubec[0],K*(scubex[6]-scubec[0]) / (scubez[6]-scubec[2]+K) + scubec[0],K*(scubex[7]-scubec[0]) / (scubez[7]-scubec[2]+K) + scubec[0]},
        scubeyp[8] = {K*(scubey[0]-scubec[0]) / (scubez[0]-scubec[2]+K) + scubec[0], K*(scubey[1]-scubec[0]) / (scubez[1]-scubec[2]+K) + scubec[0], K*(scubey[2]-scubec[0]) / (scubez[2]-scubec[2]+K) + scubec[0],K*(scubey[3]-scubec[0]) / (scubez[3]-scubec[2]+K) + scubec[0], K*(scubey[4]-scubec[0]) / (scubez[4]-scubec[2]+K) + scubec[0],K*(scubey[5]-scubec[0]) / (scubez[5]-scubec[2]+K) + scubec[0],K*(scubey[6]-scubec[0]) / (scubez[6]-scubec[2]+K) + scubec[0],K*(scubey[7]-scubec[0]) / (scubez[7]-scubec[2]+K) + scubec[0]};
    for (i = 0; i<8; i++) if (scubez[i]>scubez[notneeded]) notneeded = i;
    if (notneeded !=0 && notneeded !=4 || toggle == 0) line(scubexp[0], scubeyp[0], scubexp[4], scubeyp[4]);
    if (notneeded !=0 && notneeded !=2 || toggle == 0) line(scubexp[0], scubeyp[0], scubexp[2], scubeyp[2]);
    if (notneeded !=0 && notneeded !=1 || toggle == 0) line(scubexp[0], scubeyp[0], scubexp[1], scubeyp[1]);
    if (notneeded !=1 && notneeded !=3 || toggle == 0) line(scubexp[1], scubeyp[1], scubexp[3], scubeyp[3]);
    if (notneeded !=1 && notneeded !=5 || toggle == 0) line(scubexp[1], scubeyp[1], scubexp[5], scubeyp[5]);
    if (notneeded !=2 && notneeded !=3 || toggle == 0) line(scubexp[2], scubeyp[2], scubexp[3], scubeyp[3]);
    if (notneeded !=2 && notneeded !=6 || toggle == 0) line(scubexp[2], scubeyp[2], scubexp[6], scubeyp[6]);
    if (notneeded !=3 && notneeded !=7 || toggle == 0) line(scubexp[3], scubeyp[3], scubexp[7], scubeyp[7]);
    if (notneeded !=4 && notneeded !=5 || toggle == 0) line(scubexp[4], scubeyp[4], scubexp[5], scubeyp[5]);
    if (notneeded !=4 && notneeded !=6 || toggle == 0) line(scubexp[4], scubeyp[4], scubexp[6], scubeyp[6]);
    if (notneeded !=5 && notneeded !=7 || toggle == 0) line(scubexp[5], scubeyp[5], scubexp[7], scubeyp[7]);
    if (notneeded !=6 && notneeded !=7 || toggle == 0) line(scubexp[6], scubeyp[6], scubexp[7], scubeyp[7]);
    if (toggle)
    {
        if (notneeded !=0 && notneeded !=1 && notneeded !=2 && notneeded !=3) floodrectangle((scubexp[0]+scubexp[1]+scubexp[2]+scubexp[3])/4,(scubeyp[0]+scubeyp[1]+scubeyp[2]+scubeyp[3])/4, scubexp[0], scubeyp[0],scubexp[1], scubeyp[1],scubexp[3], scubeyp[3],scubexp[2], scubeyp[2],LIGHTGRAY, BSCUBE);
        if (notneeded !=0 && notneeded !=4 && notneeded !=6 && notneeded !=2) floodrectangle((scubexp[0]+scubexp[4]+scubexp[6]+scubexp[2])/4,(scubeyp[0]+scubeyp[4]+scubeyp[6]+scubeyp[2])/4, scubexp[0], scubeyp[0],scubexp[4], scubeyp[4],scubexp[6], scubeyp[6],scubexp[2], scubeyp[2],LIGHTGRAY, BSCUBE);
        if (notneeded !=3 && notneeded !=7 && notneeded !=6 && notneeded !=2) floodrectangle((scubexp[3]+scubexp[7]+scubexp[6]+scubexp[2])/4,(scubeyp[3]+scubeyp[7]+scubeyp[6]+scubeyp[2])/4, scubexp[3], scubeyp[3],scubexp[7], scubeyp[7],scubexp[6], scubeyp[6],scubexp[2], scubeyp[2],LIGHTGRAY, BSCUBE);
        if (notneeded !=0 && notneeded !=4 && notneeded !=1 && notneeded !=5) floodrectangle((scubexp[0]+scubexp[4]+scubexp[1]+scubexp[5])/4,(scubeyp[0]+scubeyp[4]+scubeyp[1]+scubeyp[5])/4, scubexp[0], scubeyp[0],scubexp[4], scubeyp[4],scubexp[5], scubeyp[5],scubexp[1], scubeyp[1],LIGHTGRAY, BSCUBE);
        if (notneeded !=3 && notneeded !=7 && notneeded !=5 && notneeded !=1) floodrectangle((scubexp[3]+scubexp[7]+scubexp[5]+scubexp[1])/4,(scubeyp[3]+scubeyp[7]+scubeyp[5]+scubeyp[1])/4, scubexp[3], scubeyp[3],scubexp[7], scubeyp[7],scubexp[5], scubeyp[5],scubexp[1], scubeyp[1],LIGHTGRAY, BSCUBE);
        if (notneeded !=5 && notneeded !=4 && notneeded !=6 && notneeded !=7) floodrectangle((scubexp[5]+scubexp[4]+scubexp[6]+scubexp[7])/4,(scubeyp[5]+scubeyp[4]+scubeyp[6]+scubeyp[7])/4, scubexp[5], scubeyp[5],scubexp[4], scubeyp[4],scubexp[6], scubeyp[6],scubexp[7], scubeyp[7],LIGHTGRAY, BSCUBE);
    }
    setcolor(figcolor);

}
//Считает среднее значение.
mid(double a,double b){return (a+b)/2;}
//Расстояние от точки до центра отрезка
len2mid(double xmid, double ymid, double x1, double y1, double x2, double y2){return sqrt( (xmid-mid(x1, x2))*(xmid-mid(x1, x2)) + (ymid-mid(y1, y2))*(ymid-mid(y1, y2)) );}
//Функция отрисовки тетраэдра
drawtet(int figcolor, bool toggle)
{
    setcolor(BLACK);
    //Определяем центр масс: Х, Y, и Z
    short i,j,k, temp, notneeded = 0, status = 5, excep = 0, finalexception;
    double excepsize = 10000,//проекции  точек.
        tetxp[4] = {K*(tetx[0]-tetc[0]) / (tetz[0]-tetc[2]+K) + tetc[0], K*(tetx[1]-tetc[0]) / (tetz[1]-tetc[2]+K) + tetc[0], K*(tetx[2]-tetc[0]) / (tetz[2]-tetc[2]+K) + tetc[0],K*(tetx[3]-tetc[0]) / (tetz[3]-tetc[2]+K) + tetc[0]},
        tetyp[4] = {K*(tety[0]-tetc[0]) / (tetz[0]-tetc[2]+K) + tetc[0], K*(tety[1]-tetc[0]) / (tetz[1]-tetc[2]+K) + tetc[0], K*(tety[2]-tetc[0]) / (tetz[2]-tetc[2]+K) + tetc[0],K*(tety[3]-tetc[0]) / (tetz[3]-tetc[2]+K) + tetc[0]},
        tetcp[2] = {(tetxp[0]+tetxp[1]+tetxp[2]+tetxp[3])/4,(tetyp[0]+tetyp[1]+tetyp[2]+tetyp[3])/4};
        if (toggle)
       {if (isintriangle(tetxp[1],tetyp[1],tetxp[2],tetyp[2],tetxp[3],tetyp[3],tetxp[0],tetyp[0])) status = 0;
        if (isintriangle(tetxp[0],tetyp[0],tetxp[2],tetyp[2],tetxp[3],tetyp[3],tetxp[1],tetyp[1])) status = 1;
        if (isintriangle(tetxp[1],tetyp[1],tetxp[0],tetyp[0],tetxp[3],tetyp[3],tetxp[2],tetyp[2])) status = 2;
        if (isintriangle(tetxp[1],tetyp[1],tetxp[2],tetyp[2],tetxp[0],tetyp[0],tetxp[3],tetyp[3])) status = 3;
        if (status == 5)
        {
        if (len2mid(tetcp[0],tetcp[1],tetxp[0], tetyp[0], tetxp[1], tetyp[1])< excepsize){excepsize = len2mid(tetcp[0],tetcp[1],tetxp[0], tetyp[0], tetxp[1], tetyp[1]); excep = 1;}
        if (len2mid(tetcp[0],tetcp[1],tetxp[0], tetyp[0], tetxp[2], tetyp[2])< excepsize){excepsize = len2mid(tetcp[0],tetcp[1],tetxp[0], tetyp[0], tetxp[2], tetyp[2]); excep = 2;}
        if (len2mid(tetcp[0],tetcp[1],tetxp[0], tetyp[0], tetxp[3], tetyp[3])< excepsize){excepsize = len2mid(tetcp[0],tetcp[1],tetxp[0], tetyp[0], tetxp[3], tetyp[3]); excep = 3;}
        if (excep == 1)
            {
                if(mid(tetz[0], tetz[1])<=mid(tetz[2], tetz[3]))    //012 013
                {
                    line (tetxp[0], tetyp[0], tetxp[1], tetyp[1]);
                    floodtriangle((tetxp[0]+tetxp[1]+tetxp[2])/3,(tetyp[0]+tetyp[1]+tetyp[2])/3,tetxp[0],tetyp[0],tetxp[1],tetyp[1],tetxp[2],tetyp[2],RED);
                    floodtriangle((tetxp[3]+tetxp[1]+tetxp[0])/3,(tetyp[3]+tetyp[1]+tetyp[0])/3,tetxp[3],tetyp[3],tetxp[1],tetyp[1],tetxp[0],tetyp[0],YELLOW);
                }
                else                                                //023 123
                {
                    line (tetxp[2], tetyp[2], tetxp[3], tetyp[3]);
                    floodtriangle((tetxp[3]+tetxp[0]+tetxp[2])/3,(tetyp[3]+tetyp[0]+tetyp[2])/3,tetxp[3],tetyp[3],tetxp[0],tetyp[0],tetxp[2],tetyp[2],LIGHTBLUE);
                    floodtriangle((tetxp[3]+tetxp[1]+tetxp[2])/3,(tetyp[3]+tetyp[1]+tetyp[2])/3,tetxp[3],tetyp[3],tetxp[1],tetyp[1],tetxp[2],tetyp[2],BLUE);
                }
                line (tetxp[0], tetyp[0], tetxp[2], tetyp[2]);
                line (tetxp[0], tetyp[0], tetxp[3], tetyp[3]);
                line (tetxp[1], tetyp[1], tetxp[2], tetyp[2]);
                line (tetxp[1], tetyp[1], tetxp[3], tetyp[3]);
            }
            if (excep == 2)
            {
                if(mid(tetz[0], tetz[2])<=mid(tetz[1], tetz[3]))    //012 023
                {
                    line (tetxp[0], tetyp[0], tetxp[2], tetyp[2]);
                    floodtriangle((tetxp[0]+tetxp[1]+tetxp[2])/3,(tetyp[0]+tetyp[1]+tetyp[2])/3,tetxp[0],tetyp[0],tetxp[1],tetyp[1],tetxp[2],tetyp[2],RED);
                    floodtriangle((tetxp[3]+tetxp[0]+tetxp[2])/3,(tetyp[3]+tetyp[0]+tetyp[2])/3,tetxp[3],tetyp[3],tetxp[0],tetyp[0],tetxp[2],tetyp[2],LIGHTBLUE);
                }
                else                                                //013 123
                {
                    line (tetxp[1], tetyp[1], tetxp[3], tetyp[3]);
                    floodtriangle((tetxp[0]+tetxp[1]+tetxp[3])/3,(tetyp[0]+tetyp[1]+tetyp[3])/3,tetxp[0],tetyp[0],tetxp[1],tetyp[1],tetxp[3],tetyp[3],YELLOW);
                    floodtriangle((tetxp[3]+tetxp[1]+tetxp[2])/3,(tetyp[3]+tetyp[1]+tetyp[2])/3,tetxp[3],tetyp[3],tetxp[1],tetyp[1],tetxp[2],tetyp[2],BLUE);
                }
                line (tetxp[0], tetyp[0], tetxp[1], tetyp[1]);
                line (tetxp[0], tetyp[0], tetxp[3], tetyp[3]);
                line (tetxp[1], tetyp[1], tetxp[2], tetyp[2]);
                line (tetxp[2], tetyp[2], tetxp[3], tetyp[3]);
            }
            if (excep == 3)
            {

                if(mid(tetz[0], tetz[3])<=mid(tetz[1], tetz[2]))    //023 013
                {
                    line (tetxp[0], tetyp[0], tetxp[3], tetyp[3]);
                    floodtriangle((tetxp[0]+tetxp[1]+tetxp[3])/3,(tetyp[0]+tetyp[1]+tetyp[3])/3,tetxp[0],tetyp[0],tetxp[1],tetyp[1],tetxp[3],tetyp[3],YELLOW);
                    floodtriangle((tetxp[3]+tetxp[0]+tetxp[2])/3,(tetyp[3]+tetyp[0]+tetyp[2])/3,tetxp[3],tetyp[3],tetxp[0],tetyp[0],tetxp[2],tetyp[2],LIGHTBLUE);
                }
                else                                                //012 123
                {
                    line (tetxp[1], tetyp[1], tetxp[2], tetyp[2]);
                    floodtriangle((tetxp[0]+tetxp[1]+tetxp[2])/3,(tetyp[0]+tetyp[1]+tetyp[2])/3,tetxp[0],tetyp[0],tetxp[1],tetyp[1],tetxp[2],tetyp[2],RED);
                    floodtriangle((tetxp[3]+tetxp[1]+tetxp[2])/3,(tetyp[3]+tetyp[1]+tetyp[2])/3,tetxp[3],tetyp[3],tetxp[1],tetyp[1],tetxp[2],tetyp[2],BLUE);

                }
                line (tetxp[0], tetyp[0], tetxp[1], tetyp[1]);
                line (tetxp[0], tetyp[0], tetxp[2], tetyp[2]);
                line (tetxp[1], tetyp[1], tetxp[3], tetyp[3]);
                line (tetxp[2], tetyp[2], tetxp[3], tetyp[3]);
            }
        }
        else
        {
            if (tetc[2]<tetz[status])//только передняя грань
            {
                if (status!=0 && status!=1) line(tetxp[0], tetyp[0], tetxp[1], tetyp[1]);
                if (status!=0 && status!=2) line(tetxp[0], tetyp[0], tetxp[2], tetyp[2]);
                if (status!=0 && status!=3) line(tetxp[0], tetyp[0], tetxp[3], tetyp[3]);
                if (status!=1 && status!=2) line(tetxp[1], tetyp[1], tetxp[2], tetyp[2]);
                if (status!=1 && status!=3) line(tetxp[1], tetyp[1], tetxp[3], tetyp[3]);
                if (status!=2 && status!=3) line(tetxp[2], tetyp[2], tetxp[3], tetyp[3]);
                if (status==3)floodtriangle((tetxp[0]+tetxp[1]+tetxp[2])/3,(tetyp[0]+tetyp[1]+tetyp[2])/3, tetx[0], tety[0], tetx[1], tety[1], tetx[2], tety[2], RED);
                if (status==2)floodtriangle((tetxp[0]+tetxp[1]+tetxp[3])/3,(tetyp[0]+tetyp[1]+tetyp[3])/3, tetx[0], tety[0], tetx[1], tety[1], tetx[3], tety[3], YELLOW);
                if (status==1)floodtriangle((tetxp[0]+tetxp[3]+tetxp[2])/3,(tetyp[0]+tetyp[3]+tetyp[2])/3, tetx[0], tety[0], tetx[3], tety[3], tetx[2], tety[2], LIGHTBLUE);
                if (status==0)floodtriangle((tetxp[3]+tetxp[1]+tetxp[2])/3,(tetyp[3]+tetyp[1]+tetyp[2])/3, tetx[3], tety[3], tetx[1], tety[1], tetx[2], tety[2], BLUE);
            }
            else //три грани
            {   line(tetxp[0], tetyp[0], tetxp[1], tetyp[1]);
                line(tetxp[0], tetyp[0], tetxp[2], tetyp[2]);
                line(tetxp[0], tetyp[0], tetxp[3], tetyp[3]);
                line(tetxp[1], tetyp[1], tetxp[2], tetyp[2]);
                line(tetxp[1], tetyp[1], tetxp[3], tetyp[3]);
                line(tetxp[2], tetyp[2], tetxp[3], tetyp[3]);
                if (status!=3)floodtriangle((tetxp[0]+tetxp[1]+tetxp[2])/3,(tetyp[0]+tetyp[1]+tetyp[2])/3, tetx[0], tety[0], tetx[1], tety[1], tetx[2], tety[2], RED);
                if (status!=2)floodtriangle((tetxp[0]+tetxp[1]+tetxp[3])/3,(tetyp[0]+tetyp[1]+tetyp[3])/3, tetx[0], tety[0], tetx[1], tety[1], tetx[3], tety[3], YELLOW);
                if (status!=1)floodtriangle((tetxp[0]+tetxp[3]+tetxp[2])/3,(tetyp[0]+tetyp[3]+tetyp[2])/3, tetx[0], tety[0], tetx[3], tety[3], tetx[2], tety[2], LIGHTBLUE);
                if (status!=0)floodtriangle((tetxp[3]+tetxp[1]+tetxp[2])/3,(tetyp[3]+tetyp[1]+tetyp[2])/3, tetx[3], tety[3], tetx[1], tety[1], tetx[2], tety[2], BLUE);

            }
        }
        }
        else {line (tetxp[0], tetyp[0], tetxp[1], tetyp[1]);
        line (tetxp[0], tetyp[0], tetxp[2], tetyp[2]);
        line (tetxp[0], tetyp[0], tetxp[3], tetyp[3]);
        line (tetxp[0], tetyp[0], tetxp[1], tetyp[1]);
        line (tetxp[1], tetyp[1], tetxp[2], tetyp[2]);
        line (tetxp[1], tetyp[1], tetxp[3], tetyp[3]);
        line (tetxp[2], tetyp[2], tetxp[3], tetyp[3]);
        }
}
//Функция отрисовки тени тетраэдра
drawtetsh(int figcolor, bool toggle)
{
    setcolor(LIGHTGRAY);
    short i,j,k, temp, notneeded = 0, status = 5, excep = 0, finalexception;
    double excepsize = 10000,
        stetxp[4] = {K*(stetx[0]-stetc[0]) / (stetz[0]-stetc[2]+K) + stetc[0], K*(stetx[1]-stetc[0]) / (stetz[1]-stetc[2]+K) + stetc[0], K*(stetx[2]-stetc[0]) / (stetz[2]-stetc[2]+K) + stetc[0],K*(stetx[3]-stetc[0]) / (stetz[3]-stetc[2]+K) + stetc[0]},
        stetyp[4] = {K*(stety[0]-stetc[0]) / (stetz[0]-stetc[2]+K) + stetc[0], K*(stety[1]-stetc[0]) / (stetz[1]-stetc[2]+K) + stetc[0], K*(stety[2]-stetc[0]) / (stetz[2]-stetc[2]+K) + stetc[0],K*(stety[3]-stetc[0]) / (stetz[3]-stetc[2]+K) + stetc[0]},
        stetcp[2] = {(stetxp[0]+stetxp[1]+stetxp[2]+stetxp[3])/4,(stetyp[0]+stetyp[1]+stetyp[2]+stetyp[3])/4};
        if (toggle)
       {if (isintriangle(stetxp[1],stetyp[1],stetxp[2],stetyp[2],stetxp[3],stetyp[3],stetxp[0],stetyp[0])) status = 0;
        if (isintriangle(stetxp[0],stetyp[0],stetxp[2],stetyp[2],stetxp[3],stetyp[3],stetxp[1],stetyp[1])) status = 1;
        if (isintriangle(stetxp[1],stetyp[1],stetxp[0],stetyp[0],stetxp[3],stetyp[3],stetxp[2],stetyp[2])) status = 2;
        if (isintriangle(stetxp[1],stetyp[1],stetxp[2],stetyp[2],stetxp[0],stetyp[0],stetxp[3],stetyp[3])) status = 3;
        if (status == 5)
        {
        if (len2mid(stetcp[0],stetcp[1],stetxp[0], stetyp[0], stetxp[1], stetyp[1])< excepsize){excepsize = len2mid(stetcp[0],stetcp[1],stetxp[0], stetyp[0], stetxp[1], stetyp[1]); excep = 1;}
        if (len2mid(stetcp[0],stetcp[1],stetxp[0], stetyp[0], stetxp[2], stetyp[2])< excepsize){excepsize = len2mid(stetcp[0],stetcp[1],stetxp[0], stetyp[0], stetxp[2], stetyp[2]); excep = 2;}
        if (len2mid(stetcp[0],stetcp[1],stetxp[0], stetyp[0], stetxp[3], stetyp[3])< excepsize){excepsize = len2mid(stetcp[0],stetcp[1],stetxp[0], stetyp[0], stetxp[3], stetyp[3]); excep = 3;}
        if (excep == 1)
            {
                if(mid(stetz[0], stetz[1])<=mid(stetz[2], stetz[3]))    //012 013
                {
                    line (stetxp[0], stetyp[0], stetxp[1], stetyp[1]);
                    floodtriangle((stetxp[0]+stetxp[1]+stetxp[2])/3,(stetyp[0]+stetyp[1]+stetyp[2])/3,stetxp[0],stetyp[0],stetxp[1],stetyp[1],stetxp[2],stetyp[2],LIGHTGRAY);
                    floodtriangle((stetxp[3]+stetxp[1]+stetxp[0])/3,(stetyp[3]+stetyp[1]+stetyp[0])/3,stetxp[3],stetyp[3],stetxp[1],stetyp[1],stetxp[0],stetyp[0],LIGHTGRAY);
                }
                else                                                //023 123
                {
                    line (stetxp[2], stetyp[2], stetxp[3], stetyp[3]);
                    floodtriangle((stetxp[3]+stetxp[0]+stetxp[2])/3,(stetyp[3]+stetyp[0]+stetyp[2])/3,stetxp[3],stetyp[3],stetxp[0],stetyp[0],stetxp[2],stetyp[2],LIGHTGRAY);
                    floodtriangle((stetxp[3]+stetxp[1]+stetxp[2])/3,(stetyp[3]+stetyp[1]+stetyp[2])/3,stetxp[3],stetyp[3],stetxp[1],stetyp[1],stetxp[2],stetyp[2],LIGHTGRAY);
                }
                line (stetxp[0], stetyp[0], stetxp[2], stetyp[2]);
                line (stetxp[0], stetyp[0], stetxp[3], stetyp[3]);
                line (stetxp[1], stetyp[1], stetxp[2], stetyp[2]);
                line (stetxp[1], stetyp[1], stetxp[3], stetyp[3]);
            }
            if (excep == 2)
            {
                if(mid(stetz[0], stetz[2])<=mid(stetz[1], stetz[3]))    //012 023
                {
                    line (stetxp[0], stetyp[0], stetxp[2], stetyp[2]);
                    floodtriangle((stetxp[0]+stetxp[1]+stetxp[2])/3,(stetyp[0]+stetyp[1]+stetyp[2])/3,stetxp[0],stetyp[0],stetxp[1],stetyp[1],stetxp[2],stetyp[2],LIGHTGRAY);
                    floodtriangle((stetxp[3]+stetxp[0]+stetxp[2])/3,(stetyp[3]+stetyp[0]+stetyp[2])/3,stetxp[3],stetyp[3],stetxp[0],stetyp[0],stetxp[2],stetyp[2],LIGHTGRAY);
                }
                else                                                //013 123
                {
                    line (stetxp[1], stetyp[1], stetxp[3], stetyp[3]);
                    floodtriangle((stetxp[0]+stetxp[1]+stetxp[3])/3,(stetyp[0]+stetyp[1]+stetyp[3])/3,stetxp[0],stetyp[0],stetxp[1],stetyp[1],stetxp[3],stetyp[3],LIGHTGRAY);
                    floodtriangle((stetxp[3]+stetxp[1]+stetxp[2])/3,(stetyp[3]+stetyp[1]+stetyp[2])/3,stetxp[3],stetyp[3],stetxp[1],stetyp[1],stetxp[2],stetyp[2],LIGHTGRAY);
                }
                line (stetxp[0], stetyp[0], stetxp[1], stetyp[1]);
                line (stetxp[0], stetyp[0], stetxp[3], stetyp[3]);
                line (stetxp[1], stetyp[1], stetxp[2], stetyp[2]);
                line (stetxp[2], stetyp[2], stetxp[3], stetyp[3]);
            }
            if (excep == 3)
            {

                if(mid(stetz[0], stetz[3])<=mid(stetz[1], stetz[2]))    //023 013
                {
                    line (stetxp[0], stetyp[0], stetxp[3], stetyp[3]);
                    floodtriangle((stetxp[0]+stetxp[1]+stetxp[3])/3,(stetyp[0]+stetyp[1]+stetyp[3])/3,stetxp[0],stetyp[0],stetxp[1],stetyp[1],stetxp[3],stetyp[3],LIGHTGRAY);
                    floodtriangle((stetxp[3]+stetxp[0]+stetxp[2])/3,(stetyp[3]+stetyp[0]+stetyp[2])/3,stetxp[3],stetyp[3],stetxp[0],stetyp[0],stetxp[2],stetyp[2],LIGHTGRAY);
                }
                else                                                //012 123
                {
                    line (stetxp[1], stetyp[1], stetxp[2], stetyp[2]);
                    floodtriangle((stetxp[0]+stetxp[1]+stetxp[2])/3,(stetyp[0]+stetyp[1]+stetyp[2])/3,stetxp[0],stetyp[0],stetxp[1],stetyp[1],stetxp[2],stetyp[2],LIGHTGRAY);
                    floodtriangle((stetxp[3]+stetxp[1]+stetxp[2])/3,(stetyp[3]+stetyp[1]+stetyp[2])/3,stetxp[3],stetyp[3],stetxp[1],stetyp[1],stetxp[2],stetyp[2],LIGHTGRAY);

                }
                line (stetxp[0], stetyp[0], stetxp[1], stetyp[1]);
                line (stetxp[0], stetyp[0], stetxp[2], stetyp[2]);
                line (stetxp[1], stetyp[1], stetxp[3], stetyp[3]);
                line (stetxp[2], stetyp[2], stetxp[3], stetyp[3]);
            }
        }
        else
        {
            if (stetc[2]<stetz[status])
            {
                if (status!=0 && status!=1) line(stetxp[0], stetyp[0], stetxp[1], stetyp[1]);
                if (status!=0 && status!=2) line(stetxp[0], stetyp[0], stetxp[2], stetyp[2]);
                if (status!=0 && status!=3) line(stetxp[0], stetyp[0], stetxp[3], stetyp[3]);
                if (status!=1 && status!=2) line(stetxp[1], stetyp[1], stetxp[2], stetyp[2]);
                if (status!=1 && status!=3) line(stetxp[1], stetyp[1], stetxp[3], stetyp[3]);
                if (status!=2 && status!=3) line(stetxp[2], stetyp[2], stetxp[3], stetyp[3]);
                if (status==3)floodtriangle((stetxp[0]+stetxp[1]+stetxp[2])/3,(stetyp[0]+stetyp[1]+stetyp[2])/3, stetx[0], stety[0], stetx[1], stety[1], stetx[2], stety[2], LIGHTGRAY);
                if (status==2)floodtriangle((stetxp[0]+stetxp[1]+stetxp[3])/3,(stetyp[0]+stetyp[1]+stetyp[3])/3, stetx[0], stety[0], stetx[1], stety[1], stetx[3], stety[3], LIGHTGRAY);
                if (status==1)floodtriangle((stetxp[0]+stetxp[3]+stetxp[2])/3,(stetyp[0]+stetyp[3]+stetyp[2])/3, stetx[0], stety[0], stetx[3], stety[3], stetx[2], stety[2], LIGHTGRAY);
                if (status==0)floodtriangle((stetxp[3]+stetxp[1]+stetxp[2])/3,(stetyp[3]+stetyp[1]+stetyp[2])/3, stetx[3], stety[3], stetx[1], stety[1], stetx[2], stety[2], LIGHTGRAY);
            }
            else //три грани
            {   line(stetxp[0], stetyp[0], stetxp[1], stetyp[1]);
                line(stetxp[0], stetyp[0], stetxp[2], stetyp[2]);
                line(stetxp[0], stetyp[0], stetxp[3], stetyp[3]);
                line(stetxp[1], stetyp[1], stetxp[2], stetyp[2]);
                line(stetxp[1], stetyp[1], stetxp[3], stetyp[3]);
                line(stetxp[2], stetyp[2], stetxp[3], stetyp[3]);
                if (status!=3)floodtriangle((stetxp[0]+stetxp[1]+stetxp[2])/3,(stetyp[0]+stetyp[1]+stetyp[2])/3, stetx[0], stety[0], stetx[1], stety[1], stetx[2], stety[2], LIGHTGRAY);
                if (status!=2)floodtriangle((stetxp[0]+stetxp[1]+stetxp[3])/3,(stetyp[0]+stetyp[1]+stetyp[3])/3, stetx[0], stety[0], stetx[1], stety[1], stetx[3], stety[3], LIGHTGRAY);
                if (status!=1)floodtriangle((stetxp[0]+stetxp[3]+stetxp[2])/3,(stetyp[0]+stetyp[3]+stetyp[2])/3, stetx[0], stety[0], stetx[3], stety[3], stetx[2], stety[2], LIGHTGRAY);
                if (status!=0)floodtriangle((stetxp[3]+stetxp[1]+stetxp[2])/3,(stetyp[3]+stetyp[1]+stetyp[2])/3, stetx[3], stety[3], stetx[1], stety[1], stetx[2], stety[2], LIGHTGRAY);

            }
        }
        }
        else {line (stetxp[0], stetyp[0], stetxp[1], stetyp[1]);
        line (stetxp[0], stetyp[0], stetxp[2], stetyp[2]);
        line (stetxp[0], stetyp[0], stetxp[3], stetyp[3]);
        line (stetxp[0], stetyp[0], stetxp[1], stetyp[1]);
        line (stetxp[1], stetyp[1], stetxp[2], stetyp[2]);
        line (stetxp[1], stetyp[1], stetxp[3], stetyp[3]);
        line (stetxp[2], stetyp[2], stetxp[3], stetyp[3]);
        }
        setcolor (figcolor);
}


rotatefigX(double &y, double &z, double &XC, double &YC, double &ZC, int angle)
{
    float tempy;
    y = y - YC;
    tempy = y;
    z = z - ZC;
    y = y*cos(angle*PI/180) + z*sin(angle*PI/180);
    z = -tempy*sin(angle*PI/180) + z*cos(angle*PI/180);
    y = y + YC;
    z = z + ZC;
}

rotatefigY(double &x, double &z, double &XC, double &YC, double &ZC, int angle)
{
    float tempx;
    x = x - XC;
    tempx = x;
    z = z - ZC;
    x=x*cos(angle*PI/180)+z*sin(angle*PI/180);
    z=-tempx*sin(angle*PI/180)+z*cos(angle*PI/180);
    x = x + XC;
    z = z + ZC;
}

rotatefigZ(double &x, double &y, double &XC, double &YC, double &ZC, int angle)
{
    float tempx;
    x = x - XC;
    tempx = x;
    y = y - YC;
    x=x*cos(angle*PI/180)-y*sin(angle*PI/180);
    y=tempx*sin(angle*PI/180)+y*cos(angle*PI/180);
    x = x + XC;
    y = y + YC;
}

scalefig(double &x, double &y, double &z, double &XC, double &YC, double &ZC, double scale)
{
    x = XC+scale*(x-XC);
    y = YC+scale*(y-YC);
    z = ZC+scale*(z-ZC);
}

main(int argc, char *argv[])
{
    int i, j, k;
    char ch,excolor;
    bool togglepaint = 0;
    initwindow(1500, 950);
    setbkcolor(defcolour);
    excolor = YELLOW;
    while(1)
    {
        cleardevice();
        cubec[0] = (cubex[0]+cubex[1]+cubex[2]+cubex[3]+cubex[4]+cubex[5]+cubex[6]+cubex[7])/8;
        cubec[1] = (cubey[0]+cubey[1]+cubey[2]+cubey[3]+cubey[4]+cubey[5]+cubey[6]+cubey[7])/8;
        cubec[2] = (cubez[0]+cubez[1]+cubez[2]+cubez[3]+cubez[4]+cubez[5]+cubez[6]+cubez[7])/8;
        tetc[0] = (tetx[0]+tetx[1]+tetx[2]+tetx[3])/4;
        tetc[1] = (tety[0]+tety[1]+tety[2]+tety[3])/4;
        tetc[2] = (tetz[0]+tetz[1]+tetz[2]+tetz[3])/4;
        scubec[0] = (scubex[0]+scubex[1]+scubex[2]+scubex[3]+scubex[4]+scubex[5]+scubex[6]+scubex[7])/8;
        scubec[1] = (scubey[0]+scubey[1]+scubey[2]+scubey[3]+scubey[4]+scubey[5]+scubey[6]+scubey[7])/8;
        scubec[2] = (scubez[0]+scubez[1]+scubez[2]+scubez[3]+scubez[4]+scubez[5]+scubez[6]+scubez[7])/8;
        stetc[0] = (stetx[0]+stetx[1]+stetx[2]+stetx[3])/4;
        stetc[1] = (stety[0]+stety[1]+stety[2]+stety[3])/4;
        stetc[2] = (stetz[0]+stetz[1]+stetz[2]+stetz[3])/4;
        comc[0] = (tetc[0]+cubec[0])/2;
        comc[1] = (tetc[1]+cubec[1])/2;
        comc[2] = (tetc[2]+cubec[2])/2;
        scomc[0] = (stetc[0]+scubec[0])/2;
        scomc[1] = (stetc[1]+scubec[1])/2;
        scomc[2] = (stetc[2]+scubec[2])/2;
        if(cubec[2]<=tetc[2])
        {
            if (togglepaint){
            drawtetsh(BLACK, togglepaint);
            drawcubesh(BLACK, togglepaint);}
            drawtet(BLACK, togglepaint);
            drawcube(BLACK, togglepaint);

        }
        else
        {
            if (togglepaint){
            drawcubesh(BLACK, togglepaint);
            drawtetsh(BLACK, togglepaint);}
            drawcube(BLACK, togglepaint);
            drawtet(BLACK, togglepaint);
        }
        ch = getch();
        if(ch == '`') togglepaint = !togglepaint;
        if(ch == 'r')
        {
            rotatefigX(cubey[0],cubez[0],comc[0],comc[1],comc[2],1);
            rotatefigX(cubey[1],cubez[1],comc[0],comc[1],comc[2],1);
            rotatefigX(cubey[2],cubez[2],comc[0],comc[1],comc[2],1);
            rotatefigX(cubey[3],cubez[3],comc[0],comc[1],comc[2],1);
            rotatefigX(cubey[4],cubez[4],comc[0],comc[1],comc[2],1);
            rotatefigX(cubey[5],cubez[5],comc[0],comc[1],comc[2],1);
            rotatefigX(cubey[6],cubez[6],comc[0],comc[1],comc[2],1);
            rotatefigX(cubey[7],cubez[7],comc[0],comc[1],comc[2],1);
            rotatefigX(tety[0],tetz[0],  comc[0],comc[1],comc[2],1);
            rotatefigX(tety[1],tetz[1],  comc[0],comc[1],comc[2],1);
            rotatefigX(tety[2],tetz[2],  comc[0],comc[1],comc[2],1);
            rotatefigX(tety[3],tetz[3],  comc[0],comc[1],comc[2],1);
            rotatefigX(scubey[0],scubez[0],comc[0],comc[1],comc[2],1);
            rotatefigX(scubey[1],scubez[1],comc[0],comc[1],comc[2],1);
            rotatefigX(scubey[2],scubez[2],comc[0],comc[1],comc[2],1);
            rotatefigX(scubey[3],scubez[3],comc[0],comc[1],comc[2],1);
            rotatefigX(scubey[4],scubez[4],comc[0],comc[1],comc[2],1);
            rotatefigX(scubey[5],scubez[5],comc[0],comc[1],comc[2],1);
            rotatefigX(scubey[6],scubez[6],comc[0],comc[1],comc[2],1);
            rotatefigX(scubey[7],scubez[7],comc[0],comc[1],comc[2],1);
            rotatefigX(stety[0],stetz[0],  comc[0],comc[1],comc[2],1);
            rotatefigX(stety[1],stetz[1],  comc[0],comc[1],comc[2],1);
            rotatefigX(stety[2],stetz[2],  comc[0],comc[1],comc[2],1);
            rotatefigX(stety[3],stetz[3],  comc[0],comc[1],comc[2],1);

        }
        if(ch == 'f')
        {if (cubez)
            rotatefigX(cubey[0],cubez[0],comc[0],comc[1],comc[2],-1);
            rotatefigX(cubey[1],cubez[1],comc[0],comc[1],comc[2],-1);
            rotatefigX(cubey[2],cubez[2],comc[0],comc[1],comc[2],-1);
            rotatefigX(cubey[3],cubez[3],comc[0],comc[1],comc[2],-1);
            rotatefigX(cubey[4],cubez[4],comc[0],comc[1],comc[2],-1);
            rotatefigX(cubey[5],cubez[5],comc[0],comc[1],comc[2],-1);
            rotatefigX(cubey[6],cubez[6],comc[0],comc[1],comc[2],-1);
            rotatefigX(cubey[7],cubez[7],comc[0],comc[1],comc[2],-1);
            rotatefigX(tety[0],tetz[0],comc[0],comc[1],comc[2],-1);
            rotatefigX(tety[1],tetz[1],comc[0],comc[1],comc[2],-1);
            rotatefigX(tety[2],tetz[2],comc[0],comc[1],comc[2],-1);
            rotatefigX(tety[3],tetz[3],comc[0],comc[1],comc[2],-1);
            rotatefigX(scubey[0],scubez[0],scomc[0],scomc[1],scomc[2],-1);
            rotatefigX(scubey[1],scubez[1],scomc[0],scomc[1],scomc[2],-1);
            rotatefigX(scubey[2],scubez[2],scomc[0],scomc[1],scomc[2],-1);
            rotatefigX(scubey[3],scubez[3],scomc[0],scomc[1],scomc[2],-1);
            rotatefigX(scubey[4],scubez[4],scomc[0],scomc[1],scomc[2],-1);
            rotatefigX(scubey[5],scubez[5],scomc[0],scomc[1],scomc[2],-1);
            rotatefigX(scubey[6],scubez[6],scomc[0],scomc[1],scomc[2],-1);
            rotatefigX(scubey[7],scubez[7],scomc[0],scomc[1],scomc[2],-1);
            rotatefigX(stety[0],stetz[0],scomc[0],scomc[1],scomc[2],-1);
            rotatefigX(stety[1],stetz[1],scomc[0],scomc[1],scomc[2],-1);
            rotatefigX(stety[2],stetz[2],scomc[0],scomc[1],scomc[2],-1);
            rotatefigX(stety[3],stetz[3],scomc[0],scomc[1],scomc[2],-1);

        }
        if(ch == 't')
        {
            rotatefigY(cubex[0],cubez[0],comc[0],comc[1],comc[2],1);
            rotatefigY(cubex[1],cubez[1],comc[0],comc[1],comc[2],1);
            rotatefigY(cubex[2],cubez[2],comc[0],comc[1],comc[2],1);
            rotatefigY(cubex[3],cubez[3],comc[0],comc[1],comc[2],1);
            rotatefigY(cubex[4],cubez[4],comc[0],comc[1],comc[2],1);
            rotatefigY(cubex[5],cubez[5],comc[0],comc[1],comc[2],1);
            rotatefigY(cubex[6],cubez[6],comc[0],comc[1],comc[2],1);
            rotatefigY(cubex[7],cubez[7],comc[0],comc[1],comc[2],1);
            rotatefigY(tetx[0],tetz[0],comc[0],comc[1],comc[2],1);
            rotatefigY(tetx[1],tetz[1],comc[0],comc[1],comc[2],1);
            rotatefigY(tetx[2],tetz[2],comc[0],comc[1],comc[2],1);
            rotatefigY(tetx[3],tetz[3],comc[0],comc[1],comc[2],1);
            rotatefigY(scubex[0],scubez[0],scomc[0],scomc[1],scomc[2],1);
            rotatefigY(scubex[1],scubez[1],scomc[0],scomc[1],scomc[2],1);
            rotatefigY(scubex[2],scubez[2],scomc[0],scomc[1],scomc[2],1);
            rotatefigY(scubex[3],scubez[3],scomc[0],scomc[1],scomc[2],1);
            rotatefigY(scubex[4],scubez[4],scomc[0],scomc[1],scomc[2],1);
            rotatefigY(scubex[5],scubez[5],scomc[0],scomc[1],scomc[2],1);
            rotatefigY(scubex[6],scubez[6],scomc[0],scomc[1],scomc[2],1);
            rotatefigY(scubex[7],scubez[7],scomc[0],scomc[1],scomc[2],1);
            rotatefigY(stetx[0],stetz[0],scomc[0],scomc[1],scomc[2],1);
            rotatefigY(stetx[1],stetz[1],scomc[0],scomc[1],scomc[2],1);
            rotatefigY(stetx[2],stetz[2],scomc[0],scomc[1],scomc[2],1);
            rotatefigY(stetx[3],stetz[3],scomc[0],scomc[1],scomc[2],1);

        }
        if(ch == 'g')
        {
            rotatefigY(cubex[0],cubez[0],comc[0],comc[1],comc[2],-1);
            rotatefigY(cubex[1],cubez[1],comc[0],comc[1],comc[2],-1);
            rotatefigY(cubex[2],cubez[2],comc[0],comc[1],comc[2],-1);
            rotatefigY(cubex[3],cubez[3],comc[0],comc[1],comc[2],-1);
            rotatefigY(cubex[4],cubez[4],comc[0],comc[1],comc[2],-1);
            rotatefigY(cubex[5],cubez[5],comc[0],comc[1],comc[2],-1);
            rotatefigY(cubex[6],cubez[6],comc[0],comc[1],comc[2],-1);
            rotatefigY(cubex[7],cubez[7],comc[0],comc[1],comc[2],-1);
            rotatefigY(tetx[0],tetz[0],comc[0],comc[1],comc[2],-1);
            rotatefigY(tetx[1],tetz[1],comc[0],comc[1],comc[2],-1);
            rotatefigY(tetx[2],tetz[2],comc[0],comc[1],comc[2],-1);
            rotatefigY(tetx[3],tetz[3],comc[0],comc[1],comc[2],-1);
            rotatefigY(scubex[0],scubez[0],scomc[0],scomc[1],scomc[2],-1);
            rotatefigY(scubex[1],scubez[1],scomc[0],scomc[1],scomc[2],-1);
            rotatefigY(scubex[2],scubez[2],scomc[0],scomc[1],scomc[2],-1);
            rotatefigY(scubex[3],scubez[3],scomc[0],scomc[1],scomc[2],-1);
            rotatefigY(scubex[4],scubez[4],scomc[0],scomc[1],scomc[2],-1);
            rotatefigY(scubex[5],scubez[5],scomc[0],scomc[1],scomc[2],-1);
            rotatefigY(scubex[6],scubez[6],scomc[0],scomc[1],scomc[2],-1);
            rotatefigY(scubex[7],scubez[7],scomc[0],scomc[1],scomc[2],-1);
            rotatefigY(stetx[0],stetz[0],scomc[0],scomc[1],scomc[2],-1);
            rotatefigY(stetx[1],stetz[1],scomc[0],scomc[1],scomc[2],-1);
            rotatefigY(stetx[2],stetz[2],scomc[0],scomc[1],scomc[2],-1);
            rotatefigY(stetx[3],stetz[3],scomc[0],scomc[1],scomc[2],-1);


        }
        if(ch == 'y')
        {
            rotatefigZ(cubex[0],cubey[0],comc[0],comc[1],comc[2],1);
            rotatefigZ(cubex[1],cubey[1],comc[0],comc[1],comc[2],1);
            rotatefigZ(cubex[2],cubey[2],comc[0],comc[1],comc[2],1);
            rotatefigZ(cubex[3],cubey[3],comc[0],comc[1],comc[2],1);
            rotatefigZ(cubex[4],cubey[4],comc[0],comc[1],comc[2],1);
            rotatefigZ(cubex[5],cubey[5],comc[0],comc[1],comc[2],1);
            rotatefigZ(cubex[6],cubey[6],comc[0],comc[1],comc[2],1);
            rotatefigZ(cubex[7],cubey[7],comc[0],comc[1],comc[2],1);
            rotatefigZ(tetx[0],tety[0],comc[0],comc[1],comc[2],1);
            rotatefigZ(tetx[1],tety[1],comc[0],comc[1],comc[2],1);
            rotatefigZ(tetx[2],tety[2],comc[0],comc[1],comc[2],1);
            rotatefigZ(tetx[3],tety[3],comc[0],comc[1],comc[2],1);
            rotatefigZ(scubex[0],scubey[0],scomc[0],scomc[1],scomc[2],1);
            rotatefigZ(scubex[1],scubey[1],scomc[0],scomc[1],scomc[2],1);
            rotatefigZ(scubex[2],scubey[2],scomc[0],scomc[1],scomc[2],1);
            rotatefigZ(scubex[3],scubey[3],scomc[0],scomc[1],scomc[2],1);
            rotatefigZ(scubex[4],scubey[4],scomc[0],scomc[1],scomc[2],1);
            rotatefigZ(scubex[5],scubey[5],scomc[0],scomc[1],scomc[2],1);
            rotatefigZ(scubex[6],scubey[6],scomc[0],scomc[1],scomc[2],1);
            rotatefigZ(scubex[7],scubey[7],scomc[0],scomc[1],scomc[2],1);
            rotatefigZ(stetx[0],stety[0],scomc[0],scomc[1],scomc[2],1);
            rotatefigZ(stetx[1],stety[1],scomc[0],scomc[1],scomc[2],1);
            rotatefigZ(stetx[2],stety[2],scomc[0],scomc[1],scomc[2],1);
            rotatefigZ(stetx[3],stety[3],scomc[0],scomc[1],scomc[2],1);
        }
        if(ch == 'h')
        {
            rotatefigZ(cubex[0],cubey[0],comc[0],comc[1],comc[2],-1);
            rotatefigZ(cubex[1],cubey[1],comc[0],comc[1],comc[2],-1);
            rotatefigZ(cubex[2],cubey[2],comc[0],comc[1],comc[2],-1);
            rotatefigZ(cubex[3],cubey[3],comc[0],comc[1],comc[2],-1);
            rotatefigZ(cubex[4],cubey[4],comc[0],comc[1],comc[2],-1);
            rotatefigZ(cubex[5],cubey[5],comc[0],comc[1],comc[2],-1);
            rotatefigZ(cubex[6],cubey[6],comc[0],comc[1],comc[2],-1);
            rotatefigZ(cubex[7],cubey[7],comc[0],comc[1],comc[2],-1);
            rotatefigZ(tetx[0],tety[0],comc[0],comc[1],comc[2],-1);
            rotatefigZ(tetx[1],tety[1],comc[0],comc[1],comc[2],-1);
            rotatefigZ(tetx[2],tety[2],comc[0],comc[1],comc[2],-1);
            rotatefigZ(tetx[3],tety[3],comc[0],comc[1],comc[2],-1);
            rotatefigZ(scubex[0],scubey[0],scomc[0],scomc[1],scomc[2],-1);
            rotatefigZ(scubex[1],scubey[1],scomc[0],scomc[1],scomc[2],-1);
            rotatefigZ(scubex[2],scubey[2],scomc[0],scomc[1],scomc[2],-1);
            rotatefigZ(scubex[3],scubey[3],scomc[0],scomc[1],scomc[2],-1);
            rotatefigZ(scubex[4],scubey[4],scomc[0],scomc[1],scomc[2],-1);
            rotatefigZ(scubex[5],scubey[5],scomc[0],scomc[1],scomc[2],-1);
            rotatefigZ(scubex[6],scubey[6],scomc[0],scomc[1],scomc[2],-1);
            rotatefigZ(scubex[7],scubey[7],scomc[0],scomc[1],scomc[2],-1);
            rotatefigZ(stetx[0],stety[0],scomc[0],scomc[1],scomc[2],-1);
            rotatefigZ(stetx[1],stety[1],scomc[0],scomc[1],scomc[2],-1);
            rotatefigZ(stetx[2],stety[2],scomc[0],scomc[1],scomc[2],-1);
            rotatefigZ(stetx[3],stety[3],scomc[0],scomc[1],scomc[2],-1);


        }
        if(ch == 'q')
        {
            scalefig(cubex[0],cubey[0],cubez[0],comc[0],comc[1],comc[2],1.01);
            scalefig(cubex[1],cubey[1],cubez[1],comc[0],comc[1],comc[2],1.01);
            scalefig(cubex[2],cubey[2],cubez[2],comc[0],comc[1],comc[2],1.01);
            scalefig(cubex[3],cubey[3],cubez[3],comc[0],comc[1],comc[2],1.01);
            scalefig(cubex[4],cubey[4],cubez[4],comc[0],comc[1],comc[2],1.01);
            scalefig(cubex[5],cubey[5],cubez[5],comc[0],comc[1],comc[2],1.01);
            scalefig(cubex[6],cubey[6],cubez[6],comc[0],comc[1],comc[2],1.01);
            scalefig(cubex[7],cubey[7],cubez[7],comc[0],comc[1],comc[2],1.01);
            scalefig(tetx[0],tety[0],tetz[0],comc[0],comc[1],comc[2],1.01);
            scalefig(tetx[1],tety[1],tetz[1],comc[0],comc[1],comc[2],1.01);
            scalefig(tetx[2],tety[2],tetz[2],comc[0],comc[1],comc[2],1.01);
            scalefig(tetx[3],tety[3],tetz[3],comc[0],comc[1],comc[2],1.01);
            scalefig(scubex[0],scubey[0],scubez[0],scomc[0],scomc[1],scomc[2],1.01);
            scalefig(scubex[1],scubey[1],scubez[1],scomc[0],scomc[1],scomc[2],1.01);
            scalefig(scubex[2],scubey[2],scubez[2],scomc[0],scomc[1],scomc[2],1.01);
            scalefig(scubex[3],scubey[3],scubez[3],scomc[0],scomc[1],scomc[2],1.01);
            scalefig(scubex[4],scubey[4],scubez[4],scomc[0],scomc[1],scomc[2],1.01);
            scalefig(scubex[5],scubey[5],scubez[5],scomc[0],scomc[1],scomc[2],1.01);
            scalefig(scubex[6],scubey[6],scubez[6],scomc[0],scomc[1],scomc[2],1.01);
            scalefig(scubex[7],scubey[7],scubez[7],scomc[0],scomc[1],scomc[2],1.01);
            scalefig(stetx[0],stety[0],stetz[0],scomc[0],scomc[1],scomc[2],1.01);
            scalefig(stetx[1],stety[1],stetz[1],scomc[0],scomc[1],scomc[2],1.01);
            scalefig(stetx[2],stety[2],stetz[2],scomc[0],scomc[1],scomc[2],1.01);
            scalefig(stetx[3],stety[3],stetz[3],scomc[0],scomc[1],scomc[2],1.01);


        }
        if(ch == 'e')
        {
            scalefig(cubex[0],cubey[0],cubez[0],comc[0],comc[1],comc[2],1/1.01);
            scalefig(cubex[1],cubey[1],cubez[1],comc[0],comc[1],comc[2],1/1.01);
            scalefig(cubex[2],cubey[2],cubez[2],comc[0],comc[1],comc[2],1/1.01);
            scalefig(cubex[3],cubey[3],cubez[3],comc[0],comc[1],comc[2],1/1.01);
            scalefig(cubex[4],cubey[4],cubez[4],comc[0],comc[1],comc[2],1/1.01);
            scalefig(cubex[5],cubey[5],cubez[5],comc[0],comc[1],comc[2],1/1.01);
            scalefig(cubex[6],cubey[6],cubez[6],comc[0],comc[1],comc[2],1/1.01);
            scalefig(cubex[7],cubey[7],cubez[7],comc[0],comc[1],comc[2],1/1.01);
            scalefig(tetx[0],tety[0],tetz[0],comc[0],comc[1],comc[2],1/1.01);
            scalefig(tetx[1],tety[1],tetz[1],comc[0],comc[1],comc[2],1/1.01);
            scalefig(tetx[2],tety[2],tetz[2],comc[0],comc[1],comc[2],1/1.01);
            scalefig(tetx[3],tety[3],tetz[3],comc[0],comc[1],comc[2],1/1.01);
            scalefig(scubex[0],scubey[0],scubez[0],scomc[0],scomc[1],scomc[2],1/1.01);
            scalefig(scubex[1],scubey[1],scubez[1],scomc[0],scomc[1],scomc[2],1/1.01);
            scalefig(scubex[2],scubey[2],scubez[2],scomc[0],scomc[1],scomc[2],1/1.01);
            scalefig(scubex[3],scubey[3],scubez[3],scomc[0],scomc[1],scomc[2],1/1.01);
            scalefig(scubex[4],scubey[4],scubez[4],scomc[0],scomc[1],scomc[2],1/1.01);
            scalefig(scubex[5],scubey[5],scubez[5],scomc[0],scomc[1],scomc[2],1/1.01);
            scalefig(scubex[6],scubey[6],scubez[6],scomc[0],scomc[1],scomc[2],1/1.01);
            scalefig(scubex[7],scubey[7],scubez[7],scomc[0],scomc[1],scomc[2],1/1.01);
            scalefig(stetx[0],stety[0],stetz[0],scomc[0],scomc[1],scomc[2],1/1.01);
            scalefig(stetx[1],stety[1],stetz[1],scomc[0],scomc[1],scomc[2],1/1.01);
            scalefig(stetx[2],stety[2],stetz[2],scomc[0],scomc[1],scomc[2],1/1.01);
            scalefig(stetx[3],stety[3],stetz[3],scomc[0],scomc[1],scomc[2],1/1.01);



        }
        if(ch == 'd')
        {
            for(i=0;i<10;i++){cubex[0]++,cubex[1]++,cubex[2]++,cubex[3]++,cubex[4]++,cubex[5]++,cubex[6]++,cubex[7]++,tetx[0]++,tetx[1]++,tetx[2]++,tetx[3]++;scubex[0]++,scubex[1]++,scubex[2]++,scubex[3]++,scubex[4]++,scubex[5]++,scubex[6]++,scubex[7]++,stetx[0]++,stetx[1]++,stetx[2]++,stetx[3]++;}
        }
        if(ch == 'a')
        {
            for(i=0;i<10;i++){cubex[0]--,cubex[1]--,cubex[2]--,cubex[3]--,cubex[4]--,cubex[5]--,cubex[6]--,cubex[7]--,tetx[0]--,tetx[1]--,tetx[2]--,tetx[3]--;scubex[0]--,scubex[1]--,scubex[2]--,scubex[3]--,scubex[4]--,scubex[5]--,scubex[6]--,scubex[7]--,stetx[0]--,stetx[1]--,stetx[2]--,stetx[3]--;}
        }
        if(ch == 's')
        {
            if (cubey[0]<scubey[0]&&tety[0]<stety[0])for(i=0;i<10;i++){cubey[0]++,cubey[1]++,cubey[2]++,cubey[3]++,cubey[4]++,cubey[5]++,cubey[6]++,cubey[7]++,tety[0]++,tety[1]++,tety[2]++,tety[3]++;}
        }
        if(ch == 'w')
        {
            for(i=0;i<10;i++){cubey[0]--,cubey[1]--,cubey[2]--,cubey[3]--,cubey[4]--,cubey[5]--,cubey[6]--,cubey[7]--,tety[0]--,tety[1]--,tety[2]--,tety[3]--;}
        }
    }
}

