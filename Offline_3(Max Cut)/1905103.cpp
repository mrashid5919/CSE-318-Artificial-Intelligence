#include<bits/stdc++.h>
using namespace std;

#define fastio ios::sync_with_stdio(0);cin.tie(0)

long long n,m,pos[100005],mx,mxa,mxb;
vector<pair<long long,pair<long long,long long> > > edges;
vector<pair<long long,long long> > adj[100005];

long long randomMaxCut()
{
    long long totSum=0;
    for(int x=0;x<10;x++)
    {
        for(long long i=0;i<n;i++)
            pos[i]=0;
        for(long long i=0;i<n;i++)
        {
            pos[i]=rand()%2+1;
            //cout<<pos[i]<<"\n";
        }
        for(long long i=0;i<m;i++)
        {
            if(pos[edges[i].first]!=pos[edges[i].second.first])
            {
                totSum+=edges[i].second.second;
                //cout<<i<<" "<<totSum<<"\n";
            }
        }
    }
    //doing 10 iterations and taking the average
    return totSum/10;
}

long long greedyMaxCut()
{
    for(long long i=0;i<n;i++)
        pos[i]=0;
    vector<long long> setA,setB;
    setA.push_back(mxa);
    setB.push_back(mxb);
    pos[mxa]=1;
    pos[mxb]=2;
    long long mxSigma,mxSigmaPos,mxV;
    while(setA.size()+setB.size()!=n)
    {
        mxSigma=LLONG_MIN;
        //Checking adding which node will maximize the sum of edge weights
        for(long long i=0;i<n;i++)
        {
            if(pos[i]==0)
            {
                long long curX=0,curY=0;
                for(long long k=0;k<adj[i].size();k++)
                {
                    if(pos[adj[i][k].first]==1)
                        curX+=adj[i][k].second;
                    else if(pos[adj[i][k].first]==2)
                        curY+=adj[i][k].second;
                }
                if(max(curX,curY)>mxSigma)
                {
                    mxSigma=max(curX,curY);
                    if(curX>=curY)
                        mxSigmaPos=2;
                    else
                        mxSigmaPos=1;
                    mxV=i;
                }
            }
        }
        
        //cout<<mxV<<" ";
        //adding that node to desired set
        if(mxSigmaPos==1)
        {
            setA.push_back(mxV);
            pos[mxV]=1;
        }
        else
        {
            setB.push_back(mxV);
            pos[mxV]=2;
        }
    }

    //adding all the edges which connect the two sets
    long long val=0;
    for(long long i=0;i<m;i++)
    {
        if(pos[edges[i].first]!=pos[edges[i].second.first])
            val+=edges[i].second.second;
    }
    return val;
}

int main()
{
    fastio;
    srand(time(0));
    long long i,j,a,b,w;
    cin>>n>>m;
    for(i=0;i<m;i++)
    {
        cin>>a>>b>>w;
        adj[a-1].push_back({b-1,w});
        adj[b-1].push_back({a-1,w});
        edges.push_back({a-1,{b-1,w}});

        //Identifying the most heavy edge, required for greedy
        if(i==0)
        {
            mx=w;
            mxa=a-1;
            mxb=b-1;
        }
        else if(w>mx)
        {
            mx=w;
            mxa=a-1;
            mxb=b-1;
        }
    }
    cout<<"Randomized: "<<randomMaxCut()<<"\n";
    cout<<"Greedy: "<<greedyMaxCut()<<"\n";
    //for(i=0;i<n;i++)
        //cout<<pos[i]<<" ";
}
