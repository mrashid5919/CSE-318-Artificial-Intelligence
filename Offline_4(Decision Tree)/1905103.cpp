#include<bits/stdc++.h>
using namespace std;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct Data
{
    map<string,string> attributes;
    string label;
};

struct Node
{
    string attribute; //splitting attribute
    map<string,Node*> children;
    string classification;
};

double calculateEntropy(vector<Data>& examples)
{
    map<string,long long> labels; //storing which label has how many instances
    for(long long i=0;i<examples.size();i++)
    {
        labels[examples[i].label]++;
    }

    double entropy=0.0;
    map<string,long long>::iterator it;
    for(it=labels.begin();it!=labels.end();it++)
    {
        double p=(double)it->second/examples.size();
        entropy+=p*log2(1/p);
    }
    return entropy;
}

set<string> getUniqueAttributeValues(vector<Data>& examples,string& attribute)
{
    set<string> uniqueVals;
    for(long long i=0;i<examples.size();i++)
    {
        uniqueVals.insert(examples[i].attributes[attribute]);
    }
    return uniqueVals;
}

double calculateInformationGain(vector<Data>& examples,string& attribute)
{
    double curEntropy=calculateEntropy(examples);
    set<string> uniqueVals=getUniqueAttributeValues(examples,attribute);

    double weightedEntropy=0.0;
    set<string>::iterator it;
    for(it=uniqueVals.begin();it!=uniqueVals.end();it++)
    {
        vector<Data> subset;
        for(long long j=0;j<examples.size();j++)
        {
            if(examples[j].attributes[attribute]==*(it))
                subset.push_back(examples[j]);
        }
        double subsetEntropy=calculateEntropy(subset);
        weightedEntropy+=((double)subset.size()/examples.size())*subsetEntropy;
    }

    double infoGain=curEntropy-weightedEntropy;
    return infoGain;
}

string findBestAttribute(vector<Data>& examples,vector<string>& attributes)
{
    string bestAttribute;
    double mx=-1.0;

    for(long long i=0;i<attributes.size();i++)
    {
        double infoGain=calculateInformationGain(examples,attributes[i]);
        if(infoGain>=mx)
        {
            mx=infoGain;
            bestAttribute=attributes[i];
        }
    }
    
    return bestAttribute;
}

string pluralityValue(vector<Data>& parentExamples) {
    map<string,long long> labels;

    for(long long i=0;i<parentExamples.size();i++)
    {
        labels[parentExamples[i].label]++;
    }

    string majorityLabel;
    long long mx=0;

    map<string,long long>::iterator it;

    for(it=labels.begin();it!=labels.end();it++)
    {
        if(it->second>=mx)
        {
            majorityLabel=it->first;
            mx=it->second;
        }
    }

    return majorityLabel;
}

Node* buildDecisionTree(vector<Data>& examples,vector<string>& attributes,vector<Data>& parentExamples)
{
    //cout<<"here"<<endl;
    Node* node=new Node;

    if(examples.empty())
    {
        node->classification=pluralityValue(parentExamples);
    }
    else
    {
        int flag=0;
        string firstLabel;
        for(long long i=0;i<examples.size();i++)
        {
            if(i==0)
                firstLabel=examples[i].label;
            else if(examples[i].label!=firstLabel)
            {
                flag=1;
                break;
            }
        }

        if(flag==0) //all examples have the same classification
            node->classification=firstLabel;
        else if(attributes.empty())
            node->classification=pluralityValue(examples);
        else
        {
            string bestAttribute=findBestAttribute(examples,attributes);
            //cout<<bestAttribute<<endl;
            node->attribute=bestAttribute;
            set<string> uniqueAttributeVals=getUniqueAttributeValues(examples,bestAttribute);

            set<string>::iterator it;
            for(it=uniqueAttributeVals.begin();it!=uniqueAttributeVals.end();it++)
            {
                vector<Data> exs; //subset of examples that goes through this attribute value
                for(int i=0;i<examples.size();i++)
                {
                    if(examples[i].attributes[bestAttribute]==*(it))
                        exs.push_back(examples[i]);
                }
                vector<string> remAttributes=attributes;
                remAttributes.erase(remove(remAttributes.begin(), remAttributes.end(), bestAttribute), remAttributes.end());
                Node* subtree=buildDecisionTree(exs,remAttributes,examples);
                node->children[*(it)]=subtree;
            }        

        }
    }
    return node;
}

string makePrediction(Node* root, Data& data)
{
    if(!root->children.empty())
    {
        string attributeValue = data.attributes[root->attribute];
        if (root->children.find(attributeValue)!=root->children.end()) {
            return makePrediction(root->children.at(attributeValue),data);
        }
    }
    return root->classification;
}

void deleteTree(Node* node)
{
    if(node==nullptr)
        return;
    for (auto& pair : node->children) {
        deleteTree(pair.second);
    }

    delete(node);
}

int main()
{
    freopen("car.data","r",stdin);
    string atr[6]={"buying","maint","doors","persons","lug_boot","safety"};
    double accuracySum=0.0;
    vector<string> allAttributes;
    for(int i=0;i<6;i++)
        allAttributes.push_back(atr[i]);
    vector<Data> allExamples;
    string s;
    while(cin>>s)
    {
        //cout<<"Hi"<<endl;
        vector<string> vals;
        string tmp;
        for(long long i=0;i<s.size();i++)
        {
            if(s[i]==',')
            {
                vals.push_back(tmp);
                tmp.clear();
            }
            else
                tmp+=s[i];
        }
        vals.push_back(tmp);

        Data dt;

        for(long long i=0;i<allAttributes.size();i++)
        {
            dt.attributes[allAttributes[i]]=vals[i];
        }
        dt.label=vals.back();
        allExamples.push_back(dt);
    }
    double individualAccuracy[20];
    //cout<<allExamples.size()<<endl;
    for(int loop=0;loop<20;loop++)
    {
        shuffle(allExamples.begin(),allExamples.end(),rng);
        vector<Data> trainingData,testData;
        long long x=(8*allExamples.size())/10;
        for(long long i=0;i<allExamples.size();i++)
        {
            if(i<x)
                trainingData.push_back(allExamples[i]);
            else
                testData.push_back(allExamples[i]);
        }

        vector<Data> parentExamples;
        Node* root=buildDecisionTree(trainingData,allAttributes,parentExamples);

        long long cnt=0;

        for(long long i=0;i<testData.size();i++)
        {
            string prediction=makePrediction(root,testData[i]);
            if(prediction==testData[i].label)
                cnt++;
        }

        //cout<<cnt<<endl;

        
        deleteTree(root);

        cout<<"Accuracy: "<<((double)cnt/testData.size())*100<<"%"<<endl;
        individualAccuracy[loop]=((double)cnt/testData.size())*100;
        accuracySum+=((double)cnt/testData.size())*100;
    }
    cout<<"Mean accuracy: "<<accuracySum/20<<endl;
    double difsum=0.0;
    for(long long loop=0;loop<20;loop++)
    {
        difsum+=(individualAccuracy[loop]-accuracySum/20)*(individualAccuracy[loop]-accuracySum/20);
    }
    double stdDeviation=sqrt(difsum/20);
    cout<<"Standard Deviation: "<<0.01*stdDeviation<<endl;
    return 0;
}