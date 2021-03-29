#include <iostream>
#include <time.h>
#include "utils/COLORS!.hpp"
#ifdef _WIN32
#include <conio.h>
#endif
#ifdef linux
#include "utils/conio.h"
#endif
using namespace std;

struct Node
{
    Node *next = NULL;
    int x;
    int y;
};

class Snake
{
private:
    Node *head;                         //The start of the linked list
    const int dimensions[2] = {30, 20}; //Dimensions of the map of the order x, y
    int food[2] = {15, 5};              //The initial coordinates of the food
    bool running = true;                //Signfies if the gamme is running or not
    char DEFAULT = 'd';                 //The default input
    int score = 0;                      //Score of the user
    bool paused = false;                //If the game is paused or not

public:
    Snake()
    {
        head = NULL;
        int index = 3;                                   //Index is the initial length of the snake.
        int baseCoords[3][2] = {{4, 9}, {5, 9}, {6, 9}}; //The initial coordinates of the snake.
        for (int i = 0; i < index; i++)
        {
            Node *temp = new Node;
            temp->x = baseCoords[i][0];
            temp->y = baseCoords[i][1];
            if (head == NULL)
                head = temp;
            else
            {
                Node *t = head;
                while (t->next != NULL)
                    t = t->next;
                t->next = temp;
            }
        }
        draw(); //drawing the map.
        cout << "Score: " << score << endl;
    }
    void clearScreen() //To clear the terminal screen after each frame.
    {
#ifdef _WIN32
        system("cls");
#endif
#ifdef linux
        system("clear");
#endif
    }
    bool isPresent(int a, int b) // Tells if the point (a, b) is present in the snake or not.
    {
        Node *t = head;
        while (t != NULL)
        {
            if (t->x == a && t->y == b)
                return true; //Found the point
            t = t->next;
        }
        return false;
    }
    bool collided(int a, int b) // Tells if the snake collided with itself or not.
    {
        int index = 0;
        Node *t = head;
        while (t != NULL)
        {
            if (t->x == a && t->y == b && index > 2)
                return true;
            index++;
            t = t->next;
        }
        return false;
    }
    void sleep(int milliseconds) // A cross platform sleep function.
    {
        clock_t time_end;
        time_end = clock() + milliseconds * CLOCKS_PER_SEC / 1000;
        while (clock() < time_end)
        {
        }
    }
    void draw() // Drawing the map.
    {
        while (running)
        {
            bool ate = false; // If the snake has eaten the food or not.
            if (head->x >= dimensions[0] || head->x < 0 || head->y < 0 || head->y >= dimensions[1])
            { // It collided with one of the walls.
                running = false;
                return;
            }
            if (collided(head->x, head->y))
            { // It collided with itself
                running = false;
                return;
            }
            if (head->x == food[0] && head->y == food[1])
            {                // It ate the food.
                spawnFood(); // Spawning new food.
                ate = true;
                score++;
            }
            for (int i = 0; i < dimensions[1]; i++)
            {
                for (int j = 0; j < dimensions[0]; j++)
                {
                    if ((j == dimensions[0] - 1 || j == 0) && i != 0)
                        cout << "|"; // Vertical walls
                    else if (isPresent(j, i))
                    {
                        if (head->x == j && head->y == i)
                        {
                            cout << GREEN_NO_FLASH << "*" << NC; // The head of the snake in green color.
                        }
                        else
                            cout << RED_NO_FLASH << "*" << NC; // Rest of the snake in red color.
                    }
                    else if (j == food[0] && i == food[1])
                        cout << ORANGE_NO_FLASH << "#" << NC;
                    else if (i == dimensions[1] - 1 || i == 0)
                        cout << "_"; // Horizontal walls
                    else
                        cout << " ";
                }
                cout << "\n";
            }
            cout << "Score : " << score << endl; // Showing live score at bottom.
            processInput(ate);                   // Processing the input
            sleep(90);                           // Sleeping for 90 ms.
            clearScreen();
        }
    }
    void spawnFood()
    { // Spawns new food in the plane once the food is eaten.
        while (isPresent(food[0], food[1]))
        {
            srand(time(0));
            food[0] = rand() % (dimensions[0] - 2) + 1;
            food[1] = rand() % (dimensions[1] - 2) + 1;
        }
    }
    char getInput()
    { // Gets the input from the user.
        if (kbhit())
        {
            char a = getch();
            if (a == 'w' || a == 'a' || a == 's' || a == 'd')
            {
                DEFAULT = a; // To keep the snake moving in the same direction till different key is pressed.
                return a;
            }
            else if (a == 'p') // User paused the game. We do not want this to be dafault so default is not changed.
                return a;
            else
                return DEFAULT; // User pressed a key which isnt a part of standard commands.
        }
        return DEFAULT; // No input so default is returned.
    }
    void insert(int a, int b) // Inserting new element at the start of the linked list(here, Snake)
    {
        Node *temp = new Node;
        temp->x = a;
        temp->y = b;
        temp->next = head;
        head = temp;
    }
    void pop() // Removing the last element of the linked list(here, Snake)
    {
        Node *t = head;
        while (t->next->next != NULL)
            t = t->next;
        t->next = NULL;
    }
    void pausePlay() // Pauses/Plays the game
    {
        while (paused)
        {
            if (kbhit())
            {
                char inp = getch();
                if (inp == 'p')
                    paused = !paused;
            }
        }
    }
    void processInput(bool eaten = false) // Processes the input.
    {
        char inp = getInput();
        int x = head->x;
        int y = head->y;
        if (inp == 'a')
            x -= 1;
        else if (inp == 'd')
            x += 1;
        else if (inp == 'w')
            y -= 1;
        else if (inp == 's')
            y += 1;
        else if (inp == 'p')
        {
            paused = !paused;
            if (paused)
                pausePlay();
        }
        if (!paused)
            insert(x, y);
        if (!eaten)
            pop();
    }
};

int main()
{
    Snake s;
    return 0;
}
