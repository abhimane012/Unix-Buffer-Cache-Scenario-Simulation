/*
	Author : Abhishek V. Mane.
	Roll : - A69
	PRN - 1920010081
	
	Program - Buffer Cache Simulation.
*/

#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

class buff{
   public:
	int blkno;
	char status;

  //Ctors
	buff() { status = 'N';}
	buff(int b){ blkno = b; status = 'F';}
	buff(int b, char s){ blkno = b; status = s;}

	void setStatus(char s){ status = s;}
	char getStatus(){return status;}
};



void displayHashQueue(vector<vector<buff>> &hq){
	for(int i = 0; i < hq.size(); i++){
		cout << "[ HASH QUEUE {" << i << "} ]";
		for(int j = 0; j < hq[i].size(); j++){
			 cout << " <-> " << hq[i][j].blkno << " [" << hq[i][j].status << "] "; 	
		}
		cout << endl;
	}
	cout << endl;
}// end displayHashQueue

void displayFreeList(vector<buff> &fl){
	cout << "Free List ";
	
	if(fl.size() == 0){
		cout << "EMPTY";
	}else{
		for(int i = 0; i < fl.size(); i++)
			 cout << " <-> " << fl[i].blkno << " [" << fl[i].status << "] "; 
	}
	cout << endl << endl;
}// end displayFreeList

void fillHashQandFreeList(vector<vector<buff>> &hq, vector<buff> &freeList){
	int arr[] = {28,4,64,17,5,97,98,50,10,3,35,99};
	int fl[] = {3,5,4,28,97,10};

	for(int i = 0; i < 6; i++){
		freeList.push_back(buff(fl[i]));
	}

	for(int i = 0; i < 12; i++){
		int qno = arr[i] % hq.size();
		hq[qno].push_back(buff(arr[i]));
	}
}// end fillHashQandFreeList

int isBuffonHSQ(vector<vector<buff>> &hq, int bfn){
	int hqn = bfn % hq.size();

	
	for(int i = 0; i < hq[hqn].size(); i++)
		if( hq[hqn][i].blkno == bfn )
			return i;

	return -1;
}// end isBuffonHSQ

int isBuffonFreeList(vector<buff> &fl, int bfn){
	for(int i = 0; i < fl.size(); i++)
		if(fl[i].blkno == bfn)
			return i;
	
	return -1;
}// end isBuffonFreeList

bool isStatusFree(vector<vector<buff>> &hq, int bfn){
	int hqn = bfn % hq.size();

	for(int i = 0; i < hq[hqn].size(); i++)
		if( hq[hqn][i].status == 'F' )
			return true;

	return false;
}// end isStatusFree

//insert on hashqueue
void insertBlock(vector<vector<buff>> &hq, vector<buff> &fl, int blk){
	int hqn = blk % hq.size();
	
	buff buffer =  buff(blk);
	buffer.setStatus('B');
	hq[hqn].push_back(buffer);
	
	//fl.push_back(buff(blk));
	
}// end insertBlock

void changeBufferStatus(vector<vector<buff>> &hq, vector<buff> &fl, int buff, char status){
	if (isBuffonHSQ(hq, buff) == -1){
		cout << "Buffer not present of Hash Queue";
	}else{
		int hqn = buff % hq.size();
		int idx = isBuffonHSQ(hq, buff);

		hq[hqn][idx].setStatus(status);

		for(int i = 0; i < fl.size(); i++){
			if(fl[i].blkno == buff){
				fl[i].setStatus(status);
				break;
			}
		}
		//cout << "Status Changed Successfully" << endl;
	}
}// end changeBufferStatus

void removeFromHSQ(vector<vector<buff>> &hq, int buff){
		int hqn = buff % hq.size();
		int idx = isBuffonHSQ(hq, buff);
		hq[hqn].erase(hq[hqn].begin() + idx);
}// end removeFromHSQ

