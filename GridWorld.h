/*
 CS 251 Project 2: GridWorld
 Name: Jessica DeStefano
 NetID: jdeste3
 Instructor: John Lillis
 Summer 2018
 July 15, 2018
*/

#include "GWInterface.h"
#include <vector>
#include <iostream>

using std::vector;
using namespace std;

class GridWorld : public GWInterface {

  public:
    GridWorld(unsigned nrows, unsigned ncols) {
        worldPopulation = 0; // set the world population
        this->nrows = nrows; // set rows and columns
        this->ncols = ncols;
        recycledIDs = new List;
        
        // dynamically allocate space for a 2D array of Districts
        grid = new District* [nrows];
        for (int i = 0; i < nrows; i++){
            grid[i] = new District [ncols];
        }
    }
    
    ~GridWorld(){
        // deallocate space made for 2D array of Districts
        for (int i = 0; i < nrows; i++){
            delete [] grid[i];
        }
        delete [] grid;
        
        // deallocate list made for recycled IDs
        delete recycledIDs;
    }

    bool birth(int row, int col, int &id){
        // out of bounds check
        if (row >= this->nrows || col >= this->ncols){
            return false;
        }
        
        // check if there are any dead people
        if (recycledIDs->front == nullptr){
            // append a new Person to the people vector
            Person *newPerson = new Person(row, col);
            people.push_back(*newPerson);
            
            // set the new ID - will be the same as the preincremented
            // world population since IDs start at 0
            id = worldPopulation;
        }
        else{
            // set the ID to the first node's value in recycled IDs
            // and pop it from the list, reassign row & column to input
            id = recycledIDs->front->personID;
            recycledIDs->popNode(recycledIDs->front);
            people[id].row = row;
            people[id].column = col;
        }
        
        // append ID to the linked list in the District
        grid[row][col].districtList->pushNode(id);
        
        // set the pointer in people vector to node in district
        // it will equal the most recently appended node
        people[id].districtNode = grid[row][col].districtList->back;
            
        // increment population of district
        grid[row][col].districtPopulation++;
            
        // increment world population
        worldPopulation++;
    
        // return true for success
        return true;
    }

    bool death(int personID){
        // check if the person exists and that they are not already dead
        if (people.size() == personID){
            return false;
        }
        if (people[personID].districtNode == nullptr){
            return false;
        }
        
        // push id to the recycled ID list
        recycledIDs->pushNode(personID);

        // pop from district list using the current row/col
        int r = people[personID].row;
        int c = people[personID].column;
        grid[r][c].districtList->popNode(people[personID].districtNode);
        
        // reassign the pointer in people vector to nullptr
        // (since they are dead, it points to nothing)
        people[personID].districtNode = nullptr;
        
        // decrement the disctict population
        grid[r][c].districtPopulation--;
        
        // decrement world population
        worldPopulation--;
        
        return true;
    }

    bool whereis(int id, int &row, int &col)const{
        // if the size is the same as the ID, then
        // the person doesn't exist
        if (people.size() == id){
            return false;
        }
        
        // if the pointer is nullptr, then they are dead
        if (people[id].districtNode == nullptr){
            return false;
        }
        
        // otherwise, set the row and column
        row = people[id].row;
        col = people[id].column;
        return true;
    }

    bool move(int id, int targetRow, int targetCol){
        // check if person exists and if row/col is valid
        if (people.size() == id){
            return false;
        }
        if (targetRow >= this->nrows || targetCol >= this->ncols){
            return false;
        }
        // find out what the original row/col is from people vector
        int r = people[id].row;
        int c = people[id].column;
        
        // push the ID to the new district
        grid[targetRow][targetCol].districtList->pushNode(id);
        grid[targetRow][targetCol].districtPopulation++;
        
        // pop the id from the original district
        grid[r][c].districtList->popNode(people[id].districtNode);
        grid[r][c].districtPopulation--;
        
        // update people array to hold new row, column and pointer
        people[id].row = targetRow;
        people[id].column = targetCol;
        people[id].districtNode = grid[targetRow][targetCol].districtList->back;
        
        return true;
    }

    std::vector<int> * members(int row, int col)const{
        vector<int> *membersInDistrict = new vector <int>();
        
        // return empty vector if out of bounds
        if (row >= this->nrows || col >= this->ncols){
            return membersInDistrict;
        }
        // return empty vector if there is no one in the district
        if (grid[row][col].districtList->front == nullptr){
            return membersInDistrict;
        }
        
        // transverse the list and add IDs to vector
        Node *tmp = grid[row][col].districtList->front;
        
        while (tmp != nullptr){
            membersInDistrict->push_back(tmp->personID);
            tmp = tmp->next;
        }

        return membersInDistrict;
    }
    
    int population()const{
      return this->worldPopulation;
    }
    
    int population(int row, int col)const{
        if (row >= this->nrows || col >= this->ncols){
            return 0;
        }
        
        return grid[row][col].districtPopulation;
    }

    int num_rows()const {
      return this->nrows;
    }
    
    int num_cols()const {
      return this->ncols;
    }

  private:
    
    /* -- Structs -- */
    
    /*
     Node Struct: Keeps track of person IDs, next, and previous
     nodes for the doubly linked list.
    */
    typedef struct NodeStruct {
        int personID;
        struct NodeStruct *next;
        struct NodeStruct *prev;
        
        // initialize the struct
        NodeStruct (){
            prev = nullptr;
            next = nullptr;
        }
    } Node;
    
    /*
     List Struct: Doubly linked. Used in District Struct to keep
     track of who lives in a district. Also used to keep track of
     recycled/dead IDs.
     */
    typedef struct ListStruct {
        Node *front;
        Node *back;
        
        // initialize the struct
        ListStruct (){
            front = nullptr;
            back = nullptr;
        }
        
        // push node to the end of a list
        void pushNode (int val){
            Node *newNode = new Node;
            newNode->personID = val;
            
            if (front == nullptr){
                front = back = newNode;
                front->next = back->next = nullptr;
                front->prev = back->prev = nullptr;
            }
            else{
                back->next = newNode;
                newNode->prev = back;
                newNode->next = nullptr;
                back = newNode;
            }
        }
        
        // pop a node
        void popNode (Node *deleted){
            if (front == nullptr){
                return;
            }
            else if (front == deleted){
                front = deleted->next;
            }
            else if (back == deleted){
                back = back->prev;
                back->next = nullptr;
            }
            else{
                // if in the middle, the previous node's next will equal
                // the deleted nodes next, and the next node's previous
                // if equal the deleted's prevoius node
                deleted->prev->next = deleted->next;
                deleted->next->prev = deleted->prev;
            }
            delete (deleted);
        }
        
    } List;
    
    /*
     Person Struct: Class GridWorld creates a vector of Person instances.
     Keeps track of a specific person's row and column (which district
     they live in), and a pointer to their node in this district.
     */
    typedef struct PersonStruct {
        int row;
        int column;
        Node *districtNode;
        
        // initialize the struct
        PersonStruct(int r, int c) {
            row = r;
            column = c;
            districtNode = nullptr;
        }

    } Person;
    
    typedef struct DistrictStruct {
        int districtPopulation;
        List *districtList;
        
        // initialize the struct
        DistrictStruct (){
            districtPopulation = 0;
            districtList = new List;
        }
        
    } District;
    
    /* -- GridWorld data members -- */
    int nrows, ncols;
    int worldPopulation;
    
    // people vector
    vector<Person> people;
    
    // dead/recycled ID linked list
    List *recycledIDs;
    
    // 2D array of districts
    District **grid;
    
};
