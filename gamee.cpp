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
        return top == nullptr;
    }
};
class maze
{
    undo stack;
    Node *head;
    Node *p;
    Node *k;
    Node *d;
    int b_x, b_y;
    int dx, dy, i_d, move_d;
    bool keyfound;
    bool door;
    int count;
    int undo_count;
    int total_moves;

public:
    maze()
    {
        head = nullptr;
        p = nullptr;
        k = nullptr;
        d = nullptr;
        keyfound = false;
        door = false;
        count = 5;
        undo_count = 0;
        total_moves = 0;
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
                total_moves--;
                p->ch = '.';
                p = p->next;
                p->ch = 'P';
            }
        }
        else if (m == KEY_UP)
        {

            if (p->up != nullptr)
            {
                total_moves--;
                p->ch = '.';
                p = p->up;
                p->ch = 'P';
            }
        }
        else if (m == KEY_DOWN)
        {

            if (p->down != nullptr)
            {
                total_moves--;
                p->ch = '.';
                p = p->down;
                p->ch = 'P';
            }
        }
        else if (m == KEY_LEFT)
        {

            if (p->prev != nullptr)
            {
                total_moves--;
                p->ch = '.';
                p = p->prev;
                p->ch = 'P';
            }
        }

        if (p->x_axis == k->x_axis && p->y_axis == k->y_axis)
        {
            keyfound = true;
        }

        if (m != 'u')
        {
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
                }
            }
        }
    }

    void placeRandomObjects()
    {
        srand(time(0)); 
        random_characters('P');
        random_characters('K');
        random_characters('D');

        for (int i = 0; i < 5; i++)
        {
            random_characters('C');
            random_characters('B');
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

        if (temp->ch == '.' &&
            (k == nullptr || (temp->x_axis != k->x_axis && temp->y_axis != k->y_axis)) &&
            (d == nullptr || (temp->x_axis != d->x_axis && temp->y_axis != d->y_axis)) &&
            (p == nullptr || (temp->x_axis != p->x_axis && temp->y_axis != p->y_axis)))
        {
            if (c == 'P')
            {
                p = temp;
                temp->ch = c;
                stack.push(p->x_axis, p->y_axis);
            }

            if (c == 'K')
            {
                k = temp;
                temp->ch = 'K';
            }
            if (c == 'D')
            {
                d = temp; 
                temp->ch = 'D';
            }
            if (c == 'C')
            {
                temp->ch = c;
            }
            if (c == 'B')
            {
                temp->ch = c;
            }
           
        }
        else
        {
            random_characters(c); // Retry if the spot is already taken
        }
        if (p && k && d)
        {
            total_moves = distance_player_to_key() + distance_key_to_door();
        }
    }
    int distance_key_to_door()
    {
        dx = d->x_axis - k->x_axis; 
        if (dx < 0)
        {
            dx = -dx;
        }
        dy = d->y_axis - k->y_axis;
        if (dy < 0)
        {
            dy = -dy;
        }
        return dx + dy;
    }
    int distance_player_to_key()
    {
        dx = k->x_axis - p->x_axis; 
        if (dx < 0)
        {
            dx = -dx;
        }
        dy = k->y_axis - p->y_axis; 
        if (dy < 0)
        {
            dy = -dy;
        }
        return dx + dy;
    }

    void displaymenu()
    {
        cout << "PLAY GAME" << endl;
        int i;
        cout << "press 1 for easy mode" << endl;
        cin >> i;
        if (i == 1)
        {
            moving();
            playGame(10);
        }
    }
    void moving()
    {
        printw("total moves %d", total_moves);
        printw("\n");
        i_d = move_d;
        move_d = distance_player_to_key();
        if (move_d <= i_d)
        {
            printw("moving closer");
            printw("\n");
        }
        else if (move_d > i_d)
        {
            printw("moving away");
            printw("\n");
        }
        printw("                    key status: %d", keyfound);
        printw("\n");
    }
    void displayMaze(int n)
    {
        clear(); 
        printw("display");
        printw("\n");
        moving();
        Node *row = head;
        for (int i = 0; i < n + 2; i++)
        {
            printw("# ");
        }
        printw("\n");
        for (int i = 0; i < n; i++)
        {
            Node *temp = row;
            printw("# ");
            for (int j = 0; j < n; j++)
            {
                printw("%c ", temp->ch);
                temp = temp->next;
            }
            printw("# ");
            row = row->down;
            printw("\n");
        }
        for (int i = 0; i < n + 2; i++)
        {
            printw("# ");
        }

        refresh(); // Refresh the screen to display the changes
    }

    void playGame(int n)
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

            displayMaze(n);
            moveKey = getch();
            if (moveKey == 'q') // quit
                break;
            else if (total_moves == 0)
            {
                break;
            }
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
    m.displaymenu();
    return 0;
}
