#include "AStar.h"

int AStar::calcH(int x, int y, int endX, int endY)
{
    int temp1=endX-x;
    int temp2=endY-y;
    if (temp1<0) temp1=-1*temp1;
    if (temp2<0) temp2=-1*temp2;
    return temp1+temp2;
}

void AStar::nextMove(int startX, int startY, int endX, int endY, char board[][23], int boardX, int boardY, int ret[])
{
    int gScore[boardX][boardY];
    bool closed[boardX][boardY];
    int moves[boardX][boardY];
    int parentX[boardX][boardY];
    int parentY[boardX][boardY];
    for (int i=0; i<boardX; i++)
        for (int j=0; j<boardY; j++)
        {
            gScore[i][j]=0;
            moves[i][j]=0;
            closed[i][j]=false;
            //So you know the shortest route in the end
            parentX[i][j]=-1;
            parentY[i][j]=-1;
        }

    gScore[startX][startY]=calcH(startX, startY, endX, endY);

    bool good=false;
    bool findable=false;

    while (!good)
    {
        //Output board
        /*for(int i=0; i<5; i++)
        {
            for(int j=0; j<5; j++)
            {
                cout<<gScore[j][i];
            }
            cout<<endl;
        }*/
        /////////////////////////////////
        //Check for the board for open ones, find the one with the smallest G
        int smallestGx=-1; int smallestGy=-1;
        for(int i=0; i<boardX; i++)
            for(int j=0; j<boardY; j++)
            {
                //Make sure it's not closed or 0 (meaning nothing's been calculated yet)
                if (!closed[i][j] && gScore[i][j]>0)
                {
                    //If there is no smallest, set it to be
                    if (smallestGx==-1)
                    {
                        smallestGx=i;  smallestGy=j;
                    }
                    //Otherwise, if it's smaller, make it so
                    if (gScore[i][j]<gScore[smallestGx][smallestGy])
                    {
                        smallestGx=i; smallestGy=j;
                    }
                }
            }
        //If none are
        if (smallestGx==-1)
        {
            good=true;
            findable=false;
        }
        else
        {
        closed[smallestGx][smallestGy]=true;
        //cout<<"Smallest G: ("<<smallestGx<<", "<<smallestGy<<")\n";
        /////////////////////////////////
        //Now that the smallest has been found, make it closed, and calculate the G of everything around it
        int tempMoves;
        tempMoves=moves[smallestGx][smallestGy]+1;
        //Check each around it to recalculate shortest moves, if necessary. If it is,  recalculate gScore and make this the parent. MAKE SURE IT'S NOT A WALL
        for (int i=0; i<4; i++)
        {
            int tempX, tempY;
            if (i==0) { tempX=smallestGx+1; tempY=smallestGy;}
            if (i==1) { tempX=smallestGx-1; tempY=smallestGy;}
            if (i==2) { tempX=smallestGx; tempY=smallestGy+1;}
            if (i==3) { tempX=smallestGx; tempY=smallestGy-1;}
            //Make sure it's not outside the board
            if (tempX>=0 && tempX<boardX && tempY>=0 && tempY<boardY)
            {
                if ((tempMoves<moves[tempX][tempY] || moves[tempX][tempY]==0) && (moves[tempX][tempY] == ' ' || moves[tempX][tempY] == '.' || moves[tempX][tempY] == 'O'))//<-Wall code INPUT EVERYTHING IT SHOULDN"T WALK THROUGH
                {
                    moves[tempX][tempY]=tempMoves;
                    gScore[tempX][tempY]=tempMoves+calcH(tempX, tempY, endX, endY);
                    parentX[tempX][tempY]=smallestGx;
                    parentY[tempX][tempY]=smallestGy;
                    //If it's the end square, we're good!
                    if (tempX==endX && tempY==endY)
                    {
                        good=true;
                        findable=true;
                    }
                }
            }
        }
        }
    }
    //If not findable
    /*if (findable==false)
        cout<<"Not findable"<<endl;*/
    //otherwise
    if (findable==true)
    {
        //Output board
        /*for(int i=0; i<5; i++)
        {
            for(int j=0; j<5; j++)
            {
                cout<<gScore[j][i];
            }
            cout<<endl;
        }*/
        //Make a board to visually show shortest route
        good=false;
        int tempX=endX, tempY=endY;
        //Find shortest route
        while (!good)
        {
            //Go to each square's parent square until it reaches the start square
            if (tempX==startX && tempY==startY)
            {
                good=true;
            }
            else
            {
                //If next is start, this is the square to "return"
                if (parentX[tempX][tempY]==startX && parentY[tempX][tempY]==startY)
                {
                    //RETURNS RETURN RETURNS RETURNS RETURNS//////////////////////////////////////////
                    ret[0]=tempX;
                    ret[1]=tempY;
                }
                int tX=tempX;
                tempX=parentX[tempX][tempY];
                tempY=parentY[tX][tempY];
            }
        }
        //Output board with solution
        /*for(int i=0; i<5; i++)
        {
            for(int j=0; j<5; j++)
            {
                cout<<doneBoard[j][i];
            }
            cout<<endl;
        }*/
    }
}
