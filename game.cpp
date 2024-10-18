#include <iostream>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
using namespace std;

struct Node
{
    char ch;
    Node *next;
    Node *prev;
    Node *up;
    Node *down;
    int x_axis;
    int y_axis;
};
struct moveee
{
    int x;
    int y;
    moveee *next;
};
class undo
{
    moveee *top;

public:
    undo()
    {
        top = nullptr;
    }
    void push(int xx, int yy)
    {
        moveee *NN = new moveee;
        NN->x = xx;
        NN->y = yy;
        NN->next = nullptr;
        if (top == nullptr)
        {
            top = NN;
        }
        else
        {
            NN->next = top;
            top = NN;
        }
    }
    void pop()
    {
        if (top == nullptr)
        {
            cout << "list empty nothing to delete" << endl;
        }
        moveee *temp = top;
        top = temp->next;
        temp->next = nullptr;
        delete temp;
    }
    int top_x()
    {
        return top->x;
    }
    int top_y()
    {
        return top->y;
    }
    bool isempty()
    {
        if (top == nullptr)
        {
            return true;
        }
        else
            return false;
    }
};
class maze
{
    undo stack;
    Node *head;
    Node *p;
    int k_x, k_y;
    int d_x, d_y;
    int p_x, p_y;
    int dx, dy, d;
    bool keyfound;
    bool door;
    int count = 5;
    int undo_count = 0;

public:
    maze()
    {
        head = nullptr;
        p = nullptr;
        keyfound = 0;
        door = 0;
    }

    void insert(char c, int i, int j)
    {
        Node *NN = new Node;
        NN->ch = c;
        NN->next = nullptr;
        NN->prev = nullptr;
        NN->up = nullptr;
        NN->down = nullptr;
        NN->x_axis = i;
        NN->y_axis = j;

        if (head == nullptr)
        {
            head = NN;
        }
        else
        {
            Node *temp = head;

            for (int row = 0; row < i; row++) // going to the exact row where we have to insert next node
            {
                temp = temp->down;
            }

            if (j > 0) // moving in a row and adding node at the end of the row
            {
                while (temp->next != nullptr)
                {
                    temp = temp->next;
                }
                temp->next = NN;
                NN->prev = temp;
            }
            // If j == 0, inserting at the beginning of a new row
            else
            {
                // Find the last node at j=0 index and place new node uder it
                Node *Upper_node = head;
                for (int row = 0; row < i - 1; row++)
                {
                    Upper_node = Upper_node->down;
                }

                if (Upper_node != nullptr)
                {
                    Upper_node->down = NN;
                    NN->up = Upper_node;
                }
            }

            if (i > 0 && j > 0)
            {
                Node *temp_up = head;
                for (int row = 0; row < i - 1; row++)
                {
                    temp_up = temp_up->down;
                }
                Node *col_up = temp_up;
                for (int col = 0; col < j; col++)
                {
                    col_up = col_up->next;
                }
                col_up->down = NN;
                NN->up = col_up;
            }
        }
    }

