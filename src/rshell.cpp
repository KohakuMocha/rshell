//MACRO
#define popfront() for(int z = 0; z < static_cast<int>(set.size()) - 1; ++z) \
				   {	\
				        set.at(z) = set.at(z+1); \
				   } 	\
				   set.pop_back();



#define inputcont 		string _command;                                            \
						getline(cin, _command); 									\
						char _instruct[512];			\
	    				for(int i = 0; i < static_cast<int>(_command.size()); i++) 				\
	    				{														\
	        				_instruct[i] = _command.at(i); 						\
	    				}														\
						_instruct[_command.size()] = '\0';						\
						char* _token = strtok((char*)_instruct, " ");				\
						cmds.push_back(_token);									\
				    	while(_token != NULL)									\
				    	{														\
				    		 _token = strtok(NULL, " ");							\
				    		 if(_token == NULL)									\
				    		 {													\
				        		 break;											\
				        	 }													\
				        cmds.push_back(_token);								\
				    	}

#include <iostream>
#include <vector>
#include <string>
#include <stack>

//fork, execvp, wait, stat
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

using namespace std;

//Global Variables
const unsigned buffSize = BUFSIZ;



//----------------------------------------------------------------------------------------------------------rshell prompt
void initializeUser()
{
	char* user = getlogin();
	char host[buffSize];
	int hostID = gethostname(host, 1024);
	//Guards to be added for NULL cases
	//Host to be added
	if(user == NULL)
	{
		//perror("Unidentified User");
	}
	else if(hostID == -1)
	{
		//perror("Unidentified Host");
	}
	else
	{
		cout << user << '@' << host << "$ ";
	}
}

//---------------------------------------------------------------------------------------------------------function for calling children to action
bool rshell(vector<string>& set)
{
	while(!set.empty())
	{
		if(set.at(0) == "exit")
		{
			exit(0);
		}

    	
    	//Pid / Fork
    	pid_t pid = fork();
	    if(pid == -1)
	    {
	        perror("fork");
	    }
	    
	   
	    //Child
	    if(pid == 0)
	    {
		    
		    //Convert in char* in order to call execvp
		    char* cmd_exe[512];
		    //Load cmd_exe
		    for(int i = 0; i < static_cast<int>(set.size()); i++)
		    {
		    	cmd_exe[i] = (char*)set.at(i).c_str();
		    }
		    
		    //Set NULL ending the command
		    cmd_exe[set.size()] = NULL;
		    
		    //call execvp
		    if(execvp (cmd_exe[0], cmd_exe) == -1)
			{
				perror("exec");
		    	
		    	exit(-1);
		    }
		    
	    }
	   
	   
	    //Parent
	    if(pid > 0)
		{
			int status;
			
			//Wait
			waitpid(0, &status, 0);
			
			//Error
			if(status > 0)
			{
				return false;
			}
			
			return true;
		}
			
		set.clear();
	}
	return true;
}

//------------------------------------------------------------------------------------------------------------Test function

