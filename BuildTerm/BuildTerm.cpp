// BuildTerm.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

map<string,set<string> >content;
map<string, int> freq;

size_t ChineseLength;

void initial()
	{
		string testch="测试";
		ChineseLength=testch.length()/2;
}

struct Word
	{
	private :
		string value;
		int frequence;
	public:
		Word(){}
		void setValue(string src)
			{
			   value =src;
			   if (value.length() == ChineseLength)
			   {
				   map<string ,int>::iterator it=freq.find(value);
				   if (it == freq.end())
				   {
					   frequence=1;
				   }else
					   {
						   frequence=freq[value];
				   }
			   }
			   else
				   {
					   frequence=1;
			   }
		}
		string getValue()
			{
				return value;
		}

		int getLength()
			{
				return value.length();
		}
		int getFrequence()
			{
				return frequence;
		}
};

struct Chunk
	{
	public:
		vector<Word> list;
		Chunk(){}
		void addWord(Word w)
			{
				list.push_back(w);
		}
		int getWordNumber()
			{
				return list.size();
		}
		double getVariance()
			{
				double avglen=0.0;
				int listSize=list.size();
				for (int i=0;i<listSize;i++)
				{
					avglen+=list.at(i).getLength();
				}
				avglen=1.0+avglen/listSize;
				double variance=1.0;
				for (int i=0;i<listSize;i++)
				{
					double temp=(avglen-list.at(i).getLength());
					variance+=temp*temp;
				}
				return variance;
		}
	    long getFreq()
			{
				long freqValue=0;
				int listSize=list.size();
				for (int i=0;i<listSize;i++)
				{
					freqValue+=list.at(i).getFrequence();
				}
				return freqValue;
		}
		vector<string> getVectotString()
			{
				vector<string> res;
				int size=list.size();
				for (int i=0;i<size;i++)
				{
					res.push_back(list.at(i).getValue());
				}
				return res;
		}
};

		void read_terms_from_Lexicon()
		{
			ifstream fin("Lexicon_full_words.txt");
			string dot=".";
			string s;
			string temp;
			std::map<string,set<string> >::iterator lexiconIterator;
			s.clear();
			fin>>s;
			std::cout<<"start read from lexicon_full_words.txt"<<std::endl;
			std::cout<<"reading terms: ->"<<std::endl;
			int num=0;
			while (fin>>s)
			{
				std::set<string> tempSet;
				temp.clear();
				temp=s.substr(0,ChineseLength);
				lexiconIterator=content.find(temp);
				if (lexiconIterator==content.end())
				{
					tempSet.clear();
					tempSet.insert(s);
				}
				else
				{
					tempSet=content[temp];
					tempSet.insert(s);
				}
				content[temp]=tempSet;
				std::cout<<num<<endl;
				num++;
			}
			std::cout<<"finish read the lexicon"<<std::endl;
			std::cout<<"finish read the lexiocn the lexiocn .lexicon size"<<content.size()<<std::endl;
			fin.close();
		}
		void show_lexicon()
			{
				map<string ,set<string> >::iterator lexiconIterator;
				int count=0;
				for (lexiconIterator=content.begin();lexiconIterator!=content.end();lexiconIterator++)
				{
					string first=lexiconIterator->first;
					set<string> second=lexiconIterator->second;
					std::cout<<first<<std::endl;
					set<string>::iterator setIt;
					for (setIt=second.begin();setIt!=second.end();setIt++)
					{
						std::cout<<*setIt<<"  ";
					}
					std::cout<<endl;
					count++;
					if (count==10)
					{
						break;
					}
				}
				std::cout<<"lexicon size:"<<content.size()<<endl;
		}
		void write_index()
			{
				fstream outputFile("lexiucon.index",fstream::out);
                map<string ,set<string> >::iterator lexiconIterator;
				for (lexiconIterator=content.begin();lexiconIterator!=content.end();lexiconIterator++)
				{
					string first=lexiconIterator->first;
					set<string> second=lexiconIterator->second;
					outputFile<<"#:"<<first<<std::endl;
					set<string> ::iterator setIt;
					for (setIt=second.begin();setIt!=second.end();setIt++)
					{
						outputFile<<*setIt<<"  ";
					}
				}
				std::cout<<" write index :lexicon size "<<content.size()<<std::endl;
				outputFile.close();
		}
		void write_freq()
			{
				fstream outputFile("freq.index",fstream::out);
				map<string,int>::iterator freqIterator;
				for (freqIterator=freq.begin();freqIterator!=freq.end();freqIterator++)
				{
					string first=freqIterator->first;
					int second=freqIterator->second;
					outputFile<<first<<"  "<<second;
					outputFile<<endl;
				}
				std::cout<<"write index : freq size"<<freq.size()<<std::endl;
				outputFile.close();
		}
		void build_freq()
			{
				ifstream fin("freq.index");
				string tempStr;
				int tempFreq;
				while (fin>>tempStr)
				{
					fin>>tempFreq;
					if (tempFreq>1||tempStr.length()==ChineseLength)
					{
						freq[tempStr]=tempFreq;
					}
				}
				fin.close();
				std::cout<<"index freq size "<<freq.size()<<std::endl;
		}
		void build_index()
			{
				ifstream fin("lexicon.index");
				if (fin.good()==0)
				{
					std::cout<<"build inex ,need some time ,please wait for a moment! \n";
					read_terms_from_Lexicon();
					write_index();
					fin.open("lexicon.index",ifstream::in) ;
				}
				std::cout<<"hello ,begin load index \n";
				string tempStr;
				set<string> tempSet;
				string key="";
				while(fin>>tempStr)
					{
						if (tempStr.find("#:",0)==0)
						{
							if (key!="")
							{
								content[key]=tempSet;
							}
							tempSet.clear();
							key=tempStr.substr(2);
						}
						else
							{
							    tempSet.insert(tempStr);
						}
				}
				content[key]=tempSet;
                std::cout<<" lexicon size"<<content.size()<<std::endl;
				fin.close();
				build_freq();
		}

		vector<Chunk> chunklist;
		int minChunkWordNumber;
		void mmseg_recursion(string src,Chunk tempChunk)
			{
				set<string> termslist;
				int i=0;
				int len =src.length();
				string singleWordStr;
				Word tempWord;
				singleWordStr=src.substr(0,ChineseLength);
				tempWord.setValue(singleWordStr);
				if ((i+ChineseLength)>=len)
				{
					tempChunk.addWord(tempWord);
					chunklist.push_back(tempChunk);
					if (minChunkWordNumber>tempChunk.getWordNumber())
					{
						minChunkWordNumber=tempChunk.getWordNumber();
					}
					return;
				}
				else
					{
						string tempStr;
						map<string,set<string> >::iterator lexIt;
						lexIt=content.find(singleWordStr);
						if (lexIt==content.end())
						{
							tempChunk.addWord(tempWord);
							string remain=src.substr(ChineseLength);
							mmseg_recursion(remain,tempChunk);
							tempChunk.list.pop_back();
						}
						else
							{
								termslist=content[singleWordStr];
								set<string>::iterator setIt;
								vector<string> termsVector;
								for (setIt=termslist.begin();setIt!=termslist.end();setIt++)
								{
									tempStr=*setIt;
									size_t foundit=src.find(tempStr,0);
									if (foundit==0&&tempStr!=singleWordStr)
									{
										tempWord.setValue(tempStr);
										tempChunk.addWord(tempWord);
										if (tempChunk.getWordNumber()>(minChunkWordNumber))
										{
											tempChunk.list.pop_back();
											return;
										}
										if (tempStr.length()==src.length())
										{
											chunklist.push_back(tempChunk);
											if (minChunkWordNumber>tempChunk.getWordNumber())
											{
												minChunkWordNumber=tempChunk.getWordNumber();
											}
											return;
										}
										string remain=src.substr(tempStr.length());
										mmseg_recursion(remain,tempChunk);
										tempChunk.list.pop_back();
									}
								}
								tempStr=singleWordStr;
								tempWord.setValue(tempStr);
								tempChunk.addWord(tempWord);
								if (tempChunk.getWordNumber()>(minChunkWordNumber))
								{
									tempChunk.list.pop_back();
									return;
								}
								string remain=src.substr(tempStr.length());
								mmseg_recursion(remain,tempChunk);
								tempChunk.list.pop_back();
						}
					}
		}

		vector<string> mmseg(string src)
			{
				vector<string> res;
				chunklist.clear();
				minChunkWordNumber=0X7ffffff0;
				Chunk tempChunk;
				vector<int> indexInChunkList;
				int min=0X7ffffff;
				mmseg_recursion(src,tempChunk);
				int chunklistSize=chunklist.size();
				if (chunklistSize==1)
				{
					return chunklist.at(0).getVectotString();
				}
				else
					{
						for (int i=0;i<chunklistSize;i++)
						{
							if (chunklist.at(i).getWordNumber()<min)
							{
								min=chunklist.at(i).getWordNumber();
								indexInChunkList.clear();
								indexInChunkList.push_back(i);
							}
							else
								if (chunklist.at(i).getWordNumber()==min)
								{
									indexInChunkList.push_back(i);
								}
						}
						if (indexInChunkList.size()==1)
						{
							return chunklist.at(indexInChunkList.at(0)).getVectotString();
						}
						else
							{
								double minVariance=min*src.length()*src.length();
								vector<int> tempIndex=indexInChunkList;
								indexInChunkList.clear();
								for (size_t index=0;index<tempIndex.size();index++)
								{
									int i=tempIndex.at(index);
									if (chunklist.at(i).getVariance()<minVariance)
									{
										minVariance=chunklist.at(i).getVariance();
										indexInChunkList.clear();
										indexInChunkList.push_back(i);
									}else
										if (chunklist.at(i).getVariance()==minVariance)
										{
											indexInChunkList.push_back(i);
										}
								}
								if (indexInChunkList.size()==1)
								{
									return chunklist.at(indexInChunkList.at(0)).getVectotString();
								}
								else
									{
										vector<int> tempIndex=indexInChunkList;
										indexInChunkList.clear();
										long max;
										int tempIndexSize=tempIndex.size();
										for (int index=0;index<tempIndexSize;index++)
										{
											int i=tempIndex.at(index);
											if (chunklist.at(i).getFreq()>max)
											{
												max=chunklist.at(i).getFreq();
												indexInChunkList.clear();
												indexInChunkList.push_back(i);
											}
											else
												{
													indexInChunkList.push_back(i);
											}
										}
										return chunklist.at(indexInChunkList.at(0)).getVectotString();
								}
						}
				}
		}