    void insertnodes()
    {
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                insert('.', i, j);
            }
        }
    }

    void move(int m)
    {
        if (m == KEY_RIGHT)
        {
            if (p->next != nullptr)
            {
                p->ch = '.';
                p = p->next;
                p->ch = 'P';
            }
        }
        if (m == KEY_UP)
        {
            if (p->up != nullptr)
            {
                p->ch = '.';
                p = p->up;
                p->ch = 'P';
            }
        }
        if (m == KEY_DOWN)
        {
            if (p->down != nullptr)
            {
                p->ch = '.';
                p = p->down;
                p->ch = 'P';
            }
        }
        if (m == KEY_LEFT)
        {
            if (p->prev != nullptr)
            {
                p->ch = '.';
                p = p->prev;
                p->ch = 'P';
            }
        }

        if (m != 'u')
        {
            // Push the current position to the undo stack only when it's a valid move
            stack.push(p->x_axis, p->y_axis);
        }

        // Undo functionality
        if (m == 'u' && undo_count != count)
        {
            if (!stack.isempty())
            {
                stack.pop();

                if (!stack.isempty())
                {

                    undo_count++;
                    int undo_x = stack.top_x();
                    int undo_y = stack.top_y();

                    p->ch = '.';

                    while (p->x_axis != undo_x || p->y_axis != undo_y)
                    {
                        if (p->x_axis < undo_x)
                        {
                            p = p->down;
                        }
                        else if (p->x_axis > undo_x)
                        {
                            p = p->up;
                        }
                        else if (p->y_axis < undo_y)
                        {
                            p = p->next;
                        }
                        else if (p->y_axis > undo_y)
                        {
                            p = p->prev;
                        }
                    }

                    p->ch = 'P';
                    p_x = undo_x;
                    p_y = undo_y;
                }
            }
        }
    }

    void placeRandomObjects()
    {
        srand(time(0)); // Seed the random number generator

        // Place 'P', 'K', 'D'
        random_characters('P');
        random_characters('K');
        random_characters('D');

        // Place coins (let's place 5 coins randomly)
        for (int i = 0; i < 5; i++)
        {
            random_characters('C');
        }
    }

    void random_characters(char c)
    {
        int r_x = rand() % 10;
        int r_y = rand() % 10;

        Node *temp = head;
        for (int i = 0; i < r_x; i++)
        {
            temp = temp->down;
        }
        for (int j = 0; j < r_y; j++)
        {
            temp = temp->next;
        }

        if (temp->ch == '.' && temp->x_axis != k_x && temp->y_axis != k_y && temp->x_axis != d_x && temp->y_axis != d_y && p_x != temp->x_axis && p_y != temp->y_axis)
        {
            if (c == 'P')
            {
                p = temp;
                temp->ch = c;
                p_x = temp->x_axis;
                p_y = temp->y_axis;
                stack.push(p->x_axis, p->y_axis);
            }

            if (c == 'K')
            {
                k_x = temp->x_axis;
                k_y = temp->y_axis;
                temp->ch = '.';
            }
            if (c == 'D')
            {
                d_x = temp->x_axis;
                d_y = temp->y_axis;
                temp->ch = '.';
            }
            if (c == 'C')
            {
                temp->ch = c;
            }
        }
        else
        {
            random_characters(c); // Retry if the spot is already taken
        }
    }
    int distance()
    {
        dx = k_x - p_x;
        dy = k_y - p_y;
        d = dx + dy; // Retry if the spot is already taken
        return d;
    }
    void displayMaze()
    {
        clear(); // Clear the screen
        Node *row = head;
        for (int i = 0; i < 12; i++)
        {
            printw("# ");
        }
        printw("\n");
        for (int i = 0; i < 10; i++)
        {
            Node *temp = row;
            printw("# ");
            for (int j = 0; j < 10; j++)
            {
                printw("%c ", temp->ch);
                temp = temp->next;
            }
            printw("# ");
            row = row->down;
            printw("\n");
        }
        for (int i = 0; i < 12; i++)
        {
            printw("# ");
        }

        refresh(); // Refresh the screen to display the changes
    }

    // Function to handle player movements in the game
    void playGame()
    {
        initscr();            // This function initializes the ncurses mode. It sets up the terminal to use ncurses
        cbreak();             // Disables line buffering, making each keypress immediately available to the program.
                              // Without cbreak(), input is buffered until the user presses Enter.
        noecho();             // don't want typed characters to appear on the screen
        keypad(stdscr, TRUE); // This function simplifies that by returning predefined constants like
                              // KEY_UP, KEY_DOWN, KEY_LEFT, and KEY_RIGHT when arrow keys are pressed.

        int moveKey;
        while (true)
        {
            displayMaze();
            moveKey = getch();

            if (moveKey == 'q') // quit
                break;

            move(moveKey);
        }

        endwin();
    }
};

int main()
{
    maze m;
    m.insertnodes();
    m.placeRandomObjects();
    m.playGame();
    return 0;
}
