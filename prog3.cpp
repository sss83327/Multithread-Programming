#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

char **map;

using namespace std;


struct thread_data
{
	pthread_t threadid;
	int bot_x, bot_y;
	int tbot_x, tbot_y;
	bool founded;
	int direction;
};

void *sideroad2(void *data)
{
	struct thread_data temp = * (struct thread_data*) data;
	int bot_x = temp.bot_x;
	int bot_y = temp.bot_y;
	int tbot_x = bot_x;
	int tbot_y = bot_y;
	cout << "[tid=" << pthread_self() << "] : (" <<  bot_x << "," << bot_y <<')'<< endl;
	
	if ( temp.direction == 1 )
		bot_x--;
	else if ( temp.direction == 2 )
		bot_x++;
	else if ( temp.direction == 3 )
		bot_y--;
	else if ( temp.direction == 4 )
		bot_y++;
	temp.direction = 0;
	//cout << bot_x << "				" << bot_y << "		" << tbot_x << "			" << tbot_y<<endl;
	while(1)
	{
		bool first_in = 0;
		bool left, right, top, down ;
		left=0;
		right=0;
		top=0;
		down=0;
		int sideroad = 0;
		if ( first_in == 0 ){
			if ( map[bot_x+1][bot_y] == ' ' ||map[bot_x+1][bot_y] == 'K' ){
				sideroad += 1;
				down = 1;
				//cout << "spacedown		";
			}
			if ( map[bot_x-1][bot_y] == ' ' || map[bot_x-1][bot_y] == 'K' ){
				sideroad += 1;
				top = 1;
				//cout << "spacetop		";
			}
			if ( map[bot_x][bot_y+1] == ' '|| map[bot_x][bot_y+1] == 'K' ){
				sideroad += 1;
				right = 1;
				//cout << "spaceright		";
			}
			if ( map[bot_x][bot_y-1] == ' ' || map[bot_x][bot_y-1] == 'K' ){
				sideroad += 1;
				left = 1;
				//cout << "spaceleft		";
			}
		}
		if ( sideroad == 1 ){  // END
			//cout << "NONE" ;
			//pthread_exit((void*)0);
			cout << temp.threadid <<  " (" << bot_x << ',' << bot_y << ") None!"<< endl ;
			break;
		}
		
		if ( sideroad == 3 )
		{
			//cout << bot_x << "     " << bot_y ;
			thread_data threadata1;
			threadata1.threadid = pthread_self();
			threadata1.bot_x = bot_x;
			threadata1.bot_y = bot_y;
			threadata1.tbot_x = tbot_x;
			threadata1.tbot_y = tbot_y;
			thread_data threadata2 = threadata1;
			if(1){
			if ( top == 1 && bot_x-1 != tbot_x ){
				threadata1.direction = 1;
				top = 0;
				//cout << "top1";
			}
			else if ( down == 1 && bot_x+1 != tbot_x  ){
				threadata1.direction = 2;
				down = 0;
				//cout << "down1";
			}
			else if ( left == 1 && bot_y-1 != tbot_y  ){
				threadata1.direction = 3;
				left = 0;
				//cout << "left1";
			}
			else if ( right == 1 && bot_y+1 != tbot_y  ){
				threadata1.direction = 4;
				right = 0;
				//cout << "right1";
			}
			}
			
			if ( top == 1 && bot_x-1 != tbot_x ){
				threadata2.direction = 1;
				top = 0;
				//cout << "top2";
			}
			else if ( down == 1 && bot_x+1 != tbot_x  ){
				threadata2.direction = 2;
				down = 0;
				//cout << "down2";
			}
			else if ( left == 1 && bot_y-1 != tbot_y  ){
				threadata2.direction = 3;
				left = 0;
				//cout << "left2";
			}
			else if ( right == 1 && bot_y+1 != tbot_y  ){
				threadata2.direction = 4;
				right = 0;
				//cout << "right2";
			}
			
			
			pthread_t temp1, temp2;
			void *exit1;
			void *exit2;
			
			pthread_create( &temp1, NULL, sideroad2, &threadata1 );		
			
			pthread_create( &temp2, NULL, sideroad2, &threadata2  );
			pthread_join(temp1,&exit1);			
			pthread_join(temp2,&exit2);
	
			if ( (int)exit1 == 1  || (int)exit2 == 1)
				cout << pthread_self() <<  " (" << bot_x << ',' << bot_y << ") Found!"<< endl ;
			//if ( (int)exit1 == 0  || (int)exit2 == 0)
			//	cout << pthread_self() <<  " (" << bot_x << ',' << bot_y << ") None!"<< endl ;
			//sleep(1);
			if ( (int)exit1 == 1 || (int)exit2 == 1 )
				pthread_exit((void*)1);
			else 
				pthread_exit((void*)0);
		}
		
		if( map[bot_x+1][bot_y] == ' ' && (bot_x+1 != tbot_x) )
		{
			tbot_x = bot_x;
			tbot_y = bot_y;
			bot_x += 1;			
			//cout << bot_x << "   	" << bot_y <<  "  ||  " << tbot_x << "   	" << tbot_y<<endl;
			if ( map[bot_x+1][bot_y] == 'K' )
			{
				cout << pthread_self() <<  " (" << bot_x+1 << ',' << bot_y << ") Found!"<< endl ;
				pthread_exit((void*)1);
				break;
			}
		} 
		
		else if( map[bot_x][bot_y+1] == ' ' && (bot_y+1 != tbot_y) )
		{
			tbot_x = bot_x;
			tbot_y = bot_y;
			bot_y += 1;
			//cout << bot_x << "   	" << bot_y <<  "  ||  " << tbot_x << "   	" << tbot_y<<endl;
			if ( map[bot_x][bot_y+1] == 'K' )
			{
				cout << pthread_self() <<  " (" << bot_x << ',' << bot_y+1 << ") Found!"<< endl ;
				pthread_exit((void*)1);
				break;
			}
		}
		
		else if( map[bot_x-1][bot_y] == ' ' && (bot_x-1 != tbot_x) )
		{
			tbot_x = bot_x;
			tbot_y = bot_y;
			bot_x -= 1;
			//cout << bot_x << "   	" << bot_y <<  "  ||  " << tbot_x << "   	" << tbot_y<<endl;
			//sleep(1);
			if ( map[bot_x-1][bot_y] == 'K' )
			{
				cout << pthread_self() <<  " (" << bot_x-1 << ',' << bot_y << ") Found!"<< endl ;
				pthread_exit((void*)1);
				break;
			}	
		}
		
		else if( map[bot_x][bot_y-1] == ' ' && (bot_y-1 != tbot_y) )
		{
			tbot_x = bot_x;
			tbot_y = bot_y;
			bot_y -= 1;
			//cout << bot_x << "   	" << bot_y <<  "  ||  " << tbot_x << "   	" << tbot_y<<endl;
			//sleep(1);
			if ( map[bot_x][bot_y-1] == 'K' )
			{
				cout << pthread_self() <<  " (" << bot_x << ',' << bot_y-1 << ") Found!"<< endl ;
				pthread_exit((void*)1);
				break;
			}
		}
		
	}
}