void removeFromFreeList(vector<buff> &fl, int buff){
		int idx = isBuffonFreeList(fl, buff);
		fl.erase(fl.begin() + idx);
}// end removeFromFreeList

void brelease(vector<vector<buff>> &hq, vector<buff> &fl, int buff, int &wait){
	if (isBuffonHSQ(hq, buff) == -1){
		cout << "Buffer not present of Hash Queue";
	}else{
		int hqn = buff % hq.size();
		int idx = isBuffonHSQ(hq, buff);
		hq[hqn][idx].setStatus('F');
		fl.push_back(hq[hqn][idx]);

		hq[hqn].erase(hq[hqn].begin() + idx);

		if(isBuffonHSQ(hq, wait) == -1 && fl.size() != 0){
			cout << "\t Process waiting for block No. " << wait << " is waken up" << endl;
			cout << "\t# Scenario 2 : Block not on Hash Queue & FreeList is not empty" << endl;
					insertBlock(hq, fl, wait);
					int LRU = -1;
					//remove LRU block from both FreeList & Hash Queue
					for(int i = 0; i < fl.size(); i++){
						if(fl.at(i).getStatus() != 'D'){
							LRU = fl.at(i).blkno;
							break;
						}
					}

					if(LRU != -1){
						removeFromFreeList(fl, LRU);
						removeFromHSQ(hq, LRU);
					}

			//reset wait process tracker
				wait = -1;
		
		}//end if

	}//end if-else
	
}//end brelease

void updateStatus(vector<vector<buff>> &hq, vector<buff> &fl){
	for(int i = 0; i < hq.size(); i++){
		for(int j = 0; j < hq[i].size(); j++){
			if ( isBuffonFreeList(fl, hq[i][j].blkno) == -1 )
				hq[i][j].setStatus('B');
		}
	}
}//end updateStatus


