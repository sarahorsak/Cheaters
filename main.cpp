#include <sys/types.h>
#include <dirent.h>
#include <cerrno>
#include <vector>
#include <string>
#include <iostream>
#include <queue>
#include <fstream>

using namespace std;

struct hashNode{
    int filename;
    string chunk;
    hashNode *nextCollision;
};

string makeString(queue<string> words);
int hashFunc(hashNode node, int tableSize);
void insertHash(hashNode *hashTable[], int tableIndex, hashNode &node);



/*function... might want it in some class?*/
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

int main(int argc, char *argv[])
{
    string dir = string("sm_doc_set");
    vector<string> files = vector<string>();

    getdir(dir,files);

    for (unsigned int i = 0;i < files.size();i++) {
        cout << i << files[i] << endl;
    }

    int lengthOfChunks = atoi(argv[2]);
    int num_similar = atoi(argv[3]);
    static const int tableSize = 700001; //random prime number


    int grid[files.size()][files.size()];          //create a 2d array to contain number of similarities

    for(int i = 0; i < files.size(); i++){
        for(int j = 0; j < files.size(); j++){        //initialize grid to 0
            grid[i][j] = 0;
        }
    }

    //create hash table
    hashNode* hashTable[tableSize];
    for (int j = 0; j < tableSize; j++) {         //initialize table to NULL
        hashTable[j] = NULL;
    }

    //for each file in the vector
     for (int i = 0; i < files.size(); i++){
         //open first file
         ifstream myfile;
         myfile.open(files[i].c_str());

         //create queue
         queue<string> words = queue<string>();
         string word;

         //push first n words to queue
         int index = 0;
         while ((myfile >> word) && (index < lengthOfChunks)){
             words.push(word);
             index++;
         }

         //then move the index of file name to hash table
         //get hash key based off the chunk of words
         string chunk = makeString(words);
         hashNode *current = new hashNode;
         current->chunk = chunk;
         current->filename = i;
         current->nextCollision = NULL;
         int tableIndex = hashFunc(*current, tableSize);
         insertHash(hashTable, tableIndex, *current);



        //pop first word, push next word
        while ((myfile >> word)){
            words.pop();
            words.push(word);
            //move to hash
            //get hash key based off chunk
            string chunk = makeString(words);
            hashNode *current = new hashNode;
            current->chunk = chunk;
            current->filename = i;
            current->nextCollision = NULL;
            int tableIndex = hashFunc(*current, tableSize);
            insertHash(hashTable, tableIndex, *current);
        }

    //hash every chunk of every file to same hash table
    }

//need to go through hash table and get collisions into a 2D array
//compare if collisions are > inputted collision number then print the file names
}

//makes the words from queue into a string chunk
string makeString(queue<string> words){
    string result = "";
    while (!words.empty()){
        string word = words.front();
        result += word;
        words.pop();
    }
    return result;
}

//returns the hash key based off the chunk of info
int hashFunc(hashNode node, int tableSize){
    return ((node.chunk[0]) + (27 * node.chunk[1]) + (729 * node.chunk[2])) % tableSize;
}

//inserts the node into the correct spot in the hash array
void insertHash(hashNode *hashTable[], int tableIndex, hashNode &node){
    if (hashTable[tableIndex] == NULL) {                        //if empty, add file node
        hashTable[tableIndex] = &node;
    }
    else {                                                      //if not empty, add to head of linked list
        hashNode *temp = hashTable[tableIndex];
        hashTable[tableIndex] = &node;
        node.nextCollision = temp;
    }

}