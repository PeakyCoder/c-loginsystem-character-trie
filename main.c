#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h> 

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0]) 
  
// Alphabet size (# of symbols) 
#define ALPHABET_SIZE 26
  
// Converts key current character into index 
// use only 'a' through 'z' and lower case 
#define CHAR_TO_INDEX(c) ((int)c - (int)'a') 
  
// trie node 
struct TrieNode 
{ 
    struct TrieNode *children[ALPHABET_SIZE]; 
  
    // isEndOfWord is true if the node represents 
    // end of a word 
    bool isEndOfWord; 
    char password[255];
}; 
  
// Returns new trie node (initialized to NULLs) 
struct TrieNode *getNode(){ 
    struct TrieNode *pNode = NULL; 
  
    pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode)); 
  
    if (pNode){ 
        int i; 
  
        pNode->isEndOfWord = false; 
  
        for (i = 0; i < ALPHABET_SIZE; i++) 
            pNode->children[i] = NULL; 
    } 
  
    return pNode; 
} 

bool isEmpty(struct TrieNode* root){ 
	int i;
    for (i = 0; i < ALPHABET_SIZE; i++){
    	if (root->children[i]){
        	return false; 	
		}   
	}
    return true; 
} 

int moreThanOneLeaf(struct TrieNode* root){ 
	int i, count = 0;
    for (i = 0; i < ALPHABET_SIZE; i++){
    	if (root->children[i]){
        	count++; 	
		}   
	}
    return count; 
} 

// Returns true if key presents in trie, else false 
bool search(struct TrieNode *root, const char *key) 
{ 
    int level; 
    int length = strlen(key); 
    int index; 
    struct TrieNode *iterRoot = root; 
  
    for (level = 0; level < length; level++) 
    { 
        index = CHAR_TO_INDEX(key[level]); 
  
        if (!iterRoot->children[index]) 
            return false; 
  
        iterRoot = iterRoot->children[index]; 
    } 
  
    return (iterRoot != NULL && iterRoot->isEndOfWord); 
} 

void addTrie(struct TrieNode* root, char id[], char pass[], FILE *outputFile){
    int level; 
    int length = strlen(id); 
    int index; 
  
    struct TrieNode *iterRoot = root; 
    //If there is already user 
  	if(search(iterRoot, id)){
  		fprintf(outputFile, "\"%s\" reserved username\n", id);
	}
	//Add new user
	else{
		 for (level = 0; level < length; level++) 
    	{ 
	        index = CHAR_TO_INDEX(id[level]); 
	        if (!iterRoot->children[index]){
	        	iterRoot->children[index] = getNode(); 
			}
	        
	        iterRoot = iterRoot->children[index]; 
	    } 

    // mark last node as leaf and put password 
    iterRoot->isEndOfWord = true; 
    strcpy(iterRoot->password, pass); 
    fprintf(outputFile, "\"%s\" was added\n", id);
	}
}


void getPass(struct TrieNode* root, char id[], FILE *outputFile){
	int level; 
    int length = strlen(id); 
    int index; 
    struct TrieNode *iterRoot = root; 
  	  	
    for (level = 0; level < length; level++) 
    { 	
        index = CHAR_TO_INDEX(id[level]); 
  		//If has no record
        if (!iterRoot->children[index] && level == 0){
        	fprintf(outputFile, "\"%s\" no record\n", id);
        	return;
		}
		//If it is in trie to some length but no record
		if(!iterRoot->children[index] && level != 0){
			fprintf(outputFile, "\"%s\" incorrect username\n", id);
			return;
		}   
  
        iterRoot = iterRoot->children[index]; 
    } 
    //If is correct show password
    if((iterRoot != NULL && iterRoot->isEndOfWord) == true){
    	fprintf(outputFile, "\"%s\" password \"%s\"\n", id, iterRoot->password);  
    	return;
	}
	//If it is not enough 
	if(iterRoot != NULL && !iterRoot->isEndOfWord){
		fprintf(outputFile, "\"%s\" not enough username\n", id);
		return;
	}

}

 
void login(struct TrieNode* root, char id[], char pass[], FILE *outputFile){
	int level; 
    int length = strlen(id); 
    int index; 
    struct TrieNode *iterRoot = root; 
  	  	
    for (level = 0; level < length; level++) 
    { 	
        index = CHAR_TO_INDEX(id[level]); 
  		//If there is nothing about this username from the beginning
        if (!iterRoot->children[index] && level == 0){
        	fprintf(outputFile, "\"%s\" no record\n", id);
        	return;
		}
		//If there is nothing about this username except the beginning
		if(!iterRoot->children[index] && level != 0){
			fprintf(outputFile, "\"%s\" incorrect username\n", id);
			return;
		}   
  
        iterRoot = iterRoot->children[index]; 
    } 
    //If find username in trie control situtions
    if((iterRoot != NULL && iterRoot->isEndOfWord) == true){
    	//If password is correct success
    	if(strcmp(iterRoot->password, pass) == 0){
    		fprintf(outputFile, "\"%s\" successful login\n", id);  
    		return;
		}
		//Else wrong password
		else{
			fprintf(outputFile, "\"%s\" incorrect password\n", id);  
    		return;
		}
    	
	}
	//If not enough username (It is in trie)
	if(iterRoot != NULL && !iterRoot->isEndOfWord){
		fprintf(outputFile, "\"%s\" not enough username\n", id);
		return;
	}
}  

struct TrieNode* deleteUser(struct TrieNode* root, char id[], int depth, FILE *outputFile) { 
    // If tree is empty 
    if (!root){
    	return NULL;
	}
         
