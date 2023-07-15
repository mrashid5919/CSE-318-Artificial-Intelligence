#include<bits/stdc++.h>
using namespace std;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int W1=10,W2=15,W3=20,W4=25;

class Mancala{
    public:
    int bins[14];
    int turn;
    bool isOver;
    int addMove;
    int capt;

    Mancala()
    {
        for(int i=0;i<14;i++)
        {
            if(i==6 || i==13)
                bins[i]=0;
            else
                bins[i]=4;
        }
        turn=0;
        isOver=false;
        addMove=0;
        capt=0;
    }

    void choose(int n)
    {
        if(bins[n]==0)
        {
            cout<<"Bin "<<n<<" is empty, invalid input\n";
            return;        
        }
        int tmp=(n+bins[n])%14;
        distribute((1-turn)*7+6,n,bins[n]);

        // int tmp=(n+bins[n])%14;
        if(bins[tmp]==1)
        {
            if(tmp>=turn*7 && tmp<=turn*7+5)
            {
                if(bins[12-tmp]==0)
                    capt=0;
                else
                {
                    capt=bins[tmp]+bins[12-tmp];
                    bins[turn*7+6]+=capt;
                    bins[tmp]=0;
                    bins[12-tmp]=0;
                }
            }
        }

        if(tmp==turn*7+6)
            addMove++;
        else
        {
            turn=1-turn;
            addMove=0;
        }        
    }

    void distribute(int avoid,int n,int stoneCount)
    {
        bins[n]=0;
        int dif=1;
        while(stoneCount!=0)
        {
            if((n+dif)%14!=avoid)
            {
                bins[(n+dif)%14]++;
                stoneCount--;
            }
            dif++;
        }
    }

    bool isrowEmpty()
    {
        int sum1=0,sum2=0;
        bool check=false;
        for(int i=0;i<6;i++)
            sum1+=bins[i];
        for(int i=7;i<13;i++)
            sum2+=bins[i];

        if(!sum1)
        {
            bins[13]+=sum2;
            for(int i=7;i<13;i++)
                bins[i]=0;
            check=true;
        }
        else if(!sum2)
        {
            bins[6]+=sum1;
            for(int i=0;i<6;i++)
                bins[i]=0;
            check=true;
        }
        return check;
    }

    int getStones(int x)
    {
        int sum=0;
        for(int i=x*7;i<x*7+6;i++)
            sum+=bins[i];
        return sum;
    }

    // stones_in_my_storage - stones_in_opponent_storage
    int heuristic1(int turn)
    {
        //return bins[13]-bins[6];
        if(turn==0)
            return bins[6]-bins[13];
        else
            return bins[13]-bins[6];
    }

    // W1 * (stones_in_my_storage – stones_in_opponents_storage) +
    // W2 * (stones_on_my_side – stones_on_opponents_side)
    int heuristic2(int turn)
    {
        if(turn==0)
            return W1*heuristic1(turn)+W2*(getStones(0)-getStones(1));
        else
            return W1*heuristic1(turn)+W2*(getStones(1)-getStones(0));
    }

    // W1*(stones_in_my_storage – stones_in_opponents_storage) +
    // W2*(stones_on_my_side – stones_on_opponents_side) +
    // W3*(additional_move_earned)
    int heuristic3(int turn,int addMoves)
    {
        return heuristic2(turn)+W3*addMoves;
    }

    // W1*(stones_in_my_storage – stones_in_opponents_storage) +
    // W2*(stones_on_my_side – stones_on_opponents_side) +
    // W3*(additional_move_earned) +
    // W4*(stones_captured)
    int heuristic4(int turn,int addMoves,int capt)
    {
        return heuristic3(turn,addMoves)+W4*capt;
    }

    int evalHeuristic(int heuristic,int turn,int addMoves,int capt)
    {
        if(heuristic==1)
            return heuristic1(turn);
        else if(heuristic==2)
            return heuristic2(turn);
        else if(heuristic==3)
            return heuristic3(turn,addMoves);
        else if(heuristic==4)
            return heuristic4(turn,addMoves,capt);
        else
            return heuristic1(turn);
    }

    vector<int> backupBins(vector<int> vect)
    {
        vector<int> backup;
        for(int i=0;i<14;i++)
            backup.push_back(vect[i]);
        return backup;
    }

    pair<int,int> minimaxAlgo(int depth,int turn,int alpha,int beta,int addMoves,int capt,int heuristic)
    {
        if(depth==0)
            return {evalHeuristic(heuristic,turn,addMoves,capt),1};
        if(isOver)
            return {evalHeuristic(heuristic,turn,addMoves,capt),1};
        
        int maxEva;
        int minEva;
        int eva;
        int idx=-1;

        if(turn)
        {
            maxEva=(-1)*100000;
            vector<int> tmp;
            for(int i=7;i<13;i++)
                tmp.push_back(i);
            shuffle(tmp.begin(),tmp.end(),rng);
            for(int i=0;i<6;i++)
            {
                if(bins[tmp[i]]==0)
                    continue;
                int b[14];
                for(int j=0;j<14;j++)
                    b[j]=bins[j];
                int turnBackup=this->turn;
                bool isOverBackup=this->isOver;
                int addMoveBackup=this->addMove;
                int captBackup=this->capt;
                choose(tmp[i]);
                eva=minimaxAlgo(depth-1,this->turn,alpha,beta,this->addMove,this->capt,heuristic).first;
                if(eva>=maxEva)
                {
                    maxEva=eva;
                    idx=i;
                }
                for(int j=0;j<14;j++)
                    bins[j]=b[j];
                this->turn=turnBackup;
                this->isOver=isOverBackup;
                this->addMove=addMoveBackup;
                this->capt=captBackup;
                alpha=max(alpha,maxEva);
                if(beta<=alpha)
                    break;
            }
            return make_pair(maxEva,idx);
        }
        else
        {
            minEva=100000;
            vector<int> tmp;
            for(int i=0;i<6;i++)
                tmp.push_back(i);
            shuffle(tmp.begin(),tmp.end(),rng);
            for(int i=0;i<6;i++)
            {
                if(bins[tmp[i]]==0)
                    continue;
                int b[14];
                for(int j=0;j<14;j++)
                    b[j]=bins[j];
                int turnBackup=this->turn;
                bool isOverBackup=this->isOver;
                int addMoveBackup=this->addMove;
                int captBackup=this->capt;
                choose(tmp[i]);
                eva=minimaxAlgo(depth-1,this->turn,alpha,beta,this->addMove,this->capt,heuristic).first;
                if(eva<=minEva)
                {
                    minEva=eva;
                    idx=i;
                }
                for(int j=0;j<14;j++)
                    bins[j]=b[j];
                this->turn=turnBackup;
                this->isOver=isOverBackup;
                this->addMove=addMoveBackup;
                this->capt=captBackup;
                beta=min(beta,minEva);
                if(beta<=alpha)
                    break;
            }
            return make_pair(minEva,idx);
        }
    }

