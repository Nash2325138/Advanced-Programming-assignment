#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define WIN 1
#define LOSE 2
#define DRAW 3

typedef struct Pile{
    int array[53];
    int first, last;
    char exist;
}Pile;

typedef struct Deck{
    int array[53];
    int top, bottom;
}Deck;

typedef struct State{
	int num_dealt;

    Pile pile[7];
    Deck deck;
    int poll;
}State;

int debug_condition(int num_dealt)
{
    return 1;
}
void print_deck(Deck *deck)
{
    int i;
    printf("deck: ");
    for(i=deck->top ; i!=deck->bottom ; i++, i%=53){
        printf("%d ", deck->array[i]);
    }
    printf("\n");
}
void print_pile(Pile *pile)
{
    int i, j;
    printf("7 piles :\n");
    for(i=0 ; i<7 ; i++){
        printf("#%d: ", i);
        if(pile[i].exist==0) printf("-1");
        else{
            for(j=pile[i].first ; j!=pile[i].last ; j++, j%=53){
                printf("%d ", pile[i].array[j]);
            }
        }
        printf("\n");
    }
}

void state_copy(State *src, State *dst);
void construct_state(State* state, Deck* deck, Pile* pile, int poll, int num_dealt);
int find_first_loop(State *s1, int diff);
void advance_one_step(State *s);
int stateIsEqual(State* state, Deck* deck, Pile *pile, int poll);

int popDeckTop(Deck *deck)
{
    int rtVal = deck->array[deck->top];
    deck->top++;
    deck->top %= 53;
    return rtVal;
}
void pushDeckBottom(Deck *deck, int num)
{
    deck->array[deck->bottom] = num;
    deck->bottom++;
    deck->bottom %= 53;
}
int calc_next_poll(Pile *pile, int poll)
{
	int i;
	for(i=1 ; i<=7 ; i++)
	{
		if(pile[ (i+poll)%7 ].exist==0) continue;
		else return (i+poll)%7;
	}
	return -1;
}

void index_change(int *index, int shamt)
{
    if(shamt >= 0){
        (*index) += shamt;
        (*index) %= 53;
    } else {
        (*index) += shamt;
        if((*index) < 0) (*index) += 53;
    }
}

