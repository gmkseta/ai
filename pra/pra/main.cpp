#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#define SIDE		3
#define SIZE	  	SIDE * SIDE		// board size

// data structure for states
struct listnode {
	struct listnode		*parent;	// parent state
	char		board[SIZE];		// board arrangement
	int			g;			// cost from Start to this state
	int			h;			// estimated cost from this state to Goal
	int			f;			// f = g + h
	struct listnode		*next;
};
typedef struct listnode STATE;

// Global variables for states and state pointers
STATE OPEN, CLOSED;			// dummy head for OPEN and CLOSED list
STATE STARTSTATE, GOALSTATE;		// Start and Goal states

STATE *open_ptr = &OPEN, *closed_ptr = &CLOSED;
STATE *start = &STARTSTATE, *goal = &GOALSTATE, *current, *child[4];

// functions
void generate_children(STATE *s);
int compute_h(STATE *s);
int check_open(STATE *s);
int check_closed(STATE *s);
int is_same(STATE *s1, STATE *s2);
void insert(STATE *l, STATE *s);
void print_board(STATE *s);
void print_solution(STATE *s);
void choose_to_go();
void expand_next(STATE *s);
STATE* copy_state(STATE *s);

void print_all_board();
void main()
{
	char str[10];
	int i, state_count = 0;

	// input start state (ex> 1238x4765) 
	printf("\n\n Start position : ");
	//scanf("%s", str);
	strcpy_s(str, 10, "2831x4765");

	for (i = 0; i<SIZE; i++) start->board[i] = str[i];

	//strcpy_s(str, 10, "283x14765");
	strcpy_s(str, 10, "1238x4765");
	// input goal state 
	printf("\n Goal position :  ");
	//scanf("%s", str);
	for (i = 0; i<SIZE; i++) goal->board[i] = str[i];

	start->parent = NULL;
	start->g = 0;
	start->h = compute_h(start);
	start->f = start->g + start->h;

	// A* search ///����
	//�ϴ� open_prt���ٰ� start �־��� ù state�� �־��ִµ�
	insert(open_ptr, start);
	//���� open_ptr ->next�� start�� ����

	while (open_ptr->next != NULL){
		

		printf(".");
		state_count++;//�ѹ� �������� state_count �� �ϳ��� �÷���

		// choose
		// set current as the first state in open list and remove it from open list
		choose_to_go();//open list���� ���� ù��° ������Ʈ�� current���ٰ� �ְ� remove ����
		
		// goal test
		// if current = goal, stop 
		if (is_same(current, goal))//������ Ȯ����/ ������ break
			break;

		//���� �ƴ϶�� ���� child���� Ȯ���Ͽ� �ְ� open�� close�� Ȯ���Ͽ� �̹� �ִ°Ŷ�� ���ְ� �ƴ϶�� openlist�� �־���
		// expand
		// generate next states, check open and closed list, and insert children into open list
		else
		{
			expand_next(current);
			print_all_board();

		}
		//getch();
	}

	// display search cost and path cost
	printf("\n search cost = %d", state_count);
	printf("\n path cost   = %d \n\n", current->g);
	getchar();

	// display solution path
	print_solution(current);
	getchar();

}
void expand_next(STATE *s) {
	generate_children(s);

	for (int i = 0; i < 4; i++)
	{
		if (child[i])
		{
			if (check_open(child[i]))
			{
				free(child[i]);
				child[i] = NULL;
			}
			else if (check_closed(child[i]))
			{
				free(child[i]);
				child[i] = NULL;
			}
			else
			{
				insert(open_ptr, child[i]);
			}
		}
	}
}
void choose_to_go()
{
	//�ֱٸ������ �ְ�
	current = open_ptr->next;

	//������Ͽ��� ����
	open_ptr->next = open_ptr->next->next;

	
}