    void print()
    {
        cout<<"\t1\t2\t3\t4\t5\t6\t\n";
        cout<<"---\t---\t---\t---\t---\t---\t---\t\n";
        cout<<"\t";
        for(int i=12;i>=7;i--)
            cout<<bins[i]<<"\t";
        cout<<"\t-->P2\n";
        cout<<bins[13]<<"\t\t\t\t\t\t\t"<<bins[6]<<"\n";
        cout<<"\t";
        for(int i=0;i<6;i++)
            cout<<bins[i]<<"\t";
        cout<<"\t-->P1\n";
        cout<<"---\t---\t---\t---\t---\t---\t---\t\n";
        cout<<"\t6\t5\t4\t3\t2\t1\t\n";
        cout<<"\n";
    }

    int getWinner()
    {
        cout<<"Player 1: "<<bins[6]<<"\n";
        cout<<"Player 2: "<<bins[13]<<"\n";
        if(bins[6]>bins[13])
        {
            cout<<"Player 1 is winner\n";
            return 1;
        }
        else if(bins[6]<bins[13])
        {
            cout<<"Player 2 is winner\n";
            return 2;
        }
        else
        {
            cout<<"Match Draw\n";
            return 0;
        }
    }

    int getWinnerInt()
    {
        if(bins[6]>bins[13])
            return 1;
        else if(bins[6]<bins[13])
            return 2;
        else 
            return 0;
    }
};

int game(bool csv,int choice,int heuristic1,int heuristic2,int depth1,int depth2)
{
    Mancala *mancala=new Mancala();
    if(!csv)
    {
        mancala->print();
    }

    int userInput;
    while(!mancala->isOver)
    {
        if(!csv)
            cout<<"Turn: "<<mancala->turn+1<<"\n";

        if(mancala->turn==0)
        {
            if(choice==1)
            {
                int idx=mancala->minimaxAlgo(depth1,mancala->turn,-100000,100000,0,0,heuristic1).second;
                if(!csv)
                    cout<<"Bin: "<<6-idx<<"\n";
                mancala->choose(idx);

                if(!csv)
                    mancala->print();
                mancala->isOver=mancala->isrowEmpty();
            }
            else if(choice==2)
            {
                cin>>userInput;
                cout<<"Bin: "<<userInput<<"\n";
                mancala->choose(6-userInput);
                mancala->print();
                mancala->isOver=mancala->isrowEmpty();
            }
        }
        else
        {
            int idx=mancala->minimaxAlgo(depth2,mancala->turn,-100000,100000,0,0,heuristic2).second;
            if(!csv)
                cout<<"Bin: "<<13-idx<<"\n";
            mancala->choose(13-idx);
            if(!csv)
                mancala->print();
            mancala->isOver=mancala->isrowEmpty();
        }
    }

    int winner;
    if(!csv)
    {
        winner=mancala->getWinner();
        mancala->print();
        cout<<"Game Over\n";
    }
    else
        winner=mancala->getWinnerInt();
    return winner;
}

int main()
{
    int choice;
    cout<<"Enter your choice: 1/2\n";
    cout<<"1. AI vs AI\n";
    cout<<"2. Human vs AI\n";
    cin>>choice;
    while(choice<=0 || choice>=3)
    {
        cout<<"Invalid input. Please enter 1/2\n";
        cin>>choice;
    }
    if(choice==1)
        cout<<"AI vs AI\n";
    else if(choice==2)
        cout<<"Human vs AI\n";

    int csv=0;
    if(csv)
    {
        cout<<"Depth,P1_Heuristic,P2_Heuristic,P1_Win,P2_Win, Draw\n";
        for(int m=1;m<5;m++)
        {
            for(int i=1;i<5;i++)
            {
                for(int j=1;j<5;j++)
                {
                    int cnt1=0,cnt2=0,draw=0;
                    for(int k=0;k<100;k++)
                    {
                        int result=game(csv,choice,i,j,m,m);
                        if(result==0)
                            draw++;
                        else if(result==1)
                            cnt1++;
                        else
                            cnt2++;
                    }
                    cout<<m<<","<<i<<","<<j<<","<<cnt1<<","<<cnt2<<","<<draw<<"\n";
                }
            }
            cout<<"\n";
        }
    }
    else
    {
        int heuristic1=1;
        int heuristic2=3;
        int depth1=5;
        int depth2=6;
        game(csv,choice,heuristic1,heuristic2,depth1,depth2);
    }
    return 0;
}