    // When reach the last character of username
    if (depth == strlen(id)) { 
  
        // Return enOfWord to false and delete password 
        if (root->isEndOfWord) {
        	root->isEndOfWord = false;
			root->password[0] = '\0'; 
		}
           
           
        // If there is no children of this character delete node
        if (isEmpty(root)) { 
            free (root); 
            root = NULL; 
        } 
  
        return root; 
    } 
  
    // If not the last char of username call this function recursively by ASCII of char and increase depth by one
    int index = id[depth] - 'a'; 
    root->children[index] = deleteUser(root->children[index], id, depth + 1, outputFile); 
  
	//If node is not the last char of username and empty then delete
    if (isEmpty(root) && root->isEndOfWord == false) { 
        free (root); 
        root = NULL; 
    } 
  
    return root; 
}

void findUser(struct TrieNode* root, char id[], FILE *outputFile){
	int level; 
    int length = strlen(id); 
    int index; 
    struct TrieNode *iterRoot = root; 
  	  	
    for (level = 0; level < length; level++) 
    { 	
        index = CHAR_TO_INDEX(id[level]); 
  		//If there is nothing about this username from the beginning
        if (!iterRoot->children[index] && level == 0){
        	fprintf(outputFile, "\"%s\" no record\n", id);
        	return;
		}
		//If there is nothing about this username except the beginning
		if(!iterRoot->children[index] && level != 0){
			fprintf(outputFile, "\"%s\" incorrect username\n", id);
			return;
		}   
  
        iterRoot = iterRoot->children[index]; 
    } 
    
   	//If not enough username (It is in trie)
	if(iterRoot != NULL && !iterRoot->isEndOfWord){
		fprintf(outputFile, "\"%s\" not enough username\n", id);
		return;
	}
    
    //Find username and delete from trie
    if((iterRoot != NULL && iterRoot->isEndOfWord) == true){
    	if(iterRoot->children != NULL){
    		deleteUser(root, id, 0, outputFile);
	    	fprintf(outputFile, "\"%s\" deletion is successful\n", id);
	    	return;
		}
    
	}

}
void append(char *s, char c)
{
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}


void restOfList(struct TrieNode* root, char word[], char word2[], FILE *outputFile){
	int i;

    for (i = 0; i < ALPHABET_SIZE; i++){
    	if (root->children[i] && root->children[i]->isEndOfWord == false ){
    		
        	append(word2, 'a'+i);
        	restOfList(root->children[i], word, word2, outputFile);
		} 
		else if (root->children[i] && root->children[i]->isEndOfWord == true ){
			
        	
        	if(isEmpty(root->children[i]) == true){
        		fprintf(outputFile, "\t%s%c\n", word2, 'a'+i);
        		strcpy(word2, word);
			}
        	if(isEmpty(root->children[i]) == false){
        		append(word2, 'a'+i);
        		
        		fprintf(outputFile, "\t%s", word2);	
			}
        
        	restOfList(root->children[i], word, word2, outputFile);
			
		} 

	}
}

void listOfTrie(struct TrieNode* root, char word[], char word2[], FILE *outputFile){
	int i, count = 0;
	
    for (i = 0; i < ALPHABET_SIZE; i++){
    	
    	if (root->children[i] && moreThanOneLeaf(root->children[i]) < 2){
        	
        	append(word, 'a'+i);
        	append(word2, 'a'+i);
        	
        	listOfTrie(root->children[i], word, word2, outputFile);
        	if(isEmpty(root->children[i]) == true){
        		fprintf(outputFile, "%s\n\t%s\n", word, word);	
			}
        	
        	strcpy(word, "-");
			strcpy(word2, "-");
		}   
		else if(root->children[i] && moreThanOneLeaf(root->children[i]) > 1){
				
			append(word, 'a'+i);
			append(word2, 'a'+i);
			fprintf(outputFile, "%s\n", word);
			if(root->children[i]->isEndOfWord == true){
				fprintf(outputFile, "\t%s\n", word);
			}
			restOfList(root->children[i], word, word2, outputFile);
			strcpy(word, "-");
			strcpy(word2, "-");
		}
	
	}

}


// Main Function
int main(int argc, char *argv[]) 
{ 
	//Root of Trie
	struct TrieNode *root = getNode();	
	
	//Define some string holder
	char id[255], pass[255], read[255], word[255], word2[255];
	strcpy(word, "-");
	strcpy(word2, "-");
	//Reading File 
	FILE *inputFile, *outputFile;
	inputFile = fopen(argv[1], "r");
	outputFile = fopen("output.txt", "w");
	while(fscanf(inputFile, "%s", read) == 1) {
    	
		if(		strcmp("-a", read) == 0){
			fscanf(inputFile, "%s", id);
			fscanf(inputFile, "%s", pass);
			addTrie(root, id, pass, outputFile); 
		}
		else if(strcmp("-s", read) == 0){
			fscanf(inputFile, "%s", id);
			getPass(root, id, outputFile);
		}
		else if(strcmp("-q", read) == 0){
			fscanf(inputFile, "%s", id);
			fscanf(inputFile, "%s", pass);
			login(root, id, pass, outputFile);
		}
		else if(strcmp("-d", read) == 0){
			fscanf(inputFile, "%s", id);
			findUser(root, id, outputFile);
		}
		else if(strcmp("-l", read) == 0){
			listOfTrie(root, word, word2, outputFile);
		}
		else if (feof(inputFile)) {
		    break;
		}
	
	}

		
	//Close files
	fclose(inputFile);
	fclose(outputFile);
	
    return 0; 
} 



