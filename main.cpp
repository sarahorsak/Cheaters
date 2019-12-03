#include <sys/types.h>
#include <dirent.h>
#include <cerrno>
#include <vector>
#include <string>
#include <iostream>
#include <queue>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

struct hashNode{
    int filename;
    string chunk;
    hashNode *nextCollision;
};

struct collision{
    int filename1;
    int filename2;
    int num_collisions;
};

string makeString(queue<string> words);
int hashFunc(hashNode node, int tableSize);
void insertHash(hashNode *hashTable[], int tableIndex, hashNode &node);
bool compareCollision(collision *c1, collision *c2);
int getdir (string dir, vector<string> &files);


int getdir (string dir, vector<string> &files){
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
         int fileIndex = i;

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

         string chunk = makeString(words);
         hashNode *current = new hashNode;
         current->chunk = chunk;
         current->filename = fileIndex;
         current->nextCollision = NULL;

         //get hash key based off the chunk of words
         int tableIndex = hash<std::string>{}(current->chunk);
         insertHash(hashTable, tableIndex, *current);



        //pop first word, push next word
        while ((myfile >> word)){
            words.pop();
            words.push(word);
            //move to hash
            string chunk = makeString(words);
            hashNode *current = new hashNode;
            current->chunk = chunk;
            current->filename = fileIndex;
            current->nextCollision = NULL;

            //get hash key based off chunk
            int tableIndex = hash<std::string>{}(current->chunk);
            insertHash(hashTable, tableIndex, *current);
        }

    //hash every chunk of every file to same hash table
    }

//need to go through hash table and get collisions into a 2D array
//traverse through hash table
    for (int i = 0; i < tableSize; i ++){
        //if there's a node at that location
        hashNode *first = hashTable[i];
        //while the first in the list isn't null
        while (first!= NULL){
            //set current = to first->next
            hashNode *node = first;
            hashNode *current = first->nextCollision;
            //while current isn't null, then add collision to the grid
            while (current != NULL){
                //if they are diff files, add collision
                if (first ->filename != current->filename){
                    grid[first->filename][current->filename]++;
                }
                //alter current to next one in linked list
                current = current-> nextCollision;
                //add all the collisions for first in linked list
            }
            //alter first to be the next one and repeat
            first = first->nextCollision;
            //delete the one just accessed
            delete node;
        }
    }
    //traverse through the grid and create a list of collisions
    vector<collision*> collisions;
    for (int r = 0; r < files.size(); r++){
        for (int c = 0; c < files.size(); c++){
            if (grid[r][c] >= num_similar){
                collision *temp = new collision;
                temp->filename1= r;
                temp->filename2 = c;
                temp->num_collisions = grid[r][c];
                collisions.push_back(temp);
            }
        }
    }

    //sort collision data by using built in sort
    sort(collisions.begin(), collisions.end(), compareCollision);

    //print collision data
    for (int i = 0; i < collisions.size(); i++){
        int file1 = (collisions[i]->filename1);
        int file2 = (collisions[i]->filename2);

        cout << collisions[i]->num_collisions << ": "
        << files[file1] << ", "
        << files[file2] <<endl;
    }

}

//makes the words from queue into a string chunk
string makeString(queue<string> words){
    string result = "";
    while (!words.empty()){
        string word = words.front();
        //iterate by letter
        for (int letter = 0; letter < word.length(); letter++){
            //if caps switch to lowercase
            if (word[letter]>= 'A' && word[letter]<= 'Z'){
                word[letter] = word[letter]+32;
            }
            //if valid letter then add to string
            if (word[letter]>= 'a' && word[letter]<= 'z'){
                result += word[letter];
            }
        }
        words.pop();
    }
    return result;
}

//inserts the node into the correct spot in the hash array
void insertHash(hashNode *hashTable[], int tableIndex, hashNode &node){
    if (hashTable[tableIndex] == NULL) {                        //if empty, add file node
        hashTable[tableIndex] = &node;
    }
    else {                                                                      //if not empty, add to head of linked list
        hashNode *temp = hashTable[tableIndex];
        hashTable[tableIndex] = &node;
        node.nextCollision = temp;
    }

}

bool compareCollision(collision *c1, collision *c2){
    return (c1->num_collisions > c2->num_collisions);
}