#include<bits/stdc++.h>
#include<stdio.h>
using namespace std;

#define fastio ios::sync_with_stdio(0);cin.tie(0)

long long n,m,pos[100005],mx,mxa,mxb,mn,mna,mnb;
vector<pair<long long,pair<long long,long long> > > edges;
vector<pair<long long,long long> > adj[100005];
vector<long long> setP,setQ;

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
    setP.clear();
    setQ.clear();
    for(long long i=0;i<setX.size();i++)
        setP.push_back(setX[i]);
    for(long long i=0;i<setY.size();i++)
        setQ.push_back(setY[i]);
    for(long long i=0;i<m;i++)
    {
        if(pos[edges[i].first]!=pos[edges[i].second.first])
            val+=edges[i].second.second;
    }
    return val;
}

long long LocalSearch()
{
    bool change=true;
    long long iterations=0;
    while(change)
    {
        iterations++;
        change=false;
        
        for(long long i=0;i<n;i++)
        {
            long long sigmaS=0,sigmaSbar=0;
            for(long long j=0;j<m;j++)
            {
                if(edges[j].first==i)
                {
                    if(pos[edges[j].second.first]==2)
                        sigmaS+=edges[j].second.second;
                    else
                        sigmaSbar+=edges[j].second.second;
                }
                else if(edges[j].second.first==i)
                {
                    if(pos[edges[j].first]==2)
                        sigmaS+=edges[j].second.second;
                    else
                        sigmaSbar+=edges[j].second.second;
                }
            }
            if(pos[i]==1 && sigmaSbar-sigmaS>0)
            {
                vector<long long>::iterator it;
                it=find(setP.begin(),setP.end(),i);
                if(it!=setP.end())
                    setP.erase(it);
                setQ.push_back(i);
                pos[i]=2;
                change=true;
                //iterations++;
                break;
            }
            else if(pos[i]==2 && sigmaS-sigmaSbar>0)
            {
                vector<long long>::iterator it;
                it=find(setQ.begin(),setQ.end(),i);
                if(it!=setQ.end())
                    setQ.erase(it);
                setP.push_back(i);
                pos[i]=1;
                change=true;
                //iterations++;
                break;
            }
            //if(change)
                //break;
        }
    }
    /*long long val=0;
    for(long long i=0;i<m;i++)
    {
        if(pos[edges[i].first]!=pos[edges[i].second.first])
            val+=edges[i].second.second;
    }*/
    return iterations;
}

long long grasp(long long maxIterations)
{
    long long mx,localSearchIterationSum=0,localSearchVal=0;
    for(long long i=0;i<maxIterations;i++)
    {
        long long s=semiGreedyMaxCut();
        //cout<<s<<"\n";
        long long iter=LocalSearch();
        localSearchIterationSum+=iter;
        long long val=0;
        for(long long j=0;j<m;j++)
        {
            if(pos[edges[j].first]!=pos[edges[j].second.first])
                val+=edges[j].second.second;
        }
        localSearchVal+=val;
        //cout<<"Set P: "<<setP.size()<<endl;
        /*for(long long j=0;j<setP.size();j++)
            cout<<setP[j]<<" ";
        cout<<endl;*/
        //cout<<"Set Q: "<<setQ.size()<<endl;
        /*for(long long j=0;j<setQ.size();j++)
            cout<<setQ[j]<<" ";
        cout<<endl;*/
        //cout<<val<<endl;
        if(i==0)
        {
            mx=val;
            //ansSetA=setP;
            //ansSetB=setQ;
        }
        else if(val>mx)
        {
            mx=val;
            //ansSetA=setP;
            //ansSetB=setQ;
        }
    }
    //cout<<"Local search max iteration: "<<localSearchIterationSum<<endl;
    cout<<"Local Search average value: "<<localSearchVal/maxIterations<<endl;
    cout<<"Local Search average no. of iterations: "<<localSearchIterationSum/maxIterations<<endl;
    return mx;
}

int main()
{
    fastio;
    //freopen("g27.rud","r",stdin);
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
    cout<<"Randomized: "<<randomMaxCut()<<endl;
    cout<<"Greedy: "<<greedyMaxCut()<<endl;
    long long semgreedysum=0;
    for(int i=0;i<10;i++)
    {
        semgreedysum+=semiGreedyMaxCut();
    }
    cout<<"Semigreedy: "<<semgreedysum/10<<endl;
    //cout<<"Iterations in Local Search: "<<LocalSearch()<<"\n";
    //for(i=0;i<n;i++)
        //cout<<pos[i]<<" ";
    cout<<"Best value from GRASP: "<<grasp(100)<<endl;
}

/*5 7
1 2 5
2 3 9
1 5 6
2 5 10
3 5 2
3 4 15
5 4 10*/
