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
    string chunk;
    hashNode *nextCollision;
};

string makeString(queue<string> words);
int hashFunc(hashNode node, int tableSize);
void insertHash(hashNode hashTable[], int tableIndex, hashNode &node);



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

        //for each file in vector
        for (int i = 0; i < files.size(); i++){
            //open first file
            ifstream myfile;
            myfile.open(files[i].c_str());

            //create hash table
            hashNode* hashTable[tableSize];
            for (int i = 0; i < tableSize; i++) {         //initialize table to NULL
                hashTable[i] = NULL;
            }

            //create queue
            queue<string> words = queue<string>();
            string word;

            //push first n words to queue
            int index = 0;
            while ((myfile >> word) && (index < lengthOfChunks)){
                words.push(word);
                index++;
            }

            //then move those to hash table
            string chunk = makeString(words);
            hashNode *current = new hashNode;
            current->chunk = chunk;
            current->nextCollision = NULL;
            int tableIndex = hashFunc(*current, tableSize);
            insertHash(hashTable, tableIndex, current);



        //pop first word, push next word
            while ((myfile >> word)){
                words.pop();
                words.push(word);
                //move to hash
                string chunk = makeString(words);
                hashNode *current = new hashNode;
                current->chunk = chunk;
                current->nextCollision = NULL;
                int tableIndex = hashFunc(*current, tableSize);
                insertHash(hashTable, tableIndex, current);
        }

    }

}

string makeString(queue<string> words){
    string result = "";
    while (!words.empty()){
        string word = words.front();
        result += word;
        words.pop();
    }
}

int hashFunc(hashNode node, int tableSize){
    return ((node.chunk[0]) + (27 * node.chunk[1]) + (729 * node.chunk[2])) % tableSize;
}

void insertHash(hashNode hashTable[], int tableIndex, hashNode &node){
    if (hashTable[tableIndex] == NULL) {                        //if empty, add file node
        hashTable[tableIndex] = node;
    }
    else {                                                      //if not empty, add to head of linked list
        hashNode temp = hashTable[tableIndex];
        hashTable[tableIndex] = node;
        node.nextCollision = &temp;
    }

}