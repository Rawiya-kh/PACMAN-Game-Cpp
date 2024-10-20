#include <iostream>
#include <ctime>
#include <ncurses.h>
using namespace std;
// boxes of the grid contains
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
// node to push in the undo stack , and coins queue
struct moveee
{
    int x;
    int y;
    char ch;
    moveee *next;
};
// queue to keep track of colleted coins
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
    // enqueue
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
    // get the first node
    moveee *peek()
    {
        return front;
    }
    // display the queue with ncurses functions
    void displayqueue()
    {
        moveee *temp = front;
        while (temp != nullptr)
        {
            //%d to display integers and %c for characters
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
    // push stack
    void push(int xx, int yy, int c)
    {
        moveee *NN = new moveee;
        NN->x = xx;
        NN->y = yy;
        NN->ch = c;
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
    // pop stack
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
    // get x axis of top
    int top_x()
    {
        return top->x;
    }
    // get y axis of top
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
    stack undo;         // stack object
    queue coin;         // queue object
    Node *head;         // head of the grid
    Node *p;            // pointer for player
    Node *k;            // key pointer
    Node *d;            // door pointer
    Node *p_i;          // initial position of the player for displaying original grid at last
    int dx, dy;         // coordinates of door
    int i_d;            // initial position of key and door used to compare
    int move_k, move_d; // new position of key and door
    bool keyfound;      // key flag
    bool door;          // door flag
    bool bomb;          // bomb flag
    int undo_count;     // count for undo moves
    int total_moves;    // count for total moves
    int level;          // difficulty level indicator
    int score;          // score count

public:
    maze() // constructor
    {
        head = nullptr;
        p = nullptr;
        k = nullptr;
        d = nullptr;
        keyfound = false;
        door = false;
        bomb = false;
        total_moves = 0;
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
            // going to the exact row where we have to insert next node
            for (int row = 0; row < i; row++)
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
                // Find the last node at j=0 index and place new node under it
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
            // making links to the NN
            if (i > 0 && j > 0)
            {
                // moving in a row to reach at the exact row above row of newnode
                Node *temp_up = head;
                for (int row = 0; row < i - 1; row++)
                {
                    temp_up = temp_up->down;
                }
                // moving in that particular row to find the node exaxt above the NN
                Node *col_up = temp_up;
                for (int col = 0; col < j; col++)
                {
                    col_up = col_up->next;
                }
                // making down link and up link
                col_up->down = NN;
                NN->up = col_up;
            }
        }
    }
    // i, j of nodes instertion
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
    // set level
    void set_level(int l)
    {
        level = l;
    }
    // get level
    int get_level()
    {
        return level;
    }
    // function to remove previous coins
    void remove_old_coins()
    {
        Node *row = head;
        while (row != nullptr) // till last row
        {
            Node *temp = row;
            while (temp != nullptr) // till last col
            {
                if (temp->ch == 'C') // checking each node whether it contains C
                {
                    temp->ch = '.';
                }
                temp = temp->next;
            }
            row = row->down;
        }
    }
    // adding new coins raandomly
    void new_coins(int n)
    {
        for (int i = 0; i < 5; i++)
        {
            random_characters('C', n);
        }
    }
    // move function
    void move(int m, int n)
    {
        if (m == KEY_RIGHT)
        {

            if (p->next != nullptr) // if right node exists
            {
                total_moves--;          // moves decrement
                p->ch = '.';            // placing . at the prev player position
                if (p->next->ch == 'C') // if next node conatins coin
                {
                    coin.enqueue(p->next->x_axis, p->next->y_axis, p->next->ch);
                    undo_count++; // bonus undo at coin
                    score += 2;
                }
                if (p->next->ch == 'B') // if next node is bomb
                {
                    bomb = true;
                    coin.enqueue(p->next->x_axis, p->next->y_axis, p->next->ch); // bomb enqueue (last node)
                }
                p = p->next; // moving p to new position
                p->ch = 'P'; // placing P there
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
                    undo_count++;
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
                    undo_count++;
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
                    undo_count++;
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

        if (p->x_axis == k->x_axis && p->y_axis == k->y_axis) // if player lands on the key
        {
            keyfound = true;
        }
        if (p->x_axis == d->x_axis && p->y_axis == d->y_axis && keyfound == 1) // if key found and player lands on door
        {
            door = true;
        }
        if (total_moves % 5 == 0) // new coins appearing when a multiple of 5 occurs in total moves
        {
            remove_old_coins();
            new_coins(n);
        }

        if (m != 'u') // if not undo move then push every move into the stack
        {
            undo.push(p->x_axis, p->y_axis, p->ch);
        }

        if (m == 'u' && undo_count != 0) // if undo move and undo moves >0
        {
            if (!undo.isempty()) // if stack is not empty
            {
                undo.pop(); // pop the current position of the player

                if (!undo.isempty()) // if still not empty stack
                {

                    undo_count--;              // undo decrement
                    int undo_x = undo.top_x(); // storing the undo move coordinates in variables
                    int undo_y = undo.top_y();

                    p->ch = '.'; // making curr p empty spot

                    while (p->x_axis != undo.top_x() || p->y_axis != undo_y) // till the player doesnot reaches the undo position
                    {
                        if (p->x_axis < undo_x) // if p->x is less the undo_x the players current move must be up so moving down
                        {
                            p = p->down;
                        }
                        else if (p->x_axis > undo_x) // the players current move must be down so moving up
                        {
                            p = p->up;
                        }
                        else if (p->y_axis < undo_y) // the players current move must be prev so moving next
                        {
                            p = p->next;
                        }
                        else if (p->y_axis > undo_y) // the players current move must be next so moving prev
                        {
                            p = p->prev;
                        }
                    }

                    p->ch = 'P'; // chaning the character at new p location
                }
            }
        }
    }

    void placeRandomObjects(int n)
    {
        srand(time(0)); // random generator
        // calling random_characters function with P,K,D,C,B
        random_characters('P', n);
        random_characters('K', n);
        random_characters('D', n);

        for (int i = 0; i < 5; i++) // 5 coins and bombs
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
                undo_count = 6;
                total_moves += 6; // Easy mode
            }
            else if (level == 2)
            {
                undo_count = 4;
                total_moves += 2; // Medium mode
            }
            else
                undo_count = 1;
        }
    }

    void random_characters(char c, int n)
    {

        int r_x = rand() % n; // random x axis
        int r_y = rand() % n; // random y axis

        Node *temp = head;
        // moving to the random node with the coordinates generated
        for (int i = 0; i < r_x; i++)
        {
            temp = temp->down;
        }
        for (int j = 0; j < r_y; j++)
        {
            temp = temp->next;
        }
        // if spot is empty and is not already taken and is not taken by invisible key and door
        if (temp->ch == '.' &&
            (k == nullptr || (temp->x_axis != k->x_axis && temp->y_axis != k->y_axis)) &&
            (d == nullptr || (temp->x_axis != d->x_axis && temp->y_axis != d->y_axis)))
        {
            if (c == 'P')
            {
                p_i = temp; // initilizing p_i and p to where the P is randomly placed
                p = temp;
                temp->ch = c;
                undo.push(p->x_axis, p->y_axis, p->ch); // pushing the initial position of the player
            }

            if (c == 'K')
            {
                k = temp;
                temp->ch = '.'; // making key invisible
            }
            if (c == 'D')
            {
                d = temp;
                temp->ch = '.'; // making key invisible
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
            random_characters(c, n); // if spot already taken call function again
        }
    }
    // functions for distances
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
        // initializing move_k and move_d to the inital distances of player to key and player to door
        move_k = distance_player_to_key();
        move_d = distance_player_to_door();
        playGame(n);
    }
    void moving()
    {
        printw("total moves %d", total_moves);
        printw("               undo left: %d", undo_count);
        printw("\n");
        printw("hint:");

        if (!keyfound)
        {
            i_d = move_k;                      // storing prev distance
            move_k = distance_player_to_key(); // storing new distance
            if (move_k <= i_d)                 // comparing new and prev distance
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
            i_d = move_d;                       // storing prev distance
            move_d = distance_player_to_door(); // storing new distance
            if (move_d <= i_d)                  // comparing new and prev distance
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
        clear();  // clear the prev screen
        if (door) // if door found end the game by displaying this
        {
            if (total_moves > 0)
            {
                score += 1; // if end with moves left
            }
            gameover(n); // displays initial positions of C P K and D
            printw("you won!!! \ncollected items:");
            coin.displayqueue(); // display coin queue
            printw("\nthis was the original state of the game!!\n");
        }
        if (bomb)
        {
            gameover(n);
            printw("Game over \ncollected items:");
            coin.displayqueue();
            printw("\nthis was the original state of the game!!\n");
        }
        if (total_moves == 0)
        {
            gameover(n);
            printw("Game over \ncollected items:");
            coin.displayqueue();
            printw("\nthis was the original state of the game!!\n");
        }
        printw("\n");
        printw("score: %d\n", score);
        if (!door && !bomb && total_moves != 0) // if game is conitnue call moving function for hint
            moving();
        Node *row = head;
        for (int i = 0; i < n + 2; i++) // upper border
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
        for (int i = 0; i < n + 2; i++) // lower border
        {
            printw("# ");
        }

        refresh(); // Refresh the screen to display the changes
    }
    void gameover(int n)
    {
        clear();                 // Clear the screen
        moveee *c = coin.peek(); // getting the first node in the queue and placing the coins at the collected places
        Node *row = head;

        // reseting the coin positions where they were collected
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
        while (!bomb && !door && total_moves != 0) // game continue
        {
            displayMaze(n);
            moveKey = getch(); // getting arrowkeys and function keys using ncurses function
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
            exitKey = getch(); // getting the exit key
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
