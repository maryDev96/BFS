#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <chrono>

using namespace std;


int dirRow[] = { 1, -1, 0, 0 };
int dirCol[] = { 0, 0, 1, -1 };

const int rows = 31;
const int cols = 51;


struct Point
{
    int x, y;
};

struct Vertice
{
    Point point;
    int distance;
};

void neighbours(Vertice v, char(*maze)[51], bool visited[][cols], queue<Vertice>& wizard, int& nodesNext)
{
    Point p2;
    for (int i = 0; i < 4; i++)
    {
        p2.x = v.point.x + dirCol[i];
        p2.y = v.point.y + dirRow[i];

        //cheking if p2 value is whithin our maze sizes
        if ((p2.x < 0 || p2.y < 0) || (p2.x >= cols || p2.y >= rows))
            continue;

        //doing nothing if a cell has been already visited
        if (visited[p2.y][p2.x] == true)
            continue;
        
        //doing nothing if a cell is a part of the maze walls
        if (maze[p2.y][p2.x] == '#')
            continue;

        //pushing a new cell coordinates and distance from the start position into the queue
        wizard.push({ p2, v.distance + 1 });
        visited[p2.y][p2.x] = true;
        nodesNext++;
    }
}


int BFS(queue<Vertice>& wizard, char (*maze)[51])
{
    bool visited[31][51] = { {false} };
    
    //initialize a starting point as a visited one
    visited[wizard.front().point.y][wizard.front().point.x] = true;

    Vertice verticePassed;

    int nodesLeft = 1, nodesNext = 0;
    int steps = 0;

    while (wizard.size() > 0)
    {
        verticePassed = wizard.front();
        wizard.pop();
        
        //if a wizard reached the final point, the distance is returned
        if (maze[verticePassed.point.y][verticePassed.point.x] == 'F') 
        {
            return verticePassed.distance;
        }

        //function to explore neighbouring cells
        neighbours(verticePassed, maze, visited, wizard, nodesNext);
        nodesLeft--;

        //just to calculate how many steps were taken and update nodes values
        if (nodesLeft == 0)
        {
            nodesLeft = nodesNext;
            nodesNext = 0;
            steps++;
        }
    }

    return -1;
}


int main()
{
    ifstream mazeFile;
    mazeFile.open("D:\\UL\\Advanced algorithms\\proj2_ex2\\maze.txt");
    string ch;
    Point finish, start[3];
    int rows = 31, cols = 51;
    char maze[31][51];
    int i = 0, j = 0;

    //reading maze from the file
    while (!mazeFile.eof())
    {
        getline(mazeFile, ch);
        for (int k = 0; k < ch.length(); k++)
        {
            maze[i][k] = ch[k];
            if (ch[k] == 'F')
            {
                finish.x = k;
                finish.y = i;

                cout << "Finish: x = " << k << ", y = " << i << endl;
            }
            else if (ch[k] == 'S')
            {
                start[j].x = k;
                start[j].y = i;
                cout << "Start position wizard #" << j + 1 << ": x = " << start[j].x << ", y = " << start[j].y << endl;
                j++;
            }
        }
        i++;
    }

    mazeFile.close();

    for (int i = 0; i < 31; i++)
    {
        for (int j = 0; j < 51; j++)
        {
            cout << maze[i][j];
        }
        cout << endl;
    }

    queue<Vertice> wizards[3];
    using chrono::nanoseconds;
    nanoseconds total(0);
    int distance;

    cout << "Every wizards's speed in vertices per milisecond: " << endl;
    for (int i = 0; i < 3; i++)
    {
        wizards[i].push({ start[i], 0 });
        auto start = chrono::steady_clock::now();
        distance = BFS(wizards[i], maze);
        auto end = chrono::steady_clock::now();

        total = end - start;
        double time = (double)total.count() / 1000000; //time spent in miliseconds
        cout << "Distance = " << distance << endl;
        double speed = (double)distance / time;

        cout << "Wizard #" << i + 1 << " speed = " << speed << endl;
    }

	return 0;
}