int main(){

	int n;
	cout << "Enter Number of HashQues (1 - 10) ";
	cin >> n;

	//to track waiting process
	int waitP = -1;

	vector<vector<buff>> hq(n);	
	vector<buff> freeList;	

	fillHashQandFreeList(hq, freeList);
	updateStatus(hq, freeList);

	while(1){
		
		cout << endl << "\t\t\t--------------------------------------------"<< endl;
		cout << "\t\t\t1. Display Buffer Cache." << endl;
		cout << "\t\t\t2. Insert Block in Cache." << endl;
		cout << "\t\t\t3. Change Buffer Status." << endl;
		cout << "\t\t\t4. Allocate Buffer(getblk)" << endl;
		cout << "\t\t\t5. Release Buffer (brelese)" << endl;
		cout << "\t\t\t6. Exit" << endl;
		cout << "\t\t\t--------------------------------------------"<< endl;

		int ch;
		cout << "Enter Your Choice ";
		cin >> ch;
		cout << endl;
		

		switch(ch){
			case 1:{
					//updateStatus(hq, freeList);
					displayHashQueue(hq);
					displayFreeList(freeList);

					cout << "press X to Continue "; 
					char p;
					cin >> p;
					break;
			}//case 1

			case 2:{
				int blk;
				cout << "Enter Block No. to insert on Cache. ";
				cin >> blk;
				insertBlock(hq, freeList, blk);
				cout << "Block No. " << blk << " inserted successfully." << endl; 

				cout << "press X to Continue "; 
				char p;
				cin >> p;
				break;
			}//case 2

			case 3:{
				cout << "F - Free \t B - Busy \t D - Delayed Write" << endl;
				int blk;
				char st;
				cout << "Enter Block No. to change its status. ";
				cin >> blk;
				cout << "Enter its Status. ";
				cin >> st;

				changeBufferStatus(hq, freeList, blk, st);
				cout << "press X to Continue "; 
				char p;
				cin >> p;
				break;
			}//case 3

			case 4: {
				int blkno;
				cout << "Enter Block no to allocate : ";
				cin >> blkno;

				int hqn = blkno % hq.size();
				if( isBuffonHSQ(hq, blkno) >= 0 && hq[hqn][isBuffonHSQ(hq, blkno)].status == 'B'){
					cout << "\t# Scenario 5 : Block on Hash Queue & But its Status is busy" << endl;
					cout << "\t Waiting for any buffer to become free" << endl;
					waitP = blkno;
				}else if( isBuffonHSQ(hq, blkno) >= 0 && isBuffonFreeList(freeList, blkno) >= 0 && isStatusFree(hq, blkno)){
					//Scenario 1
					cout << "\t# Scenario 1 : Block on Hash Queue & its Status is free" << endl;
					
					//change status to busy
					changeBufferStatus(hq, freeList, blkno, 'B');
					//remove from free list
					removeFromFreeList(freeList, blkno);

				}else if(isBuffonHSQ(hq, blkno) == -1 && freeList.size() > 0 && freeList.at(0).getStatus() != 'D'){
					cout << "\t# Scenario 2 : Block not on Hash Queue & FreeList is not empty" << endl;
					//insert block on HSQ
					insertBlock(hq, freeList, blkno);
					
					int LRU = -1;
					//remove LRU block from both FreeList & Hash Queue
					for(int i = 0; i < freeList.size(); i++){
						if(freeList.at(i).getStatus() != 'D'){
							LRU = freeList.at(i).blkno;
							break;
						}
					}

					if(LRU != -1){
						removeFromFreeList(freeList, LRU);
						removeFromHSQ(hq, LRU);
					}
				}else if(isBuffonHSQ(hq, blkno) == -1 && freeList.size() > 0 && freeList.at(0).getStatus() == 'D'){
					cout << "\t# Scenario 3 : Block not on Hash Queue & FreeList Buffer marked for Delayed Write" << endl;
					cout <<"\t Block No. " <<  freeList.at(0).blkno << " Marked for Async Write" << endl;

					//change block staus to W 
					changeBufferStatus(hq, freeList, freeList[0].blkno, 'W');
					removeFromFreeList(freeList, freeList[0].blkno);

					// Again using Scenario 2 Allocate Buffer

					cout << "\t# Scenario 2 : Block not on Hash Queue & FreeList is not empty" << endl;
					insertBlock(hq, freeList, blkno);
					int LRU = -1;
					//remove LRU block from both FreeList & Hash Queue
					for(int i = 0; i < freeList.size(); i++){
						if(freeList.at(i).getStatus() != 'D'){
							LRU = freeList.at(i).blkno;
							break;
						}
					}

					if(LRU != -1){
						removeFromFreeList(freeList, LRU);
						removeFromHSQ(hq, LRU);
					}

				}else if(isBuffonHSQ(hq, blkno) == -1 && freeList.size() == 0){
					cout << "\tScenario 4 : Block not on Hash Queue & FreeList is empty" << endl;
					cout << "\t Waiting for any buffer to become free" << endl;
					waitP = blkno;
				}

				cout << "press X to Continue "; 
				char p;
				cin >> p;
				break;
			}//case 4

			case 5:{
				int blk;
				cout << "Enter Block No. to Release. ";
				cin >> blk;

				if(blk == waitP){
					cout << "\t Process waiting for block No. " << blk << " is waken up" << endl;
					cout << "\t# Scenario 1 : Block on Hash Queue & its Status is free" << endl;
					waitP = -1;
				}else{
					cout << "\t Process waiting for block No. " << waitP << " is waken up" << endl;
					cout << "\t# Scenario 5 : Block on Hash Queue & But its Status is busy" << endl;
					brelease(hq, freeList, blk, waitP);
				}

				cout << "press X to Continue "; 
				char p;
				cin >> p;
				break;
			}//case 5
			
			case 6:{
				exit(1);
				break;
			}//case 6

			default :
				cout << "Enter Correct Chocie" << endl;
		}

	}//End While

	return 0;
}