void showTermsSegment(vector<string> src)
	{
		std::cout<<"segment like this :";
		int size=src.size();
		for (int i=0;i<size;i++)
		{
			std::cout<<src.at(i)<<"";
		}
		std::cout<<endl;
}
int _tmain(int argc, _TCHAR* argv[])
{
	initial();
	//	read_terms_from_Lexicon();
	//	write_index();
	build_index();
	//	show_Lexicon();
	string test = "中华人民共和国在1949年建立";
	test = "从此开始了新中国的伟大篇章";
	test = "研究生命起源";
	test = "北京天安门";
	//	从此开始了新中国的伟大篇章中华人民共和国在一九四九年建立
	test = "主要是因为研究生命起源北京天安门";
	test = "从此开始了新中国的伟大篇章中华人民共和国在一九五五年建立主要是因为研究生命起源北京天安门";

	//	test ="国际化企业中华人民共和国";
	//	size_t found;
	//	found = test.find("开始", 10);
	//	bool flag = (found != string::npos);
	//	cout << test.substr(test.length(), 3) << endl;
	//	cout << test.substr(24, 4) << endl;
	//	test = "，";
	//	cout << test.length();

	//	vector<string> res = mmseg(test);
	//	int min = 0x7fffffff;
	//	cout << min;
	vector<string> seg = mmseg(test);
	//	seg.push_back("从");
	//	sort(seg.begin(), seg.end());
	cout << endl;
	cout << "test string :" << test << endl;
	showTermsSegment(seg);

	test = "主要是因为研究生死";
	seg = mmseg(test);
	cout << endl;
	cout << "test string :" << test << endl;
	showTermsSegment(seg);
	system("pause");
	return 0;
}