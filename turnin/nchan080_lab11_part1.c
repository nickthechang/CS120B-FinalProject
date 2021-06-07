/*	Author: Nicholas Chang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #11  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#endif



typedef struct task {
	signed char state; 
	unsigned long int period; 
	unsigned long int elapsedTime; 
	int (*TickFct)(int); 
} task;


unsigned long int findGCD(unsigned long int a, unsigned long int b){
        unsigned long int c;
        while (1){
                c = a%b;
                if (c == 0) {return b;}
                a = b;
                b = c;
        }
        return 0;
}


int SIZE = 1;

int valinarray(short val, short val2, short arr[], short arr2[]){
	for(int i = 0; i< SIZE;i++){
		if(arr[i] == val && arr2[i] == val2) return 1;
	}
	return 0;
}

unsigned short randc[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
unsigned short randr[5] = {0xEF, 0xF7, 0xFB, 0xFD, 0xFE}; 

//int randomnumber;
//int randomnumber2;
//int randomnumber = (rand() % 8) ;
//int randomnumber2 = (rand() % 5) ;

int randomnumber = 4;
int randomnumber2 = 4;

unsigned short columns[40] = {0x20, /*0x10, 0x08, 0x04, 0x02,*/0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned short rows[40] = {0xFB, /*0xFB, 0xFB, 0xFB, 0xFB*/0,0,0,0 ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 


enum Direction_States {b_start, b_release, b_press};
int Direction_Tick(int state) {

	unsigned char A0 = ~PINA & 0x01;
	unsigned char A1 = ~PINA & 0x02;
	unsigned char A2 = ~PINA & 0x04;
	unsigned char A3 = ~PINA & 0x08;


	unsigned char temp;
	unsigned char temp2;


	switch (state) {
		case b_start:
			state = b_release;
			break;

		case b_release:
			if (A0 || A1 || A2 || A3){
				state = b_press;
			} else {
				state = b_release;
			}
			break;

		case b_press:
			if (A0 || A1 || A2 || A3){
				state = b_press;
			} else {
				state = b_release;
			}
			break;

		default:
			state = b_press;
			break;
	}
	switch (state) {
                case b_start:
                        break;

                case b_release:
                        break;

		case b_press:	

			if (A1){ //down
				temp = rows[0]<<1| 0x01;
				temp2 = columns[0];
				if(rows[0] == 0xEF)
					rows[0] = 0xEF;
				else if(valinarray(temp, temp2, rows, columns) == 1){
					break;				
				}
				else{
					for(int i = SIZE-1; i> 0; i--){
						rows[i] = rows[i-1];
						columns[i] = columns[i-1];
					}
					rows[0] = temp;	
					columns[0] = temp2;		
				}	
				
			}if (A0){ //up
				temp = rows[0]>>1| 0x80;
				temp2 = columns[0];
				if(rows[0] == 0xFE)
					rows[0] = 0xFE;
				else if(valinarray(temp, temp2, rows, columns) == 1){
					break;				
				}
				else{
					for(int i = SIZE-1; i> 0; i--){
						rows[i] = rows[i-1];
						columns[i] = columns[i-1];
					}
					rows[0] = temp;	
					columns[0] = temp2;		
				}	
			}if (A3){ //right
				temp = rows[0];
				temp2 = columns[0]>>1;
				if(columns[0] == 0x01)
					columns[0] = 0x01;
				else if(valinarray(temp, temp2, rows, columns) == 1){
					break;				
				}
				else{
					for(int i = SIZE-1; i> 0; i--){
						rows[i] = rows[i-1];
						columns[i] = columns[i-1];
					}
					rows[0] = temp;	
					columns[0] = temp2;		
				}	
			}if (A2){ //left
				temp = rows[0];
				temp2 = columns[0]<<1;
				if(columns[0] == 0x80)
					columns[0] = 0x80;
				else if(valinarray(temp, temp2, rows, columns) == 1){
					break;				
				}
				else{
					for(int i = SIZE-1; i> 0; i--){
						rows[i] = rows[i-1];
						columns[i] = columns[i-1];
					}
					rows[0] = temp;	
					columns[0] = temp2;		
				}	
			}

                        break;
		default:
			break;
	}		
	return state;
}

int index = 0;

enum Draw_state {snek_0};
int Draw_Tick(int state) {
	switch (state) {
		case snek_0:
			state = snek_0;	
			break;

		default:	
			state = snek_0;
			break;
	}	
	switch (state) {
		case snek_0:
			
			PORTC = columns[index];
			PORTD = rows[index];
			index++;
			if(index == SIZE){
				index = 0;			
			}
			break;
		
		default:
			break;
	}
	return state;
}



enum Interact_state {start_0, dot_1};
int Interact_Tick(int state) {
	switch (state) {
		case start_0:
			state = dot_1;	
			break;
		case dot_1:
			if(rows[0] == randr[randomnumber2]  && columns[0] == randc[randomnumber]){
				rows[SIZE] = rows[SIZE-1];
				columns[SIZE] = columns[SIZE-1]; 
				SIZE++; 
				randomnumber = (rand() % 8) ;
				randomnumber2 = (rand() % 5) ;
				while(valinarray(randr[randomnumber2], randc[randomnumber], rows, columns) == 1){
					randomnumber = (rand() % 8) ;
					randomnumber2 = (rand() % 5) ;
				}	
				
				state = start_0;
			}
			break;
			
		default:	
			state = start_0;
			break;
	}	
	switch (state) {
		case start_0:	
			PORTC = randc[randomnumber];
			PORTD = randr[randomnumber2];
			break;

		case dot_1:
			
			PORTC = randc[randomnumber];
			PORTD = randr[randomnumber2];
			break;
		
		default:
			break;
	}
	return state;
}





int main(void) {
	srand(time(0));
 
	DDRA = 0x00;  	PORTA = 0xFF;
	DDRC = 0xFF;  	PORTC = 0x00;
	DDRD = 0xFF;  	PORTD = 0x00;

	
	static task task1, task2, task3;
	task *tasks[] = {&task1, &task2, &task3};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = -1;

	tasks[0]->state = start;
	tasks[0]->period = 100;
	tasks[0]->elapsedTime = tasks[0]->period;
	tasks[0]->TickFct = &Direction_Tick;

	tasks[1]->state = start;
	tasks[1]->period = 1;
	tasks[1]->elapsedTime = tasks[1]->period;
	tasks[1]->TickFct = &Draw_Tick;

	tasks[2]->state = start;
	tasks[2]->period = 1;
	tasks[2]->elapsedTime = tasks[2]->period;
	tasks[2]->TickFct = &Interact_Tick;

	unsigned short i;
	unsigned long gcd = tasks[0]->period;
	for (i = 1; i < numTasks; i++){
		gcd = findGCD(gcd, tasks[i]->period);
	}

	TimerSet(gcd);
	TimerOn();

    while (1) {
	for(i = 0; i < numTasks; i++){
		if(tasks[i]->elapsedTime == tasks[i]->period){
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += gcd;
	}
	while (!TimerFlag);
	TimerFlag = 0;
    }
    return 0;
}