bool test(const vector<string>& shadow)
{
    //Initialize struct
    struct stat attributes;
    
    
    //Check size of vector: base (1) and (2)
    if(shadow.size() == 1)
    {
    	return false;
    }
    else if(shadow.size() == 2)
    {
    	return true;
    }
    //Assume the entire set is passed in; structure: test -flags /path/filename
    // Ignore item 1, check flags
    
    unsigned int index = 1;
    bool reg = false;
    bool dir = false;
    bool key = false;
	
	
	//While prefix has '-', look for specified flags.
	while(index < shadow.size() && shadow.at(index).at(0) == '-')
	{
		//Base case: size != means too many flags, cout error.
		if(shadow.at(index).size() != 2)
		{
			cout << "-Rshell: test: " << shadow.at(index) << ": unary operator expected" << endl;
			return false;
		}
		
		if(shadow.at(index).at(1) == 'e' && key == false)
		{
			key = true;
		}
		else if(shadow.at(index).at(1) == 'f' && key == false)
		{
			reg = true;
			key = true;
		}
		else if(shadow.at(index).at(1) == 'd' && key == false)
		{
			dir = true;
			key = true;
		}
		else
		{
			cout << "-Rshell: test: too many arguments" << endl;
			return false;
		}
		index++;
	}
    
    //Next index is the specified name, if detect 'preceding \', then delete.
    string name;
	if(index < shadow.size())
	{
	    if(shadow.at(index).at(0) == '/')
	    {
	        name = shadow.at(index).substr(1);
	    }
	    else
	    {
	        name = shadow.at(index);
	    }
	}   
	
	if(name == "")
	{
		return true;
	}
	
    cout << "name: " << name << endl;
    // Pass struct memory as pointer on stat()
    stat((char*)name.c_str(),&attributes);
    
    //Perform checks
    if(!access((char*)name.c_str(), F_OK))
    {
        if(reg)
        {    
            if(S_ISREG(attributes.st_mode))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        
        if(dir)
        {
            if(S_ISDIR(attributes.st_mode))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    
    //This gives the error message "no such file or directory"
    perror("rshell: ");
    return false;
}

//----------------------------------------------------------------------------------------------------------------Esuna function
vector<string> esuna(const vector<string>& comment)
{
	vector<string> cmds;
	//Filter cmds: look for '#', ';', "||", "&&"
    for(unsigned int i = 0; i < comment.size(); i++)
    {
    	if(comment.at(i).at(0) == '#' || comment.at(i).at(0) == '\t')
    	{
    		break;
    	}
    	//Filter ';'
    	else if(comment.at(i).find(';') != string::npos && comment.at(i).size() != 1)
    	{
    		int s_index = 0;
    		for(unsigned int j = 0; j < comment.at(i).size(); j++)
	    	{

	    		if(comment.at(i).find(';', j) != string::npos)
	    		{
		    	 	j = comment.at(i).find(';', j);
		    	 	//Found/Parse/Push
		    	 	string breaker = comment.at(i).substr(s_index, j - s_index);
		    	 	string semi = comment.at(i).substr(j, 1);
		    	 	s_index = (j + 1);
		    	 	if (breaker != "") cmds.push_back(breaker);
		    	 	cmds.push_back(semi);
	    		}
	    		else
	    		{
		    	 	string breaker = comment.at(i).substr(j, comment.at(i).size() - j);
	    			cmds.push_back(breaker);
	    			break;
	    		}
	    	}
    	}
    	//Filter "&&"
    	else if(comment.at(i).find("&&") != string::npos && comment.at(i).size() != 1)
    	{
    		int s_index = 0;
    		for(unsigned int j = 0; j < comment.at(i).size(); j++)
	    	{

	    		if(comment.at(i).find("&&", j) != string::npos)
	    		{
		    	 	j = comment.at(i).find("&&", j);
		    	 	//Found/Parse/Push
		    	 	string breaker = comment.at(i).substr(s_index, j - s_index);
		    	 	string a = comment.at(i).substr(j, 2);

		    	 	s_index = (j + 2);
		    	 	if (breaker != "") cmds.push_back(breaker);
		    	 	cmds.push_back(a);
		    	 	j += 1;
	    		}
	    		else
	    		{
		    	 	string breaker = comment.at(i).substr(j, comment.at(i).size() - j);
	    			cmds.push_back(breaker);
	    			break;
	    		}
	    	}
    	}
    	//Filter "||"
    	else if(comment.at(i).find("||") != string::npos && comment.at(i).size() != 1)
    	{
    		int s_index = 0;
    		for(unsigned int j = 0; j < comment.at(i).size(); j++)
	    	{

	    		if(comment.at(i).find("||", j) != string::npos)
	    		{
		    	 	j = comment.at(i).find("||", j);
		    	 	//Found/Parse/Push
		    	 	string breaker = comment.at(i).substr(s_index, j - s_index);
		    	 	string a = comment.at(i).substr(j, 2);
		    	 	s_index = (j + 2);
		    	 	if (breaker != "") cmds.push_back(breaker);
		    	 	cmds.push_back(a);
		    	 	j += 1;
	    		}
	    		else
	    		{
		    	 	string breaker = comment.at(i).substr(j, comment.at(i).size() - j);
	    			cmds.push_back(breaker);
	    			break;
	    		}
	    	}
    	}
    	//Filter '('
    	else if(comment.at(i).find('(') != string::npos && comment.at(i).size() != 1)
    	{
    		if(comment.at(i).at(0) == '(')
    		{
    			string breaker = comment.at(i).substr(0, 1);
    			string left = comment.at(i).substr(1, comment.at(i).size() - 1);

    			if (breaker != "") cmds.push_back(breaker);
    			cmds.push_back(left);
    		}
    	}
		//Filter ')'
    	else if(comment.at(i).find(')') != string::npos && comment.at(i).size() != 1)
    	{
    		int s_index = 0;
    		for(unsigned int j = 0; j < comment.at(i).size(); j++)
	    	{

	    		if(comment.at(i).find(')', j) != string::npos)
	    		{
		    	 	j = comment.at(i).find(')', j);
		    	 	//Found/Parse/Push
		    	 	string breaker = comment.at(i).substr(s_index, j - s_index);
		    	 	string semi = comment.at(i).substr(j, 1);
		    	 	s_index = (j + 1);
		    	 	if (breaker != "") cmds.push_back(breaker);
		    	 	cmds.push_back(semi);
	    		}
	    		else
	    		{
		    	 	string breaker = comment.at(i).substr(j, comment.at(i).size() - j);
	    			cmds.push_back(breaker);
	    			break;
	    		}
	    	}
    	}
    	else
    	{
    		cmds.push_back(comment.at(i));
    	}
    }
    return cmds;
}

//=========================================================================================================================== Recursive Call

bool functionality (vector<string> cmds, unsigned &j) {
	
		//boolean for determining successive operations
	    bool status = true;
	    
	    //actual child-progress command storage
	    vector<string> set;
	    
	    if (cmds.at(j) == "(") {
	    	++j;
	    	status = functionality(cmds, j);
	    }
	    
	    if(j >= cmds.size()) return status;
	    
	    //Base case command
	    if ((j != 0 && cmds.at(j - 1) != ")") || j == 0) {
		    for(; j < cmds.size(); ++j) {
		    	
		    	if ((cmds.at(j) != "||") &&
		    		(cmds.at(j) != "&&") &&
		    		(cmds.at(j) != ";")  &&
		    		(cmds.at(j) != "#")  &&
		    		(cmds.at(j) != ")") ) 
		    		{
		    			set.push_back(cmds.at(j));
		    		}
		    		else break;
		    }
	    }
		
		if(!set.empty() && (set.at(0) == "test" || set.at(0) == "["))
		{
			status = test(set);
				
			if(status)
			{
				cout << "(True)" << endl;
			}
			else
			{
				cout << "(False)" << endl;
			}
			set.clear();
		}
		else
		{
			if(!set.empty()) {
			    status = rshell(set);
			    set.clear();
			}
		}

	    //loading in successive commands after base case
	    for(; j < cmds.size(); ++j)
	    {
	    	if(!set.empty() && cmds.at(j) == "(" ) {
	    		if(set.at(0) == "||" && status == false) {
			    	set.clear();
			    	++j;
			    	status = functionality(cmds, j);
	    		}
	    		else if(set.at(0) == "&&" && status == true) {
	    			set.clear();
			    	++j;
			    	status = functionality(cmds, j);
	    		}
	    		else if(set.at(0) == ";") {
	    			set.clear();
	    			++j;
	    			status = functionality(cmds, j);
	    		}
	    		else {
	    			set.clear();
	    			while (cmds.at(j) != ")") ++j;
	    		}
	    	}
	    	
	    	if(j >= cmds.size()) return status;
	    	
	    	if (!set.empty() && cmds.at(j) == ")") {
	    		++j;
	    		
	    		if(set.at(0) == "||" && status == false) {
			    	popfront();
			    	if (set.at(0) == "test" || set.at(0) == "[") return test(set);
	    			else return rshell(set);
	    		}
	    		else if(set.at(0) == "&&" && status == true) {
	    			popfront();
			    	if (set.at(0) == "test" || set.at(0) == "[") return test(set);
	    			else return rshell(set);
	    		}
	    		else if(set.at(0) == ";") {
	    			popfront();
	    			if (set.at(0) == "test" || set.at(0) == "[") return test(set);
	    			else return rshell(set);
	    		}
	    		else {
	    			set.clear();
	    			return status;
	    		}
	    	}
	    	set.push_back(cmds.at(j));
	    	
	    	if (set.size() > 1 && (cmds.at(j) == "||" || 
	    						   cmds.at(j) == "&&" || 
	    						   cmds.at(j) == ";"  ||
	    						   cmds.at(j) == "#")) 
	    	{
			    if(set.at(0) == ";")
			    {
			    	--j;
			    	set.pop_back();
			    	popfront();
			    	
			    	//Either call rshell or test
			    	if (set.at(0) == "test" || set.at(0) == "[") status = test(set);
	    			else return status = rshell(set);
			    	set.clear();
			    }	
			    else if(set.at(0) == "&&")
			    {
			    	--j;
			    	set.pop_back();
			    	popfront();
			    	
			    	//Either call rshell or test
			    	if(status == true)
			    	{
			    		if (set.at(0) == "test" || set.at(0) == "[") status = test(set);
	    				//else return status = rshell(set);
	    				else
	    				{
	    					status = rshell(set);
	    				}
			    	}
			    	set.clear();
			    }
			    else if(set.at(0) == "||")
			    {
			    	--j;
			    	set.pop_back();
			    	popfront();
			    	
			    	//Either call rshell or test
			    	if(status == false)
			    	{
			    		if (set.at(0) == "test" || set.at(0) == "[") status = test(set);
	    				else return status = rshell(set);
			    	}
			    	
			    	set.clear();
			    }
	    	}
	    	else if(set.size() == 1 && set.at(0) == "#") break;
	    }
	    
	    //dealing with last base case, (where last string != connector)
	    if(set.size() != 0 && set.at(0) == ";")
		{
	    	popfront();
	    	
	    	if (set.size() > 1) {
		    	if (set.at(0) == "test" || set.at(0) == "[") status = test(set);
		    	else return status = rshell(set);
	    	}
	    }	
	    else if(set.size() != 0 && set.at(0) == "&&")
	    {
		   	popfront();
		   	
	    	if(status == true)
	    	{
	    		if (set.at(0) == "test" || set.at(0) == "[") status = test(set);
	    		else return status = rshell(set);
	    	}
	    }
	    else if(set.size() != 0 && set.at(0) == "||")
	    {
	    	popfront();
	    	
	    	if(status == false)
	    	{
	    		if (set.at(0) == "test" || set.at(0) == "[") status = test(set);
	    		else return status = rshell(set);
	    	}
	    }
	    
	    return status;

}

//=========================================================================================================================== MAIN
//=========================================================================================================================== MAIN

int main()
{
	//rshell
	
    //fork
    while(1)
    {
		string command;		
		
		
		//rshell prompt
    	initializeUser();
    	
		//retrieve line and test incorrect line cases
		while (command.empty()) {
			cout << "Heilii@Alexina.Server $ ";
			getline(cin, command);
			bool reset = false;
			bool statement = false;
			bool statement2 = false;
			bool statement3 = false;
			bool statement4 = false;
			
			int parenCheckerO = 0;
			int parenCheckerC = 0;
			
			if(!command.empty()) {
				for(unsigned parenChecker = 0; parenChecker < command.size(); ++parenChecker) {
					if (command.at(parenChecker) == '(') {++parenCheckerO; statement3 = true;}
						for(unsigned stateChecker = parenChecker + 1; stateChecker < command.size(); ++ stateChecker) {
							if (command.at(stateChecker) != ' ' && command.at(stateChecker) != ')') statement3 = false;
							if (command.at(stateChecker) == ')') break;
						}
					if (statement3) {reset = true; break;}
					if (command.at(parenChecker) == '#') break;
					if (command.at(parenChecker) == ')') {
						++parenCheckerC;
						if( parenChecker + 1 < command.size()) {
							if (command.at(parenChecker + 1) != ';' && 
								command.at(parenChecker + 1) != '#' && 
								command.at(parenChecker + 1) != ')'	  ) {
								if( parenChecker + 2 < command.size()) {
									if (((command.at(parenChecker + 1) == '|' && command.at(parenChecker + 2) != '|') &&
						                (command.at(parenChecker + 1) == '&' && command.at(parenChecker + 2) != '&')) ||
						                 command.at(parenChecker + 1) == ' ') {
										if (command.at(parenChecker + 2) != ';' && 
											command.at(parenChecker + 2) != '#' && 
											command.at(parenChecker + 2) != ')'   ) {
											if( parenChecker + 3 < command.size() && 
												(command.at(parenChecker + 2) == '|' ||
												 command.at(parenChecker + 2) == '&'   ) ) {
												if ((command.at(parenChecker + 2) == '|' && command.at(parenChecker + 3) != '|') &&
							             			(command.at(parenChecker + 2) == '&' && command.at(parenChecker + 3) != '&') ){statement4 = true; reset = true;}
											}
											else {statement4 = true; reset = true;}
										}
									}
								}
								else {statement4 = true; reset = true;}
							}
					 	}
					}
				}
				if (parenCheckerO != parenCheckerC) {reset = true; statement3 = true;}
				if (command.at(0) == ' ') reset = true;
				if(command.at(0) == '	') reset = true;
				if(command.at(0) == '#') reset = true;
				if(command.at(0) == ';') {reset = true; statement = true;} 
				if(command.size() > 1) {
					if(command.at(0) == '|' && command.at(1) == '|') {reset = true; statement2 = true;}
					if(command.at(0) == '&' && command.at(1) == '&') {reset = true; statement2 = true;}
				}
			}
			if(statement) cout << "bash: syntax error near unexpected token `;\'" << endl;
			else if(statement2) cout << "bash: syntax error near unexpected token `" << command.at(0) << command.at(1) << "\'" << endl;
			else if(statement3) cout << "bash: syntax error near unexpected token `)\'" << endl;
			else if(statement4) cout << "bash: syntax error after token `)\'" << endl;
			if(reset) command.clear();
		}
	
		char instruct[512];
		
	    // Load command into instruct
	    for(int i = 0; i < static_cast<int>(command.size()); i++)
	    {
        	instruct[i] = command.at(i);
	    }
	  	
		//NULL
		instruct[command.size()] = '\0';
		
		//first Initalization
		vector<string> cmds;
		vector<string> mana;
		//comment container
		vector<string> comment;
		
		char* token = strtok((char*)instruct, " ");
		
		//Push main comment
		if(token != '\0')
		{
			comment.push_back(token);
		}
		
		
		//Insert arguments / cmds / connectors
	    while(token != NULL)
	    {
	        token = strtok(NULL, " ");
	        
	        //NULL Guard
	        if(token == NULL)
	        {
	            break;
	        }
	        
	        comment.push_back(token);
	    }
	   	
	   	bool clear = false;
	   	mana = esuna(comment);
	   	//Iterate and check cmds for debuffs
	   	while(!clear)
	   	{
	   		for(unsigned int y = 0; y < mana.size(); y++)
	   		{
	   			if(mana.at(y).at(0) != ';')
	   			{
	   				if(mana.at(y).find(';') != string::npos)
	   				{
	   					mana = esuna(mana);
	   					clear = false;
	   					break;
	   				}
	   			}
	   			if(mana.at(y).at(0) != '&')
	   			{
	   				if(mana.at(y).find("&&") != string::npos)
	   				{
	   					mana = esuna(mana);
	   					clear = false;
	   					break;
	   				}
	   			}
	   			if(mana.at(y).at(0) != '|')
	   			{
		   			if(mana.at(y).find("||") != string::npos)
		   			{
		   				mana = esuna(mana);
		   				clear = false;
		   				break;
		   			}
	   			}
	 			if(mana.at(y).at(0) == '(')
	   			{
	   				if(mana.at(y).size() != 1)
	   				{
	   					mana = esuna(mana);
	   					clear = false;
	   					break;
	   				}
	   			}
	   			if(mana.at(y).at(0) != ')')
	   			{
		   			if(mana.at(y).find(")") != string::npos)
		   			{
		   				mana = esuna(mana);
		   				clear = false;
		   				break;
		   			}
	   			}
	   			clear = true;
	   		}
	   	}
	   	
	   	cout << "-----------------------------------------------mana-----------------------------------------------------" << endl;
	    for(unsigned int i = 0; i < mana.size(); i++)
	    {
	    	cout << mana.at(i) << endl;
	    	if(mana.at(i) == "#")
	    	{
	    		break;
	    	}
	    	else
	    	{
	    		cmds.push_back(mana.at(i));
	    	}
	    }
	    
	   	cout << "-----------------------------------------------mana-----------------------------------------------------" << endl;
	    //Check if cmds is empty
	    if(!cmds.empty())
	    {
	    	while (cmds.at(cmds.size() - 1) == "&&" || cmds.at(cmds.size() - 1) == "||") 
	    	{
	    		cout << "> ";
	    		
	    		inputcont;
	    	}
	    }
	    //By now all are parsed by space
	    unsigned starter = 0;
	    functionality(cmds, starter);
    }

    return 0;
}

