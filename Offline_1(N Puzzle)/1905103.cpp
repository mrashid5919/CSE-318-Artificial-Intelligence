#include<bits/stdc++.h>
using namespace std;

long long n,i,cnt,expanded,explored;
vector<int> stState,dest,dist,parent;
vector<vector<int> > states,steps;
map<vector<int>,int> mp;

int hammingDistance(vector<int> &vect)
{
    int mismatch=0;
    for(int i=0;i<vect.size();i++)
    {
        if(vect[i]==0)
            continue;
        if(vect[i]!=i+1)
            mismatch++;
    }
    return mismatch;
}

int manhattanDistance(vector<int> &vect)
{
    int x=0,curx,cury;
    for(int i=0;i<vect.size();i++)
    {
        if(vect[i]==0)
            continue;
        curx=i/n;
        cury=i%n;
        x+=abs(curx-(vect[i]-1)/n) + abs(cury-(vect[i]-1)%n);
    }
    return x;
}

bool solvable(int blank)
{
    int blankx,blanky,inversionCount=0;
    blankx=blank/n;
    blanky=blank%n;
    for(int i=0;i<n*n;i++)
    {
        if(stState[i]==0)
            continue;
        for(int j=0;j<i;j++)
        {
            if(stState[j]==0)
                continue;
            if(stState[j]>stState[i])
                inversionCount++;
        }
    } 
    if(n%2==1 && inversionCount%2==1)
        return false;
    else if(n%2==0 && inversionCount%2 != (n-1-blankx)%2)
        return false;
    return true;
}

void astar(int h)
{
    explored=0;
    expanded=0;
    cnt=2;
    dist.clear();
    parent.clear();
    mp.clear();
    states.clear();
    for(i=0;i<2;i++)
        dist.push_back(0);
    dist.push_back(-1);
    for(i=0;i<3;i++)
        parent.push_back(-1);
    mp[stState]=1;
    mp[dest]=2;
    for(i=0;i<2;i++)
        states.push_back(stState);
    states.push_back(dest);

    priority_queue<pair<int,int>,vector<pair<int,int> >,greater<pair<int,int> > > pq;
    pq.push({dist[1],mp[stState]});
    while(!pq.empty())
    {
        int x=pq.top().second;

        explored++;
        if(x==2)
        {
            if(h==0)
                cout<<"Using Hamming Distance:\n";
            else
                cout<<"Using Manhattan Distance:\n";
            cout<<"Explored: "<<explored<<"\n";
            cout<<"Expanded: "<<expanded<<"\n";
            return;
        }
        pq.pop();
        int bl;
        for(int i=0;i<states[x].size();i++)
        {
            if(states[x][i]==0)
            {
                bl=i;
                break;
            }
        }
        int blx=bl/n;
        int bly=bl%n;

        for(int i=0;i<4;i++)
        {
            int nblx;
            int nbly;
            if(i==0)
            {
                nblx=blx+1;
                nbly=bly;
            }
            else if(i==1)
            {
                nblx=blx-1;
                nbly=bly;
            }
            else if(i==2)
            {
                nblx=blx;
                nbly=bly+1;
            }
            else
            {
                nblx=blx;
                nbly=bly-1;
            }
            if(nblx<0 || nblx>=n || nbly<0 || nbly>=n)
                continue;
            int nblank=nblx*n+nbly;
            vector<int> nxt=states[x];
            swap(nxt[bl],nxt[nblank]);
            if(mp[nxt]==0)
            {
                cnt++;
                mp[nxt]=cnt;
                states.push_back(nxt);
                dist.push_back(-1);
                parent.push_back(-1);
            }
            int t=mp[nxt];
            if(dist[t]==-1 || dist[x]+1<dist[t])
            {
                dist[t]=dist[x]+1;
                parent[t]=x;
                if(h==0)
                    pq.push({dist[t]+hammingDistance(nxt),t});
                else
                    pq.push({dist[t]+manhattanDistance(nxt),t});
                expanded++;
            }
        }
    }
}

void print(int id)
{
    while(id!=-1)
    {
        steps.push_back(states[id]);
        id=parent[id];
    }
    cout<<"Minimum number of moves = "<<steps.size()-1<<"\n";
    for(int i=steps.size()-1;i>=0;i--)
    {
        for(int j=0;j<steps[i].size();j++)
        {
            cout<<steps[i][j]<<" ";
            if((j+1)%n==0)
                cout<<"\n";
        }
        cout<<"\n";
    }
}

int main()
{
    cin>>n;
    string s;
    long long blankpos=0;
    for(i=0;i<n*n;i++)
    {
        cin>>s;
        if(s[0]=='0' || s[0]=='*')
        {
            blankpos=i;
            stState.push_back(0LL);
        }
        else
        {
            int x=0;
            for(int j=0;j<s.size();j++)
                x=(x*10)+(s[j]-'0');
            stState.push_back(x);
        }
    }

    if(!solvable(blankpos))
    {
        cout<<"Unsolvable puzzle\n";
        return 0;
    }

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(i==n-1 && j==n-1)
                dest.push_back(0LL);
            else
                dest.push_back(i*n+j+1);
        }
    }

    astar(0); //Using Hamming distance

    astar(1); //Using Manhattan distance
    
    print(2); //Index of destination node = 2
}