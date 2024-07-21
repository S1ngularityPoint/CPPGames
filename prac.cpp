#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdlib> // for srand() and rand()
#include <ctime>   // for time()

using namespace std;

class Board{
    public:
        int size=10;
        pair<int,int> food;
        vector<vector<int>> region;
        Board(){
            region=vector<vector<int>>(size,vector<int>(size,0));
            spawnFood();
        } 
        
        void spawnFood(){
            food={rand()%size,rand()%size};
        }

        void update(vector<pair<int,int>> snake){
            fill(region.begin(), region.end(), vector<int>(size, 0));
            for(int i=0;i<snake.size();i++){
                region[snake[i].first][snake[i].second]=1;
            }
        }

        void print(){
            for(int i=0;i<size;i++){
                for (int j=0;j<size;j++){
                    if(food.first==i && food.second==j)
                        cout<<"2";
                    else
                        cout<<region[i][j];
                }
                cout<<"\n";
            }
        }
};

class Snake{
public:
    int score=0;
    vector<pair<int,int>> body;
    pair<int,int> head;
    pair<int,int> tail;
    unordered_map<char,pair<int,int>> moves;
    char last_move;
    Board board;

    Snake(){
        body=vector<pair<int,int>>{{0,0}};
        head={0,0};
        tail={0,0};

        moves['U']={-1,0};
        moves['D']={1,0};
        moves['L']={0,-1};
        moves['R']={0,1};
        last_move='R';
    }

    bool isValid(pair<int,int> coord){
        if(coord.first<board.size && coord.second<board.size && coord.first>=0 && coord.second>=0){
            if(board.region[coord.first][coord.second]!=1){
                return true;
            }
        }
        return false;
    }

    bool eat(){
        if(head==board.food){
            pair<int,int> dirn;
            // if(body.size()!=1)
            // dirn={body.rbegin()->first-(--body.rbegin())->first,body.rbegin()->second-(--body.rbegin())->second};
            // else 
            dirn=moves[last_move];
            if(isValid({tail.first+dirn.first,tail.second+dirn.second})){
                body.push_back({tail.first+dirn.first,tail.second+dirn.second});
                tail=body.back();
                board.spawnFood();
                return true;
            }
            else
            {
                if(isValid({tail.first+(-1),tail.second+(0)})){
                    body.push_back({tail.first+(-1),tail.second+(0)});
                    tail=body.back();
                }
                else if(isValid({tail.first+(1),tail.second+(0)})){
                    body.push_back({tail.first+(1),tail.second+(0)});
                    tail=body.back();
                }
                else if(isValid({tail.first+(0),tail.second+(-1)})){
                    body.push_back({tail.first+(0),tail.second+(-1)});
                    tail=body.back();
                }
                else if(isValid({tail.first+(0),tail.second+(1)})){
                    body.push_back({tail.first+(0),tail.second+(1)});
                    tail=body.back();
                }
                else{
                    cerr<<"DEBUG";
                }
            }
            board.spawnFood();
            return true;
        }
        else
        return false;
    }
    bool move(pair<int,int> dirn){
        for(int i=1;i<body.size();i++){
            body[i]=body[i-1];
        }
        body[0]={body[0].first+dirn.first,body[0].second+dirn.second};
        head=body[0];
        tail=body.back();
        if(!isValid(head))
        return false;
        else{
            if(eat()){
                score+=100;
            }
        }
        board.update(body);
        return true;
    }

    bool play(char c){
        if(c=='\0'){
            if(!move(moves[last_move])){
                cout<<"CRASH";
                return false;
            }
        }
        else{
            if(!move(moves[c])){
                cout<<"CRASH";
                return false;
            }
            last_move=c;
        }
        board.print();
        cout<<"\n----------\nscore: "<<score<<"\n----------\n";
        return true;
    }
};

int main() {
    srand(time(0)); 
    Snake S;
    S.play('\0');
    while(true){
        char c;
        cin>>c;
        if(c=='E')
            break;
        if(!S.play(c))
            break;
    }
}