int main()
{
    /*if( freopen("246.in", "r", stdin) < 0) perror("freopen error");
    if( freopen("mine.out", "w", stdout) < 0) perror("freopen error");*/

    int i;
    Pile pile[7];
    Deck deck;
    State lastState;
    State last_lastState;
    int num_dealt;
    int poll, flag, diff;

    /*int input[52];  // for debug
    FILE *write_special_case = fopen("special_case.in", "w");*/

    scanf("%d", &deck.array[0]);
    /*input[0] = deck.array[0];*/
    while(deck.array[0]!=0)
    {
        for(i=1 ; i<52 ; i++){
            scanf("%d", &deck.array[i]);
           /* input[i] = deck.array[i];*/
        }
        deck.top = 0;
        deck.bottom = 52;   /* left(top) close right(bottom) open */

        for(i=0 ; i<7 ; i++){
            pile[i].exist = 1;
            pile[i].first = 0;   /* left(first) close right(last) open */
        	pile[i].last = 1;
        	pile[i].array[0] = popDeckTop(&deck);
        }

        /*if( debug_condition(1)){
            print_deck(&deck);
            print_pile(pile);
        }*/
        poll=6;
        num_dealt = 7;
        construct_state(&lastState, &deck, pile, poll, num_dealt);
        /* start playing the game */
        while(1)
        {
            poll = calc_next_poll(pile, poll);
            if(poll==-1){
                flag = WIN;
                break;
            }
            if( deck.top==deck.bottom ){
                flag = LOSE;
                break;
            }

        	/*draw a card to the last of the polled pile*/
        	pile[poll].array[ pile[poll].last++] = popDeckTop(&deck);
        	pile[poll].last %= 53;
            num_dealt++;

            /*if( debug_condition( num_dealt) ){
                printf("\nnum_dealt == %d, poll==%d(before)\n", num_dealt, poll);
                print_deck(&deck);
                print_pile(pile);
            }*/
            /* examine whether the pile has fit one of the three rules
            if it has, repeat examining again */
            while(1) {
                if( (  (pile[poll].last < pile[poll].first )? 53 : 0 ) + pile[poll].last - pile[poll].first < 3 ) break;

                int cardValue[5];
                /* the first two */
                cardValue[0] = pile[poll].array[ pile[poll].first ];
                cardValue[1] = pile[poll].array[ (pile[poll].first +1)%53 ];
                /* the last three */
                cardValue[2] = pile[poll].array[ pile[poll].last-3 + ( (pile[poll].last-3 < 0) ? 53 : 0) ];
                cardValue[3] = pile[poll].array[ pile[poll].last-2 + ( (pile[poll].last-2 < 0) ? 53 : 0) ];
                cardValue[4] = pile[poll].array[ pile[poll].last-1 + ( (pile[poll].last-1 < 0) ? 53 : 0)];

                /*if(debug_condition( num_dealt)){
                    printf("cardValue[]: ");
                    for(i=0 ; i<5 ; i++){
                        printf("%d ", cardValue[i]);
                    }
                    printf("\n");
                }*/

                if( (cardValue[0] + cardValue[1] + cardValue[4] ) % 10 == 0 ){
                    pushDeckBottom( &deck, cardValue[0] );
                    pushDeckBottom( &deck, cardValue[1] );
                    pushDeckBottom( &deck, cardValue[4] );
                    index_change(&(pile[poll].first), 2);
                    index_change(&(pile[poll].last), -1);
                } else if ( (cardValue[0] + cardValue[3] + cardValue[4] ) % 10 == 0 ){
                    pushDeckBottom( &deck, cardValue[0] );
                    pushDeckBottom( &deck, cardValue[3] );
                    pushDeckBottom( &deck, cardValue[4] );
                    index_change(&(pile[poll].first), 1);
                    index_change(&(pile[poll].last), -2);
                } else if( (cardValue[2] + cardValue[3] + cardValue[4] ) % 10 == 0 ){
                    pushDeckBottom( &deck, cardValue[2] );
                    pushDeckBottom( &deck, cardValue[3] );
                    pushDeckBottom( &deck, cardValue[4] );
                    index_change(&(pile[poll].last), -3);
                } else {
                    break;
                }
            }
            if(pile[poll].first == pile[poll].last ) pile[poll].exist = 0;

            if( stateIsEqual(&lastState, &deck, pile, poll) ){
                flag = DRAW;
                diff = num_dealt - lastState.num_dealt;
                break;
            }

            if( num_dealt == lastState.num_dealt * 2){
                state_copy(&lastState, &last_lastState);
                construct_state(&lastState, &deck, pile, poll, num_dealt);
            }
            /*if(debug_condition( num_dealt)){
                printf("\nnum_dealt == %d, poll==%d(after)\n", num_dealt, poll);
                print_deck(&deck);
                print_pile(pile);
            }*/

        }
        if(flag==WIN) {
            printf("Win : %d\n", num_dealt);
        } else if(flag==LOSE) {
            printf("Loss: %d\n", num_dealt);
        } else if(flag==DRAW) {
            int first_loop = find_first_loop(&last_lastState, diff);
            printf("Draw: %d\n", first_loop);

            /*if (first_loop==190 || first_loop==217) {
                for(i=0 ; i<52 ; i++){
                    fprintf(write_special_case, "%d ", input[i]);
                }
                fprintf(write_special_case, "\n");
            }*/
        }
        scanf("%d", &deck.array[0]);
        /*input[0] = deck.array[0];*/
    }

    /*fclose(write_special_case);*/
    return 0;
}

int find_first_loop(State *s1, int diff)
{
    State s2;
    state_copy(s1, &s2);

   /* if( debug_condition( -1 ) ){
        printf("\nnum_dealt == %d, poll==%d(before)\n", s2.num_dealt, s2.poll);
        print_deck(&s2.deck);
        print_pile(s2.pile);
    }*/
    int i;
    for(i=0 ; i<diff ; i++) {
        advance_one_step(&s2);
       /* if( debug_condition( -1 ) ){
            printf("\nnum_dealt == %d, poll==%d(before)\n", s2.num_dealt, s2.poll);
            print_deck(&s2.deck);
            print_pile(s2.pile);
        }*/
    }

    while(1)
    {
        if(stateIsEqual(s1, &s2.deck, s2.pile, s2.poll)) return s2.num_dealt;
        advance_one_step(s1);
        advance_one_step(&s2);
    }
}

