#include <iostream>
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
    char ch;
    moveee *next;
};
struct coins
{
    int x;
    int y;
};
class queue
{
    moveee *front;
    moveee *tail;

public:
    queue()
    {
        front = nullptr;
        tail = nullptr;
    }
    void enqueue(int xx, int yy, int c)
    {
        moveee *NN = new moveee;
        NN->next = nullptr;
        NN->x = xx;
        NN->y = yy;
        NN->ch = c;
        if (front == nullptr)
        {
            front = NN;
            tail = NN;
            return;
        }
        tail->next = NN;
        tail = NN;
    }
    moveee *peek()
    {
        return front;
    }
    void displayqueue()
    {
        moveee *temp = front;
        while (temp != nullptr)
        {
            printw("(%d, %d,%c) ", temp->x, temp->y, temp->ch);
            temp = temp->next;
        }
    }
};
class stack
{
    moveee *top;

public:
    stack()
    {
        top = nullptr;
    }
    void push(int xx, int yy, int c)
    {
        moveee *NN = new moveee;
        NN->x = xx;
        NN->y = yy;
        NN->ch == c;
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
    stack undo;
    queue coin;
    Node *head;
    Node *p;
    Node *k;
    Node *d;
    Node *p_i;
    int b_x, b_y;
    int dx, dy, i_d, move_k, move_d;
    coins *c1, *c2, *c3, *c4, *c5;
    int coin_count;
    bool keyfound;
    bool door;
    bool bomb;
    int count;
    int undo_count;
    int total_moves;
    int level;
    int score;

public:
    maze()
    {
        head = nullptr;
        p = nullptr;
        k = nullptr;
        d = nullptr;
        keyfound = false;
        door = false;
        bomb = false;
        undo_count = 0;
        total_moves = 0;
        coin_count = 0;
        score = 0;
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

    void insertnodes(int n)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                insert('.', i, j);
            }
        }
    }
    void set_level(int l)
    {
        level = l;
    }
    int get_level()
    {
        return level;
    }
    void move(int m, int n)
    {
        if (m == KEY_RIGHT)
        {

            if (p->next != nullptr)
            {
                total_moves--;
                p->ch = '.';
                if (p->next->ch == 'C')
                {
                    coin.enqueue(p->next->x_axis, p->next->y_axis, p->next->ch);
                    count++;
                    score += 2;
                }
                if (p->next->ch == 'B')
                {
                    bomb = true;
                    coin.enqueue(p->next->x_axis, p->next->y_axis, p->next->ch);
                }
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
                if (p->up->ch == 'C')
                {
                    coin.enqueue(p->up->x_axis, p->up->y_axis, p->up->ch);
                    count++;
                    score += 2;
                }
                if (p->up->ch == 'B')
                {
                    bomb = true;
                    coin.enqueue(p->up->x_axis, p->up->y_axis, p->up->ch);
                }
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
                if (p->down->ch == 'C')
                {
                    coin.enqueue(p->down->x_axis, p->down->y_axis, p->down->ch);
                    count++;
                    score += 2;
                }
                if (p->down->ch == 'B')
                {
                    bomb = true;
                    coin.enqueue(p->down->x_axis, p->down->y_axis, p->down->ch);
                }
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
                if (p->prev->ch == 'C')
                {
                    coin.enqueue(p->prev->x_axis, p->prev->y_axis, p->prev->ch);
                    count++;
                    score += 2;
                }
                if (p->prev->ch == 'B')
                {
                    bomb = true;
                    coin.enqueue(p->prev->x_axis, p->prev->y_axis, p->prev->ch);
                }
                p = p->prev;
                p->ch = 'P';
            }
        }

        if (p->x_axis == k->x_axis && p->y_axis == k->y_axis)
        {
            keyfound = true;
        }
        if (p->x_axis == d->x_axis && p->y_axis == d->y_axis && keyfound == 1)
        {
            door = true;
        }

        if (m != 'u')
        {
            undo.push(p->x_axis, p->y_axis, p->ch);
        }

        if (m == 'u' && undo_count != count)
        {
            if (!undo.isempty())
            {
                undo.pop();

                if (!undo.isempty())
                {

                    count--;
                    int undo_x = undo.top_x();
                    int undo_y = undo.top_y();

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

    void placeRandomObjects(int n)
    {
        srand(time(0));
        random_characters('P', n);
        random_characters('K', n);
        random_characters('D', n);

        for (int i = 0; i < 5; i++)
        {
            random_characters('C', n);
            random_characters('B', n);
        }
        if (p && k && d) // Check if player, key, and door are all initialized
        {
            // default hard mode
            total_moves = distance_player_to_key() + distance_key_to_door(); // Calculate the distance

            // Add moves based on the level
            if (level == 1)
            {
                count = 6;
                total_moves += 6; // Easy mode
            }
            else if (level == 2)
            {
                count = 4;
                total_moves += 2; // Medium mode
            }
            else
                count = 1;
        }
    }

    void random_characters(char c, int n)
    {

        int r_x = rand() % n;
        int r_y = rand() % n;

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
                p_i = temp;
                p = temp;
                temp->ch = c;
                undo.push(p->x_axis, p->y_axis, p->ch);
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
            random_characters(c, n);
        }
    }

    int distance_player_to_door()
    {
        dx = d->x_axis - p->x_axis;
        if (dx < 0)
        {
            dx = -dx;
        }
        dy = d->y_axis - p->y_axis;
        if (dy < 0)
        {
            dy = -dy;
        }
        return dx + dy;
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

    void displaymenu(int n)
    {
        // initializing move_k and move_d
        move_k = distance_player_to_key();
        move_d = distance_player_to_door();
        playGame(n);
    }
    void moving()
    {
        printw("total moves %d", total_moves);
        printw("               undo left: %d", count);
        printw("\n");

        if (!keyfound)
        {
            i_d = move_k;
            move_k = distance_player_to_key();
            printw("hint:");
            if (move_k <= i_d)
            {
                printw("moving closer to key");
                printw("\n");
            }
            else if (move_k > i_d)
            {
                printw("moving away from key");
                printw("\n");
            }
        }
        else
        {

            i_d = move_d;
            move_d = distance_player_to_door();
            if (move_d <= i_d)
            {
                printw("moving closer to door ");
                printw("\n");
            }
            else if (move_d > i_d)
            {
                printw("moving away from door");
                printw("\n");
            }
        }
        printw("key status: %d", keyfound);
        printw("\n");
    }
    void displayMaze(int n)
    {
        clear();
        if (door)
        {
            if (total_moves > 0)
            {
                score += 1;
            }
            gameover(n);
            printw("you won!!! \ncollected items:");
            coin.displayqueue();
        }
        if (bomb)
        {
            gameover(n);
            printw("Game over \ncollected items:");
            coin.displayqueue();
        }
        if (total_moves == 0)
        {
            gameover(n);
            printw("Game over \ncollected items:");
            coin.displayqueue();
        }
        printw("\n");
        printw("score: %d\n", score);
        if (!door && !bomb && total_moves != 0)
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
    void gameover(int n)
    {
        clear(); // Clear the screen
        moveee *c = coin.peek();
        Node *row = head;

        // resenting the coin positions where they were collected
        for (int i = 0; i < n; i++)
        {
            Node *temp = row;
            for (int j = 0; j < n; j++)
            {
                // Check if the current node matches the coin's position
                if (c != nullptr && c->x == temp->x_axis && c->y == temp->y_axis)
                {
                    temp->ch = 'C';
                    c = c->next;
                }

                // Reset the player's previous position
                if (temp->ch == 'P')
                {
                    temp->ch = '.';
                }

                temp = temp->next;
            }
            row = row->down;
        }
        // placing player key and door on their original positions
        if (p_i != nullptr)
        {
            p_i->ch = 'P';
            d->ch = 'D';
            k->ch = 'K';
        }
    }

    void playGame(int n)
    {
        initscr();            // This function initializes the ncurses mode.
        cbreak();             // Disable line buffering.
        noecho();             // Don't display typed characters.
        keypad(stdscr, TRUE); // Enable special keys.

        int moveKey;
        while (!bomb && !door && total_moves != 0)
        {
            displayMaze(n);
            moveKey = getch();
            if (moveKey == 'q')
                break;

            move(moveKey, n);
        }
        int exitKey = 0;
        while (exitKey != 'e') // Wait until 'e' is pressed
        {
            displayMaze(n);
            printw("\nPress 'e' to exit.");
            refresh();
            exitKey = getch();
        }
        endwin(); // End ncurses mode
    }
};

int main()
{
    maze m;
    cout << "PLAY GAME" << endl;
    cout << "press 1 for easy mode" << endl;
    cout << "press 2 for medium mode" << endl;
    cout << "press 3 for hard mode" << endl;
    int l, n;
    cin >> l;
    if (l > 0 && l <= 3)
    {
        if (l == 1)
        {
            n = 10;
        }
        if (l == 2)
            n = 15;
        if (l == 3)
            n = 20;
        m.set_level(l);
        m.insertnodes(n);
        m.placeRandomObjects(n);
        m.displaymenu(n);
    }
    else
    {
        cout << "invlaid input" << endl;
    }
    return 0;
}