// generate next states (child[i]) of s
void generate_children(STATE *s)
{
	int i, blank, row, col;

	for (i = 0; i<4; i++) child[i] = NULL;

	for (i = 0; i<SIZE; i++)
		if (s->board[i] == 'x'|| s->board[i] == 'X') blank = i;	// position of x
	row = blank / SIDE; // ��
	col = blank % SIDE;//��

	// down �� �Ʒ��� �ƴ϶�� child �� 
	if (row != SIDE - 1) {
		child[0] = (STATE *)malloc(sizeof(STATE));
		*child[0] = *s;//�Ű������� ���� ���忡�� 
		child[0]->board[blank] = child[0]->board[blank + SIDE]; 
		child[0]->board[blank + SIDE] = 'x';
		//x�� side��ŭ �߰��� index�� ��ġ�� �ٲ��� ( down)
	}
	// right ���� �������� �ƴ϶��
	if (col != SIDE - 1) {
		child[1] = (STATE *)malloc(sizeof(STATE));
		*child[1] = *s;
		child[1]->board[blank] = child[1]->board[blank + 1];
		child[1]->board[blank + 1] = 'x';
		//x�� 1��ŭ �߰��ѰͰ� �ٲ��� ( right)
	}
	// up //������ ���� ����
	if (row != 0) {
		child[2] = (STATE *)malloc(sizeof(STATE));
		*child[2] = *s;
		child[2]->board[blank] = child[2]->board[blank - SIDE];
		child[2]->board[blank - SIDE] = 'x';
	}
	// left 
	if (col != 0) {
		child[3] = (STATE *)malloc(sizeof(STATE));
		*child[3] = *s;
		child[3]->board[blank] = child[3]->board[blank - 1];
		child[3]->board[blank - 1] = 'x';
	}

	for (i = 0; i<4; i++) {
		if (child[i]) {//child �� ������
			if ((s->parent != NULL) && (is_same(child[i], s->parent))) {
				//parent�� null�� �ƴϸ鼭  
				//child �� parent�� ���ٸ� ( �� ���� state ��� ) �׳� free child , null �־���
				free(child[i]);
				child[i] = NULL;
			}
			else {//�װ� �ƴ϶�� ����
				child[i]->parent = s;
				child[i]->g = s->g + 1;
				child[i]->h = compute_h(child[i]);
				child[i]->f = child[i]->g + child[i]->h;
			}
		}
	}
}

// compute heuristic value h of state s
int compute_h(STATE *s)
{
	//SJ
	//�켱 ���� �����ϰ� ��� ��� ���� ������ ���ϴ� �Լ���.
	int size = sizeof(s->board) / sizeof(s->board[0]);
	int count = 9;
	for (int i = 0; i < size; i++) {
		if (goal->board[i] == s->board[i])
			count--;
	}
	s->h = count;
	// estimated distance from s to Goal
	// ex> number of tiles out of place
	return count;

}

// check whether s is in OPEN and f(s) is smaller
// if so, change f(s) value and return TRUE
int check_open(STATE *s)
{
	STATE* temp;
	temp = open_ptr;
	while (temp->next)
	{
		if (is_same(s, temp))
		{
			if (s->f < temp->f)
			{
				insert(open_ptr, s);
				
				return 1;
			}
		}
		temp = temp->next;
	}
	// for each state n in open list
	// if s = n and f(s) < f(n), update n, return 1
	// if s is not in open list, return 0
	return 0;

}
// check whether s is in CLOSED and f(s) is smaller
// if so, change f(s) value and insert s back to OPEN
int check_closed(STATE *s)
{
	STATE* temp;
	temp = closed_ptr;
	while (temp->next)
	{
		if (is_same(s, temp))
		{
			if (s->f < temp->f)
			{
				insert(open_ptr, s);
				return 1;
			}
		}

		temp = temp->next;
	}
	// for each state n in closed list
	// if s = n and f(s) < f(n), 
	// remove n from closed list, insert s into open list, return 1
	// if s is not in closed list, return 0
	return 0;

}

// TRUE if s1 and s2 are same
int is_same(STATE *s1, STATE *s2)
{
	int i;

	for (i = 0; i<SIZE; i++) {
		if (s1->board[i] != s2->board[i])
			return (0);
	}
	return (1);
}
STATE* copy_state(STATE *s)
{
	STATE *temp;
	temp = (STATE *)malloc(sizeof(STATE));
	temp->parent = s->parent;
	temp->f = s->f;
	temp->g = s->g;
	temp->h = s->h;
	temp->next = NULL;
	for (int i = 0; i < SIZE; i++)
	{
		temp->board[i] = s->board[i];
	}
	return temp;

}
// insert state s to list l. list l is in sorted order
void insert(STATE *l, STATE *s)
{
	STATE *temp;

	while (l->next != NULL) {
		if (s->f < l->next->f) break;
		l = l->next;
	}

	temp = copy_state(s);
	temp->next = l->next;
	l->next = temp;

}

// print solution by display states sequentially from Start to Goal
void print_solution(STATE *s)
{
	if (s == NULL) return;
	print_solution(s->parent);
	getchar();
	print_board(s);
}

// display 3 x 3 board of state s
void print_board(STATE *s)
{
	int i;

	//	clrscr();
	//system("cls");
	//printf("\n\n");
	for (i = 0; i<SIZE; i++) {
		if (i%SIDE == 0) printf("\n");
		if (s->board[i] == 'x') printf("  %c", 'X');
		else printf("  %c", s->board[i]);
	}
	//printf("\n\n");
}

void print_all_board() {
	system("cls");

	printf("\n\n");
	print_board(current);

	printf("\n\n");
	print_board(open_ptr->next);
	printf("\ng: %d , h: %d ,f: %d", open_ptr->next->g, open_ptr->next->h,open_ptr->next->f);

}