void advance_one_step(State *s)
{
            /* pick a pile that's still exist just after the last polled pile */
            s->poll = calc_next_poll(s->pile, s->poll);
            int poll = s->poll;
        	/* draw a card to the last of the polled pile */
        	s->pile[poll].array[ s->pile[poll].last++] = popDeckTop(&s->deck);
        	s->pile[poll].last %= 53;
            s->num_dealt++;

            /* examine whether the pile has fit one of the three rules
             if it has, repeat examining again */
            while(1) {
                if( (  (s->pile[poll].last < s->pile[poll].first )? 53 : 0 ) + s->pile[poll].last - s->pile[poll].first < 3 ) break;

                int cardValue[5];
                /* the first two */
                cardValue[0] = s->pile[poll].array[ s->pile[poll].first ];
                cardValue[1] = s->pile[poll].array[ (s->pile[poll].first +1)%53 ];
                /* the last three */
                cardValue[2] = s->pile[poll].array[ s->pile[poll].last-3 + ( (s->pile[poll].last-3 < 0) ? 53 : 0) ];
                cardValue[3] = s->pile[poll].array[ s->pile[poll].last-2 + ( (s->pile[poll].last-2 < 0) ? 53 : 0) ];
                cardValue[4] = s->pile[poll].array[ s->pile[poll].last-1 + ( (s->pile[poll].last-1 < 0) ? 53 : 0)];

                if( (cardValue[0] + cardValue[1] + cardValue[4] ) % 10 == 0 ){
                    pushDeckBottom( &s->deck, cardValue[0] );
                    pushDeckBottom( &s->deck, cardValue[1] );
                    pushDeckBottom( &s->deck, cardValue[4] );
                    index_change(&(s->pile[poll].first), 2);
                    index_change(&(s->pile[poll].last), -1);
                } else if ( (cardValue[0] + cardValue[3] + cardValue[4] ) % 10 == 0 ){
                    pushDeckBottom( &s->deck, cardValue[0] );
                    pushDeckBottom( &s->deck, cardValue[3] );
                    pushDeckBottom( &s->deck, cardValue[4] );
                    index_change(&(s->pile[poll].first), 1);
                    index_change(&(s->pile[poll].last), -2);
                } else if( (cardValue[2] + cardValue[3] + cardValue[4] ) % 10 == 0 ){
                    pushDeckBottom( &s->deck, cardValue[2] );
                    pushDeckBottom( &s->deck, cardValue[3] );
                    pushDeckBottom( &s->deck, cardValue[4] );
                    index_change(&(s->pile[poll].last), -3);
                } else {
                    break;
                }
            }
            if(s->pile[poll].first == s->pile[poll].last ) s->pile[poll].exist = 0;
}

void state_copy(State *src, State *dst)
{
    dst->num_dealt = src->num_dealt;
    dst->poll = src->poll;

    int i, j;
    dst->deck.top = src->deck.top;
    dst->deck.bottom = src->deck.bottom;
    for(i=0 ; i<53 ; i++) dst->deck.array[i] = src->deck.array[i];

    for(i=0 ; i<7 ; i++){
        dst->pile[i].exist = src->pile[i].exist;
        dst->pile[i].first = src->pile[i].first;
        dst->pile[i].last = src->pile[i].last;
        for(j=0; j<53 ; j++){
            dst->pile[i].array[j] = src->pile[i].array[j];
        }
    }
}

void construct_state(State* state, Deck* deck, Pile* pile, int poll, int num_dealt)
{
    int i, j;
    state->num_dealt = num_dealt;

    state->poll = poll;
    state->deck.top = deck->top;
    state->deck.bottom = deck->bottom;
    for(i=0 ; i<53 ; i++) state->deck.array[i] = deck->array[i];

    for(i=0 ; i<7 ; i++){
        state->pile[i].exist = pile[i].exist;
        state->pile[i].first = pile[i].first;
        state->pile[i].last = pile[i].last;
        for(j=0; j<53 ; j++){
            state->pile[i].array[j] = pile[i].array[j];
        }
    }
}

int stateIsEqual(State* state, Deck* deck, Pile *pile, int poll)
{
    int i, j, k;

    int sdTop = state->deck.top , dTop = deck->top;
    int sdBottom = state->deck.bottom , dBottom = deck->bottom;
    if(sdBottom-sdTop + ( (sdTop > sdBottom) ? 53:0 )
       != dBottom-dTop + ( (dTop > dBottom) ? 53:0 ) ) return 0;
    for(i=sdTop, j=dTop ; i!=sdBottom && j!=dBottom ; i++, j++){
        i %= 53;
        j %= 53;
        if(state->deck.array[i] != deck->array[j]) return 0;
    }

    int state_npoll = calc_next_poll(state->pile, state->poll);
    poll = calc_next_poll(pile, poll);

    for(k=0 ; k<7 ; k++)
    {
        if (state->pile[state_npoll].exist != pile[poll].exist) return 0;

        int spFirst = state->pile[state_npoll].first , spLast = state->pile[state_npoll].last;
        int pFirst = pile[poll].first , pLast = pile[poll].last;
        if(spLast-spFirst + ( (spFirst > spLast) ? 53:0 )
            != pLast-pFirst + ( (pFirst > pLast) ? 53:0 ) ) return 0;

        for(i=spFirst, j=pFirst ; i!=spLast && j!=pLast ; i++, j++){
            i %= 53;
            j %= 53;
            if(state->pile[state_npoll].array[i] != pile[poll].array[j]) return 0;
        }
        state_npoll = calc_next_poll(state->pile, state_npoll);
        poll = calc_next_poll(pile, poll);
    }

    return 1;
}
