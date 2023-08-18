#include<bits/stdc++.h>
using namespace std;

#define fastio ios::sync_with_stdio(0);cin.tie(0)

long long n,m,pos[100005],mx,mxa,mxb,mn,mna,mnb;
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

long long semiGreedyMaxCut()
{
    for(long long i=0;i<n;i++)
        pos[i]=0;
    vector<long long> setX,setY;
    long long alphaInt=rand()%101;
    float alpha=alphaInt*0.01;
    float miu=mn+alpha*(mx-mn);
    vector<pair<long long,long long> > RCLe;
    for(long long i=0;i<m;i++)
    {
        if((float)edges[i].second.second>=miu)
            RCLe.push_back({edges[i].first,edges[i].second.first});
    }
    //cout<<"here"<<"\n";
    long long idx=rand()%RCLe.size();
    setX.push_back(RCLe[idx].first);
    setY.push_back(RCLe[idx].second);
    pos[RCLe[idx].first]=1;
    pos[RCLe[idx].second]=2;
    long long mxSigma,mnSigma;
    while(setX.size()+setY.size()!=n)
    {
        mxSigma=LLONG_MIN,mnSigma=LLONG_MAX;
        vector<pair<long long,pair<long long,long long> > > mxSigmaInfo;
        for(long long i=0;i<n;i++)
        {
            if(pos[i]==0)
            {
                long long curX=0,curY=0;
                for(long long k=0;k<adj[i].size();k++)
                {
                    if(pos[adj[i][k].first]==1)
                        curY+=adj[i][k].second;
                    else if(pos[adj[i][k].first]==2)
                        curX+=adj[i][k].second;
                }
                if(curY>=curX)
                    mxSigmaInfo.push_back({curY,{i,2}}); //2 indicates wanting to assign it to setY
                else
                    mxSigmaInfo.push_back({curX,{i,1}}); //1 indicates wanting to assign it to setX
                if(max(curX,curY)>mxSigma)
                {
                    mxSigma=max(curX,curY);
                    /*if(curX>=curY)
                        mxSigmaPos=2;
                    else
                        mxSigmaPos=1;
                    mxV=i;*/
                }
                if(min(curX,curY)<mnSigma)
                    mnSigma=min(curX,curY);
            }
        }
        
        miu=mnSigma+alpha*(mxSigma-mnSigma);
        vector<pair<long long,long long> > RCLv;
        for(long long i=0;i<mxSigmaInfo.size();i++)
        {
            if((float)mxSigmaInfo[i].first>=miu)
                RCLv.push_back(mxSigmaInfo[i].second); //storing the vertex number and the set we desire to insert it into
        }
        long long idx=rand()%RCLv.size();
        if(RCLv[idx].second==1)
        {
            setX.push_back(RCLv[idx].first);
        }
        else
            setY.push_back(RCLv[idx].first);
        pos[RCLv[idx].first]=RCLv[idx].second;
    }
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

        //Identifying the most heavy edge and most light edge, required for greedy, semigreedy
        if(i==0)
        {
            mx=w;
            mxa=a-1;
            mxb=b-1;
            mn=w;
            mna=a-1;
            mnb=b-1;
        }
        else if(w>mx)
        {
            mx=w;
            mxa=a-1;
            mxb=b-1;
        }
        else if(w<mn)
        {
            mn=w;
            mna=a-1;
            mnb=b-1;
        }
    }
    //cout<<"here\n";
    cout<<"Randomized: "<<randomMaxCut()<<"\n";
    cout<<"Greedy: "<<greedyMaxCut()<<"\n";
    cout<<"Semigreedy: "<<semiGreedyMaxCut()<<"\n";
    //for(i=0;i<n;i++)
        //cout<<pos[i]<<" ";
}