int main()
{
	struct timeval tv1, tv2;
	gettimeofday( &tv1, NULL);
	
	ifstream infile;// ("map.txt", ios::in);
	infile.open("map.txt", ios::in);
	if( !infile )
	{
		cout << "file could not be opened" << endl;
		exit(1);
	}
	//char **map = new char*[22]; // 先配置第一維大小為22個元素且每個元素為point to int
	map = new char*[22];
	for (int i = 0; i < 22; i++)
	{
		map[i] = new char[22];//將一個一個point to int的元素在配置指向一個一維陣列
	}

	int m = 0;
	while (infile.getline(map[m], 30))
	{
		m++;
	}
	
	pthread_t main_tid;
	main_tid = pthread_self();
	int start_x, start_y;
	int bot_x, bot_y;
	
	for ( int i = 0 ; i < 22; i++ )
	{
		for( int j = 0; j < 22 ; j++ )
		{
			if ( map[i][j] == 'S' )
			{
				start_x = i;
				start_y = j;
				break;
			}	
		}
	}
	int tbot_x, tbot_y; // trace of bot
	bot_x = start_x;
	bot_y = start_y;	
	tbot_x = bot_x;
	tbot_y = bot_y;
	cout << "[tid=" << pthread_self() << "] : (" <<  bot_x << "," << bot_y <<')'<< endl;
	int stop = 0 ;
	
	while(stop == 0)
	{
		bool left, right, top, down = 0;
		int sideroad = 0;
		if ( map[bot_x-1][bot_y] == ' ' ){
			sideroad += 1;
			top = 1;
			//cout << "spacetop 		";
		}
		if ( map[bot_x+1][bot_y] == ' ' ){
			sideroad += 1;
			down = 1;
			//cout << "spacedown		";
		}
		if ( map[bot_x][bot_y+1] == ' ' ){
			sideroad += 1;
			right = 1;
			//cout << "spaceright		";
		}
		if ( map[bot_x][bot_y-1] == ' ' ){
			sideroad += 1;
			left = 1;
			//cout << "spaceleft		";
		}
		
		if ( sideroad == 3 )
		{
			//cout << bot_x << "     " << bot_y ;
			thread_data threadata1;
			threadata1.threadid = pthread_self();
			threadata1.bot_x = bot_x;
			threadata1.bot_y = bot_y;
			threadata1.tbot_x = tbot_x;
			threadata1.tbot_y = tbot_y;
			thread_data threadata2 = threadata1;
		
			if(1){
			if ( top == 1 && bot_x-1 != tbot_x ){
				threadata1.direction = 1;
				top = 0;
				//cout << "1top";
			}
			else if ( down == 1 && bot_x+1 != tbot_x  ){
				threadata1.direction = 2;
				down = 0;
				//cout << "1down";
			}
			else if ( left == 1 && bot_y-1 != tbot_y  ){
				threadata1.direction = 3;
				left = 0;
				//cout << "1left";
			}
			else if ( right == 1 && bot_y+1 != tbot_y  ){
				threadata1.direction = 4;
				right = 0;
				//cout << "1right";
			}
			}
			if ( top == 1 && bot_x-1 != tbot_x ){
				threadata2.direction = 1;
				top = 0;
				//cout << "2top";
			}
			else if ( down == 1 && bot_x+1 != tbot_x  ){
				threadata2.direction = 2;
				down = 0;
				//cout << "2down";
			}
			else if ( left == 1 && bot_y-1 != tbot_y  ){
				threadata2.direction = 3;
				left = 0;
				//cout << "2left";
			}
			else if ( right == 1 && bot_y+1 != tbot_y  ){
				threadata2.direction = 4;
				right = 0;
				//cout << "2right";
			}
			pthread_t temp1, temp2;
			void *exit1;
			void *exit2;
			
			pthread_create( &temp1, NULL, sideroad2, &threadata1 );		
			pthread_create( &temp2, NULL, sideroad2, &threadata2  );
			pthread_join(temp1,&exit1);	
			pthread_join(temp2,&exit2);
			if ( (int)exit1 == 1  || (int)exit2 == 1)
				cout << pthread_self() <<  " (" << bot_x << ',' << bot_y << ") Found!"<< endl ;
			//if ( (int)exit1 == 0  || (int)exit2 == 0)
			//	cout << pthread_self() <<  " (" << bot_x << ',' << bot_y << ") None!"<< endl ;
			
			break;
		}
		if ( sideroad == 4 ){
			cout << "ERROR" <<endl;
			exit(0);
		}
		if( map[bot_x+1][bot_y] == ' ' && (bot_x+1 != tbot_x) )
		{
			tbot_x = bot_x;
			tbot_y = bot_y;
			bot_x += 1;			
			//cout << bot_x << "   	" << bot_y <<  "  ||  " << tbot_x << "   	" << tbot_y<<endl;
			if ( map[bot_x+1][bot_y] == 'K' )
			{
				cout << pthread_self() <<  " (" << bot_x+1 << ',' << bot_y << ") Found!"<< endl ;		
				break;
			}
		} 
		
		else if( map[bot_x][bot_y+1] == ' ' && (bot_y+1 != tbot_y) )
		{
			tbot_y = bot_y;
			tbot_x = bot_x;
			bot_y += 1;
			//cout << bot_x << "   	" << bot_y <<  "  ||  " << tbot_x << "   	" << tbot_y<<endl;
			if ( map[bot_x][bot_y+1] == 'K' )
			{
				cout << pthread_self() <<  " (" << bot_x << ',' << bot_y+1 << ") Found!"<< endl ;
				break;
			}
		}
		
		else if( map[bot_x-1][bot_y] == ' ' && (bot_x-1 != tbot_x) )
		{
			tbot_y = bot_y;
			tbot_x = bot_x;
			bot_x -= 1;
			//cout << bot_x << "   	" << bot_y <<  "  ||  " << tbot_x << "   	" << tbot_y<<endl;
			if ( map[bot_x-1][bot_y] == 'K' )
			{
				cout << pthread_self() <<  " (" << bot_x-1 << ',' << bot_y << ") Found!"<< endl ;
				break;
			}	
		}
		
		else if( map[bot_x][bot_y-1] == ' ' && (bot_y-1 != tbot_y) )
		{
			tbot_y = bot_y;
			tbot_x = bot_x;
			bot_y -= 1;
			//cout << bot_x << "   	" << bot_y <<  "  ||  " << tbot_x << "   	" << tbot_y<<endl;
			if ( map[bot_x][bot_y-1] == 'K' )
			{
				cout << pthread_self() <<  " (" << bot_x << ',' << bot_y-1 << ") Found!"<< endl ;
				break;
			}
		}
		
	}
	for (int i = 0; i < 22; i++)
	{
		delete[] map[i]; //先將每一個元素所指向的一維陣列先刪除
	}
	delete[] map; //在刪除全部的陣列
	

	infile.close();
	gettimeofday(&tv2,NULL);
	cout << "Total: " << (tv2.tv_usec  - tv1.tv_usec)/1000 <<"ms."  << endl;
    
}


