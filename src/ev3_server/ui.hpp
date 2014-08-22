//      parameter.hpp
//      
//      Copyright 2009 Marius Christ <marius@gmail.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.


//#include <iostream>
#include <string>


namespace tavaron {
	namespace ui {
		
		class parameter
		{
			private:
				std::string *arguments, *pathes;
				std::string target, arguPossible, source;
				int arguCount, arguAmount, arguSet;
				int pathCount;
				bool alreadySet;
				bool * optionsState;
				char * options;

			public:
				parameter(int argc, char** argv, std::string argp)
				:arguAmount(argp.length()), arguCount(0), arguSet(0),
				 pathCount(0), alreadySet(false), arguPossible(argp)
				{
					options = new char[argp.length()];
					optionsState = new bool[argp.length()];
					for(int i=1;i<argc;i++)
						if(argv[i][0]=='-')
							arguCount++;
					if(arguCount>0)
						arguments = new std::string[arguCount];
					else
					{
						arguments = new std::string;
						*arguments = '0';
					}
						
					for(int i=1;i<argc;i++)
						if(argv[i][0]!='-')
							pathCount++;
					pathes = new std::string[pathCount];
					argc--;
					arguments[0]="";
					for(int i=0;i<argp.length();i++)
						optionsState[i]=false;
					for(int i=1,j=0,k=0;i<=argc;i++)
					{
						if((argv[i][0]=='-')&&(i))
						{
							if(j<arguCount)
							{
								arguments[j]=argv[i];
								j++;
							}
						}
						if((argv[i][0]!='-')&&(i))
						{
							if(k<pathCount)
							{	
								pathes[k]=argv[i];
								k++;
							}
						}
					}
					for(int i=0;i<arguCount;i++)
					{
							for(int j=1;j<=arguments[i].length();j++)
							{
								for(int k=0;k<argp.length();k++)
								{
									alreadySet=false;
									if(argp[k]==arguments[i][j])
									{
										*(optionsState+k)=true;
										arguSet++;
										/*for(int l=0;l<argp.length();l++)
											if(*(options+l)==arguments[i][j])
												alreadySet=true;
										if(!alreadySet)
										{
											//for(int l=0;l<argp.length();l++)
											//	if(argp[l]=
											*(optionsState+arguSet)=true;
											arguSet++;
										}*/
									}
								}
							}
					}
				}
				~parameter();
				char getOptions(int pos);
				int getCounterPathes();
				int getCounterArguments();
				int getCounterParameter();
				int getCounterOptions();
				int getCounterPossible();
				bool isTrue(char argu);
				int getCounterSource();
				std::string getSource();
				std::string getSource(int pos);
				std::string getTarget();
		};

		int parameter::getCounterSource()
		{
			return pathCount-1;
		}

		std::string parameter::getSource()
		{
			return *pathes;
		}

		std::string parameter::getSource(int pos)
		{
			std::string val="";
			if(pos<=pathCount)
				val=*(pathes+pos);
			return val;
		}

		std::string parameter::getTarget()
		{
			return *(pathes+pathCount-1);
		}

		parameter::~parameter()
		{
			delete optionsState;
			delete options;
			delete arguments;
			delete pathes;
		}

		char parameter::getOptions(int pos)
		{
			char value = '0';
			if(pos<arguPossible.length())
				value=*(options+pos);
			return value;
		}

		int parameter::getCounterPossible()
		{
			return arguAmount;
		}

		int parameter::getCounterOptions()
		{
			return arguSet;
		}

		int parameter::getCounterArguments()
		{
			return arguCount;
		}

		int parameter::getCounterParameter()
		{
			return arguCount+pathCount;
		}

		int parameter::getCounterPathes()
		{
			return pathCount;
		}
	
		/*
		bool parameter::getState(char argu)
		{
			bool value=false;
			
			return value;
		}
		*/

		bool parameter::isTrue(char argu)
		{
			bool status=false;
			for(int i=0;i<getCounterPossible();i++)
				if(argu==arguPossible[i])
					status=*(optionsState+i);
			return status;
		}

	
	}
}

